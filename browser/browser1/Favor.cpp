#include "utilities.h"
#include "../../sqlite/sqlite3.h"


using std::string;



CFavorManager::CFavorManager(CppSQLite3DB* db)throw()
{
	m_db = db;
	string sql = "CREATE TABLE IF NOT EXISTS FAVOR(\
				 ID integer primary key,\
				 TITLE text NOT NULL,\
				 IMG blob NOT NULL,\
				 FOLDER text NOT NULL,\
				 URL text NOT NULL,\
				 ADDDATE DATETIME NOT NULL)";

	db->execDML(sql.c_str());

	


}

	CFavorManager::~CFavorManager()
	{
		;
	}

	 CFavorManager::VRECORD& CFavorManager::Query(int query)
	 {

		 m_result.clear();

		 string sql;
		 if(query == q_all)
		 {
			 sql = "select * from favor;";
		 }
		 else if(query == q_thisweek)
		 {
			 sql = "select * from favor where ADDDATE "
				   "between datetime('now','start of week')"
				   "and datetime('now');";
		 }
		 else if(query == q_thismonth)
		 {
			 sql = "select * from favor where ADDDATE "
				 "between datetime('now','start of month', '-1 second')"
				 "and datetime('now');";
		 }
		 else if(query == q_rest)
		 {
			 sql = "select * from favor where ADDDATE "
				 "between datetime('1900-01-01 00:00:00')"
				 "and datetime('now','start of month', '-1 second');";
		 }
		 else
		 {
			 throw std::exception("invalid parameter");
		 }


		 m_dbtable = m_db->getTable(sql.c_str());

		 for (int j = 0; j < m_dbtable.numRows(); ++j)
		 {
			 RECORD rc;
			 for (int i = 0; i < m_dbtable.numFields(); ++i)
			 {
				 m_dbtable.setRow(j);

				 
				 rc.id = m_dbtable.getIntField("ID");
				 rc.folder = m_dbtable.getStringField("FOLDER");
				 const  char* blob = m_dbtable.getStringField("IMG");
				 CppSQLite3Binary bin;
				 bin.setEncoded((const unsigned char*)blob);
				 rc.img.assign((const char*)bin.getBinary(), bin.getBinaryLength());


				 rc.title = m_dbtable.getStringField("TITLE");
				 rc.url = m_dbtable.getStringField("URL");
				 rc.time = m_dbtable.getStringField("ADDDATE");
			 }
			 m_result.push_back(rc);

		 }



		return m_result;

	}

	 CFavorManager& CFavorManager::Add(const RECORD& record)
	 {
		 CppSQLite3Buffer   sql;
		

		unsigned char bin[256];
		CppSQLite3Binary blob;

		
		blob.setBinary((const unsigned char*)record.img.c_str(), record.img.size());

		sql.format("INSERT INTO FAVOR "
			"VALUES(NULL,%Q,%Q,%Q,%Q, datetime('now'));", 
			record.title.c_str(),blob.getEncoded(), record.folder.c_str(), record.url.c_str());

		try
		{
			m_db->execDML(sql);
		}
		catch (CppSQLite3Exception& e)
		{
			MessageBoxA(NULL, "334", e.errorMessage(), MB_OK);
		}
		

		return *this;
	 }


	 xstring CFavorManager::TestRoutine()
	 {
		 //Çå¿Õ
		 clear();
		 //test1: Ìí¼Ó150Ìõ

		 xstring result = "adding 100 items\r\n";
		 for (int i = 0; i < 500; ++i)
		 {
			 RECORD rc;
			 xstring tmp;
			 rc.folder.assign(tmp.format("fold-%d", i % 3).c_str());
			 rc.url.assign(tmp.format("http://baidu%d.com", i).c_str());
			 rc.img.assign(tmp.format("imag.....%d", i).c_str());
			 rc.title.assign(tmp.format("title-%d", i).c_str());
			 Add(rc);
		 }


		 //test2 query
		 VRECORD records = Query(q_all);
		 for (int i = 0; i < records.size(); ++i)
		 {
			 result.append(xstring().format("%d | %s | %s | %s | %s | %s", records[i].id, records[i].title.c_str(), records[i].folder.c_str(),
				 records[i].url.c_str(), records[i].time.c_str(), records[i].img.c_str())).append("\r\n");
		 }

		 result.append("delete id between 0 - 99\r\n");
		 
		 records = Delete(0, 99).Query(q_all);
		 for (int i = 0; i < records.size(); ++i)
		 {
			 result.append(xstring().format("%d | %s | %s | %s | %s | %s", records[i].id, records[i].title.c_str(), records[i].folder.c_str(),
				 records[i].url.c_str(), records[i].time.c_str(), records[i].img.c_str())).append("\r\n");
		 }


		 result.append("delete id between 100-119 and folder-2\r\n");

		 records = Delete(0, 99,"folder-2").Query(q_all);
		 for (int i = 0; i < records.size(); ++i)
		 {
			 result.append(xstring().format("%d | %s | %s | %s | %s | %s", records[i].id, records[i].title.c_str(), records[i].folder.c_str(),
				 records[i].url.c_str(), records[i].time.c_str(), records[i].img.c_str())).append("\r\n");
		 }





		 //É¾³ý
		

		 
		 return result;
	 }

	 CFavorManager& CFavorManager::clear()
	 {
		 string sql = "DELETE FROM FAVOR";
		 m_db->execDML(sql.c_str());
		 return *this;
	 }

	 CFavorManager& CFavorManager::Delete(int startpos, int endpos)
	 {
		 xstring sql;
		 sql.format("DELETE FROM FAVOR WHERE ID BETWEEN %d AND %d;", startpos, endpos);
		 
		 m_db->execDML(sql.c_str());
		 return *this;
	 }

	 CFavorManager& CFavorManager::Delete(int startpos, int endpos, const string& folder)
	 {
		 xstring sql;
		 sql.format("DELETE FROM FAVOR WHERE (ID BETWEEN %d AND %d) AND folder='%s';", startpos, endpos, folder.c_str());

		 m_db->execDML(sql.c_str());
		 return *this;
	 }






	CHistoryMgr::CHistoryMgr(CppSQLite3DB*db) throw()
	{

		m_db = db;
		
		string sql = "CREATE TABLE IF NOT EXISTS HISTORY(\
					 ID integer primary key,\
					 TITLE text NOT NULL,\
					 IMG blob NOT NULL,\
					 FOLDER text ,\
					 URL text NOT NULL,\
					 ADDDATE DATETIME NOT NULL);";

		db->execDML(sql.c_str());

	}


	CHistoryMgr& CHistoryMgr::clear()
	{
		string sql = "DELETE FROM HISTORY";
		m_db->execDML(sql.c_str());
		return *this;
	}

	CHistoryMgr& CHistoryMgr::Add(const RECORD& record)
	{
		
		CppSQLite3Buffer   sql;
		

		unsigned char bin[256];
		CppSQLite3Binary blob;

		for (int i = 0; i < sizeof bin; i++)
		{
			bin[i] = i;
		}
		blob.setBinary((const unsigned char*)record.img.c_str(), record.img.size());

		sql.format("INSERT INTO HISTORY "
			"VALUES(NULL,%Q,%Q,%Q,%Q, datetime('now'));", 
			record.title.c_str(), blob.getEncoded(), record.folder.c_str(), record.url.c_str());

		try
		{
			m_db->execDML(sql);
		}
		catch (CppSQLite3Exception& e)
		{
			MessageBoxA(NULL, "334", e.errorMessage(), MB_OK);
		}
		

		return *this;
	}


	CHistoryMgr::~CHistoryMgr(){
	}


	CHistoryMgr::VRECORD& CHistoryMgr::Query(int query)
	{

		string sql;
		if(query == q_all)
		{
			sql = "select * from history;";
		}
		else if(query == q_thisweek)
		{
			sql = "select * from history where ADDDATE "
				"between datetime('now','start of week')"
				"and datetime('now');";
		}
		else if(query == q_thismonth)
		{
			sql = "select * from history where ADDDATE "
				"between datetime('now','start of month', '-1 second')"
				"and datetime('now');";
		}
		else if(query == q_rest)
		{
			sql = "select * from history where ADDDATE "
				"between datetime('1900-01-01 00:00:00')"
				"and datetime('now','start of month', '-1 second');";
		}
		else
		{
			throw std::exception("invalid parameter");
		}


		m_dbtable = m_db->getTable(sql.c_str());



		for (int j = 0; j < m_dbtable.numRows(); ++j)
		{
			RECORD rc;
			for (int i = 0; i < m_dbtable.numFields(); ++i)
			{
				m_dbtable.setRow(j);


				rc.id = m_dbtable.getIntField("ID");
				rc.folder = m_dbtable.getStringField("FOLDER");
				const  char* blob = m_dbtable.getStringField("IMG");
				CppSQLite3Binary bin;
				bin.setEncoded((const unsigned char*)blob);
				rc.img.assign((const char*)bin.getBinary(), bin.getBinaryLength());


				rc.title = m_dbtable.getStringField("TITLE");
				rc.url = m_dbtable.getStringField("URL");
				rc.time = m_dbtable.getStringField("ADDDATE");
			}
			m_result.push_back(rc);

		}


		return m_result;

	}

	xstring CHistoryMgr::TestRoutine()
	{
		return xstring();
	}





