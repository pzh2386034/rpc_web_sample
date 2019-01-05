/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "rpc_clnt.h"
#include "../common/commonFun.h"
#include "../common/errorcode.h"
#include "../common/rpc_api.h"
#include "stdio.h"
#include "unistd.h"

/* Default timeout can be changed using clnt_control() */

static struct timeval TIMEOUT = {25, 0};
RPCOutput *all_api_in_one_1(RPCInput *argp, CLIENT *clnt)
{
    static RPCOutput clnt_res;
    gint32 ret = 0;
    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    ret = clnt_call(clnt, ALL_API_IN_ONE, (xdrproc_t)xdr_RPCInput, argp, (xdrproc_t)xdr_RPCOutput,
                    &clnt_res, TIMEOUT);
    if (ret != RPC_SUCCESS)
    {
        timelog("[%s] clnt_call failed, ret:%d\n", __func__, ret);
        return (NULL);
    }
    return (&clnt_res);
}

RPCIDOutput *rpc_identify_remote_1(RPCIDInput *argp, CLIENT *clnt)
{
    static RPCIDOutput clnt_res;
    gint32 ret = 0;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    ret = clnt_call(clnt, RPC_identify_remote, (xdrproc_t)xdr_RPCIDInput, argp,
                    (xdrproc_t)xdr_RPCIDOutput, &clnt_res, TIMEOUT);
    if (ret != RPC_SUCCESS)
    {
        return (NULL);
    }
    return (&clnt_res);
}
void rpcapiprog_1(char *host)
{
    CLIENT *clnt;
    /* RPCOutput *result_1; */
    /* RPCInput all_api_in_one_1_arg; */
    RPCIDOutput *result_2;
    RPCIDInput rpc_identify_remote_1_arg;
    clnt = clnt_create(host, RPCAPIPROG, RPCAPIVERS, "udp");
    if (clnt == NULL)
    {
        clnt_pcreateerror(host);
        exit(1);
    }
    /* result_1 = all_api_in_one_1(&all_api_in_one_1_arg, clnt); */
    /* if (result_1 == NULL) { */
    /*   clnt_perror(clnt, "call failed:"); */
    /* } */
    result_2 = rpc_identify_remote_1(&rpc_identify_remote_1_arg, clnt);
    if (result_2 == NULL)
    {
        clnt_perror(clnt, "call failed:");
    }
    clnt_destroy(clnt);
}

guint call_remote(gchar *host, guchar *userip, guchar *username, guchar *passwd, int id)
{
    CLIENT *clnt;
    RPCInput input;
    RPCOutput *output;

    /* 获取用户信息 date:<2018-11-02>*/
    char userid[3] = {0};
    getlogin_r(userid, 3);
    printf("userid = %s\n", userid);

    SYSTIME_INP in;
    SYSTIME_OUT out;
    memset(&input, 0, sizeof(RPCInput));
    in.in_time                      = time(NULL) - 1;
    input.input_para.input_para_val = (u_char *)&in;
    input.input_para.input_para_len = sizeof(SYSTIME_INP);
    input.out_para_len              = sizeof(SYSTIME_OUT);
    input.fun_index                 = GET_SYS_TIME;

    clnt = clnt_create(host, RPCAPIPROG, RPCAPIVERS, "udp");
    if (0 == clnt)
    {
        printf("%s:client create failed.\n", __FUNCTION__);
        clnt_pcreateerror(host);
        return FALSE;
    }
    output = all_api_in_one_1(&input, clnt);
    if (output == NULL)
    {
        clnt_perror(clnt, "call fail:");
        return FALSE;
    }
    memcpy(&out, output->output_para.output_para_val, output->output_para.output_para_len);

    printf("%s:client ask ok, ret time:%ld.\n", __FUNCTION__, out.out_time / 3600);
    clnt_destroy(clnt);
    return TRUE;
}
