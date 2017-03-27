#ifndef CRASH_REPORT_CENTER_H_2008_KYG
#define CRASH_REPORT_CENTER_H_2008_KYG

#include <Windows.h>
#define CRASHCENTERFILE "crashcenter.xml"
//////////////////////////////////////////////////////////////////////////
// 类说明:
//		该类主要用在读取和保存错误服务中心地址和端口到crashcenter.xml配置文件中 
//		
//		edit by youzx 2010.4.10
//
/////////////////////////////////////////////////////////////////////////

struct CCrashReportCenter
{

	CCrashReportCenter(const char ip[16],const unsigned short port);
	CCrashReportCenter();

	/*
	 *	中心的IP和PORT
	 */
	char centerIP[16];
	unsigned short centerPort; 
	bool loadConfig();
	bool saveConfig();
};

bool getCurrentDirectory_t(char path[MAX_PATH]);

#endif
