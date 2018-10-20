#include "errno.h"
#include "execinfo.h" /* for backtrace() date:<2018-10-07>*/
#include "glib/gprintf.h"
#include "rpc.h"
#include "rpc_resource.hpp"
#include "rpc_svc.h"
#include "signal.h" /* for signal date:<2018-10-07>*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "threadMonitor.h"
#include "unistd.h"
#define BACKTRACE_SIZE 16
static void dump(void)
{
    gint j, nptrs;
    void *buffer[BACKTRACE_SIZE];
    gchar **strings;
    nptrs = backtrace(buffer, BACKTRACE_SIZE);
    printf("%s, backtrace returned %d addresses\n", __func__, nptrs);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        printf("%s, backtrace_symbols err, %d\n", __func__, errno);
        exit(EXIT_FAILURE);
    }
    for (j = 0; j < nptrs; j++)
    {
        printf("[%02d] %s\n", j, strings[j]);
    }
    free(strings);
}
static void rpc_thread_monitor_timeout_sigle(int signo)
{
    printf("%s, catch sigle, start dump\n", __func__);
    dump();
    printf("%s, catch sigle, end dump\n", __func__);
    signal(signo, SIG_DFL); /* 恢复默认处理方式 date:<2018-10-08>*/
    raise(signo);           /* 重新发送信号 date:<2018-10-08>*/
    return;
}
int main()
{
    g_printf("%s, main in\n", __func__);
    /* 注册SIGSEGV信号处理函数 date:<2018-10-07>*/
    signal(SIGSEGV, rpc_thread_monitor_timeout_sigle);
    GThread *gthread = NULL;
    rpc_resource_init();

    gthread = g_thread_new("rpcSer", rpc_server_main, NULL);
    if (gthread == NULL)
    {
        g_printf("%s, create rpc serve thread failed\n", __func__);
        return -1;
    }
    /* 启动thread monitor thread date:<2018-10-08>*/
    thread_monitor_init();
    while (1)
    {
        sleep(10000);
    }

    return 0;
}
