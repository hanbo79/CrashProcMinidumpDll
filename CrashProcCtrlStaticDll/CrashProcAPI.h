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
 *	��װ��������DUMP�Ĵ�������prama Ϊ0ʱ��ֻ��ȡץջ�� 1��ץȡջ�Ͷѣ��ļ��ܴ�,һ�㶼�ü�ʮ��byte��
 */
CrashProcDLL_API void installCrashProcCtrl(int prama = 0) ;

/*
 * ���ô����ռ���ַ
 */
CrashProcDLL_API void setErrorReportCenter(const char centerip[16],const unsigned short centerPort);


//������������������ǰ���̵�dump
CrashProcDLL_API void startCrashProc(int prama = 0) ;
#endif