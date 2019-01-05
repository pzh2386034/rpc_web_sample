#include "errno.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
int sscanf_s(const char *buffer, const char *format, ...)
{
    int ret = 0;
    va_list arglist;

    va_start(arglist, format);
    ret = vsscanf_s(buffer, format, arglist);
    va_end(arglist);

    return ret;
}
errno_t strcat_s(char *strDest, size_t destMax, const char *strSrc)
{
    char *pHeader                        = strDest;
    size_t availableSize                 = destMax;
    IN_REGISTER const char *overlapGuard = NULL;

    if (destMax == 0 || destMax > SECUREC_STRING_MAX_LEN)
    {
        SECUREC_ERROR_INVALID_RANGE("strcat_s");
        return ERANGE;
    }

    if (strDest == NULL || strSrc == NULL)
    {
        SECUREC_ERROR_INVALID_PARAMTER("strcat_s");
        if (strDest != NULL)
        {
            pHeader[0] = '\0';
            return EINVAL_AND_RESET;
        }
        return EINVAL;
    }

    if (strDest < strSrc)
    {
        overlapGuard = strSrc;
        while (availableSize > 0 && *strDest != 0)
        {
            if (strDest == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("strcat_s");
                return EOVERLAP_AND_RESET;
            }
            /*seek to string end*/
            strDest++;
            availableSize--;
        }

        /* strDest unterminated, return error. */
        if (availableSize == 0)
        {
            pHeader[0] = '\0';
            SECUREC_ERROR_INVALID_PARAMTER("strcat_s");
            return EINVAL_AND_RESET;
        }
        /* if available > 0, then excute the strcat operation */
        while ((*strDest++ = *strSrc++) != 0 && --availableSize > 0)
        {
            if (strDest == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("strcat_s");
                return EOVERLAP_AND_RESET;
            }
        }
    }
    else
    {
        overlapGuard = strDest;
        while (availableSize > 0 && *strDest != '\0')
        {
            /*seek to string end, and no need to check overlap*/
            strDest++;
            availableSize--;
        }

        /* strDest unterminated, return error. */
        if (availableSize == 0)
        {
            pHeader[0] = '\0';
            SECUREC_ERROR_INVALID_PARAMTER("strcat_s");
            return EINVAL_AND_RESET;
        }
        while ((*strDest++ = *strSrc++) != '\0' && --availableSize > 0)
        {
            if (strSrc == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("strcat_s");
                return EOVERLAP_AND_RESET;
            }
        }
    }

    /* strDest have not enough space, return error */
    if (availableSize == 0)
    {
        pHeader[0] = '\0';
        SECUREC_ERROR_INVALID_RANGE("strcat_s");
        return ERANGE_AND_RESET;
    }

    return EOK;
}
