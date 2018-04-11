#include "utilities.h"

#include <stdlib.h>
#include <time.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp")

void Log(const wchar_t* format, ... )
{
	WCHAR Buf[0x2048] = {0};


	va_list vList;
	va_start(vList, format);


	_vsnwprintf(Buf, 0x2047,format,vList);

	va_end(vList);


	OutputDebugStringW(Buf);

}


void Log(const char* format, ... )
{
	CHAR Buf[0x2048] = {0};


	va_list vList;
	va_start(vList, format);


	_vsnprintf(Buf,0x2047, format,vList);

	va_end(vList);


	OutputDebugStringA(Buf);

}


LONG WINAPI UnhandleExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{

	TCHAR szAppDataDir[256] = {0};


	::ExpandEnvironmentStrings(_T("%APPDATA%\\browser"), szAppDataDir, MAX_PATH);


	if(!PathFileExists(szAppDataDir) )
	{
		CreateDirectory(szAppDataDir, NULL);

	}

	TCHAR dumpfilename[256] = {0};

	SYSTEMTIME  sysTime;
	GetLocalTime(&sysTime);



	struct tm tmtime = {0};
	tmtime.tm_year = sysTime.wYear - 1900;
	tmtime.tm_mon = sysTime.wMonth - 1;
	tmtime.tm_mday = sysTime.wDay;
	tmtime.tm_hour = sysTime.wHour;
	tmtime.tm_min = sysTime.wMinute;
	tmtime.tm_sec = sysTime.wSecond;
	tmtime.tm_wday = 0;
	tmtime.tm_yday = 0;
	tmtime.tm_isdst = -1;



	_stprintf(dumpfilename, _T("%08X.dmp"), mktime(&tmtime));



	PathAppend(szAppDataDir, dumpfilename);


	HANDLE   hFile   =   CreateFile(szAppDataDir,   GENERIC_WRITE,   FILE_SHARE_WRITE,   NULL,   CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,   NULL   );

	if   (hFile!=INVALID_HANDLE_VALUE)
	{ 
		MINIDUMP_EXCEPTION_INFORMATION   ExInfo; 

		ExInfo.ThreadId   =   ::GetCurrentThreadId();
		ExInfo.ExceptionPointers   =   ExceptionInfo;
		ExInfo.ClientPointers   =   NULL;

		int nDumpType = MiniDumpNormal;
		MiniDumpWithThreadInfo | MiniDumpWithHandleData | MiniDumpWithProcessThreadData;

		//   write   the   dump
		BOOL   bOK   =   MiniDumpWriteDump(GetCurrentProcess(),   GetCurrentProcessId(),   hFile,
			(MINIDUMP_TYPE)nDumpType,  &ExInfo,   NULL,   NULL   );
		CloseHandle(hFile); 
	} 

	//PostThreadMessage(GetCurrentThreadId(),WM_QUIT,0,0);
	return 0;
}