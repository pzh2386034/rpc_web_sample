#ifndef __CLASS_V_MUTEX_H__
#define __CLASS_V_MUTEX_H__
/**************************************
 * date:<2018-10-21>
define 中的三个特殊符号：#  ##  #@
##: 连接
#define Conn(x, y) x##y
int n = Conn(123,456)  ====> n=123456
char * str = Conn("asd", "adf") =====>str="asdadf"


#@: 给x加上单引号，结果返回一个const char
#define ToChar(x) #@x
char a=ToChar(1)  =====> a='1'


#: 给x加上双引号, 结果返回char *
#define ToString(x) #x
char *str = ToString(1234) ========> str="1234"

 * author:  pzh2467908@163.com
 **************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "glib-2.0/glib.h"
#include "stdio.h"
#define ERROR 1
#define OFFSET(type, field) ((size_t) & ((type *)0)->field)
#define FSIZ(type, field) sizeof(((type *)0)->field)
#define ARR_SIZE(a) (sizeof(a) / sizeof((a[0])))
#define SEM_TYPE_ERR 0x50000001
#define SEM_ERRNO_OBJNF 0x50000002
#define SEM_ERRNO_CANTP 0x50000003
#define SEM_ERRNO_CANTV 0x50000004
#define SEM_ERRNO_TIMEOUT 0x50000005
#define NOT_ENOUGH_MEM 0x40000001

    typedef void (*cleanuphandler)(void *arg);

    typedef struct _thread_list_t
    {
        pthread_t tid;
        struct _thread_list_t *next;
    } thread_list_t, *pthread_list_t;

    typedef enum
    {
        SEM_BINARY = 0,
        SEM_COUNTER,
        SEM_MUTEX,
        SEM_RWLOCK,
    } SEM_TYPE;
    typedef enum
    {
        WRITE_FLAG = 0,
        READ_FLAG  = 1
    } RWMUXFLAG;
    typedef struct _pt
    {
        pthread_t tid;
        char *name;
        struct _pt *next;
    } thread_name_t;

    typedef struct _timed_mutex
    {

        char name[5];
        pthread_mutex_t lock;    /* lock for structure date:<2018-10-24>*/
        pthread_rwlock_t rwlock; /* lock for readwrite date:<2018-10-24>*/
        pthread_cond_t cv;       /* waiting list control date:<2018-10-24>*/
        pthread_cond_t cv_readers;

        int nreaders;
        int nwriters;
        int nwaiters;
        int owned;       /* TRUE|FALSE date:<2018-10-24>*/
        int recu;        /* recursive count date:<2018-10-24>*/
        int count;       /* P V count date:<2018-10-24>*/
        int max;         /* max count date:<2018-10-24>*/
        int type;        /* sem4 type date:<2018-10-24>*/
        pthread_t owner; /* NULL_TID or TID date:<2018-10-24>*/
        GSList *pwaiter_list_head;

    } thread_timed_mutex_t, *pthread_timed_mutex_t;

    guint32 thread_timed_mutex_lock(guint64 *id, guint32 timeout_rw);
    guint32 thread_timed_mutex_trylock(guint64 *id, guint32 r_w);
    guint32 thread_timed_mutex_unlock(guint64 *id);
    guint32 VOS_SmP(guint32 Sm_ID, guint32 ulTimeOutInMillSec);
    guint32 VOS_SmV(guint32 Sm_ID);
    guint32 VOS_Sm_AsyP(guint64 Sm_ID);
    guint32 VOS_SmRdP(guint64 Sm_ID);
    guint32 VOS_Sm_AsyRdP(guint64 Sm_ID);
    guint32 VOS_SmWrP(guint64 Sm_ID);
    guint32 VOS_Sm_AsyWrP(guint64 Sm_ID);
    guint32 VOS_SmCreate(const char sm_name[4], guint32 flag, guint32 *sm_id, int count);
    void thread_timed_mutex_show(guint64 *id);

    /* ------------------------------------------------------------------------- date:<2018-10-27>*/

