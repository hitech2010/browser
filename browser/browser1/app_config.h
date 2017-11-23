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
	string m_startup_page_policy; // {"0":"�ϴ�ҳ��"��"1","��ҳ"�� "2","һ��ҳ��"}

	vector<SEARCHENGINE_CONFIG>  m_search_engines;

	SEARCHENGINE_CONFIG  m_default_search_engine;
	string m_ui_show_homepage;//{"0","����"��"1","��ʾ"}
	string m_ui_show_bookmark;//{"0","����"��"1","��ʾ"}
	string m_ui_show_shortcut; //{"0", "����"��"1", "��ʾ"}
	string m_ui_show_restore_recent;//{"0","����"��"1","��ʾ"}

//��ǩ����
	string m_tabset_qiantaitiaozhuan;//{"0","�������ʱǰ̨��ʾ"��"1","�������ʱ��̨��ʾ"}
	string m_tabset_close_dblclick;		//{"0","��","1","˫���ر�",
	string m_tabset_close_rightclick;		//"0","��"��"1","˫���ر�"}
	string m_tabset_quit_whencloselast;   //"0","�ر�����ǩ���ر�"��"1","�ر�����ǩ�ر������"}
	string m_tabset_newtab_whenclickbookmark;//"0","�����ǩ���ǵ�ǰҳ","1","�����ǩ�´��ڴ�"}
	string m_tabset_newtab_navigateaddress; //"0","��ַ���򿪵�ǰҳ","1","��ַ�����±�ǩҳ"}
	string m_tabset_newtab_position;			//"0","�Ҳཨ���±�ǩ/Ĭ�����","1","��ཨ���±�ǩ"}
	string m_tabset_activepos_whenclosetab;		//"0","�������","1","�����Ҳ�"}
//��������	
	string m_dnload_location;					//����λ��
	string m_dnload_ask_befroe_newtask;			//��������ʾ
	string m_dnload_info_whenover;				//��ɺ���ʾ

//��ͼ����
	string m_shortcut_ext;						//��ͼ��ʽ
	string m_location;							//��ͼλ��
public:
	void Lock();
	void Unlock();
	void config(const string & key,const string& value);
	string config(const string& key);


public:	
	AppConfig();
	virtual ~AppConfig();

	//��ҳ]
	string get_startup_page_policy();
	string get_ui_show_homepage();
	string get_ui_show_bookmark();
	string get_ui_show_shortcut();
	string get_ui_show_restore_recent();




	AppConfig::SEARCHENGINE_CONFIG get_default_searchengine();
	vector<AppConfig::SEARCHENGINE_CONFIG> get_all_searchengine();

	//��ǩ����
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




	//��������

	string get_dnload_location();
	string get_dnload_ask_befroe_newtask();
	string get_dnload_info_whenover();




	//��ͼ����

	string get_shortcut_ext();
	string get_location();


	




	void setRegKey(const string& keyname);

	bool IfAskBeforeClose();
	string getAskBeforeClose();
	string getConfigFile();

};




class BrowserApp :public AppConfig
{

	

	CppSQLite3DB db;//����browser������
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