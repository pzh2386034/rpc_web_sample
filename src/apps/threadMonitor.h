#ifndef __RESOURCE_MONITOR_H__
#define __RESOURCE_MONITOR_H__
#include "glib-2.0/glib.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus date:<2018-10-07>*/
    typedef enum
    {
        /* 监控线程给处理函数 发的信号 date:<2018-10-07>*/
        THR_MON_PRETMOUT = 0, /* 超时一半进入此状态 date:<2018-10-07>*/
        THR_MON_TMOUT,        /* 超时进入此状态 date:<2018-10-07>*/
        THR_MON_HEARTBEART    /* 正常心跳检测此状态 date:<2018-10-07>*/
    } HANDLE_ENUM;

    struct thr_state_st;
    typedef void (*THR_MON_HANDLE)(HANDLE_ENUM, struct thr_state_st *, void *);
    typedef struct thr_state_st
    {
        guint8 enable;
        guint8 heartb_now;
        const char *thr_name;
        pid_t thr_tid; /* 内核级线程id，系统唯一 date:<2018-10-07>*/
        pthread_t thr_id; /* glibc库的线程id，实际上是线程控制块tcb首地址 date:<2018-10-07>*/
        guint8 timeout_max;
        THR_MON_HANDLE timeout_handle;
        guint8 heartb_last;
        guint8 timeout_cnt;
    } THR_STATE_ST;

    typedef enum
    {
        ENABLE = 0, /* 开启该线程心跳检测 date:<2018-10-08>*/
        UNABLE = 1
    } MONITOR_ENABLE;
    void thread_monitor_init();
    void thread_monitor_register(THR_STATE_ST **pphandle,
                                 guint8 enable,
                                 guint32 timeout,
                                 THR_MON_HANDLE timout_handle,
                                 const char *thr_name);
    void thread_monitor_heart_beat(THR_STATE_ST *handle, guint8 enable, gpointer oem_data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
