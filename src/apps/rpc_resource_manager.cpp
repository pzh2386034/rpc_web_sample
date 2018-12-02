#include <string>

#include "rpc_api.h"
#include "rpc_resource_manager.hpp"
rpc_call_smm RpcresGetRpcFunc(guint func_id)
{
    RPC_RESOURCE_ITEM *item = RpcResource::FindItem(func_id);
    if (item == nullptr)
    {
        printf("%s, can not find func_id:%d", __func__, func_id);
        return nullptr;
    }

    return item->GetRpcFunCall();
}
guint8 RpcresGetPrivilege(guint32 func_id)
{
    RPC_RESOURCE_ITEM *item = RpcResource::FindItem(func_id);
    if (item == NULL)
    {
        return RPC_PRIVILLEGE_ERROR;
    }
    return item->GetPrivilege();
}
guint32 RpcresNeedRecordLog(guint32 func_id)
{
    RPC_RESOURCE_ITEM *item = RpcResource::FindItem(func_id);
    if (item == NULL)
    {
        return 0;
    }
    return item->GetRpcNeedTransLog();
}
guint32 RpcresCopyDescryption(guint32 func_id, char *outDescryption, size_t outBuflen)
{
    RPC_RESOURCE_ITEM *item = RpcResource::FindItem(func_id);
    if (item == NULL)
    {
        return RPC_RES_ERR_INDEX_NOT_EXIST;
    }
    const string &des = item->GetRpcFunDescription();
    (void)strcpy(outDescryption, des.c_str());
    return RPC_RES_ERR_OK;
}

const gchar *RpcresGetDescryption(guint32 func_id)
{
    RPC_RESOURCE_ITEM *item = RpcResource::FindItem(func_id);
    if (item == NULL)
    {
        return NULL;
    }

    const string &des = item->GetRpcFunDescription();
    return des.c_str();
}
guint32 RpcresTransLogProcess(PARALIST_LOG_FUNC)
{
    RPC_RESOURCE_ITEM *item = RpcResource::FindItem(func_id);
    if (item == NULL)
    {
        return RPC_RES_ERR_INDEX_NOT_EXIST;
    }

    item->RpcTransLog(user_name, user_ip, usermode, input, inputlen, output, outputlen, func_id);
    return RPC_RES_ERR_OK;
}
