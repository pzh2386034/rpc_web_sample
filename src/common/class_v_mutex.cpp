#include "class_v_mutex.h"
#include <time.h>
#include "errno.h"
#include "glib.h"
#include "pthread.h"
#include "stdio.h"

guint32 GetAbsTime(struct timespec *abstime, guint32 timeout_rw);
// guint32 thread_timed_mutex_lock(guint64 Sm_ID, guint32 ulTimeOutInMillSec);
// guint32 thread_timed_mutex_unlock(guint64 Sm_ID);
guint32 thread_timed_mutex_init(const char *name, guint64 *id, int type, int count)
{
    int err                 = 0;
    thread_timed_mutex_t *m = (pthread_timed_mutex_t)id;
    if (NULL == m)
    {
        return SEM_ERRNO_OBJNF;
    }
    (void)memset(m->name, 0, sizeof(m->name));
    (void)strcpy(m->name, name);
    m->owned = FALSE;
    m->count = count;
    if (SEM_COUNTER == type)
    {
        m->max = count;
    }
    else
    {
        m->max = 1;
    }
    m->type              = type;
    m->owner             = NULL;
    m->pwaiter_list_head = NULL;
    m->nwaiters          = 0;
    m->recu              = 0;
    switch (type)
    {
        case SEM_BINARY:
        case SEM_COUNTER:
        case SEM_MUTEX:
            err = pthread_mutex_init(&(m->lock), NULL);
            if (0 == err)
            {
                err = pthread_cond_init(&(m->cv), NULL);
            }
            break;
        case SEM_RWLOCK:
            err = pthread_rwlock_init(&(m->rwlock), NULL);
            break;
        default:
            return SEM_TYPE_ERR;
    }

    return TRUE;
}
guint64 *thread_timed_malloc_id()
{
    guint64 *psm = (guint64 *)malloc(sizeof(thread_timed_mutex_t));
    return psm;
}
guint32 VOS_SmP(guint64 Sm_ID, guint32 ulTimeOutInMillSec)
{
    if (0 == Sm_ID)
    {
        printf("SM_P failed, sm_id = NULL.");
        return ERROR;
    }

    return thread_timed_mutex_lock((guint64 *)Sm_ID, ulTimeOutInMillSec);
}

guint32 VOS_SmV(guint64 sm_id)
{
    if (0 == sm_id)
    {
        printf("SM_V failed, sm_id = NULL.");
        return ERROR;
    }
    return thread_timed_mutex_unlock((guint64 *)sm_id);
}
guint32 VOS_Sm_AsyP(guint64 Sm_ID)
{
    if (!Sm_ID)
        return SEM_ERRNO_OBJNF;

    return thread_timed_mutex_trylock((guint64 *)Sm_ID, 0);
}

guint32 VOS_SmRdP(guint64 Sm_ID)
{
    if (!Sm_ID)
        return SEM_ERRNO_OBJNF;
    return thread_timed_mutex_lock((guint64 *)Sm_ID, 1);
}

guint32 VOS_Sm_AsyRdP(guint64 Sm_ID)
{
    if (!Sm_ID)
        return SEM_ERRNO_OBJNF;
    return thread_timed_mutex_trylock((guint64 *)Sm_ID, 1);
}

// ²»¾ß±¸µÝ¹éÊôÐÔ£¬Èô¶à´ÎPÒ»´ÎV£¬Ôò²»»áËÀËø£¬Èô¶à´ÎP¶à´ÎV¾Í»áËÀËø
guint32 VOS_SmWrP(guint64 Sm_ID)
{
    if (!Sm_ID)
        return SEM_ERRNO_OBJNF;
    return thread_timed_mutex_lock((guint64 *)Sm_ID, 0);
}

guint32 VOS_Sm_AsyWrP(guint64 Sm_ID)
{
    if (!Sm_ID)
        return SEM_ERRNO_OBJNF;
    return thread_timed_mutex_trylock((guint64 *)Sm_ID, 0);
}

