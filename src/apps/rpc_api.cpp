#include "rpc_api.h"
#include "rpc_resource.hpp"
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
