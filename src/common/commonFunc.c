#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "time.h"
void timelog(const char *format, ...)
{
    char log_buf[1024] = {0};
    va_list args;

    /*lint -save -e530*/
    (void)va_start(args, format);
    /*lint -restore*/
    (void)vsprintf(log_buf, format, args);
    (void)va_end(args);

    struct timeval tv;
    struct tm TmpTime = {0};
    char time_buf[32] = {0};
    FILE *fp          = NULL;
    int time_buf_len;

    (void)gettimeofday(&tv, NULL);
    if (NULL == localtime_r(&(tv.tv_sec), &TmpTime))
    {
        return;
    }
    time_buf_len = sprintf(time_buf, "%4d-%02d-%02d %02d:%02d:%02d %06d ", TmpTime.tm_year + 1900,
                           TmpTime.tm_mon + 1, TmpTime.tm_mday, TmpTime.tm_hour, TmpTime.tm_min,
                           TmpTime.tm_sec, tv.tv_usec);
    if (time_buf_len <= 0)
    {
        return;
    }
    fp = fopen("/tmp/timelog.txt", "a");
    if (NULL == fp)
    {
        return;
    }
    fprintf(fp, "%s%s\n", time_buf, log_buf);
    (void)fclose(fp);
}
