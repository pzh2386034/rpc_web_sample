#include "threadMonitor.h"
#include "errno.h"
#include "pthread.h"
#include "rpc.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/syscall.h"
#include "unistd.h"

#ifndef MEM_FREE
#define MEM_FREE(p)   \
    do                \
    {                 \
        if (p)        \
        {             \
            delete p; \
            p = NULL; \
        }             \
    } while (0);
;
#endif

#define MAX_MONITOR_THREAD_NUM 3
#define THR_MON_PRECI 10
THR_STATE_ST *g_thr_state[MAX_MONITOR_THREAD_NUM] = {0};

/**************************************
 * date:<2018-10-07>
线程监控主任务
 * author:  pzh2467908@163.com
 **************************************/
gpointer thread_monitor_task(void)
{
    gint i = 0;
    for (i = 0; i < MAX_MONITOR_THREAD_NUM + 1; i++)
    {
        if (i == MAX_MONITOR_THREAD_NUM || g_thr_state[i] == NULL)
        {
            /* 当i==最大监控线程数或者结构体未初始化时，表示一次监控已经完成，从0再开始
             * date:<2018-10-07>*/

            (void)sleep(THR_MON_PRECI);
            i = -1;
            continue;
        }
        if (g_thr_state[i]->enable == UNABLE)
        {
            /* 该线程目前状态是否需要检测 date:<2018-10-08>*/
            /* rpc线程在没用rpc调用的时候不需要检测 date:<2018-10-08>*/
            continue;
        }
        if (g_thr_state[i]->heartb_now != g_thr_state[i]->heartb_last)
        {
            g_thr_state[i]->heartb_last = g_thr_state[i]->heartb_now;
            g_thr_state[i]->timeout_cnt = 0;
            continue;
        }
        if (g_thr_state[i]->timeout_cnt < g_thr_state[i]->timeout_max)
        {
            if (g_thr_state[i]->timeout_max / 2 == g_thr_state[i]->timeout_cnt)
            {
                printf("%s:thread:%s:%d:%ld pre time out!\n", __func__, g_thr_state[i]->thr_name,
                       g_thr_state[i]->thr_tid, (long)g_thr_state[i]->thr_id);
                g_thr_state[i]->timeout_handle(THR_MON_PRETMOUT, g_thr_state[i], NULL);
            }
            g_thr_state[i]->timeout_cnt++;
            continue;
        }
        if (g_thr_state[i]->timeout_cnt == g_thr_state[i]->timeout_max)
        {
            printf("%s:thread:%s:%d:%ld time out!\n", __func__, g_thr_state[i]->thr_name,
                   g_thr_state[i]->thr_tid, (long)g_thr_state[i]->thr_id);
            g_thr_state[i]->timeout_handle(THR_MON_TMOUT, g_thr_state[i], NULL);
            g_thr_state[i]->timeout_cnt++;
            continue;
        }
    }
    return NULL;
}

/**************************************
 * date:<2018-10-07>
线程监控模块初始化，常驻线程，不考虑使用线程池
 * author:  pzh2467908@163.com
 **************************************/
void thread_monitor_init()
{
    GThread *gthread = g_thread_new("rpcSer", (gpointer(*)(gpointer))thread_monitor_task, NULL);
    if (gthread == NULL)
    {
        printf("%s, create monitor thread failed\n", __func__);
        return;
    }
    printf("%s, create monitor thread success\n", __func__);
}

void thread_monitor_register(THR_STATE_ST **pphandle,
                             guint8 enable,
                             guint32 timeout,
                             THR_MON_HANDLE timout_handle,
                             const char *thr_name)

{
    gint i = 0;
    if (NULL == pphandle || NULL == thr_name)
    {
        return;
    }
    for (i = 0; i < MAX_MONITOR_THREAD_NUM; i++)
    {
        if (!g_thr_state[i])
        {
            break;
        }
    }
    if (i == MAX_MONITOR_THREAD_NUM)
    {
        printf("%s, monitor thread reached MAX number\n", __func__);
    }
    *pphandle = (THR_STATE_ST *)malloc(sizeof(THR_STATE_ST));
    memset(*pphandle, 0, sizeof(THR_STATE_ST));
    g_thr_state[i]                 = *pphandle;
    g_thr_state[i]->thr_name       = thr_name;
    g_thr_state[i]->thr_tid        = syscall(SYS_gettid);
    g_thr_state[i]->enable         = enable;
    g_thr_state[i]->thr_id         = pthread_self();
    g_thr_state[i]->timeout_max    = timeout / THR_MON_PRECI;
    g_thr_state[i]->timeout_handle = timout_handle;
    g_thr_state[i]->heartb_now     = g_thr_state[i]->heartb_last + 1;
    printf("%s:thr:%s:%d:%ld:%x begin monitor success\n", __func__, g_thr_state[i]->thr_name,
           g_thr_state[i]->thr_tid, (long)g_thr_state[i]->thr_id, getpid());
}

/**************************************
 * date:<2018-10-07>
给被监控线程发送心跳
 * author:  pzh2467908@163.com
 **************************************/
void thread_monitor_heart_beat(THR_STATE_ST *handle, guint8 enable, gpointer data)
{
    if (NULL == handle)
    {
        return;
    }
    handle->heartb_now++; /* heartb_last状态由监控线程刷新 date:<2018-10-07>*/
    handle->enable = enable;
    handle->timeout_handle(THR_MON_HEARTBEART, handle, data);
}
