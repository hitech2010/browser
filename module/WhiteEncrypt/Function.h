#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <wininet.h>
#include <UrlHist.h>
#include <ShlObj.h>
#include <openssl/sha.h>
using namespace std;
#include "sqlite3.h"
#pragma comment (lib,"sqlite3.lib")

enum DEL_CACHE_TYPE //要删除的类型。
{
	File,//表示internet临时文件
	Cookie //表示Cookie
};


const vector<string>& Get_table(int name_id);

bool creatTable(int name_id);

bool insert(int name_id,string id,string name,string value);

bool Update(int name_id,string id, string name );

bool Delete(int name_id,string id);

bool AddHistorySite( string url, string captain = "")
{
	const int tableid = 0;
	creatTable(tableid);

	if(captain == "")
	{
		captain = "无标题";
	}
	insert(tableid, "", captain, url);


	return true;
}
bool DeleteHistorySite(string id)
{
	const int tableid = 0;
	return Delete(tableid, id);
}
vector<string> GetHistorySite()
{
	const int tableid = 0;
	return Get_table(tableid);
}


bool AddFavorateSite(string url, string captain = "")
{
	const int tableid = 1;
	creatTable(tableid);

	if(captain == "")
	{
		captain = "无标题";
	}
	insert(tableid, "", captain, url);


	return true;
}
bool DeleteFavorateSite(string id)
{
	const int tableid = 1;
	return Delete(tableid, id);

}
vector<string> GetFavorateSite()
{
	const int tableid = 1;
	return Get_table(tableid);
}





//删除数据 id == "" 则删除表中所有数据

//删除本地操作
BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE,   BOOL bWipeIndexDat = FALSE);
// 删除Internet 临时数据
bool DeleteInterFile();
// 删除Cookie
bool DeleteCookie();
// 删除系统临时文件
void DeleteTemp();


static string Sha1Hash(const string & Msg, bool bHex = true, bool bLower = true)
{

	string ret;
#if 0
	if (Msg.size())
	{
		unsigned char md[20];
		SHA1(reinterpret_cast<const unsigned char *>(Msg.c_str()), Msg.size(), md);
		if (bHex)
		{
			char tmp[3] = { '\0' };
			const char * szFormat;
			if (bLower)
				szFormat = "%02x";
			else
				szFormat = "%02X";
			for (int i = 0; i < 20; i++)
			{
#if defined(_WINDOWS) || defined(_WIN32)
				_snprintf_s(tmp, 3, szFormat, md[i]);
#else
				snprintf(tmp, 3, szFormat, md[i]);
#endif
				ret.append(tmp);
			}
		}
		else
		{
			ret.assign(reinterpret_cast<char*>(md), 20);
		}
	}
#endif
	return ret;
}
