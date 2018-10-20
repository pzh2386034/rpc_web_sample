struct RPCInput
{
unsigned int userid;
unsigned char username[MAX_LEN_USERNAME];
unsigned char userIP[MAX_LEN_USERIP];
unsigned char usermode[MAX_LEN_USERMODE];
unsigned int fun_index;
unsigned int out_para_len;
unsigned char input_para<>;
};
struct RPCOutput
{
unsigned char output_para<>;
};
struct RPCIDInput
{
unsigned char username[MAX_LEN_USERNAME];
unsigned char userIP[MAX_LEN_USERIP];
unsigned char password[MAX_LEN_PASSWD];
};
struct RPCIDOutput
{
unsigned int rev;
unsigned int id;
};
program RPCAPIPROG{
version RPCAPIVERS{
RPCOutput ALL_API_IN_ONE(RPCInput) = 1;
RPCIDOutput RPC_identify_remote(RPCIDInput) = 2;
} = 1;

} = 0x30090987;
