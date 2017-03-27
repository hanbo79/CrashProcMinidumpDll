#ifndef CRASH_REPORT_CENTER_H_2008_KYG
#define CRASH_REPORT_CENTER_H_2008_KYG

#include <Windows.h>
#define CRASHCENTERFILE "crashcenter.xml"
//////////////////////////////////////////////////////////////////////////
// ��˵��:
//		������Ҫ���ڶ�ȡ�ͱ������������ĵ�ַ�Ͷ˿ڵ�crashcenter.xml�����ļ��� 
//		
//		edit by youzx 2010.4.10
//
/////////////////////////////////////////////////////////////////////////

struct CCrashReportCenter
{

	CCrashReportCenter(const char ip[16],const unsigned short port);
	CCrashReportCenter();

	/*
	 *	���ĵ�IP��PORT
	 */
	char centerIP[16];
	unsigned short centerPort; 
	bool loadConfig();
	bool saveConfig();
};

bool getCurrentDirectory_t(char path[MAX_PATH]);

#endif
