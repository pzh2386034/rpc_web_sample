#include "rpc_resource.hpp"
#include "../common/rpc_api.h"

enum op_log
{
    NO_OP_LOG = 0,
    NEED_OP_LOG
};

#define TRANS_OP_LOG(username, userip, usermode, location, data, funreturn)   \
    {                                                                         \
        if (TRUE == funreturn)                                                \
        {                                                                     \
            (void)operatelog(username, userip, usermode, location, data)      \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            (void)operate_Errlog(username, userip, usermode, location, data); \
        }                                                                     \
    }
#define CALL_PRT_OP_LOG(log_buf, location, funreturn)                                            \
    {                                                                                            \
        TRANS_OP_LOG((gchar *)user_name, (gchar *)user_ip, (gchar *)usermode, location, log_buf, \
                     funreturn);                                                                 \
    }
#define CP_RESULT(funreturn, res_buffer, buffer_len)                           \
    {                                                                          \
        if (TRUE == funreturn)                                                 \
        {                                                                      \
            (void)sprintf(res_buffer, buffer_len, "%s", "succeed");            \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            (void)sprintf(res_buffer, buffer_len, "failed(0x%lx)", funreturn); \
        }                                                                      \
    }

#define CP_RETURN_RESULT(cp_output_type, cp_output, cp_member, cp_buffer)                  \
    {                                                                                      \
        CP_RESULT(((cp_output_type *)cp_output)->cp_member, cp_buffer, sizeof(cp_buffer)); \
    }
#define PARA_CHECK()                                                                             \
    {                                                                                            \
        if ((NULL == user_name) || (NULL == user_ip) || (NULL == usermode) || (NULL == input) || \
            (NULL == output))                                                                    \
        {                                                                                        \
            return;                                                                              \
        }                                                                                        \
    }

/**************************************
 * date:<2018-10-05>
rpc_resource_item 类成员函数实现, 每一个rpc接口都时一个单独的类，继承自rpc_resource_item
 * author:  pzh2467908@163.com
 **************************************/
RPC_RESOURCE_ITEM::RPC_RESOURCE_ITEM()
{
    mRpcFunIndex = 0xFFFFFFFF;
    mRpcFunCall  = NULL;
    mPrivilege   = RPC_USER_ADMIN;
    mNeedOpLog   = 1;
}

RPC_RESOURCE_ITEM::~RPC_RESOURCE_ITEM(void) {}

const string &RPC_RESOURCE_ITEM::GetRpcFunDescription(void)
{
    return mFunDes;
}
#define DECLEAR_NEW_RPC_RES_ITEM_GENERIC(item_name)                 \
    class item_name : public RPC_RESOURCE_ITEM                      \
    {                                                               \
      public:                                                       \
        item_name();                                                \
        rpc_call_smm GetRpcFunCall(void);                           \
        void RpcTransLog(PARALIST_LOG_FUNC);                        \
        guint32 RpcresUserDomainAuthority(PARALIST_RPC_AUTHORITY);  \
        RPC_RESOURCE_ITEM *Clone(void) { return new item_name(); }; \
    };

#define DECLEAR_NEW_RPC_RES_ITEM(item_name, rpc_id, rpc_description, privilege, need_oplog) \
    DECLEAR_NEW_RPC_RES_ITEM_GENERIC(item_name) item_name::item_name()                      \
    {                                                                                       \
        mRpcFunIndex = rpc_id;                                                              \
        mFunDes      = rpc_description;                                                     \
        mPrivilege   = privilege;                                                           \
        mNeedOpLog   = need_oplog;                                                          \
    };

#define DECLEAR_NEW_RPC_RES_ITEM_NO_TRANSLOG(item_name, rpc_id, rpc_description, privilege)        \
    DECLEAR_NEW_RPC_RES_ITEM(item_name, rpc_id, rpc_description, privilege, RPC_RES_NO_NEED_OPLOG) \
    void item_name::RpcTransLog(RPC_TRANSLOG_ARGS) { return; };

#define DECLEAR_NEW_RPC_RES_ITEM_CONSTRUCT(item_name, rpc_id, rpc_description, privilege, \
                                           need_oplog, rpc_func)                          \
    DECLEAR_NEW_RPC_RES_ITEM(item_name, rpc_id, rpc_description, privilege, need_oplog)   \
    rpc_call_smm item_name::GetRpcFuncall(void) { return rpc_func; };
/**************************************                                  \
 * date:<2018-10-05>                                                     \
rpc_resource资源初始化，成员函数实现                         \
 * author:  pzh2467908@163.com                                           \
 **************************************/

map<guint, RPC_RESOURCE_ITEM *> RpcResource::smRpcResTab;
gint32 RpcResource::LinkItem(RPC_RESOURCE_ITEM *item)
{
    if (item == NULL)
    {
        return -1;
    }
    if (smRpcResTab.find(item->GetRpcIndex()) != smRpcResTab.end())
    {
        return -1;
    }
    smRpcResTab[item->GetRpcIndex()] = item;
    return 0;
}
gint32 RpcResource::AddItem(RPC_RESOURCE_ITEM &item)
{
    RPC_RESOURCE_ITEM *insClone = item.Clone();
    if (insClone == NULL)
    {
        return -1;
    }
    gint32 ret = LinkItem(insClone);
    if (ret != 0)
    {
        MEM_FREE(insClone);
    }
    return ret;
}
RPC_RESOURCE_ITEM *RpcResource::FindItem(guint rpc_index)
{
    iterRpcResTab iter = smRpcResTab.find(rpc_index);
    if (iter == smRpcResTab.end())
    {
        return NULL;
    }
    return iter->second;
}

/**************************************
 * date:<2018-10-05>
rpc resources
 * author:  pzh2467908@163.com
 **************************************/
DECLEAR_NEW_RPC_RES_ITEM(RPC_GET_SYS_TIME,
                         GET_SYS_TIME,
                         "get systime",
                         RPC_USER_COMMON,
                         RPC_RES_NO_NEED_OPLOG);

rpc_call_smm RPC_GET_SYS_TIME::GetRpcFunCall(void)
{
    return rpccall_api_get_systime;
}
void RPC_GET_SYS_TIME::RpcTransLog(PARALIST_LOG_FUNC)
{
    printf("%s, i am in translog, %s, %s, %d, %d, %d, %ud", __func__, user_name, user_ip, usermode,
           (int)inputlen, (int)outputlen, func_id);
    return;
}
guint32 RPC_GET_SYS_TIME::RpcresUserDomainAuthority(PARALIST_RPC_AUTHORITY)
{
    printf("%s, i am in UserDomainAuthority, %s, %s, %d, %d, %d, %ud", __func__, user_name, user_ip,
           usermode, (int)inputlen, (int)outputlen, func_id);
    return TRUE;
}

/**************************************
 * date:<2018-11-22>
add rpc resource
admin login
 * author:  pzh2467908@163.com
 **************************************/
DECLEAR_NEW_RPC_RES_ITEM(RPC_ADMIN_USER_LOGIN,
                         ADMIN_USER_LOGIN,
                         "admin login authentication",
                         RPC_USER_COMMON,
                         RPC_RES_NEED_OPLOG);
rpc_call_smm RPC_ADMIN_USER_LOGIN::GetRpcFunCall(void)
{
    return rpccall_api_admin_login;
}
void RPC_ADMIN_USER_LOGIN::RpcTransLog(PARALIST_LOG_FUNC)
{
    return;
}
guint32 RPC_ADMIN_USER_LOGIN::RpcresUserDomainAuthority(PARALIST_RPC_AUTHORITY)
{
    return 0;
}
void rpc_resource_init()
{
    static RPC_GET_SYS_TIME rpc_get_sys_time_ins;
    RpcResource::AddItem(rpc_get_sys_time_ins);
    static RPC_ADMIN_USER_LOGIN rpc_admin_user_login;
    RpcResource::AddItem(rpc_admin_user_login);
}
