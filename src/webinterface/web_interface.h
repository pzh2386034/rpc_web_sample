#ifndef _WEB_INTERFACE_H
#define _WEB_INTERFACE_H
/* 解决emunate SUCCESS redefine date:<2018-12-21>*/
// #include "../common/rpc_api.h"
#include "arpa/inet.h"
#include "glib-2.0/glib.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */
#define SESSION_LEN 256

#define TOKEN_LEN 256
#define MAX_LEN_USERNAME 80
    typedef struct online_user_rec
    {
        char sid[SESSION_LEN];
        guchar username[MAX_LEN_USERNAME];
        guint loginipaddr;
        time_t logintime;
        time_t lasttime;
        guint mode;
        guint ip_type;
        struct in6_addr ipv6_addr;
        char token[TOKEN_LEN];
    } _ONLINE_USER_REC, *_pONLINE_USER_REC;
    int add(int a, int b);
    int login_http(char *username, char *passwd);
    void RPC_Webui_GetUserId_Mode_Name_Ip(char *username,
                                          guint32 namebuflen,
                                          char *userIP,
                                          guint32 ipbuflen,
                                          guint32 *userID,
                                          char *usermode);
    int web_request_end();
    int web_request();
    int filelock();
    int fileunlock();
    int process_mutex(int semkey, int opflag);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
