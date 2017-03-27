#ifndef CRASHPROC_API_H_20170326
#define CRASHPROC_API_H_20170326


namespace Log
{
    void openlog(char *logfile);
    void writeError(int m,int s,char *msg, ...);
     void writeError2(char *msg, ...);
}

#endif