namespace SQLiteHelper
{
	int GetItem_CB(void *para, int argc, char **argv, char **azColName)
	{
		favor_recode_item* hri = (favor_recode_item*)para;
		for(int i=0; i<argc; i++){
			//Log("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			if(string(azColName[i]) == string("TITLE"))
				hri->title = argv[i] ? argv[i] : "";
			if(string(azColName[i]) == string("URL"))
				hri->url = argv[i] ? argv[i] : "";
			if(string(azColName[i]) == string("ID"))
				hri->id = atoi(argv[i]) ? atoi(argv[i]) : -1;
		}

		return 0;
	}

	int callback(void *para, int argc, char **argv, char **azColName)
	{
		FAVOR_LIST* hl = (FAVOR_LIST*)para;
		favor_recode_item hri;
		for(int i=0; i<argc; i++){
			//Log("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			if(string(azColName[i]) == string("TITLE"))
				hri.title = argv[i] ? argv[i] : "";
			if(string(azColName[i]) == string("URL"))
				hri.url = argv[i] ? argv[i] : "";
			if(string(azColName[i]) == string("ID"))
				hri.id = atoi(argv[i]) ? atoi(argv[i]) : -1;
		}
		hl->push_back(hri);

		return 0;
	}

	bool CreateTable(const string& strDBPath,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		string strSQL;

		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);

		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return false;
		}else{
			fprintf(stdout, "Opened database successfully\n");
		}

		/* Create SQL statement */
		strSQL = "CREATE TABLE "+ tablename + "("  \
			"ID	INTEGER PRIMARY KEY     NOT NULL," \
			"TITLE         TEXT     NOT NULL," \
			"URL            TEXT     NOT NULL)";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), callback, 0, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Table created successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	bool InsertItem(const string& strDBPath,const string& title,const string& url,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;

		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			exit(0);
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create SQL statement */
		strSQL = "INSERT INTO " + tablename + "(ID,TITLE,URL) ";
		strSQL = strSQL + "VALUES (NULL,'" + title + "','" + url + "');"; 

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), callback, 0, &zErrMsg);

		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Records created successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	bool GetAllItem(const string& strDBPath,FAVOR_LIST& fl,string tablename)
	{
		CreateTable(strDBPath,tablename);

		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;
		const char* data = "Callback function called";
		
		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create SQL statement */
		strSQL = "SELECT * from " + tablename;

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), callback, (void*)&fl, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	bool GetItem(const string& strDBPath,string title, favor_recode_item& fri,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;
		
		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create SQL statement */
		strSQL = "SELECT * from " + tablename + " WHERE TITLE='" + title + "'";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), GetItem_CB, (void*)&fri, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	bool ModifyItem(const string& strDBPath,const string& title,const string& newtitle,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;
		const char* data = "Callback function called";

		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			exit(0);
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create merged SQL statement */
		strSQL = "UPDATE "+ tablename + " set TITLE = " + newtitle + " where TITLE="+ title + "; " \
			"SELECT * from HISTROY";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), callback, (void*)data, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	bool DeleteItem(const string& strDBPath,const string& title,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;

		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			exit(0);
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create merged SQL statement */
		strSQL = "DELETE from "+ tablename + " where TITLE = '" + title + "'; " \
			"SELECT * from " + tablename;

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(),NULL,NULL, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return 0;
	}

	bool GetItem(const string& strDBPath,int id, favor_recode_item& fri,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;
		
		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return false;
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create SQL statement */
		strSQL = "SELECT * from " + tablename + " WHERE ID='" + to_string((long long)id) + "'";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), GetItem_CB, (void*)&fri, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return true;
	}

	bool ModifyItem(const string& strDBPath,int id,const string& newtitle,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;
		const char* data = "Callback function called";

		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return false;
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create merged SQL statement */
		strSQL = "UPDATE "+ tablename + " set TITLE = " + newtitle + " where ID="+ to_string((long long)id) + "; " \
			"SELECT * from HISTROY";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), callback, (void*)data, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return true;
	}

	bool DeleteItem(const string& strDBPath,int id,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;

		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return false;
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create merged SQL statement */
		strSQL = "DELETE from "+ tablename + " where ID = " + to_string((long long)id) + "; ";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), NULL, NULL, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return true;
	}

	bool DeleteAllItem(const string& strDBPath,string tablename)
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		string strSQL;

		/* Open database */
		rc = sqlite3_open(strDBPath.c_str(), &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return false;
		}else{
			fprintf(stderr, "Opened database successfully\n");
		}

		/* Create merged SQL statement */
		strSQL = "DELETE from "+ tablename + "; ";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, strSQL.c_str(), NULL, NULL, &zErrMsg);
		if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}else{
			fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		return true;
	}
};

