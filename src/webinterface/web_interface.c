#include "glib-2.0/glib.h"
#include "stdio.h"
#include "stdlib.h"

int add(int a, int b)
{
    return a + b;
}
int login_http(char *username, char *passwd)
{
    printf("%s, username:%s, passwd:%s", __func__, username, passwd);
    return 0;
}
int admin_user_login(char *username, char *passwd)
{
    char *RPC_INPUT;
}