#define DECLARE_OBJLEVEL_PROTECT(type)              \
    guint32 m_ulSemID_##type;                       \
    class CMux                                      \
    {                                               \
      public:                                       \
        CMux(type &obj, const char *file, int line) \
        {                                           \
            __file     = file;                      \
            __line     = line;                      \
            __ulSemID  = obj.m_ulSemID_##type;      \
            __ulResult = VOS_SmP(__ulSemID, 0);     \
        }                                           \
        virtual ~CMux()                             \
        {                                           \
            if (0 == __ulResult)                    \
            {                                       \
                (void)VOS_SmV(__ulSemID);           \
            }                                       \
        }                                           \
                                                    \
      protected:                                    \
        guint32 __ulSemID;                          \
        guint32 __ulResult;                         \
        char *__file;                               \
        int __line;                                 \
    };
#define INITIAL_OBJLEVEL_PROTECT(type)              \
    m_ulSem_##type = 0;                             \
    /* 创建信号量锁保护 date:<2018-10-26>*/ \
    (void)VOS_SmCreate("mux", SEM_MUTEX, &m_ulSem_##type, 1);
#define APPLY_OBJLEVEL_PROTECT CMux mux(*this, __FILE__, __LINE__);
#define APPLY_OBJLEVEL_PROTECT_OF(type, pobj) type::CMux mux##type(*(pobj), __FILE__, __LINE__);

    /* ------------------------------------------------------------- date:<2018-10-27>*/
#define DECLARE_OBJLEVEL_ACCESS_PROTECT(type)                       \
    guint32 m_ulSemID_##type;                                       \
    class CRWMux                                                    \
    {                                                               \
      public:                                                       \
        CRWMux(type &obj, guint32 ulLockType, char *file, int line) \
        {                                                           \
            __file      = file;                                     \
            __line      = line;                                     \
            __ulRWSemID = obj.m_ulSemID_##type;                     \
            if (0 == ulLockType)                                    \
            {                                                       \
                __ulResult = VOS_SmWrP(__ulRWSemID);                \
            }                                                       \
            else                                                    \
            {                                                       \
                __ulResult = VOS_SmRdP(__ulRWSemID);                \
            }                                                       \
        }                                                           \
        virtual ~CRWMux()                                           \
        {                                                           \
            if (0 == __ulResult)                                    \
            {                                                       \
                (void)VOS_SmV(__ulRWSemID);                         \
            }                                                       \
        }                                                           \
                                                                    \
      protected:                                                    \
        guint32 __ulRWSemID;                                        \
        guint32 __ulResult;                                         \
        char *__file;                                               \
        int __line;                                                 \
    }

#define INITIAL_OBJLEVEL_ACCESS_PROTECT(type) \
    m_ulSemID_##type = 0;                     \
    (void)VOS_SmCreate("rwx", SEM_RWMUX, &m_ulSemID_##type, 1);

#define DESTROY_OBJLEVEL_PROTECT(type)    \
    (void)VOS_SmDelete(m_ulSemID_##type); \
    m_ulSemID_##type = 0;

#define APPLY_OBJ_READ_PROTECT CRWMux mux(*this, READ_FLAG, __FILE__, __LINE__);
#define APPLY_OBJ_MODIFY_PROTECT CRWMux mux(*this, WRITE_FLAG, __FILE__, __LINE__);
#define APPLY_OBJ_READ_PROTECT_OF(type, pobj) \
    type::CRWMux mux##type(*(pobj), READ_FLAG, __FILE__, __LINE__);
#define APPLY_OBJ_MODIFY_PROTECT_OF(type, pobj) \
    type::CRWMux mux##type(*(pobj), WRITE_FLAG, __FILE__, __LINE__);

    /* ---------------------------------------------------------------------------------------------
     * date:<2018-10-27>*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif
