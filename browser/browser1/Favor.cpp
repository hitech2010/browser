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

	vector<string>   CFavorManager::QueryFolders()
	{
		vector<string> folders;

		string sql = "select DISTINCT FOLDER from favor ";

		m_dbtable = m_db->getTable(sql.c_str());

		for (int j = 0; j < m_dbtable.numRows(); ++j)
		{
			string folder;
			for (int i = 0; i < m_dbtable.numFields(); ++i)
			{
				m_dbtable.setRow(j);


				
				folder = m_dbtable.getStringField("FOLDER");
				
			}
			folders.push_back(folder);

		}

		return folders;
	}

	 CFavorManager::VRECORD& CFavorManager::Query(int query )
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

	 CFavorManager::VRECORD& CFavorManager::Query(const string& keyword)
	 {
		 m_result.clear();

		 xstring sql;
		 sql.format("select * from favor where Title like '%%%s%%' or \
													URL like '%%%s%%'", keyword.c_str(), keyword.c_str());


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

	 CFavorManager& CFavorManager::QueryById(int id)
	 {

		 xstring sql;
		 if (id == -1)
		 {
			 sql.assign("SELECT * FROM    FAVOR WHERE   ID = (SELECT MAX(ID)  FROM FAVOR)");
		 }
		 else
		 {
			 sql.format("SELECT * FROM    FAVOR WHERE   ID = %d", id);
		 }


		 m_dbtable = m_db->getTable(sql.c_str());
		 m_result.clear();

		 
		 if (m_dbtable.numRows() != 1)
		 {
			 throw std::exception("no record found!");
		 }


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
		  
		 return *this;

	 }

	 CFavorManager& CFavorManager::Edit(const RECORD& newRecord)
	 {
		 xstring sql;
		 sql.format("update favor set TITLE=\"%s\",FOLDER=\"%s\",URL=\"%s\" where id = %d",
			 newRecord.title.c_str(),
			 newRecord.folder.c_str(),
			 newRecord.url.c_str(),
			 newRecord.id);

		 m_db->execDML(sql.c_str());

		 return *this;
	 }

	 const CFavorManager::VRECORD& CFavorManager::GetResult()
	 {
		 return m_result;
	 }

	 CFavorManager& CFavorManager::QueryByFolder(const string& folder)
	 {
		 xstring sql;
		 sql.format("select * from favor  where folder = \"%s\"", folder.c_str());

		 m_dbtable = m_db->getTable(sql.c_str());
		 m_result.clear();


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
					 ADDDATE DATETIME NOT NULL,\
					 SHORTCUT text);";

		db->execDML(sql.c_str());

	}


	CHistoryMgr& CHistoryMgr::Clear()
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
			"VALUES(NULL,%Q,%Q,%Q,%Q, datetime('now'),NULL);", 
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

	CHistoryMgr::VRECORD& CHistoryMgr::Query(const string& keyword)
	{
		m_result.clear();

		xstring sql;
		sql.format( "select * from history where Title like '%%%s%%' or \
			URL like '%%%s%%'", keyword.c_str(), keyword.c_str());


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
	CHistoryMgr::VRECORD& CHistoryMgr::Query(int query)
	{

		m_result.clear();
		string sql;
		if (query == -1)
		{
			sql = "SELECT * FROM    history WHERE   ID = (SELECT MAX(ID)  FROM history)";
		}

		else if(query == q_all)
		{
			sql = "select * from history;";
		}
		else if (query == q_today)
		{
			sql = "select * from history where ADDDATE "
				"between datetime('now','start of day')"
				"and datetime('now');";
		}
		else if (query == q_yesterday)
		{
			sql = "select * from history where ADDDATE "
				"between datetime('now','start of day','-1 days')"
				"and datetime('now','start of day', '-1 seconds');";
		}
		else if(query == q_thisweek)
		{
			sql = "select * from history where ADDDATE "
				"between datetime('now','start of day','-6 days')"
				"and datetime('now','start of day','-2 days','-1 seconds');";
		}
		else if(query == q_thismonth)
		{
			sql = "select * from history where ADDDATE "
				"between datetime('now','start of day', '-30 days')"
				"and datetime('now','start of day', '-6 days','-1 seconds');";
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


	CHistoryMgr& CHistoryMgr::Delete(const string& tuple)
	{
		xstring sql;
		sql.format("DELETE FROM HISTORY WHERE ID IN %s;", tuple.c_str());

		m_db->execDML(sql.c_str());
		return *this;
	}

	xstring CHistoryMgr::TestRoutine()
	{
		return xstring();
	}

	CHistoryMgr& CHistoryMgr::setTitle(string& url, string& title)
	{

		
		xstring sql;
		sql.format("update history set title = \"%s\" where URL = \"%s\"", title.c_str(), url.c_str());



		m_db->execDML(sql.c_str());

		return *this;


	}

	CHistoryMgr& CHistoryMgr::AddShotcut(string id, string filename)
	{

		xstring sql0 = "select * from history where shortcut is not NULL ORDER BY ID DESC liMIT 19";
		m_dbtable = m_db->getTable(sql0.c_str());
		int cnt = m_dbtable.numRows();


		xstring  tuple = "update history set SHORTCUT = NULL where SHORTCUT is not NULL and id not in (";
		for (int j = 0; j < m_dbtable.numRows(); ++j)
		{
			if (j != 0)
			{
				tuple.append(",");
			}

			 xstring tmp;
			m_dbtable.setRow(j);
			int id = m_dbtable.getIntField("ID");
			tmp.format("%d", id);
			tuple.append(tmp);
		}

		tuple.append(")");
		m_db->execDML(tuple.c_str());
		

		xstring sql;
		sql.format("update history set SHORTCUT = \"%s\" where id = \"%s\"", _encoding(filename).en_base64().get().c_str(), id.c_str());
		m_db->execDML(sql.c_str());
		return *this;

	}



	CHistoryMgr& CHistoryMgr::GetTop4ShortCut(map<int, string>& result)
	{

		xstring sql0 = "select * from history where shortcut is not NULL ORDER BY ID DESC liMIT 4";
		m_dbtable = m_db->getTable(sql0.c_str());
		int cnt = m_dbtable.numRows();


		
		for (int j = 0; j < m_dbtable.numRows(); ++j)
		{
			int nCnt = m_dbtable.numFields();

			xstring tmp;
			m_dbtable.setRow(j);

			result[j] = _encoding(m_dbtable.getStringField("SHORTCUT", "")).de_base64().get();
			result[j + 4] = m_dbtable.getStringField("URL", "#");
			result[j + 8] = m_dbtable.getStringField("TITLE", "#");
		
		}

		return *this;

	}



	CFavorFolder::CFavorFolder(CppSQLite3DB*db) throw()
	{

		m_db = db;

		string sql = "CREATE TABLE IF NOT EXISTS FAVOR_FOLDER(\
					ID integer primary key,\
					TITLE text NOT NULL);";
		

		db->execDML(sql.c_str());

	}

	CFavorFolder& CFavorFolder::QueryById(int id)
	{

		xstring sql;
		if (id == -1)
		{
			sql.assign("SELECT * FROM    FAVOR_FOLDER WHERE   ID = (SELECT MAX(ID)  FROM FAVOR_FOLDER)");
		}
		else
		{
			sql.format("SELECT * FROM    FAVOR_FOLDER WHERE   ID = %d", id);
		}


		m_dbtable = m_db->getTable(sql.c_str());
		m_result.clear();


		if (m_dbtable.numRows() != 1)
		{
			throw std::exception("no record found!");
		}


		for (int j = 0; j < m_dbtable.numRows(); ++j)
		{
			RECORD rc;
			for (int i = 0; i < m_dbtable.numFields(); ++i)
			{
				m_dbtable.setRow(j);
				rc.folder = m_dbtable.getStringField("TITLE");
				rc.id = m_dbtable.getIntField("ID");
			}
			m_result.push_back(rc);

		}

		return *this;

	}


	CFavorFolder& CFavorFolder::Clear()
	{
		string sql = "DELETE FROM FAVOR_FOLDER";
		m_db->execDML(sql.c_str());
		return *this;
	}

	CFavorFolder& CFavorFolder::Add(const RECORD& record)
	{

		CppSQLite3Buffer   sql;


		sql.format("INSERT INTO FAVOR_FOLDER "
			"VALUES(NULL,%Q);",
			record.folder.c_str());

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

	CFavorFolder& CFavorFolder::Delete(int startpos, int endpos)
	{
		xstring sql;
		sql.format("DELETE FROM FAVOR_FOLDER WHERE ID BETWEEN %d AND %d;", startpos, endpos);

		m_db->execDML(sql.c_str());
		return *this;
	}
	CFavorFolder::~CFavorFolder(){
	}


	CFavorFolder::VRECORD& CFavorFolder::Query()
	{

		string sql;
		
		sql = "select * from FAVOR_FOLDER;";
		
		m_result.clear();


		m_dbtable = m_db->getTable(sql.c_str());



		for (int j = 0; j < m_dbtable.numRows(); ++j)
		{
			RECORD rc;
			for (int i = 0; i < m_dbtable.numFields(); ++i)
			{
				m_dbtable.setRow(j);
				rc.folder = m_dbtable.getStringField("TITLE");
				rc.id = m_dbtable.getIntField("ID");
			}
			m_result.push_back(rc);

		}

		return m_result;

	}



	const CFavorFolder::VRECORD& CFavorFolder::GetResult()
	{
		return m_result;
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








