#include "stdafx.h"
#include "Function.h"

#define DB_NAME "test.db"

#define Table_name "COLLECTION"

#define Table_name1 "COLLECTION1"




static int callback(void *NotUsed, int argc, char **argv, char **azColName)  
{  
	int i = 0;  
	for(i=0; i < argc; i++){  
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");  
	}  
	printf("\n");  

	return 0;  
} 

bool opendb(sqlite3** db)
{
	int ret = 0;
	//sqlite3 *db = NULL;

	char *zErrMsg = NULL;
	ret = sqlite3_open(DB_NAME, db);
	if (ret != SQLITE_OK)
	{
		return false;
	}
	return true;
}

string get_db_name(int name_id)
{
	
	string table;
	if (name_id)
	{
		table = Table_name1;
	}
	else
	{
		table = Table_name;
	}
	return table;
}

bool  creatTable(int name_id)
{
	int ret = 0;
	sqlite3 *db = NULL;
	char *sql = NULL;
	char *zErrMsg = NULL;
	if (!opendb(&db))
	{
		return false;
	}
	if (name_id)
	{
		sql = "CREATE TABLE COLLECTION1("\
			"ID INT PRIMARY KEY    NOT NULL," \
			"NAME           TEXT   NOT NULL," \
			"TIME           TEXT   NOT NULL," \
			"ADDRESS        CHAR(102));";
	}
	else
	{
		sql = "CREATE TABLE COLLECTION("\
			"ID INT PRIMARY KEY    NOT NULL," \
			"NAME           TEXT   NOT NULL," \
			"TIME           TEXT   NOT NULL," \
			"ADDRESS        CHAR(102));";
	}
	
	ret = sqlite3_exec(db, sql,callback, 0, &zErrMsg);
	if(ret != SQLITE_OK)
	{
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return false;
	}
	sqlite3_close(db);
	return true;								
}

bool insert(int name_id,string id,string name,string value)
{
	int ret = 0;
	sqlite3* db = NULL;

	SYSTEMTIME stUTC;  

	::GetSystemTime(&stUTC);

	const int nBufSize = 256;  

	TCHAR chBuf[nBufSize];  

	wsprintf(chBuf,_T("%u/%u/%u %u:%u:%u:%u %d"),              

		stUTC.wYear, stUTC.wMonth, stUTC.wDay,  

		stUTC.wHour, stUTC.wMinute, stUTC.wSecond,  

		stUTC.wMilliseconds,stUTC.wDayOfWeek);

	//Tchar to char
	int iLength ;  
	char buf[nBufSize];
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);  
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, buf, iLength, NULL, NULL); 
	string strid = value + buf;
	char *zErrMsg = NULL;
	if (!opendb(&db))
	{
		return false;
	}

	if(id == "")
	{
		id = Sha1Hash(value);
	}


	if (id.length()
		&& name.length()
		&& value.length())
	{
		char buf[500];
		string table = get_db_name(name_id);
		sprintf(buf,"insert into '%s'(ID, NAME, TIME, ADDRESS)values('%s','%s','%s','%s')",table.c_str(),id.c_str(),name.c_str(),buf,value.c_str());
		ret = sqlite3_exec(db,buf,callback,0,&zErrMsg);
		if (ret != SQLITE_OK)
		{
			sqlite3_close(db);
			sqlite3_free(zErrMsg);
			return false;
		}
	}
	sqlite3_close(db);
	return true;
}

