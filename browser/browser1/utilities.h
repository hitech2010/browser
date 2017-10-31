#define WIN32_LEAN_AND_MEAN



#include <Windows.h>
#include <tchar.h>
#include <wchar.h>
#include <Shlwapi.h>

#pragma  comment(lib, "Shlwapi.lib")

#include <stdio.h>
#include <stdlib.h>

//atl
#include <atlbase.h>

//wtl
#include <ShellAPI.h>
#include <atlbase.h>          // 基本的ATL类  
#include <atlapp.h>              // 基本的WTL类  
#include <atlwin.h>               // ATL窗口类  
#include <atlddx.h>               // WTL数据交换类  
#include <atlframe.h>         //WTL主框架窗口类  
#include <atlctrls.h>         //WTL控件类  
#include <atlctrlx.h>           
#include <atldlgs.h>          //WTL对话框类  
#include <atlctrlw.h>           



//stl
#include<string>
#include <map>
#include <vector>
#include <queue>
using namespace std;


//string 
#include "xxstring.h"

//_re
#include "_re.h"

//Log
#include <varargs.h>


void Log(const wchar_t* format, ... );
void Log(const char* format, ... );


//proxy



#include "eventpool.h"



int startproxy();
void clearproxy();
void setproxy(bool bflag);
bool isGMSSL(string &strt, bool &b);



//#pragma  comment(lib, "HTTP_Proxy_ServerD.lib")



//string


class Codec
{
public:
	static std::wstring string2wstring(const std::string strIn, UINT CodePage = CP_UTF8);
	static std::string wstring2string(const std::wstring strIn, UINT CodePage = CP_UTF8);
};

//sqlite3

#include "CppSQLite3.h"



//收藏以及历史及道路

#include "Favor.h"


//app
#include "app_config.h"
//


