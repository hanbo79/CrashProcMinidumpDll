#ifndef CRASHPROC_API_H_2008_GOUMANG
#define CRASHPROC_API_H_2008_GOUMANG


#ifdef __cplusplus
#		ifdef CrashProcDLL_API_EXPORTS
#			define CrashProcDLL_API extern "C" __declspec(dllexport)
#		else
#			define CrashProcDLL_API extern "C" __declspec(dllimport)
#		endif
#else
#define CrashProcDLL_API
#endif

/*
 *	安装崩溃产生DUMP的处理，参数prama 为0时，只获取抓栈， 1：抓取栈和堆（文件很大,一般都好几十兆byte）
 */
CrashProcDLL_API void installCrashProcCtrl(int prama = 0) ;

/*
 * 设置错误收集地址
 */
CrashProcDLL_API void setErrorReportCenter(const char centerip[16],const unsigned short centerPort);


//启动辅助进程主区当前进程的dump
CrashProcDLL_API void startCrashProc(int prama = 0) ;
#endif