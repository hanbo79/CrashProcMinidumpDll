#include "CrashProcCtrl.h"
#include "CrashProcAPI.h"


void installCrashProcCtrl(int prama) 
{
	CCrashProcCtrl::installCrashProcCtrl((CCrashProcCtrl::MY_MINI_DUMP_TYPE)prama);
}

void setErrorReportCenter(const char centerip[16],const unsigned short centerPort)
{
	CCrashProcCtrl::setErrorReportCenter(centerip, centerPort);
}

void startCrashProc(int prama)
{
    CCrashProcCtrl::startCrashProc(prama);
}