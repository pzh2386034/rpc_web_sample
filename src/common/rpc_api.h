#ifndef RPC_API_H
#define RPC_API_H
#include "glib-2.0/glib.h"
#include "rpc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define PARALIST_LOG_FUNC                                                                      \
    const guchar *user_name, const guchar *user_ip, const guint usermode, const guchar *input, \
        size_t inputlen, const guchar *output, size_t outputlen, guint32 func_id

#define PARALIST_RPC_CALL                                                             \
    guchar *username, guchar *userip, guint usermode, guchar *input, size_t inputLen, \
        guchar *output, size_t outputlen

#define PARALIST_RPC_AUTHORITY                                                                 \
    const guchar *user_name, const guchar *user_ip, const guint usermode, const guchar *input, \
        size_t inputlen, const guchar *output, size_t outputlen, guint32 func_id

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
#define RPC_RES_ERR_INDEX_NOT_EXIST 0x40000001
#define RPC_PRIVILEGE_ERROR 0X40000002
#define RPC_INPUT_PARAMETER_ERR 0X40000003
#define MAX_IPV6_ADDR_LEN 128
#define TOKEN_LEN 256
    enum RPC_PRIVILEGE
    {
        RPC_USER_ADMIN  = 0,
        RPC_USER_COMMON = 1,
        RPC_USER_MAX
    };
    enum RPC_RES_OPLOG
    {
        RPC_RES_NO_NEED_OPLOG = 0,
        RPC_RES_NEED_OPLOG
    };

#define GET_SYS_TIME 1
#define ADMIN_USER_LOGIN 2

    typedef struct
    {
        time_t in_time;

    } SYSTIME_INP;
    typedef struct
    {
        guint32 funret;
        time_t out_time;
    } SYSTIME_OUT;
    /* admin login date:<2018-11-21>*/

    enum RPC_LOGIN_TYPE
    {
        WEBUI_AUTH = 0,
        CLI_AUTH   = 1
    };
    typedef struct
    {
        char username[MAX_LEN_USERNAME];
        char passwd[MAX_LEN_PASSWD];
        char userIP[MAX_IPV6_ADDR_LEN];
        guint32 login;
        enum RPC_LOGIN_TYPE authtype;
    } ADMIN_USER_LOGIN_INP;
    typedef struct
    {
        int userid;
    } ADMIN_USER_LOGIN_OUT;
#define ACCOUNT_AUTH_ERR 0x52000001
#define ACCOUNT_LOCK_ERR 0x52000002
    /* end admin login date:<2018-11-22>*/

    guint32 rpccall_api_get_systime(PARALIST_RPC_CALL);

    guint32 rpccall_api_admin_login(PARALIST_RPC_CALL);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif
