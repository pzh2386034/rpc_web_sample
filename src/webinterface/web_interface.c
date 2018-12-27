#include "web_interface.h"
#include "../common/commonFun.h"
#include "../common/errorcode.h"
#include "../common/rpc_api.h"
#include "../smp/rpc_call_fun.h"
#include "glib-2.0/glib.h"
#include "rpc/rpc.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "unistd.h"
/* 初始化rpc句柄，在web模块被加载时创建，在退出时销毁 date:<2018-12-09>*/
extern CLIENT *smm_rpc_handle;
/* 引入rpc_call_fun中的回掉函数，通过该函数获取登入用户信息. date:<2018-12-09>*/
extern RPC_INFO_HOOK *gFUNC;
int add(int a, int b)
{
    return a + b;
}
int login_http(char *username, char *passwd)
{
    printf("%s, username:%s, passwd:%s", __func__, username, passwd);
    return 0;
}
guint get_current_user_info(_pONLINE_USER_REC puser_info)
{

    char session_id[SESSION_LEN] = {0};
    char *pcsid                  = NULL;
    guint i                      = 0;
    puser_info->mode             = WEBUI_AUTH;

    return 0;
}
void rpc_webui_get_info(guchar *username,
                        guint username_len,
                        guchar *userip,
                        guint ip_len,
                        guint *userid,
                        guint *usermode)
{
    _ONLINE_USER_REC user_info;
    guchar ip[MAX_IPV6_ADDR_LEN];
    guint uret = 0;
    timelog("[%s] call rpc_webui_get_info.\n", __func__);
    return;
    if (NULL == userid || NULL == username || NULL == userip)
    {
        printf("[%s] null point.\n", __func__);
        return;
    }
    (void)memset_s(&user_info, sizeof(_ONLINE_USER_REC), 0, sizeof(_ONLINE_USER_REC));
    if (SUCCESS != get_current_user_info(&user_info))
    {
        printf("[%s] get current user info failed.\n", __func__);
        return;
    }
    *usermode = user_info.mode;
    memcpy(username, user_info.username, sizeof(user_info.username));
    /* if (SUCCESS != get_rpc_userID(userid)) */
    /* { */
    /*     printf("[%s] get current user id failed.\n", __func__); */
    /*     return; */
    /* } */
    /* uret = web_conver_ip_to_str(user_info.ipv6_addr, ip, sizeof(ip)); */
    /* if (uret != SUCCESS) */
    /* { */
    /*     printf("[%s] convert ip to str failed.\n", __func__); */
    /*     return; */
    /* } */
    memcpy(userip, ip, sizeof(ip));
    return;
}

int admin_user_login(char *username, const char *passwd, const char *userip)
{
    ADMIN_USER_LOGIN_INP input;
    ADMIN_USER_LOGIN_OUT output;
    guint ulret = 0;
    if (NULL == username || NULL == passwd || NULL == userip)
    {
        timelog("%s: null point.\n", __func__);
        return INPUT_NULL;
    }
    if (strlen(username) + 1 >= MAX_LEN_USERNAME || strlen(passwd) + 1 >= MAX_LEN_PASSWD ||
        strlen(userip) + 1 >= MAX_LEN_PASSWD)
    {
        timelog("%s: parameters is too long.\n", __func__);
        return INPUT_PARA_ERR;
    }
    memset_s(&input, sizeof(ADMIN_USER_LOGIN_INP), 0, sizeof(ADMIN_USER_LOGIN_INP));
    memset_s(&output, sizeof(ADMIN_USER_LOGIN_OUT), 0, sizeof(ADMIN_USER_LOGIN_OUT));
    strncpy(input.username, username, sizeof(input.username) - 1);
    strncpy(input.userIP, userip, sizeof(input.userIP) - 1);
    strncpy(input.passwd, passwd, sizeof(input.passwd) - 1);
    input.authtype = WEBUI_AUTH;
    input.login    = 1;
    ulret = rpc_call_interface(ADMIN_USER_LOGIN, (guchar *)&input, sizeof(input), (guchar *)&output,
                               sizeof(output));
    if (ulret != SUCCESS)
    {
        timelog("[%s] LOGIN FAILED.", __func__);
        return VOS_ERR;
    }
    memset_s(&output, sizeof(output), 0, sizeof(output));

    return SUCCESS;
}
int web_request_end(void)
{
    if (smm_rpc_handle != NULL)
    {
        clnt_destroy(smm_rpc_handle);
        smm_rpc_handle = NULL;
    }
    return SUCCESS;
}
int web_request(void)
{
    int ret         = 0;
    struct stat buf = {0};
    int debugfilefd = -1;
    /* 需要加信号量互斥 date:<2018-12-09>*/
    /* if (errorlogfd >= 0) */
    /* { */
    /*     if ((0 == fstat(errorlogfd, &buf)) && (buf.st_size > (APACHE_ERROR_LOG_LIMIT << 20))) */
    /*     { */
    /*         BackupLogfile(errorlogfd, WEBLOGFILE_TYPE_ERRORLOG); */
    /*         (void)ftruncate(errorlogfd, 0); */
    /*         (void)lseek(errorlogfd, 0, SEEK_SET); */
    /*     } */
    /* } */

    /* if (accesslogfd >= 0) */
    /* { */
    /*     if ((0 == fstat(accesslogfd, &buf)) && (buf.st_size > (APACHE_ACCESS_LOG_LIMIT << 20)))
     */
    /*     { */
    /*         /\* Modify by p00143978, 2016/2/26   ÎÊÌâµ¥ºÅ:DTS2016022508579_ApacheÈÕÖ¾Õû¸Ä*\/ */
    /*         BackupLogfile(accesslogfd, WEBLOGFILE_TYPE_ACCESSLOG); */
    /*         (void)ftruncate(accesslogfd, 0); */
    /*         (void)lseek(accesslogfd, 0, SEEK_SET); */
    /*     } */
    /* } */

    /* (void)fileunlock(); */

    /* debugfilefd = fileno(DebugFile); */
    /* if (debugfilefd >= 0) */
    /* { */
    /*     if ((0 == fstat(debugfilefd, &buf)) && (buf.st_size > (APACHE_WEBUI_LOG_LIMIT << 20))) */
    /*     { */
    /*         (void)ftruncate(debugfilefd, 0); */
    /*         (void)lseek(debugfilefd, 0, SEEK_SET); */
    /*     } */
    /* } */
    /* /\* END  : Modify by zhuxiongxiong, 2014/7/18 PN: Security Assessment *\/ */

    /* checkresult = requesthandler(); */
    /* if (METHOD_CHECK_FAIL == checkresult || URL_CHECK_FAIL == checkresult) */
    /* { */
    /*     return VOS_ERR; */
    /* } */

    /* /\* BEGIN: Modify by zhuxiongxiong, 2013/4/22   PN:R2C02 Webui*\/ */
    /* syncwebuser(); /\* (void)filelock(); *\/ */
    timelog("[%s] begin to create rpc handle.", __func__);
    if (NULL == smm_rpc_handle)
    {
        ret = rpc_create(0);
        if (0 != ret)
        {
            printf("[%s] failed to create rpc client.\n", __func__);
        }
        else
        {
            timelog("[%s] create rpc client successfully.", __func__);
        }
    }
    /* 初始化rpc句柄 date:<2018-12-18>*/
    init_rpc_get_info(rpc_webui_get_info);

    /* register_rpc_client_info_hook(RPC_Webui_GetUserId_Mode_Name_Ip); */
    return ret;
}
