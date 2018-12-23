#ifndef _RPC_CLIENT_H_
#define _RPC_CLIENT_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */
#include "../common/rpc.h"
    CLIENT *smm_rpc_handle = NULL;
    GMutex rpc_mutex;
    guint64 rpc_call_interface(guint funindex,
                               void *pszinput,
                               guint inputlen,
                               void *pszoutput,
                               guint outputlen);
    guint32 all_api_in_one(guint userid,
                           guchar *username,
                           guchar *userIP,
                           guint usermode,
                           guint32 fun_index,
                           guint32 input_para_len,
                           guchar *input_para_val,
                           guint32 output_para_len,
                           guchar *output_para_val);
    typedef void (*RPC_CLIENT_INFO)(guchar *username,
                                    guint username_len,
                                    guchar *userip,
                                    guint ip_len,
                                    guint *userid,
                                    guint *usermode);
    typedef struct login_info_hook
    {
        RPC_CLIENT_INFO Func;
    } RPC_INFO_HOOK;
    guint rpc_create(gchar *hostname);
    extern RPC_INFO_HOOK *gFUNC = NULL;
    void init_rpc_get_info(RPC_CLIENT_INFO fun);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif
