#pragma once

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;


enum 
{
	q_all		= 0,
	q_thisweek = 1,
	q_thismonth = 2,
	q_rest     =3,
	q_today	= 4,
	q_yesterday = 5

};


struct favor_recode_item
{
	int id;
	string folder;
	string title;
	string url;
	string img;
	string time;
};

struct history_recode_item
{
	int id;
	string folder;
	string title;
	string url;
	string img;
	string time;
};

struct favorfolder_recode_item
{
	int id;
	string folder;
};

typedef vector<favor_recode_item> FAVOR_LIST;
typedef int H_INDEX;





class CFavorManager//收藏标签管理//throw std::exception
{
public:
	const char* m_table ;
	char   m_szPath[MAX_PATH];
	CppSQLite3DB* m_db;
	CppSQLite3Table m_dbtable;
	typedef favor_recode_item  RECORD;
	typedef vector<RECORD>     VRECORD;
	VRECORD               m_result;

public:
	CFavorManager(CppSQLite3DB* db)throw();
	

	~CFavorManager();

	VRECORD& Query(int query);

	CFavorManager& Add(const RECORD& record);
	CFavorManager& Edit(const RECORD& newRecord);
	CFavorManager& QueryById(int id = -1);
	CFavorManager& QueryByFolder(const string& folder);
	int            CountOf(const string& url);
	const VRECORD& GetResult();
	CFavorManager& Delete(int startpos, int endpos);
	CFavorManager& Delete(int startpos, int endpos, const string& folder);
	CFavorManager& clear();
	VRECORD& Query(const string& keyword);

	xstring TestRoutine();//返回测试结果
	vector<string> QueryFolders();


};



class CFavorFolder//收藏标签文件夹管理//throw std::exception
{
public:
#define NOT_USING_SQLITE 
	const char* m_table;
	char   m_szPath[MAX_PATH];
	CppSQLite3DB* m_db;
	CppSQLite3Table m_dbtable;
	typedef favorfolder_recode_item  RECORD;
	typedef vector<RECORD>     VRECORD;
	VRECORD               m_result;
	map<int, string>		m_memdb;

public:
	CFavorFolder(CppSQLite3DB* db)throw();


	~CFavorFolder();
	const VRECORD& GetResult();

	VRECORD& Query();
	CFavorFolder& Add(const RECORD& record);

	CFavorFolder& Clear();
	CFavorFolder& QueryById(int id);
	CFavorFolder& Delete(int startpos, int endpos);
};


class CHistoryMgr//历史标签管理//throw std::exception
{
public:
	const char* m_table ;
	char   m_szPath[MAX_PATH];
	CppSQLite3DB* m_db;
	CppSQLite3Table m_dbtable;

	typedef history_recode_item  RECORD;
	typedef vector<RECORD>     VRECORD;
	VRECORD                   m_result;
	




public:
	CHistoryMgr(CppSQLite3DB*db)throw();
	xstring TestRoutine();
	CHistoryMgr& setTitle(string& url, string& title);
	~CHistoryMgr();
	CHistoryMgr& Add(const RECORD& record);
	VRECORD& Query(int query);
	CHistoryMgr::VRECORD& Query(const string& keyword);
	CHistoryMgr& Clear();
	CHistoryMgr& Delete(const string& tuple);
	CHistoryMgr& AddShotcut(string id, string filename);
	CHistoryMgr& GetTop4ShortCut(map<int,string>& result);
	CHistoryMgr& GetTop4MostRrequentShortCut(map<int,string>& result);
};



