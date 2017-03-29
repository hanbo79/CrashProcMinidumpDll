
#include <stdarg.h>
#include <string>
#include "CrashProcLog.h"

namespace Log
{
    FILE *g_log_logfile = NULL;
    void openlog(char *logfile)
    {
        if(NULL == g_log_logfile)
        {
            g_log_logfile = fopen(logfile,"wb");
        }
    }
    void writeError2(char *msg, ...)
    {
        if(NULL == g_log_logfile)
        { 
            return ;
        }
        va_list arg_ptr;

        va_start(arg_ptr, msg);
        vfprintf(g_log_logfile, msg, arg_ptr);
        fprintf(g_log_logfile,"\r\n");
        va_end(arg_ptr);
        fflush(g_log_logfile);
    }
    void writeError(int m,int s,char *msg, ... )    
    {    
        if(NULL == g_log_logfile)
        {
            return ;
        }
        va_list arg_ptr;

        va_start(arg_ptr, msg);
        vfprintf(g_log_logfile, msg, arg_ptr);
        fprintf(g_log_logfile,"\r\n");
        va_end(arg_ptr);
        fflush(g_log_logfile);
    }  
}
