/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "rpc.h"

bool_t xdr_RPCInput(XDR *xdrs, RPCInput *objp)
{

    if (!xdr_u_int(xdrs, &objp->userid))
        return (FALSE);
    if (!xdr_vector(xdrs, (char *)objp->username, MAX_LEN_USERNAME, sizeof(u_char),
                    (xdrproc_t)xdr_u_char))
        return (FALSE);
    if (!xdr_vector(xdrs, (char *)objp->userIP, MAX_LEN_USERIP, sizeof(u_char),
                    (xdrproc_t)xdr_u_char))
        return (FALSE);
    if (!xdr_vector(xdrs, (char *)objp->usermode, MAX_LEN_USERMODE, sizeof(u_char),
                    (xdrproc_t)xdr_u_char))
        return (FALSE);
    if (!xdr_u_int(xdrs, &objp->fun_index))
        return (FALSE);
    if (!xdr_u_int(xdrs, &objp->out_para_len))
        return (FALSE);
    if (!xdr_array(xdrs, (char **)&objp->input_para.input_para_val,
                   (u_int *)&objp->input_para.input_para_len, ~0, sizeof(u_char),
                   (xdrproc_t)xdr_u_char))
        return (FALSE);
    return (TRUE);
}

bool_t xdr_RPCOutput(XDR *xdrs, RPCOutput *objp)
{
    if (!xdr_array(xdrs, (char **)&objp->output_para.output_para_val,
                   (u_int *)&objp->output_para.output_para_len, ~0, sizeof(u_char),
                   (xdrproc_t)xdr_u_char))
        return (FALSE);
    return (TRUE);
}

bool_t xdr_RPCIDInput(XDR *xdrs, RPCIDInput *objp)
{

    if (!xdr_vector(xdrs, (char *)objp->username, MAX_LEN_USERNAME, sizeof(u_char),
                    (xdrproc_t)xdr_u_char))
        return (FALSE);
    if (!xdr_vector(xdrs, (char *)objp->userIP, MAX_LEN_USERIP, sizeof(u_char),
                    (xdrproc_t)xdr_u_char))
        return (FALSE);
    if (!xdr_vector(xdrs, (char *)objp->password, MAX_LEN_PASSWD, sizeof(u_char),
                    (xdrproc_t)xdr_u_char))
        return (FALSE);
    return (TRUE);
}

bool_t xdr_RPCIDOutput(XDR *xdrs, RPCIDOutput *objp)
{
    if (!xdr_u_int(xdrs, &objp->rev))
        return (FALSE);
    if (!xdr_u_int(xdrs, &objp->id))
        return (FALSE);
    return (TRUE);
}
