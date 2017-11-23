#ifndef INCLUDE_APP_CONFIG_H
#define INCLUDE_APP_CONFIG_H

#include "json/json.h"

#include <atlstr.h>
#include <atlimage.h>

string HttpDownloadEvent( std::map<string,string>& para);
string HttpWebShortcutEvent( std::map<string, string>& para);
unsigned int __stdcall  sync_setting_proc(void* para);

#define str_a(a) #a



class AppConfig
{

public:
	string defaultconfig;
	string inifile;
	string regkey;
	string inipath;

	Json::Value   m_jroot;
	CRITICAL_SECTION m_cs;

	

	typedef vector<string> SEARCHENGINE_CONFIG;
	
private:
	
	string m_ask_before_close;
	string m_startup_page_policy; // {"0":"上次页面"，"1","主页"， "2","一组页面"}

	vector<SEARCHENGINE_CONFIG>  m_search_engines;

	SEARCHENGINE_CONFIG  m_default_search_engine;
	string m_ui_show_homepage;//{"0","隐藏"，"1","显示"}
	string m_ui_show_bookmark;//{"0","隐藏"，"1","显示"}
	string m_ui_show_shortcut; //{"0", "隐藏"，"1", "显示"}
	string m_ui_show_restore_recent;//{"0","隐藏"，"1","显示"}

//标签设置
	string m_tabset_qiantaitiaozhuan;//{"0","点击链接时前台显示"，"1","点击链接时后台显示"}
	string m_tabset_close_dblclick;		//{"0","无","1","双击关闭",
	string m_tabset_close_rightclick;		//"0","无"，"1","双击关闭"}
	string m_tabset_quit_whencloselast;   //"0","关闭最后标签不关闭"，"1","关闭最后标签关闭浏览器"}
	string m_tabset_newtab_whenclickbookmark;//"0","点击书签覆盖当前页","1","点击书签新窗口打开"}
	string m_tabset_newtab_navigateaddress; //"0","地址栏打开当前页","1","地址栏打开新标签页"}
	string m_tabset_newtab_position;			//"0","右侧建立新标签/默认情况","1","左侧建立新标签"}
	string m_tabset_activepos_whenclosetab;		//"0","激活左侧","1","激活右侧"}
//下载设置	
	string m_dnload_location;					//下载位置
	string m_dnload_ask_befroe_newtask;			//新任务提示
	string m_dnload_info_whenover;				//完成后提示

//截图设置
	string m_shortcut_ext;						//截图格式
	string m_location;							//截图位置
public:
	void Lock();
	void Unlock();
	void config(const string & key,const string& value);
	string config(const string& key);


public:	
	AppConfig();
	virtual ~AppConfig();

	//首页]
	string get_startup_page_policy();
	string get_ui_show_homepage();
	string get_ui_show_bookmark();
	string get_ui_show_shortcut();
	string get_ui_show_restore_recent();




	AppConfig::SEARCHENGINE_CONFIG get_default_searchengine();
	vector<AppConfig::SEARCHENGINE_CONFIG> get_all_searchengine();

	//标签设置
	string get_tabset_qiantaitiaozhuan();
	string get_tabset_close_dblclick();
	string get_tabset_close_rightclick();
	string get_tabset_quit_whencloselast();
	string get_tabset_newtab_whenclickbookmark();
	string get_tabset_newtab_navigateaddress();
	string get_tabset_newtab_position();
	string get_tabset_activepos_whenclosetab();

	string get_home_page();
	string get_user_startpage();




	//下载设置

	string get_dnload_location();
	string get_dnload_ask_befroe_newtask();
	string get_dnload_info_whenover();




	//截图设置

	string get_shortcut_ext();
	string get_location();


	




	void setRegKey(const string& keyname);

	bool IfAskBeforeClose();
	string getAskBeforeClose();
	string getConfigFile();

};




class BrowserApp :public AppConfig
{

	

	CppSQLite3DB db;//关于browser的数据
	string       m_appdir;
	string       m_appfile;
	CFavorManager* m_favor;
	CFavorFolder*  m_favorfolder;
	CHistoryMgr*   m_history;
	CRegKey        m_regedit;
	

	CGeeMeeEventPool m_pool;



	
public:

	BrowserApp();
	~BrowserApp();

	void FixIEEmulation();
	CHistoryMgr* History();
	BrowserApp& SetDefaultBrowser();
	CFavorManager* Favor();
	CFavorFolder*  FavorFolder();
	string         getAppdir();
	string         getAppFile();
	Json::Value&   getJsonValue();



	void			RunSelf();

	CGeeMeeEventPool& Pool();

};








extern BrowserApp theApp;

#endif