guint32 thread_timed_mutex_destory(guint64 *id)
{
    thread_timed_mutex_t *m = (pthread_timed_mutex_t)id;
    int err                 = 0;
    if (!m)
    {
        return SEM_ERRNO_OBJNF;
    }
    switch (m->type)
    {
        case SEM_BINARY:
        case SEM_COUNTER:
        case SEM_MUTEX:
            (void)pthread_mutex_destroy(&(m->lock));
            (void)pthread_cond_destroy(&(m->cv));
            break;
        case SEM_RWLOCK:
            (void)pthread_rwlock_destroy(&(m->rwlock));
            break;
        default:
            break;
    }
    return err;
}
guint32 VOS_SmCreate(const char sm_name[4], guint32 flags, guint64 *sm_id, int count)
{
    guint64 *psm = thread_timed_malloc_id();
    if (NULL == psm)
    {
        return NOT_ENOUGH_MEM;
    }
    *sm_id = (guint64)psm;
    switch (flags)
    {
        case SEM_BINARY:
        case SEM_COUNTER:
        case SEM_MUTEX:
        case SEM_RWLOCK:
            (void)thread_timed_mutex_init(sm_name, sm_id, flags, count);
            break;
        default:
            break;
    }
    return thread_timed_mutex_init(sm_name, psm, SEM_MUTEX, 1);
}
guint32 VOS_SmDelete(guint64 smid)
{
    if (smid == 0)
    {
        printf("%s, null smid.", __FUNCTION__);
    }
    thread_timed_mutex_destory((guint64 *)smid);
    free((guint64 *)smid);
    return TRUE;
}
static guint32 mutex_lock_COUNT(pthread_timed_mutex_t m, guint32 timeout_rw)
{
    guint32 err   = TRUE;
    pthread_t tid = pthread_self();
    struct timespec abstime;
    (void)pthread_mutex_lock(&(m->lock));
    pthread_cleanup_push((cleanuphandler)pthread_mutex_unlock,
                         (&(m->lock))); /* 线程退出前执行的函数 date:<2018-10-25>*/
    if ((m->count <= 0) || (m->nwaiters > 0))
    {
        /* 没有空余的count且有人在等待, 则加入等待队列 date:<2018-10-25>*/
        if ((0 == timeout_rw) && (m->owner != NULL) && (pthread_equal(tid, m->owner)))
        {
            printf("%s, thread %ld DEAD LOCK by %s.\n", __FUNCTION__, (long)tid, m->name);
            err = SEM_ERRNO_CANTP;
        }
        else
        {
            m->count--;
            m->pwaiter_list_head = g_slist_append(m->pwaiter_list_head, tid);
            m->nwaiters++;
            if (0 == timeout_rw)
            {
                (void)pthread_cond_wait(&(m->cv), &(m->lock));
            }
            else
            {
                GetAbsTime(&abstime, timeout_rw);
                err = pthread_cond_timedwait(&(m->cv), &(m->lock), &abstime);
                if (ETIMEDOUT == err)
                {
                    err = SEM_ERRNO_TIMEOUT;
                    m->count++;
                }
                else if (err)
                {
                    err = SEM_ERRNO_CANTP;
                    m->count++;
                }
            }
            m->pwaiter_list_head = g_slist_remove(m->pwaiter_list_head, tid);
            m->nwaiters--;
        }
    }
    else
    {
        /* 否则为count>0，或者nwaiters=0,此时可以直接sem date:<2018-10-25>*/
        m->count--;
    }
    if (0 == err)
    {
        /* if no err, then this thread get sem. date:<2018-10-25>*/
        m->owner = tid;
        m->owned = TRUE;
    }
    pthread_cleanup_pop(1);
    return err;
}

static guint32 mutex_lock_MUTEX(pthread_timed_mutex_t m, guint32 timeout_rw)
{
    guint32 err = TRUE;
    struct timespec abstime;
    pthread_t tid = pthread_self();
    (void)pthread_mutex_lock(&(m->lock));
    pthread_cleanup_push((cleanuphandler)pthread_mutex_unlock, &(m->lock));
    if ((m->owner != NULL) && (pthread_equal(m->owner, tid)))
    {
        printf("%s, thread %ld DEAD LOCK by %s.\n", __FUNCTION__, (long)tid, m->name);
        /* m not used now. date:<2018-10-25>*/
        err = SEM_ERRNO_CANTP;
    }
    else if (m->count > 0)
    {
        m->count--;
    }
    else
    {
        m->nwaiters++;
        m->pwaiter_list_head = g_slist_append(m->pwaiter_list_head, tid);
        if (0 == timeout_rw)
        {
            (void)pthread_cond_wait(&(m->cv), &(m->lock));
        }
        else
        {
            (void)GetAbsTime(&abstime, (guint32)timeout_rw);
            err = pthread_cond_timedwait(&(m->cv), &(m->lock), &abstime);
            if (ETIMEDOUT == err)
            {
                err = SEM_ERRNO_TIMEOUT;
            }
            else if (err)
            {
                err = SEM_ERRNO_CANTP;
            }
        }
        m->nwaiters--;
        m->pwaiter_list_head = g_slist_remove(m->pwaiter_list_head, tid);
    }
    if (0 == err)
    {
        m->owner = tid;
        m->owned = TRUE;
    }
    pthread_cleanup_pop(1);
    return err;
}
static guint32 mutex_lock_RWLOCK(pthread_timed_mutex_t m, guint32 rw_flag)
{
    guint32 err = 0;
    if (0 == rw_flag)
    {
        err = pthread_rwlock_wrlock(&(m->rwlock));
    }
    else
    {
        err = pthread_rwlock_rdlock(&(m->rwlock));
    }
    if (0 != err)
    {
        err = SEM_ERRNO_CANTP;
    }
    return err;
}
static guint32 mutex_try_lock_MUTEX(pthread_timed_mutex_t m)
{
    guint32 err  = TRUE;
    pthread_t id = pthread_self();
    (void)pthread_mutex_lock(&(m->lock));
    if ((m->owner != NULL) && (pthread_equal(m->owner, id)))
    {
        m->count = 0;
    }
    if (m->count > 0)
    {
        m->count--;
        m->owner = id;
        m->owned = TRUE;
    }
    else
    {
        printf("%s, try lock failed, mux name:%s", __FUNCTION__, m->name);
        err = SEM_ERRNO_CANTP;
    }
    pthread_mutex_unlock(&(m->lock));
    return err;
}
static guint32 mutex_unlock_MUTEX(pthread_timed_mutex_t m)
{
    guint32 err   = 0;
    pthread_t tid = pthread_self();
    (void)pthread_mutex_lock(&(m->lock));
    if (m->recu)
    {
        if (pthread_equal(m->owner, tid))
            m->recu--;
        else if (ESRCH == pthread_kill(m->owner, 0))
            m->recu = 0;
    }
    else
    {
        m->owned = FALSE;
        m->owner = NULL;
        if (0 == m->nwaiters)
        {
            if (m->count < m->max)
                m->count++;
        }
        else
        {
            /* 如果有人在等待，则一定是等待信号激活 date:<2018-10-26>*/
            err = pthread_cond_signal(&(m->cv));
        }
    }
    err = pthread_mutex_unlock(&(m->lock));
    return err;
}
guint32 thread_timed_mutex_lock(guint64 *id, guint32 timeout_rw)
{
    thread_timed_mutex_t *m = (pthread_timed_mutex_t)id;
    guint32 err             = TRUE;
    int type;
    if (NULL == m)
    {
        return SEM_ERRNO_OBJNF;
    }
    type = m->type;
    switch (type)
    {
        case SEM_BINARY:
        case SEM_COUNTER:
            err = mutex_lock_COUNT(m, timeout_rw);
            break;
        case SEM_MUTEX:
            err = mutex_lock_MUTEX(m, timeout_rw);
            break;
        case SEM_RWLOCK:
            err = mutex_lock_RWLOCK(m, timeout_rw);
            break;
        default:
            return SEM_TYPE_ERR;
    }
    return err;
}

