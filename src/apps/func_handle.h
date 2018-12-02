#ifndef FUNC_HANDLE_H
#define FUNC_HANDLE_H
#include "../common/rpc_api.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */
#define VOS_ERR 1
#define VOS_OK 0
    typedef struct auth_msg_st
    {
        char username[MAX_LEN_USERNAME];
        char passwd[MAX_LEN_PASSWD];
        char *errormsg;
        guint32 errorlen;
    } AUTH_MSG_ST;
    int pam2login_conversation(int n,
                               const struct pam_message **msg,
                               struct pam_response **resp,
                               void *data);

    guint32 admin_login(ADMIN_USER_LOGIN_INP *authdata);
    void SetWebLoginFailLog(ADMIN_USER_LOGIN_INP *authdata);
    void SetWebLoginSuccessLog(ADMIN_USER_LOGIN_INP *authdata);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
