#include "../common/rpc_api.h"
#include "../common/commonFun.h"
#include "func_handle.h"
#include "rpc_resource.hpp"
#include "security/pam_appl.h"

guint32 rpccall_api_get_systime(PARALIST_RPC_CALL)
{

    SYSTIME_INP in;
    SYSTIME_OUT *out;
    out = (SYSTIME_OUT *)output;
    memcpy(&in, input, inputLen);

    printf("%s, i am reached in rpc_api.cpp, time:%ld\n", __func__, in.in_time / 3600);
    out->funret   = 0;
    out->out_time = time(NULL);
    return 0;
}

guint32 rpccall_api_admin_login(PARALIST_RPC_CALL)
{
    ADMIN_USER_LOGIN_INP authin;
    ADMIN_USER_LOGIN_OUT *pauthout = NULL;
    char oplog[512]                = {0};
    printf("begin to check para.\n");
    if (NULL == username)
    {
        printf("username null.\n");
    }
    if (NULL == userip)
    {
        printf("username null.\n");
    }
    if (NULL == input)
    {
        printf("input null.\n");
    }
    if (NULL == output)
    {
        printf("output null.\n");
    }
    ASSERT_RPC_PARAMETER_VALID(ADMIN_USER_LOGIN_INP, ADMIN_USER_LOGIN_OUT, ADMIN_USER_LOGIN);
    (void)memset_s(&authin, sizeof(ADMIN_USER_LOGIN_INP), 0, sizeof(ADMIN_USER_LOGIN_INP));
    (void)memset_s(pauthout, sizeof(ADMIN_USER_LOGIN_OUT), 0, sizeof(ADMIN_USER_LOGIN_OUT));

    memcpy(&authin, input, inputLen);
    guint uret = admin_login(&authin);
    if (authin.authtype == WEBUI_AUTH && uret == ACCOUNT_LOCK_ERR)
    {
        sprintf(oplog, "[%s], forcing the web user(%s) ip(%s) to logout", __func__, username,
                userip);
        guchar data[512] = {0};
        timelog("%s, %s", RpcresGetDescryption(ADMIN_USER_LOGIN), oplog);
    }

    pauthout->funret = uret;
    memcpy(output, pauthout, sizeof(ADMIN_USER_LOGIN_OUT));

    return 0;
}
