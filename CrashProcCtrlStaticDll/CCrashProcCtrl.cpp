#include <Windows.h>
#include "CrashProcCtrl.h"
#include <DbgHelp.h>
#include <iostream>
#include <string>
#include <Psapi.h>
//#include "QNLog\Log_Remote\LogPub.h"
#include "CrashProcLog.h"

using namespace std;

CCrashProcCtrl *CCrashProcCtrl::m_pThis = NULL;
CCrashProcCtrl::MY_MINI_DUMP_TYPE CCrashProcCtrl::m_mini_dump_type = CCrashProcCtrl::ONLYSTACK;
CCrashProcCtrl::CCrashProcCtrl()
{
	//��ʼ����������
	memset(m_errCenter.centerIP, 0, 16);
	m_errCenter.centerPort = 0;
}
CCrashProcCtrl::~CCrashProcCtrl()
{}

void CCrashProcCtrl::setErrorReportCenter(CCrashReportCenter errCenter)
{
	CCrashProcCtrl *pThis = getInst();
	if (pThis)
	{
		pThis->m_errCenter = errCenter;
		pThis->m_errCenter.saveConfig();
	}

}
void CCrashProcCtrl::setErrorReportCenter(const char centerip[16],const unsigned short centerPort)
{
	setErrorReportCenter(CCrashReportCenter(centerip,centerPort));
}

void myInvalidParameterHandler1(const wchar_t* expression,  
    const wchar_t* function,   
    const wchar_t* file,   
    unsigned int line,   
    uintptr_t pReserved)  
{  

    printf("Invalid parameter detected in function 2");
    string dumpFileName;
    string logFileName;

    CCrashProcCtrl* pThis = CCrashProcCtrl::getInst();

    logFileName = pThis->getFileName();
    logFileName += ".log";
    Log::openlog((char*)logFileName.c_str());

    Log::writeError2("InvalidParameterHandler ");
    //abort();
    //����dmp�ļ�,���浽����
   // dumpFileName = pThis->getFileName();
   // dumpFileName += ".dmp";
   // pThis->generateDumpFile(dumpFileName, NULL);

    throw 1;

}
#include <signal.h>
void SignalHandler(int signal)
{
    printf("SignalHandler :%d",signal);  
    
    CCrashProcCtrl* pThis = CCrashProcCtrl::getInst();

    string logFileName;
    logFileName = pThis->getFileName();
    logFileName += ".log";
    Log::openlog((char*)logFileName.c_str());

    char buf[128];
    sprintf(buf,"SignalHandler:%d",signal);
    Log::writeError2(buf);

    //����dmp�ļ�,���浽����
//      dumpFileName = pThis->getFileName();
//      dumpFileName += "2.dmp";
//      pThis->generateDumpFile(dumpFileName, NULL);

    throw 2;
}

void myPurecallHandler(void)  
{  
    printf("myPurecallHandler ");  
    string dumpFileName;
    string logFileName;

    CCrashProcCtrl* pThis = CCrashProcCtrl::getInst();

    logFileName = pThis->getFileName();
    logFileName += ".log";
    Log::openlog((char*)logFileName.c_str());

    char buf[128];
    sprintf(buf,"PurecallHandler ");
    Log::writeError2(buf);

    //����dmp�ļ�,���浽����
    //dumpFileName = pThis->getFileName();
    //dumpFileName += "Purecall.dmp";
    //pThis->generateDumpFile(dumpFileName, NULL);  

    throw 3;
}  
void CCrashProcCtrl::installCrashProcCtrl(MY_MINI_DUMP_TYPE etyp)
{
	if(SetUnhandledExceptionFilter(CrashCallBack) == NULL)
 	{
 		cout << "there is no current top-level exception handler" << endl;
 	}

    _set_invalid_parameter_handler(myInvalidParameterHandler1);  
     // Disable the message box for assertions.  
     _CrtSetReportMode(_CRT_ASSERT, 0);  

     /*�����Ч����*/
     //char* formatString;  
     //formatString = NULL;  
     //printf(formatString);


      typedef void (*SignalHandlerPointer)(int);
      SignalHandlerPointer previousHandler;
      signal(SIGABRT, SignalHandler);
      signal(SIGSEGV, SignalHandler);
     
     _set_purecall_handler(myPurecallHandler);  

#ifdef _DEBUG
	_set_error_mode(_OUT_TO_MSGBOX);
#else
	_set_error_mode(_OUT_TO_STDERR);
#endif
	m_mini_dump_type = etyp;
}

long __stdcall CCrashProcCtrl::CrashCallBack(EXCEPTION_POINTERS * pExcPtr)
{
	string dumpFileName;
    string logFileName;

	CCrashProcCtrl* pThis = getInst();
	
	dumpFileName = pThis->getFileName();
    logFileName = dumpFileName;
	dumpFileName += ".dmp";
	
    logFileName += ".log";
    Log::openlog((char*)logFileName.c_str());

     Log::writeError(1<<10,1024,"SetUnhandledExceptionFilter\n");

	//����dmp�ļ�,���浽����
	pThis->generateDumpFile(dumpFileName, pExcPtr);

	//����dmp�ļ�
	char cmd[520] = {0} ;
	
	pThis->getCurrentDirectory(cmd);
	strcat(cmd,"\\");
	
	char commandParam[MAX_PATH] = {0};
	sprintf(commandParam,"%s \"%s\" %s:%d",CRASH_REPORT_SENDER, dumpFileName.c_str(), pThis->m_errCenter.centerIP,pThis->m_errCenter.centerPort);
	strcat(cmd, commandParam);

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	if ((strlen(pThis->m_errCenter.centerIP) != 0) && (pThis->m_errCenter.centerPort != 0))
	{
		BOOL bret = CreateProcess(NULL,cmd,NULL,NULL,FALSE,CREATE_NO_WINDOW,0,0,&si,&pi);
		if(bret != 0)
		{
			//WaitForSingleObject(pi.hProcess, INFINITE);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
			//cout << "���̣�\t" << cmd << endl;
			//cout << "IP:\t" << pThis->m_errCenter.centerIP << endl;
			//cout << "Port:\t" << pThis->m_errCenter.centerPort << endl;
			
			return 0;
		}
		else{
			//д������־����������dmp�ļ�����ʧ��
			size_t curr_memory = pThis->GetMemoryInfo();
			double curr_cpu = pThis->GetCpuUsage();
			Log::writeError(1<<10,1024,"������־���ͽ���ʧ�ܣ���ǰ����ʹ���ڴ棺%I64u,��ǰ����cpu��%f",curr_memory, curr_cpu);
			return 0;
		}
	}
	//else
	//{
	//	//д������־:�����IP��Port���ô���
	//	ULONG_PTR curr_memory = pThis->GetMemoryInfo();
	//	double curr_cpu = pThis->GetCpuUsage();

	//	cout << "************************" << endl;
	//	Log::writeError(1<<10,1024,"%s��ǰ�����ڴ棺%lu, ��ǰ����cpu��%f","��������IP��PORT���ô���.",curr_memory, curr_cpu);
	//	return 0;
	//}
    return 0;
}
bool CCrashProcCtrl::generateDumpFile(std::string dumpName, EXCEPTION_POINTERS* pExPtr)
{
    Log::writeError(1<<10,1024,"����dmp����ʼ\n");
	//����dump�ļ�
	HANDLE hDumpFile = CreateFile(  
		dumpName.c_str(),  
		GENERIC_READ | GENERIC_WRITE,  
		FILE_SHARE_READ | FILE_SHARE_WRITE,  
		NULL,  
		CREATE_ALWAYS,  
		FILE_ATTRIBUTE_NORMAL,  
		NULL  
		);
	if (hDumpFile == INVALID_HANDLE_VALUE)
	{
		int retval = GetLastError();
		//д������־

		CCrashProcCtrl* pThis = getInst();
		size_t curr_memory = pThis->GetMemoryInfo();
		double curr_cpu = pThis->GetCpuUsage();
		Log::writeError(1<<10,1024,",����dump�ļ�ʧ�ܣ�LastErrorCode��%u, ��ǰ����ʹ���ڴ棺%I64u,��ǰ����cpu��%f",retval,curr_memory, curr_cpu);

		return false;
	}
	//дdump�ļ�
	MINIDUMP_EXCEPTION_INFORMATION  mdei;
	mdei.ClientPointers = FALSE;
	mdei.ExceptionPointers = pExPtr;
	mdei.ThreadId = GetCurrentThreadId();

    MINIDUMP_EXCEPTION_INFORMATION *pmdei = &mdei;
    if(NULL == pExPtr)
    {
        pmdei = NULL;
    }

	int minidump_type;
	if (m_mini_dump_type == ONLYSTACK)
	{
		minidump_type = MiniDumpNormal|MiniDumpWithThreadInfo|MiniDumpWithHandleData|MiniDumpWithIndirectlyReferencedMemory;
		//minidump_type = MiniDumpNormal;
	}
	else
		minidump_type = MiniDumpNormal|MiniDumpWithThreadInfo|MiniDumpWithHandleData|MiniDumpWithIndirectlyReferencedMemory|MiniDumpWithFullMemory;

    bool bret = MiniDumpWriteDump(
		OpenProcess(READ_CONTROL,false,5972),
		5972,
		hDumpFile,
		(MINIDUMP_TYPE)minidump_type,
		pmdei,
		NULL,
		NULL);
	if (bret != true)
	{
		int retval = GetLastError();
		//д������־
		CCrashProcCtrl* pThis = getInst();
		size_t curr_memory = pThis->GetMemoryInfo();
		double curr_cpu = pThis->GetCpuUsage();
		Log::writeError(1<<10,1024,"дdump�ļ�ʧ��,LastErrorCode��%u, ��ǰ����ʹ���ڴ棺%I64u,��ǰ����cpu��%f",retval,curr_memory, curr_cpu);

        savePid(pExPtr);

	}
	CloseHandle(hDumpFile);

     Log::writeError(1<<10,1024,"����dmp������\n");
	return true;
}

