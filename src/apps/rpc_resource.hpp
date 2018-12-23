#ifndef __RPC_RESOURCE_HPP__
#define __RPC_RESOURCE_HPP__
#include <iostream>
#include "../common/rpc_api.h"
#include "glib-2.0/glib.h"
#include "map"
#include "string.h"
using namespace std;

/* define fun point for write with different rpc index. date:<2018-10-04>*/
typedef void (*rpc_trans_log)(PARALIST_LOG_FUNC);
typedef struct rpc_tran_log_fun
{
    unsigned int funIndex;
    rpc_trans_log funcall;
    char *descript;
} RPC_TRAN_LOG_FUN;
typedef guint32 (*rpc_call_smm)(PARALIST_RPC_CALL);
/**************************************
 * date:<2018-10-04>
rpc resource class , every rpc index is a class;
 * author:  pzh2467908@163.com
 **************************************/
class RPC_RESOURCE_ITEM
{
  public:
    RPC_RESOURCE_ITEM(void);
    virtual ~RPC_RESOURCE_ITEM(void);

  protected:
    guint32
        mRpcFunIndex; /* every rpc interface maps a rpc index; named mRpcFunInde date:<2018-10-04>*/
    rpc_call_smm mRpcFunCall; /* rpc interface function date:<2018-10-04>*/
    string mFunDes;
    guint8 mPrivilege;  /* this rpc interface need privilege date:<2018-10-04>*/
    guint32 mNeedOpLog; /* if need to write oplog date:<2018-10-04>*/
  public:
    virtual rpc_call_smm GetRpcFunCall(void) = 0;
    guint32 GetRpcNeedTransLog(void) { return mNeedOpLog; }
    const string &GetRpcFunDescription(void);
    virtual RPC_RESOURCE_ITEM *Clone(void) = 0;
    virtual void RpcTransLog(PARALIST_LOG_FUNC) { return; };
    virtual guint32 RpcresUserDomainAuthority(PARALIST_RPC_AUTHORITY) = 0;
    virtual guint8 GetPrivilege(void) { return mPrivilege; }
    virtual guint32 GetRpcIndex(void) { return mRpcFunIndex; }
};
class RpcResource
{
  public:
    static gint32 LinkItem(RPC_RESOURCE_ITEM *item);
    static gint32 AddItem(RPC_RESOURCE_ITEM &item);
    static RPC_RESOURCE_ITEM *FindItem(guint rpc_index);

  protected:
    RpcResource();
    static map<guint, RPC_RESOURCE_ITEM *> smRpcResTab;
    typedef map<guint, RPC_RESOURCE_ITEM *>::iterator iterRpcResTab;
};
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */
    void rpc_resource_init(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif
