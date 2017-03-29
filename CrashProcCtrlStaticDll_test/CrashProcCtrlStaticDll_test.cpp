// CrashProcCtrlStaticDll_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <stdlib.h>
#include<crtdbg.h>

#include "CrashProcAPI.h"

void fun(int x,int y)
{
    char buf[2];

    memcpy(buf,0,200);
}

#include <stdio.h>  
#include <stdlib.h>  
#include <crtdbg.h>  // For _CrtSetReportMode  

void tesetabort()
{
    abort();  
}

class CDerived;  
class CBase  
{  
public:  
    CBase(CDerived *derived): m_pDerived(derived) {};  
    ~CBase();  
    virtual void function(void) = 0;  

    CDerived * m_pDerived;  
};  

class CDerived : public CBase  
{  
public:  
    CDerived() : CBase(this) {};   // C4355  
    virtual void function(void) 
    {
        CBase::m_pDerived -> function();
    };  
};  

CBase::~CBase()  
{  
    m_pDerived -> function();  
}  
void funpur()
{
    {
        CDerived myDerived;  
        //myDerived.function();
    }
}

DWORD  __stdcall LPTHREAD_START_ROUTINE_cb( 
    LPVOID lpThreadParameter)
{
    int *alloc_test = NULL;
    while(true)
    {
        //printf("\nnew");
        alloc_test = new int[10240];
    }

}

int _tmain(int argc, _TCHAR* argv[])
{    
    installCrashProcCtrl(1);   //注册导出
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);  //防止windows弹出崩溃界面
    //_set_abort_behavior(0,_WRITE_ABORT_MSG); 
    //_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

    CreateThread(NULL,0,LPTHREAD_START_ROUTINE_cb,NULL,0,NULL);


   // funpur();

     //tesetabort();

//     _invalid_parameter_handler oldHandler, newHandler;  
//     newHandler = myInvalidParameterHandler1;  
//     oldHandler = _set_invalid_parameter_handler(newHandler);  
// 
//     // Disable the message box for assertions.  
//     _CrtSetReportMode(_CRT_ASSERT, 0);  

    // Call printf_s with invalid parameters.  

    //char* formatString;  
    //formatString = NULL;  
    //printf(formatString);  

   //int *p =NULL;
   //*p = 2;

   //free((void*)12);
   //fun(0,1);
   // int xa = 0;
   // int x = 3/xa;

   // printf("%d",x);
   // int y = 4+x;
    //printf("%d",y);

    
   // printf("hell");
   // getchar();

   
    printf("hello \n");
    getchar();

	return 0;
}