bool Update(int name_id,string id, string name )
{
	int ret = 0;
	sqlite3 *db = NULL;

	char *zErrMsg = NULL;
	if (!opendb(&db))
	{
		return false;
	}
	if (id.length()&&name.length())
	{
		char buf[500];
		string table = get_db_name(name_id);
		sprintf(buf,"update '%s' set NAME = '%s' where ID = '%s'",table.c_str(),name.c_str(),id.c_str());
		ret = sqlite3_exec(db,buf,callback,0,&zErrMsg);
		if (ret != SQLITE_OK)
		{
			sqlite3_close(db);
			sqlite3_free(zErrMsg);
			return false;
		}
	}
	sqlite3_close(db);
	return true;
}
bool Delete(int name_id,string id)
{
	int ret = 0;
	sqlite3 *db = NULL;

	char *zErrMsg = NULL;
	if (!opendb(&db))
	{
		return false;
	}
	char buf[500];
	string table = get_db_name(name_id);
	if (id.length())
	{
		sprintf(buf,"delete from '%s' where ID = '%s'",table.c_str(),id.c_str());
	}
	else
	{
		sprintf(buf,"delete from '%s'",table.c_str());
	}
	
	ret = sqlite3_exec(db,buf,callback,0,&zErrMsg);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return false;
	}
	sqlite3_close(db);
	return true;
}
const vector<string>& Get_table(int name_id)
{
	int ret = 0;
	sqlite3 *db = NULL;

	vector<string> content;

	char *zErrMsg = NULL;
	if (!opendb(&db))
	{
		return content;
	}

	char buf[500];
	string table = get_db_name(name_id);
	sprintf(buf,"select ID from '%s'",table.c_str());
	char** pResult;
	int nRow;
	int nCol;
	ret = sqlite3_get_table(db,buf,&pResult,&nRow,&nCol,&zErrMsg);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return content;
	}
	string strout = "";
	int nIndex = nCol;
	for (int i= 0;i<nRow;i++)
	{
		string oneitem = "{";

		for (int j = 0; j < nCol;j++)
		{
// 			strout += pResult[j];
// 			strout += ",";
			oneitem += "\"";
			oneitem += pResult[nIndex];
			oneitem += "\"";

			if(j != nCol-1)
			{
				oneitem += ",";
			}
			++nIndex;
		}
		oneitem += "}";

		content.push_back(oneitem);
	}
	sqlite3_free_table(pResult);
	sqlite3_close(db);
	return content;
}

bool DeleteUrlCache(DEL_CACHE_TYPE type)
{
	BOOL bRet = FALSE;
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
	DWORD dwEntrySize;

	//delete the files
	dwEntrySize = 0;
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
	if (!hEntry)
	{
		goto cleanup;
	}

	do
	{
		if (type == File &&
			!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else if (type == Cookie &&
			(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}

		dwEntrySize = 0;
		FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
		delete [] lpCacheEntry; 
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	}
	while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));

	bRet = TRUE;
cleanup:
	if (lpCacheEntry)
	{
		delete [] lpCacheEntry; 
	}
	return bRet;
}
#define SWEEP_BUFFER_SIZE			10000
BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	char	sZero[SWEEP_BUFFER_SIZE];
	memset(sZero, 0, SWEEP_BUFFER_SIZE);

	sPath = szDir;
	sPath += _T('\\');
	sPath += szFile;

	hFile = CreateFile(sPath, GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	dwSize = GetFileSize(hFile, NULL);

	//skip file header (actually, I don't know the file format of index.dat)
	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);

	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(hFile, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}

	CloseHandle(hFile);
	return TRUE;
}
BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, 
	BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += _T("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 || 
			_tcscmp(wfd.cFileName, _T("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, _T("desktop.ini")) == 0))
		{
			continue;
		}

		sFullPath = szPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;

		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else
		{
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, _T("index.dat")) == 0)
			{
				WipeFile(szPath, wfd.cFileName);
			}
			DeleteFile(sFullPath);
		}
	}
	while (FindNextFile(hFind, &wfd));
	FindClose(hFind);

	return TRUE;
}
bool DeleteInterFile()
{
	// //清internet临时文件
	TCHAR szPath[MAX_PATH];
	DeleteUrlCache(File);
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE))
	{  //得到临时目录，并清空它.
		EmptyDirectory(szPath);
		return TRUE;
	}
	return FALSE;
}

bool DeleteCookie()
{
	TCHAR szPath[MAX_PATH];
	DeleteUrlCache(Cookie);
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE))
	{  //得到临时目录，并清空它.
		EmptyDirectory(szPath);
		return TRUE;
	}
	return FALSE;
}

void DeleteTemp()
{
	// //清系统临时文件夹
	TCHAR szPath[MAX_PATH];	
	if (GetTempPath(MAX_PATH, szPath))//得到系统临时目录
	{
		EmptyDirectory(szPath, TRUE);
	}
}