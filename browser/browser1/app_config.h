#ifndef INCLUDE_APP_CONFIG_H
#define INCLUDE_APP_CONFIG_H


class AppConfig
{



	string regkey;
public:	
	AppConfig();
	void setRegKey(const string& keyname);
	virtual ~AppConfig();
};


class BrowserApp:public AppConfig
{
	CppSQLite3DB db;//关于browser的数据
	string       m_appdir;
	CFavorManager* m_favor;
	CHistoryMgr*   m_history;
	CRegKey        m_regedit;
public:
	BrowserApp();
	~BrowserApp();
	CHistoryMgr* History();
	CFavorManager* Favor();
};



extern BrowserApp theApp;

#endif