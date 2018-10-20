#include "../common/rpc.h"

RPCOutput *all_api_in_one_1(RPCInput *arg, CLIENT *clnt);
guint call_remote(gchar *host, guchar *userip, guchar *username, guchar *passwd, int id);
void rpcapiprog_1(char *host);
RPCIDOutput *rpc_identify_remote_1(RPCIDInput *argp, CLIENT *clnt);
