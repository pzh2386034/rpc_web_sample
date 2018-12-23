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
    (void)memset_s(&authin, sizeof(ADMIN_USER_LOGIN_INP), 0, sizeof(ADMIN_USER_LOGIN_INP));
    (void)memset_s(pauthout, sizeof(ADMIN_USER_LOGIN_OUT), 0, sizeof(ADMIN_USER_LOGIN_OUT));
    // strcpy_s(authin.username, sizeof(authin.username), username);
    return 0;
}
