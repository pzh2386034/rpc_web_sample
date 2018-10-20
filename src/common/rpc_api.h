#include "glib-2.0/glib.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define RPC_RES_ERR_INDEX_NOT_EXIST 0x40000001
    enum RPC_PRIVILEGE
    {
        RPC_USER_ADMIN  = 0,
        RPC_USER_COMMON = 1,
        RPC_USER_MAX
    };
    enum RPC_RES_OPLOG
    {
        RPC_RES_NO_NEED_OPLOG = 0,
        RPC_RES_NEED_OPLOG
    };

#define GET_SYS_TIME 1

    typedef struct
    {
        time_t in_time;

    } SYSTIME_INP;
    typedef struct
    {
        guint32 funret;
        time_t out_time;
    } SYSTIME_OUT;

    guint32 rpccall_api_get_systime(guchar *username,
                                    guchar *userip,
                                    guchar *usermode,
                                    guchar *input,
                                    size_t inputLen,
                                    guchar *output,
                                    size_t outputlen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
