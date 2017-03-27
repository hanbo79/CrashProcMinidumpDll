#include "MarkupSTL.h"
#include "CCrashReportCenter.h"
using namespace std;

CCrashReportCenter::CCrashReportCenter()
{
	memset(centerIP, 0, 16);
	centerPort = 0;
}
CCrashReportCenter::CCrashReportCenter(const char ip[16],const unsigned short port)
{

	memcpy(centerIP, ip, 16);
	centerPort = port;
}

bool CCrashReportCenter::loadConfig()
{
	CMarkupSTL xml;
	char buf[7000] = {0};
	char fileName[MAX_PATH] = {0};
	if (!getCurrentDirectory_t(fileName))
	{
		printf("--not find current directory \n");
		return false;
	}
#ifdef WIN32
	strcat( fileName, "\\" );
#else
	strcat( fileName, "/");
#endif
	strcat( fileName, CRASHCENTERFILE );

	if (!xml.Load(fileName))
		return false;

	if( !xml.IsWellFormed() )
		return false;

	if (!xml.FindChildElem("ip"))
		return false;

	strncpy(centerIP, xml.GetChildData().c_str(), 15);
	xml.ResetChildPos();
	if (!xml.FindChildElem("port"))
		return false;

	centerPort = atoi(xml.GetChildData().c_str());
	return true;
}


bool CCrashReportCenter::saveConfig()
{
	CMarkupSTL xml;
	char fileName[MAX_PATH] = {0};
	if (!getCurrentDirectory_t(fileName))
	{
		printf("--not find current directory \n");
		return false;
	}
#ifdef WIN32
	strcat( fileName, "\\" );
#else
	strcat( fileName, "/");
#endif
	strcat( fileName, CRASHCENTERFILE );
	xml.AddElem("crashcenter");;
	xml.AddChildElem("ip",centerIP);
	xml.AddChildElem("port",centerPort);
	bool result = xml.Save(fileName);
	printf("save config:%s, centerIP:%s, centerPort:%d \n", fileName, centerIP, centerPort);
	return(result);
}

bool getCurrentDirectory_t(char path[MAX_PATH])
{
	bool bRtn = false;
	char szFileName[_MAX_PATH] = {0};
#ifdef WIN32
	GetModuleFileName(NULL, szFileName, _MAX_FNAME);
#else
	int ret = readlink("/proc/self/exe", szFileName, _MAX_PATH); 
	if ( ret < 0 || ret >= _MAX_PATH ) 
	{
		strcpy(szFileName, "./");
	}
#endif 
	std::string  fnstr = szFileName;
#ifdef WIN32
	int pos = fnstr.find_last_of('\\');
#else
	int pos = fnstr.find_last_of('/');
#endif 
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
