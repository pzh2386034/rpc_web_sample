#include "func_handle.h"
#include "../common/rpc_api.h"
#include "security/pam_appl.h"

/**************************************
 * date:<2018-11-25>
应用服务调用pam_start函数发起pam绘画时，将对会话函数进行注册
在sshd中会话函数为
sshpam_thread_conv;
pam2login_conversation为自定义会话函数
 * author:  pzh2467908@163.com
 **************************************/
int pam2login_conversation(int n,
                           const struct pam_message **msg,
                           struct pam_response **resp,
                           void *data)
{
    struct pam_response *reply = NULL;
    int i;
    AUTH_MSG_ST *authmsg = NULL;
    char *loginmsg       = NULL;
    size_t loginmsglen   = 0;

    *resp = NULL;
    if (NULL == data)
    {
        printf("pam conversation data is NULL.\n");
        return (PAM_CONV_ERR);
    }
    authmsg = (AUTH_MSG_ST *)data;
    if (n <= 0 || n > PAM_MAX_NUM_MSG)
    {
        printf("[%s] pam conversation n is too big(%d).\n", __func__, n);
    }
    if ((reply = (pam_response *)malloc(n * sizeof(*reply))) == NULL)
    {
        return (PAM_CONV_ERR);
    }
    memset_s(reply, n * sizeof(*reply), 0, n * sizeof(struct pam_response));

    for (i = 0; i < n; i++)
    {
        reply[i].resp_retcode = 0;
        reply[i].resp         = NULL;
        switch (msg[i]->msg_style)
        {
            case PAM_PROMPT_ECHO_ON:
                printf("In PAM_PROMPT_ECHO_ON.\n");
                reply[i].resp = strdup(authmsg->username);
                if (NULL == reply[i].resp)
                {
                    goto FAILED;
                }
                break;
            case PAM_PROMPT_ECHO_OFF:
                printf("In PAM_PROMPT_ECHO_OFF.\n");
                reply[i].resp = strdup(authmsg->passwd);
                if (NULL == reply[i].resp)
                {
                    goto FAILED;
                }
                break;
            case PAM_ERROR_MSG:
            case PAM_TEXT_INFO:
                printf("In PAM MESG\n");
                loginmsglen = strlen(msg[i]->msg) + 1;
                loginmsg    = (char *)malloc(loginmsglen);
                if (NULL != loginmsg)
                {
                    (void)memset_s(loginmsg, loginmsglen, 0, loginmsglen);
                    if (loginmsglen > 1)
                    {
                        (void)strcpy(loginmsg, msg[i]->msg);
                    }
                    authmsg->errormsg = loginmsg;
                    authmsg->errorlen = loginmsglen;
                }
                goto FAILED;
            default:
                goto FAILED;
        }
    }
    *resp = reply;

    return PAM_SUCCESS;
FAILED:
    for (i = 0; i < n; i++)
    {
        if (NULL != reply[i].resp)
        {
            (void)memset_s(reply[i].resp, strlen(reply[i].resp), 0, strlen(reply[i].resp));
            MEM_FREE(reply[i].resp);
        }
    }
    (void)memset_s(reply, n * sizeof(struct pam_response), 0, n * sizeof(struct pam_response));
    MEM_FREE(reply);
    *resp = NULL;
    return PAM_CONV_ERR;
}
guint32 admin_login(ADMIN_USER_LOGIN_INP *authdata)
{
    gchar servicename[128] = {0};
    pam_handle_t *pamh     = NULL;
    struct pam_conv conversation;
    gint32 pamret;
    AUTH_MSG_ST authmsg;

    if (NULL == authdata)
    {
        printf("null point\n");
        return RPC_INPUT_PARAMETER_ERR;
    }
    if (WEBUI_AUTH == authdata->authtype)
    {
        (void)strcpy(servicename, "webui");
    }
    else if (CLI_AUTH == authdata->authtype)
    {
        (void)strcpy(servicename, "cli");
    }
    else
    {
        printf("unsupported auth type(%ul)\n", authdata->authtype);
        return RPC_INPUT_PARAMETER_ERR;
    }
    /* conv为一个对话函数，实现应用程序与pam module之间的信息交互 date:<2018-11-24>*/
    conversation.conv = pam2login_conversation;
    (void)memset_s(&authmsg, sizeof(authmsg), 0, sizeof(authmsg));
    (void)strcpy(authmsg.username, authdata->username);
    (void)strcpy(authmsg.passwd, authdata->passwd);
    authmsg.errormsg         = NULL;
    authmsg.errorlen         = 0;
    conversation.appdata_ptr = &(authmsg);

    if (PAM_SUCCESS != pam_start(servicename, authdata->username, &conversation, &pamh))
    {
        goto FAILED;
    }
    if (PAM_SUCCESS != pam_set_item(pamh, PAM_RHOST, authdata->userIP))
    {
        goto FAILED;
    }
    if (PAM_SUCCESS != pam_set_item(pamh, PAM_USER, authdata->username))
    {
        goto FAILED;
    }
    pamret = pam_authenticate(pamh, 0);
    if (PAM_SUCCESS != pamret)
    {
        SetWebLoginFailLog(authdata);
        if (NULL != authmsg.errormsg)
            if (NULL != strstr(authmsg.errormsg, "Account locked"))
            {
                printf("PAM auth failed msg: %s.", authmsg.errormsg);
                (void)memset_s(authmsg.errormsg, authmsg.errorlen, 0, authmsg.errorlen);
                free(authmsg.errormsg);
                (void)memset_s(&authmsg, sizeof(authmsg), 0, sizeof(authmsg));
                (void)pam_end(pamh, 0);
                return ACCOUNT_LOCK_ERR;
            }
        (void)memset_s(&authmsg, sizeof(authmsg), 0, sizeof(authmsg));
        (void)pam_end(pamh, 0);
        return ACCOUNT_AUTH_ERR;
    }
    if (PAM_SUCCESS != pam_acct_mgmt(pamh, 0))
    {
        goto FAILED;
    }
    if (NULL != authmsg.errormsg)
    {
        (void)memset_s(authmsg.errormsg, authmsg.errorlen, 0, authmsg.errorlen);
        free(authmsg.errormsg);
    }
    SetWebLoginSuccessLog(authdata);
    (void)memset_s(&authmsg, sizeof(authmsg), 0, sizeof(authmsg));
    (void)pam_end(pamh, 0);
    return VOS_OK;
FAILED:
    printf("[%s]pam: set item failed\n", __func__);
    if (NULL != authmsg.errormsg)
    {
        (void)memset_s(authmsg.errormsg, authmsg.errorlen, 0, authmsg.errorlen);
        free(authmsg.errormsg);
    }
    SetWebLoginFailLog(authdata);
    (void)memset_s(&authmsg, sizeof(authmsg), 0, sizeof(authmsg));
    (void)pam_end(pamh, 0);
    return ACCOUNT_AUTH_ERR;
}
void SetWebLoginSuccessLog(ADMIN_USER_LOGIN_INP *authdata)
{
    guint32 retcode                = VOS_ERR;
    char ip_str[MAX_IPV6_ADDR_LEN] = {0};
    if (WEBUI_AUTH != authdata->authtype)
    {
        return;
    }
    printf("[%s] login in from web success.\n", __func__);
    return;
}
void SetWebLoginFailLog(ADMIN_USER_LOGIN_INP *authdata)
{
    guint32 retcode                = VOS_ERR;
    char ip_str[MAX_IPV6_ADDR_LEN] = {0};
    if (WEBUI_AUTH != authdata->authtype)
    {
        return;
    }
    printf("[%s] login in from web success.\n", __func__);
    return;
}
