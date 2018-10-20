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
