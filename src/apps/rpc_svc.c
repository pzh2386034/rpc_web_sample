/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "rpc_svc.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <memory.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/ttycom.h>
#include <syslog.h>
#include <unistd.h>
#include "../common/commonFun.h"
#include "errno.h"
#include "rpc/rpc.h"
#include "threadMonitor.h"

#ifdef __STDC__
#define SIG_PF void (*)(int)
#endif

#ifdef DEBUG
#define RPC_SVC_FG
#endif

#define _RPCSVC_CLOSEDOWN 120
static int _rpcpmstart;  /* Started by a port monitor ? */
static int _rpcfdtype;   /* Whether Stream or Datagram ? */
static int _rpcsvcdirty; /* Still serving ? */
THR_STATE_ST *g_rpc_hb_handle = NULL;

static void _msgout(msg) char *msg;
{
#ifdef RPC_SVC_FG
    if (_rpcpmstart)
        syslog(LOG_ERR, "%s", msg);
    else
        (void)fprintf(stderr, "%s\n", msg);
#else
    syslog(LOG_ERR, "%s", msg);
#endif
}

/**************************************
 * date:<2018-10-07>
日志记录函数
* author:  pzh2467908@163.com
**************************************/
static void log_rpc_info(RPCInput *arg)
{
    if (NULL == arg)
    {
        printf("%s, null point.", __func__);
        return;
    }
    printf("%s:user:%s,ip:%s,mode:%d,fun_index:%d,out_len:%d,in_len:%d,in:%s\n", __func__,
           arg->username, arg->userIP, arg->usermode, arg->fun_index, arg->out_para_len,
           arg->input_para.input_para_len, arg->input_para.input_para_val);
}
/**************************************
 * date:<2018-10-07>
rpc线程的监控回掉函数，用于心跳时的记录信息，或超时的处理
 * author:  pzh2467908@163.com
 **************************************/
static void rpc_thread_handle(HANDLE_ENUM type, THR_STATE_ST *handle, gpointer data)
{
    static RPCInput *s_prp_info = NULL;
    static GMutex mutex         = G_STATIC_MUTEX_INIT;
    g_mutex_lock(&mutex);
    switch (type)
    {
        case THR_MON_HEARTBEART:
            if (handle->enable)
            {
                s_prp_info = data;
            }
            else
            {
                s_prp_info = NULL;
            }
            break;
        case THR_MON_PRETMOUT:
            log_rpc_info(data);
            break;
        case THR_MON_TMOUT:
            log_rpc_info(data);
            printf("%s, begin kill:%d...\n", __func__, handle->thr_tid);
            if (0 != kill(handle->thr_tid, SIGSEGV))
            {
                printf("%s, kill failed, errno:%d\n", __func__, errno);
            }
            else
            {
                printf("%s, kill successed\n", __func__);
                (void)pause();
                printf("%s, pause exit\n", __func__);
            }
            break;
        default:
            printf("%s, unknow state: %d\n", __func__, type);
            break;
    }
    g_mutex_unlock(&mutex);
}
static void closedown()
{
    if (_rpcsvcdirty == 0)
    {
        extern fd_set svc_fdset;
        static int size;
        int i, openfd;

        if (_rpcfdtype == SOCK_DGRAM)
            exit(0);
        if (size == 0)
        {
            size = getdtablesize();
        }
        for (i = 0, openfd = 0; i < size && openfd < 2; i++)
            if (FD_ISSET(i, &svc_fdset))
                openfd++;
        if (openfd <= (_rpcpmstart ? 0 : 1))
            exit(0);
    }
    (void)alarm(_RPCSVC_CLOSEDOWN);
}

