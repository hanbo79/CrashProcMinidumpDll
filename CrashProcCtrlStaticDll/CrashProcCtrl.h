#ifndef CRASHPROCCTRL_H_2006_CHANNELSOFT
#define CRASHPROCCTRL_H_2006_CHANNELSOFT

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include "CCrashReportCenter.h"

#define  CRASH_REPORT_SENDER "BugReport.exe"
/*
 *	崩溃处理控制类
 */
class  CCrashProcCtrl
{
public:
	CCrashProcCtrl();
	enum MY_MINI_DUMP_TYPE
	{
		ONLYSTACK = 0x0000,
		FULLMEMORY = 0x0001
	};
	static void setErrorReportCenter(CCrashReportCenter errCenter);
	static void setErrorReportCenter(const char centerip[16],const unsigned short centerPort);
	//安装崩溃处理
	static void installCrashProcCtrl(MY_MINI_DUMP_TYPE etyp = ONLYSTACK); 
	~CCrashProcCtrl();
public:
	/*
	 *	提供给崩溃处理时的回调用
	 */
	static long __stdcall CrashCallBack(EXCEPTION_POINTERS * pExcPtr);
	
	/*
	 *	获得应用程序名
	 */
	std::string getAppName();
	/*
	*	获取当前进程父路径
	*/
	bool getCurrentDirectory(char path[MAX_PATH]);
	///*
	// *	判断文件是否存在
	// */
	//bool isFileExisting(std::string filename);

	//bool generateZipPackage(std::string zipName,EXCEPTION_POINTERS *pExPtr=NULL);
	bool generateDumpFile(std::string dumpName, EXCEPTION_POINTERS* pExPtr = NULL);

    void savePid(EXCEPTION_POINTERS* pExPtr);
    int readPid(DWORD &pid);
    void saveDmpByCreshProcessId(int pid,int prama);
    static void startCrashProc(int prama);
public:
    /*
	 *	获得ZIP名字
	 */
	std::string getFileName();
    static CCrashProcCtrl * getInst();
private:
	/*
	 *	singlton
	 */
	static CCrashProcCtrl *m_pThis;				//当前对象指针
	//static CCrashProcCtrl * getInst();
	static MY_MINI_DUMP_TYPE m_mini_dump_type;	//dump文件类型
	/*
	 *	错误中心
	 */
	CCrashReportCenter m_errCenter;

#ifdef WIN32
	//抓dump失败后，获取当前进程内存
	bool Get_Memory_Statistics(DWORDLONG &currProcessMemory, DWORDLONG &ullTotalPhys);
#endif

#ifdef WIN32
	double GetCpuUsage() ;
	size_t GetMemoryInfo();   

	
	////在抓dump失败后，获取cpu使用率时所用的结构体
	//typedef struct
	//{
	//	DWORD dwUnknown1;
	//	ULONG uKeMaximumIncrement;
	//	ULONG uPageSize;
	//	ULONG uMmNumberOfPhysicalPages;
	//	ULONG uMmLowestPhysicalPage;
	//	ULONG uMmHighestPhysicalPage;
	//	ULONG uAllocationGranularity;
	//	PVOID pLowestUserAddress;
	//	PVOID pMmHighestUserAddress;
	//	ULONG uKeActiveProcessors;
	//	BYTE bKeNumberProcessors;
	//	BYTE bUnknown2;
	//	WORD wUnknown3;
	//} SYSTEM_BASIC_INFORMATION;

	//typedef struct
	//{
	//	LARGE_INTEGER liIdleTime;
	//	DWORD dwSpare[312];
	//} SYSTEM_PERFORMANCE_INFORMATION;

	//typedef struct
	//{
	//	LARGE_INTEGER liKeBootTime;
	//	LARGE_INTEGER liKeSystemTime;
	//	LARGE_INTEGER liExpTimeZoneBias;
	//	ULONG uCurrentTimeZoneId;
	//	DWORD dwReserved;
	//} SYSTEM_TIME_INFORMATION;
#endif
};

__int64 FileTimeToInt64(const FILETIME& time);
int GetProcessNumber();
#endif