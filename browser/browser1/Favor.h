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
	q_rest     =3

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

typedef vector<favor_recode_item> FAVOR_LIST;
typedef int H_INDEX;

class CFavor
{
	static const string s_dbpath;
	static const string s_tablename;
public:
	CFavor(void);
	~CFavor(void);
	static bool GetAllItem(FAVOR_LIST& fl);
	static bool AddItem(const favor_recode_item& fri);
	static bool DeleteItem(string title);
	static bool ModifyItem(string title,string newtitle);
	static bool GetItem(string title,favor_recode_item& fri);
};

class CHistory
{
	static const string s_dbpath;
	static const string s_tablename;
public:
	static FAVOR_LIST s_hl;
	CHistory(void);
	~CHistory(void);
	static bool GetAllItem(FAVOR_LIST& fl = s_hl);
	static bool AddItem(const favor_recode_item& fri);
	static bool DeleteItem(int index);
	static bool ModifyItem(int index,string newtitle);
	static bool GetItem(int index,favor_recode_item& fri);
	static bool DeleteAllItem();
};



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
	CFavorManager& Delete(int startpos, int endpos);
	CFavorManager& Delete(int startpos, int endpos, const string& folder);
	CFavorManager& clear();


	xstring TestRoutine();//返回测试结果



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

	~CHistoryMgr();
	CHistoryMgr& Add(const RECORD& record);
	VRECORD& Query(int query);
	CHistoryMgr& clear();
};
