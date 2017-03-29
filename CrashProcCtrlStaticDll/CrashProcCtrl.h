#ifndef CRASHPROCCTRL_H_2006_CHANNELSOFT
#define CRASHPROCCTRL_H_2006_CHANNELSOFT

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include "CCrashReportCenter.h"

#define  CRASH_REPORT_SENDER "BugReport.exe"
/*
 *	�������������
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
	//��װ��������
	static void installCrashProcCtrl(MY_MINI_DUMP_TYPE etyp = ONLYSTACK); 
	~CCrashProcCtrl();
public:
	/*
	 *	�ṩ����������ʱ�Ļص���
	 */
	static long __stdcall CrashCallBack(EXCEPTION_POINTERS * pExcPtr);
	
	/*
	 *	���Ӧ�ó�����
	 */
	std::string getAppName();
	/*
	*	��ȡ��ǰ���̸�·��
	*/
	bool getCurrentDirectory(char path[MAX_PATH]);
	///*
	// *	�ж��ļ��Ƿ����
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
	 *	���ZIP����
	 */
	std::string getFileName();
    static CCrashProcCtrl * getInst();
private:
	/*
	 *	singlton
	 */
	static CCrashProcCtrl *m_pThis;				//��ǰ����ָ��
	//static CCrashProcCtrl * getInst();
	static MY_MINI_DUMP_TYPE m_mini_dump_type;	//dump�ļ�����
	/*
	 *	��������
	 */
	CCrashReportCenter m_errCenter;

#ifdef WIN32
	//ץdumpʧ�ܺ󣬻�ȡ��ǰ�����ڴ�
	bool Get_Memory_Statistics(DWORDLONG &currProcessMemory, DWORDLONG &ullTotalPhys);
#endif

#ifdef WIN32
	double GetCpuUsage() ;
	size_t GetMemoryInfo();   

	
	////��ץdumpʧ�ܺ󣬻�ȡcpuʹ����ʱ���õĽṹ��
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