std::string CCrashProcCtrl::getFileName()
{
	SYSTEMTIME nowtime;
	char filename[MAX_PATH] = {0};
	char path[MAX_PATH] = {0};
	//getCurrentDirectory_t(path);
	CCrashProcCtrl* pTemp = getInst();
	pTemp->getCurrentDirectory(path);

	::GetLocalTime(&nowtime);
	_snprintf( filename,MAX_PATH-1,"%s\\%s_%d%d%d(%d_%d_%d)",path,getAppName().c_str(), (short)nowtime.wYear,(short)nowtime.wMonth,(short)nowtime.wDay,
		(short)nowtime.wHour,(short)nowtime.wMinute,(short)nowtime.wSecond);

	return filename;
}

string CCrashProcCtrl::getAppName()
{
	char szFileName[_MAX_PATH];
	GetModuleFileName(NULL, szFileName, _MAX_FNAME);

	string sAppName; // Extract from last '\' to '.'
	sAppName = szFileName;

	// Trim path off file name
	string exefile = sAppName.substr(sAppName.rfind('\\') +1);
	return exefile.substr(0, exefile.find('.'));
}

bool CCrashProcCtrl::getCurrentDirectory(char path[MAX_PATH])
{
	bool bRtn = false;
	char szFileName[_MAX_PATH];
	GetModuleFileName(NULL, szFileName, _MAX_FNAME);
	std::string  fnstr = szFileName;
	int pos = fnstr.find_last_of('\\');
	if (pos != string::npos)
	{
		fnstr = fnstr.substr( 0,pos);
	}

	if (!fnstr.empty())
	{
		strcpy(path, fnstr.c_str());
		bRtn = true;
	}
	return bRtn;
}

//bool CCrashProcCtrl::isFileExisting(std::string filename)
//{
//	bool bRtn = false;
//	FILE *pF = fopen(filename.c_str(),"r");
//	if (pF != NULL)
//	{
//		bRtn = true;
//		fclose(pF);
//	}
//
//
//	return bRtn;
//}

CCrashProcCtrl * CCrashProcCtrl::getInst()
{
	if (m_pThis == NULL)
	{
		m_pThis = new CCrashProcCtrl();
	}
	return m_pThis;
}