guint32 thread_timed_mutex_trylock(guint64 *id, guint32 timeout_rw)
{
    thread_timed_mutex_t *m = (pthread_timed_mutex_t)id;
    guint32 err             = 0;
    int type                = 0;
    pthread_t tid           = pthread_self();
    if (NULL == m)
    {
        return SEM_ERRNO_OBJNF;
    }
    type = m->type;

    switch (type)
    {
        case SEM_BINARY:
        case SEM_COUNTER:
            (void)pthread_mutex_lock(&(m->lock));
            if ((m->count <= 0) || (m->nwaiters > 0))
            {
                err = SEM_ERRNO_CANTP;
            }
            else
            {
                m->owner = tid;
                m->count--;
                m->owned = TRUE;
            }
            pthread_mutex_lock(&(m->lock));
            break;
        case SEM_MUTEX:
            err = mutex_try_lock_MUTEX(m);
            break;
        case SEM_RWLOCK:
            /* timeout_rw 在这表示是读还是些 date:<2018-10-26>*/
            if (0 == timeout_rw)
            {
                err = pthread_rwlock_trywrlock(&(m->rwlock));
            }
            else
            {
                err = pthread_rwlock_tryrdlock(&(m->rwlock));
            }
            if (0 == err)
            {
                printf("%s, get rwlock failed, name:%s, err:%d", __FUNCTION__, m->name, err);
                err = SEM_ERRNO_CANTP;
            }
            break;
        default:
            return SEM_TYPE_ERR;
    }
    return err;
}

guint32 thread_timed_mutex_unlock(guint64 *id)
{
    thread_timed_mutex_t *m = (pthread_timed_mutex_t)id;
    int err                 = TRUE;
    if (NULL == m)
    {
        return SEM_ERRNO_OBJNF;
    }
    int type = m->type;
    switch (type)
    {
        case SEM_BINARY:
        case SEM_COUNTER:
            (void)pthread_mutex_lock(&(m->lock));
            if (m->count < m->max)
                m->count++;
            m->owner = NULL;
            m->owned = FALSE;
            if (m->nwaiters > 0)
            {
                (void)pthread_mutex_unlock(&(m->lock));
                (void)pthread_cond_signal(&(m->cv));
                break;
            }
            err = pthread_mutex_unlock(&(m->lock));
            break;
        case SEM_MUTEX:
            err = mutex_unlock_MUTEX(m);
            break;
        case SEM_RWLOCK:
            err = pthread_rwlock_unlock(&(m->rwlock));
            if (err)
            {
                printf("%s, sem v failed, name:%s, err:%d", __FUNCTION__, m->name, err);
                err = SEM_ERRNO_CANTV;
            }
            break;
        default:
            printf("%s, sem v failed, name:%s, unknow type.", __FUNCTION__, m->name);
            break;
    }
    return err;
}
char *thread_name(pthread_t tid)
{
    char s[100];
    thread_name_t *n;

    static pthread_mutex_t lock        = PTHREAD_MUTEX_INITIALIZER;
    static thread_name_t *thread_names = NULL;

    if (pthread_equal(NULL, tid))
        tid = pthread_self();
    pthread_mutex_lock(&lock);

    for (n = thread_names; n != NULL; n = n->next)
    {
        if (pthread_equal(n->tid, tid))
            goto L;
    }
    n      = (thread_name_t *)malloc(sizeof(thread_name_t));
    n->tid = tid;
    sprintf(s, "T@%ld", (long)tid);
    n->name = (char *)malloc(strlen(s) + 1);
    strcpy(n->name, s);
    n->next      = thread_names;
    thread_names = n;

L:
    pthread_mutex_unlock(&lock);
    return (n->name);
}
void thread_timed_mutex_show(guint64 *id)
{
    if (!id)
        return;
    GSList *plist;
    thread_timed_mutex_t *m = (pthread_timed_mutex_t)id;
    (void)pthread_mutex_lock(&(m->lock));
    printf("\r\n\r\nNAME   COUNT     OWNER\r\n");
    printf("\r\n%s     %d        %ld    ", m->name, m->count, (guint64)m->owner);
    plist = m->pwaiter_list_head;
    printf("\r\nWAITER LIST:");
    while (plist)
    {
        gchar *pName = thread_name((pthread_t)plist->data);
        if (pName)
        {
            printf("\r\n\t%s", pName);
        }
        else
        {
            printf("\r\n\t%ld(deleted)", (guint64)plist->data);
        }
        plist = plist->next;
    }
    printf("\r\n");
    (void)pthread_mutex_unlock(&(m->lock));
}

guint32 GetAbsTime(struct timespec *abstime, guint32 timeout_rw)
{

    return 1;
}
