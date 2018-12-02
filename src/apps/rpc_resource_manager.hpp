
#ifndef __RPC_RESOURCE_MANAGER_HEADER__
#define __RPC_RESOURCE_MANAGER_HEADER__

#include "rpc_resource.hpp"
/* #include "rpc_index.h" */
/* #include "rpc_api.h" */
#include "glib-2.0/glib.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define RPC_RES_ERR_OK 0
#define RPC_RES_ERR_GENERIC -1
#define RPC_RES_ERR_INDEX_NOT_EXIST2 -2
#define RPC_PRIVILLEGE_ERROR 0xFF

    gint32 RpcresTransLogProcess(PARALIST_RPC_CALL);

    guint32 RpcresUserDomainAuthority(PARALIST_RPC_AUTHORITY);

    guint32 RpcresCopyDescryption(guint func_id, char *outDescryption, size_t outBuflen);

    const gchar *RpcresGetDescryption(guint func_id);

    rpc_call_smm RpcresGetRpcFunc(guint func_id);

    guint8 RpcresGetPrivilege(guint func_id);

    guint32 RpcresNeedRecordLog(guint func_id);

    void RpcresInit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  //__RPC_RESOURCE_MANAGER_HEADER__