double CCrashProcCtrl::GetCpuUsage()
{
	//cpu����  
	static int processor_count_ = -1;  
	//��һ�ε�ʱ��  
	static __int64 last_time_ = 0;  
	static __int64 last_system_time_ = 0;  

	FILETIME now;  
	FILETIME creation_time;  
	FILETIME exit_time;  
	FILETIME kernel_time;  
	FILETIME user_time;  
	__int64 system_time;  
	__int64 time;  

	double cpu = -1;  

	if(processor_count_ == -1)  
	{  
		processor_count_ = GetProcessNumber();  
	}  

	GetSystemTimeAsFileTime(&now);  

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, GetCurrentProcessId());  
	if (!hProcess)  
	{  
		return -1;  
	}  
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  
	{  
		return -1;  
	}  
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPUʹ��ʱ��  
	time = FileTimeToInt64(now);        //���ڵ�ʱ��  

	last_system_time_ = system_time;  
	last_time_ = time;  
	CloseHandle( hProcess );  

	Sleep(1000);  

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, GetCurrentProcessId());  
	if (!hProcess)  
	{  
		return -1;  
	}  
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  
	{  
		return -1;  
	}  
	GetSystemTimeAsFileTime(&now);  
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPUʹ��ʱ��  
	time = FileTimeToInt64(now);        //���ڵ�ʱ��  

	CloseHandle( hProcess );  

	cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;  
	return cpu; 
}

size_t CCrashProcCtrl::GetMemoryInfo()  
{  
	HANDLE hProcess = GetCurrentProcess();   

	PROCESS_MEMORY_COUNTERS pmc; 
	if (GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )  
	{           
		CloseHandle( hProcess );
		return pmc.WorkingSetSize;	//��λΪ�ֽ� 
	} 
	else
	{
		CloseHandle(hProcess);
		return -1;
	}
}  

int GetProcessNumber()  
{  
	SYSTEM_INFO info;  
	GetSystemInfo(&info);  
	return (int)info.dwNumberOfProcessors;  
}  