static void rpcapiprog_1(rqstp, transp) struct svc_req *rqstp;
SVCXPRT *transp;
{
    union {
        RPCInput all_api_in_one_1_arg;
        RPCIDInput rpc_identify_remote_1_arg;
    } argument;
    char *result;
    bool_t (*xdr_argument)(), (*xdr_result)();
    char *(*local)();

    _rpcsvcdirty = 1;
    switch (rqstp->rq_proc)
    {
        case NULLPROC:
            (void)svc_sendreply(transp, (xdrproc_t)xdr_void, (char *)NULL);
            _rpcsvcdirty = 0;
            return;

        case ALL_API_IN_ONE:
            xdr_argument = xdr_RPCInput;
            xdr_result   = xdr_RPCOutput;
            local        = (char *(*)())all_api_in_one_1_svc;
            break;

        case RPC_identify_remote:
            xdr_argument = xdr_RPCIDInput;
            xdr_result   = xdr_RPCIDOutput;
            local        = (char *(*)())rpc_identify_remote_1_svc;
            break;

        default:
            svcerr_noproc(transp);
            _rpcsvcdirty = 0;
            return;
    }
    (void)memset((char *)&argument, 0, sizeof(argument));
    if (!svc_getargs(transp, xdr_argument, (caddr_t)&argument))
    {
        svcerr_decode(transp);
        _rpcsvcdirty = 0;
        return;
    }
    /* 每一次rpc调用前发送心跳，传入本次rpc调用的参数， 并通知开始检测rpc server 线程
     * date:<2018-10-08>*/
    thread_monitor_heart_beat(g_rpc_hb_handle, ENABLE, &argument);
    timelog("%s, begin to ask rpc.funidx:%d\n", __func__, argument.all_api_in_one_1_arg.fun_index);
    result = (*local)(&argument, rqstp);
    /* rpc返回后，停止检测rpc server线程 date:<2018-10-08>*/
    thread_monitor_heart_beat(g_rpc_hb_handle, UNABLE, NULL);
    if (result != NULL && !svc_sendreply(transp, (xdrproc_t)xdr_result, result))
    {
        svcerr_systemerr(transp);
    }
    if (!svc_freeargs(transp, xdr_argument, (caddr_t)&argument))
    {
        _msgout("unable to free arguments");
        exit(1);
    }
    _rpcsvcdirty = 0;
    return;
}
int CreateRPCTCPSock(void)
{
    struct sockaddr_in localAddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        return sockfd;
    }
    memset_s(&localAddr, sizeof(struct sockaddr_in), 0, sizeof(struct sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port   = 0;
    if (1 != inet_pton(AF_INET, "127.0.0.1", &(localAddr.sin_addr.s_addr)))
    {
        close(sockfd);
        return -1;
    }
    if (bind(sockfd, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0)
    {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int CreateRPCUDPSock(void)
{
    struct sockaddr_in localAddr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        return sockfd;
    }
    memset_s(&localAddr, sizeof(struct sockaddr_in), 0, sizeof(struct sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port   = 0;
    if (1 != inet_pton(AF_INET, "127.0.0.1", &(localAddr.sin_addr.s_addr)))
    {
        close(sockfd);
        return -1;
    }
    if (bind(sockfd, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0)
    {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

gpointer rpc_server_main(gpointer argv)
{
    SVCXPRT *transptcp, *transpudp = NULL;
    argv             = NULL;
    int sockLocalTCP = 0, sockLocalUDP = 0;
    int proto    = 0;
    sockLocalTCP = CreateRPCTCPSock();
    if (sockLocalTCP < 0)
    {
        printf("%s, create tcp listen sock failed, ret:%d", __func__, sockLocalTCP);
    }
    sockLocalUDP = CreateRPCUDPSock();
    if (sockLocalUDP < 0)
    {
        printf("%s, create udp listen sock failed, ret:%d", __func__, sockLocalUDP);
    }

    /* 获得一个套接字的ip地址 date:<2018-11-01>*/
    /*     if (getsockname(0, (struct sockaddr *)&saddr, (socklen_t *)&asize) == 0) */
    /*     { */
    /*         int ssize = sizeof(int); */

    /*         if (saddr.sin_family != AF_INET) /\* ipv4地址族 date:<2018-11-01>*\/ */
    /*             exit(1); */
    /*         /\* 获取任意类型，任意状态套接字的选项的当前值，并把结果存入para4 date:<2018-11-01>
     */
    /* @para1 套接字描述符 */
    /* @para2 选择定义层级 */
    /* @para3 需获取的套接字选项 */
    /* @para4 optval获得选项值的缓冲区 */
    /* @para5 optval长度 */
    /*          *\/ */
    /*         if (getsockopt(0, SOL_SOCKET, SO_TYPE, (char *)&_rpcfdtype, (socklen_t *)&ssize) ==
     * -1) */
    /*             exit(1); */
    /*         sock        = 0; */
    /*         _rpcpmstart = 1; */
    /*         proto       = 0; */
    /*         openlog("rpc", LOG_PID, LOG_DAEMON); */
    /*     } */
    /*     else */
    /*     { */
    /* #ifndef RPC_SVC_FG */
    /*         int size; */
    /*         int pid, i; */

    /*         pid = fork(); */
    /*         if (pid < 0) */
    /*         { */
    /*             perror("cannot fork"); */
    /*             exit(1); */
    /*         } */
    /*         if (pid) */
    /*             exit(0); /\* close father terminal date:<2018-10-06>*\/ */
    /*         /\* 获取当前进程文件描述表的项数，循环关闭 date:<2018-10-06>*\/ */
    /*         size = getdtablesize(); */
    /*         for (i = 0; i < size; i++) */
    /*             (void)close(i); */
    /*         i = open("/dev/console", 2); /\* open serial console date:<2018-10-06>*\/ */
    /*         (void)dup2(i, 1);            /\* redirect stdout to serial console
     * date:<2018-10-06>*\/ */
    /*         (void)dup2(i, 2);            /\* redirect stderr to serial console
     * date:<2018-10-06>*\/ */
    /*         i = open("/dev/tty", 2); */
    /*         if (i >= 0) */
    /*         { */
    /*             (void)ioctl(i, TIOCNOTTY, (char *)NULL); */
    /*             (void)close(i); */
    /*         } */
    /*         openlog("rpc", LOG_PID, LOG_DAEMON); */
    /* #endif */
    /*         sock = RPC_ANYSOCK; */
    /*         (void)pmap_unset(RPCAPIPROG, RPCAPIVERS); */
    /*     } */

    transpudp = svcudp_create(sockLocalUDP);
    if (transpudp == NULL)
    {
        _msgout("cannot create udp service.");
        exit(1);
    }
    if (!_rpcpmstart)
        proto = IPPROTO_UDP;
    if (!svc_register(transpudp, RPCAPIPROG, RPCAPIVERS, rpcapiprog_1, proto))
    {
        _msgout("unable to register (RPCAPIPROG, RPCAPIVERS, udp).");
        svc_destroy(transpudp);
        close(sockLocalUDP);
        exit(1);
    }

    transptcp = svctcp_create(sockLocalTCP, 0, 0);
    if (transptcp == NULL)
    {
        _msgout("cannot create tcp service.");
        exit(1);
    }
    if (!_rpcpmstart)
        proto = IPPROTO_TCP;
    if (!svc_register(transptcp, RPCAPIPROG, RPCAPIVERS, rpcapiprog_1, proto))
    {
        _msgout("unable to register (RPCAPIPROG, RPCAPIVERS, tcp).");
        svc_destroy(transptcp);
        close(sockLocalTCP);
        exit(1);
    }

    if (_rpcpmstart)
    {
        (void)signal(SIGALRM, (void (*)())closedown);
        (void)alarm(_RPCSVC_CLOSEDOWN);
    }
    thread_monitor_register(&g_rpc_hb_handle, UNABLE, 60 * 0.5, rpc_thread_handle, "rpcServ");
    svc_run();
    close(sockLocalUDP);
    close(sockLocalTCP);
    svc_destroy(transpudp);
    svc_destroy(transptcp);
    _msgout("svc_run returned");

    exit(1);
    /* NOTREACHED */
}
