
#include "rpc_call_fun.h"
#include "../common/commonFun.h"
#include "../common/errorcode.h"
#include "../common/rpc.h"
#include "../common/rpc_api.h"
#include "rpc_clnt.h"
guint64 rpc_call_interface(guint funindex,
                           void *pszinput,
                           guint inputlen,
                           void *pszoutput,
                           guint outputlen)
{
    guint64 ulret                     = 0;
    guint usermode                    = 0;
    guint uluserid                    = 0;
    guchar username[MAX_LEN_USERNAME] = {0};
    guchar userip[MAX_IPV6_ADDR_LEN]  = {0};
    if (!gFUNC)
    {
        timelog("%s: invalid rpc info register function\n", __func__);
        return INPUT_PARA_ERR;
    }
    g_mutex_lock(&rpc_mutex);
    /* 获取执行命令的用户信息，用于rpc校验 date:<2018-12-09>*/
    gFUNC->Func((guchar *)username, sizeof(username), userip, sizeof(userip), &uluserid, &usermode);
    /* 调用同一rpc接口，传入用户信息 date:<2018-12-09>*/
    ulret = all_api_in_one(uluserid, username, userip, usermode, funindex, inputlen, pszinput,
                           outputlen, pszoutput);
    if (RPC_APP_FAILED == ulret || RPC_NO_CONNECTION == ulret)
    {
        timelog("[%s] rpccall failed.\n", __func__);
        if (smm_rpc_handle != NULL)
        {
            clnt_destroy(smm_rpc_handle);
            smm_rpc_handle = NULL;
        }
        (void)rpc_create(0);
    }
    g_mutex_unlock(&rpc_mutex);
    return ulret;
}
void init_rpc_get_info(RPC_CLIENT_INFO fun)
{
    gFUNC       = (RPC_INFO_HOOK *)malloc(sizeof(RPC_INFO_HOOK));
    gFUNC->Func = fun;
    return;
}
guint32 all_api_in_one(guint userid,
                       guchar *username,
                       guchar *userIP,
                       guint usermode,
                       guint32 fun_index,
                       guint32 input_para_len,
                       guchar *input_para_val,
                       guint32 output_para_len,
                       guchar *output_para_val)
{
    RPCInput input_param;
    RPCOutput *output_param;
    if ((NULL == username) || (NULL == userIP))
    {
        timelog("[%s] invalid param.\n", __func__);
        return INPUT_NULL;
    }
    input_param.userid = userid;
    memset_s(input_param.username, sizeof(input_param.username), 0, sizeof(input_param.username));
    strcpy((gchar *)input_param.username, (gchar *)username);
    memset_s(input_param.userIP, sizeof(input_param.userIP), 0, sizeof(input_param.userIP));
    strcpy((gchar *)input_param.userIP, (gchar *)userIP);
    input_param.usermode                  = usermode;
    input_param.fun_index                 = fun_index;
    input_param.out_para_len              = output_para_len;
    input_param.input_para.input_para_len = input_para_len;
    input_param.input_para.input_para_val = input_para_val;

    if (0 == smm_rpc_handle)
    {
        timelog("[%s] no RPC connection.\n", __func__);
        return RPC_NO_CONNECTION;
    }
    output_param = all_api_in_one_1(&input_param, smm_rpc_handle);
    if (NULL == output_param)
    {
        timelog("[%s] RPC failed: rpc timeout or unknown errors happen", __func__);
        return RPC_FAILED;
    }
    if (output_param->output_para.output_para_val == NULL)
    {
        timelog("[%s] RPC call failed: output parameter len is 0", __func__);
        return RPC_FAILED;
    }
    if ((sizeof(guchar)) == output_param->output_para.output_para_len)
    {
        timelog("[%s] RPC FAILED: call api func failed...\n", __func__);
        clnt_freeres(smm_rpc_handle, (xdrproc_t)xdr_RPCOutput, (char *)output_param);
        return RPC_FAILED;
    }

    clnt_freeres(smm_rpc_handle, (xdrproc_t)xdr_RPCOutput, (char *)output_param);
    return RPC_SUCCESS;
}
guint32 rpc_create(gchar *hostname)
{
    char MN[MAX_LEN_USERIP];
    if (NULL == hostname)
    {
        strcpy(MN, "127.0.0.1");
    }
    else
    {
        strcpy(MN, hostname);
    }
    smm_rpc_handle = clnt_create(MN, RPCAPIPROG, RPCAPIVERS, "tcp");
    if (0 == smm_rpc_handle)
    {
        timelog("[%s] could not contact rpc server\n", __func__);
        return RPC_CREATE_FAILED;
    }
    else
    {
        timelog("[%s] rpc create successfully.\n", __func__);
        return RPC_CREATE_SUCCEED;
    }
}