// ʱ���ʽת��  
__int64 FileTimeToInt64(const FILETIME& time)  
{  
	ULARGE_INTEGER tt;  
	tt.LowPart = time.dwLowDateTime;  
	tt.HighPart = time.dwHighDateTime;  
	return(tt.QuadPart);  
}  
void CCrashProcCtrl::savePid(EXCEPTION_POINTERS* pExPtr)
{
    CCrashProcCtrl* pThis = getInst();
    char cmd[512];
    string tempStr;
     pThis->getCurrentDirectory(cmd);
     tempStr = cmd;
     tempStr += "\\savedmpinfo.txt";

    HANDLE hFile = CreateFile(tempStr.c_str(),
        GENERIC_WRITE,//���ļ��Ĳ���
        0, // ����ķ�ʽ 0 ���ܹ���
        NULL,// ��ȫ���� ��ȱʡ��
        CREATE_ALWAYS, //����һ���µ��ļ�
        FILE_ATTRIBUTE_NORMAL, // �ļ�����
        NULL); // ģ���ļ��ľ��
    //д������

    if(INVALID_HANDLE_VALUE ==hFile)
    {
        int retval = GetLastError();
        Log::writeError2("����pid�ļ���ʧ��:errno:%u,file: %s",retval,tempStr.c_str());
        return ;
    }
    char bufer[512];
    sprintf(bufer,"%u",GetCurrentProcessId());

     Log::writeError2("����pid:%s,file: %s",bufer,tempStr.c_str());

    DWORD dwWrites;
    WriteFile(hFile, //�ļ����
        bufer, //ָ�� ���ļ�д������� 
        strlen(bufer), // ��Ҫд�����ݳ���
        &dwWrites, //ʵ��д��ĳ���
        NULL); //ͬ��IO�����첽IO�ķ�ʽ �����ͬ�� ����ᱻ����һֱ����д���
    CloseHandle(hFile);


//     string tempStr;
//     pThis->getCurrentDirectory(cmd);
//     tempStr = cmd;
//     tempStr += "\\savedmpinfo.data";
//     //�����쳣����Ϣ
//     FILE *pfile = fopen(tempStr.c_str(),"wb");
//     if(NULL == pfile)
//     {
//         Log::writeError2("����info����ʧ��file: %s",tempStr.c_str());
//         return;
//     }

    pThis->getCurrentDirectory(cmd);
    tempStr = cmd;
    tempStr += "\\crashProc.exe";

    Log::writeError2("��������dump����:%s",tempStr.c_str());

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BOOL bbret = CreateProcess(NULL,(char*)tempStr.c_str(),NULL,NULL,FALSE,0,0,0,&si,&pi);    
    if(bbret != 0)
    {
        Log::writeError2("��������dmp���̳ɹ�,�ȴ�");
        WaitForSingleObject(pi.hProcess, INFINITE);
        Log::writeError2("��������dmp���̳ɹ�,����");

        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);
       
    }
    else
    {
        int err = GetLastError();
        Log::writeError(1<<10,1024,"��������dmp����ʧ��,err=%u",err);
    }
}
 int CCrashProcCtrl::readPid(DWORD &pid)
 {
     CCrashProcCtrl* pThis = getInst();
     char cmd[512];
     string tempStr;
     pThis->getCurrentDirectory(cmd);
     tempStr = cmd;
     tempStr += "\\savedmpinfo.txt";

     HANDLE hFile;
     hFile=CreateFile(tempStr.c_str(),
         GENERIC_READ,
         0,
         NULL,
         OPEN_EXISTING,//�Ѿ����ڵ�
         FILE_ATTRIBUTE_NORMAL,
         NULL);
     if(INVALID_HANDLE_VALUE == hFile)
     {
         int retval = GetLastError();
         Log::writeError2("����pid�ļ���ʧ��:errno:%u,file: %s",retval,tempStr.c_str());
         return -1;
     }

     // ���ļ�
     char ch[512];
     DWORD dwReads;
     ReadFile(hFile,//���
         ch, //���ջ���
         100, //��Ҫ������
         &dwReads, //ʵ�ʶ����ĸ���
         NULL);
     ch[dwReads]=0;  //�Ѷ��������һ���� ����Ϊ0 ��ʾ�ļ�����
     CloseHandle(hFile);

     Log::writeError2("��ȡ����ID:%s",ch);
     pid = atoi(ch);
     return 0;
 }

  void CCrashProcCtrl::startCrashProc(int prama)
  {
      string dumpFileName;
      string logFileName;
      CCrashProcCtrl* pThis = getInst();

      dumpFileName = pThis->getFileName();
      logFileName = dumpFileName;
      dumpFileName += "_save.dmp";

      logFileName += "_save.log";
      Log::openlog((char*)logFileName.c_str());

      DWORD tpid;
      if(0 != pThis->readPid(tpid))
      {
          return;
      }
      pThis->saveDmpByCreshProcessId(tpid,prama);

  }

  void CCrashProcCtrl::saveDmpByCreshProcessId(int pid,int prama)
  {
      CCrashProcCtrl* pThis = getInst();
      string dumpName;
      dumpName = pThis->getFileName();
      dumpName += "_save.dmp";
      //����dump�ļ�
      HANDLE hDumpFile = CreateFile(  
          dumpName.c_str(),  

          GENERIC_READ | GENERIC_WRITE,  
          FILE_SHARE_READ | FILE_SHARE_WRITE,  
          NULL,  
          CREATE_ALWAYS,  
          FILE_ATTRIBUTE_NORMAL,  
          NULL  
          );
      if(INVALID_HANDLE_VALUE == hDumpFile)
      {
          int retval = GetLastError();
          Log::writeError2("����dum�ļ���ʧ��:errno:%u,file: %s",retval,dumpName.c_str());
          return ;
      }
      HANDLE openhandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
      BOOL bret = MiniDumpWriteDump(
          openhandle,
          pid,
          hDumpFile,
          MiniDumpNormal,
          NULL,
          NULL,
          NULL);
      CloseHandle(hDumpFile);
      if (bret != TRUE)
      {
          int retval = GetLastError();
          Log::writeError(1<<10,1024,"дdump�ļ�ʧ��,LastErrorCode:%u",retval);
      }
  }