const string CFavor::s_dbpath = "GMBrowser.db";
const string CFavor::s_tablename = "FAVOR";

CFavor::CFavor(void)
{
}


CFavor::~CFavor(void)
{
}

bool CFavor::GetAllItem(FAVOR_LIST& hl)
{
	return SQLiteHelper::GetAllItem(s_dbpath,hl,s_tablename);
}

bool CFavor::AddItem(const favor_recode_item& hri)
{
	return SQLiteHelper::InsertItem(s_dbpath,hri.title,hri.url,s_tablename);
}

bool CFavor::DeleteItem(string title)
{
	return SQLiteHelper::DeleteItem(s_dbpath,title,s_tablename);
}

bool CFavor::ModifyItem(string title, string newtitle)
{
	return SQLiteHelper::ModifyItem(s_dbpath,title,newtitle,s_tablename);
}

bool CFavor::GetItem(string title,favor_recode_item& fri)
{
	return SQLiteHelper::GetItem(s_dbpath,title,fri,s_tablename);
}


const string CHistory::s_dbpath = "GMBrowser.db";
const string CHistory::s_tablename = "HISTORY";
FAVOR_LIST CHistory::s_hl;

CHistory::CHistory(void)
{
}


CHistory::~CHistory(void)
{
}

bool CHistory::GetAllItem(FAVOR_LIST& hl)
{
	s_hl.clear();
	return SQLiteHelper::GetAllItem(s_dbpath,hl,s_tablename);
}

bool CHistory::AddItem(const favor_recode_item& hri)
{
	return SQLiteHelper::InsertItem(s_dbpath,hri.title,hri.url,s_tablename);
}

bool CHistory::DeleteItem(int index)
{
	return SQLiteHelper::DeleteItem(s_dbpath,s_hl[index].id,s_tablename);
}

bool CHistory::ModifyItem(int index, string newtitle)
{
	return SQLiteHelper::ModifyItem(s_dbpath,s_hl[index].id,newtitle,s_tablename);
}

bool CHistory::GetItem(int index,favor_recode_item& fri)
{
	return SQLiteHelper::GetItem(s_dbpath,s_hl[index].id,fri,s_tablename);
}

bool CHistory::DeleteAllItem()
{
	s_hl.clear();
	return SQLiteHelper::DeleteAllItem(s_dbpath,s_tablename);
}