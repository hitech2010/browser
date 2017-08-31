#include "utilities.h"

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