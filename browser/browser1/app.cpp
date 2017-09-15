
#include "utilities.h"
#include <fstream>

#include "app_config.h"
#include "appframe.h"


#pragma  comment(lib, "jsoncpp.lib")

bool isGMSSL(string &strt, bool &b)
{
	return false;
}





	AppConfig::AppConfig()
	{
		inifile = "gmbrowser.ini";
		regkey = "GMbrowser";
		m_ask_before_close = "yes";
		m_startup_page_policy="1"; // {"0":"上次页面"，"1","主页"， "2","一组页面"}
		//m_search_engines;
		m_default_search_engine["百度"] = "http://baidu.com";
		m_ui_show_homepage="0";//{"0","隐藏"，"1","显示"}
		m_ui_show_bookmark="1";//{"0","隐藏"，"1","显示"}
		m_ui_show_shortcut="0"; //{"0", "隐藏"，"1", "显示"}
		m_ui_show_restore_recent="1";//{"0","隐藏"，"1","显示"}
		//标签设置
		m_tabset_qiantaitiaozhuan="0";//{"0","点击链接时前台显示"，"1","点击链接时后台显示"}
		m_tabset_close_dblclick="0";		//{"0","无","1","双击关闭",
		m_tabset_close_rightclick="0";		//"0","无"，"1","双击关闭"}
		m_tabset_quit_whencloselast="1";   //"0","关闭最后标签不关闭"，"1","关闭最后标签关闭浏览器"}
		m_tabset_newtab_whenclickbookmark="1";//"0","点击书签覆盖当前页","1","点击书签新窗口打开"}
		m_tabset_newtab_navigateaddress="1"; //"0","地址栏打开当前页","1","地址栏打开新标签页"}
		m_tabset_newtab_position="0";			//"0","右侧建立新标签/默认情况","1","左侧建立新标签"}
		m_tabset_activepos_whenclosetab="1";		//"0","激活左侧","1","激活右侧"}
		//下载设置	
		m_dnload_location="C:\\downloads\\";					//下载位置
		m_dnload_ask_befroe_newtask="1";			//新任务提示
		m_dnload_info_whenover="1";				//完成后提示

		//截图设置
		m_shortcut_ext="png";						//截图格式
		m_location="D:\\downloads\\";							//截图位置


		defaultconfig = "{\
			\"appname\":\"gmbrowser\",\
			\"appconfig\" :\
		{\
			\"ask_before_close\":\"yes\",\
				\"startup_page_policy\" : \"1\",\
				\"ui_show_homepage\" : \"0\",\
				\"ui_show_bookmark\" : \"1\",\
				\"ui_show_shortcut\" : \"0\",\
				\"ui_show_restore_recent\" : \"1\",\
				\"tabset_qiantaitiaozhuan\" : \"0\",\
				\"tabset_close_dblclick\" : \"0\",\
				\"tabset_close_rightclick\" : \"0\",\
				\"tabset_quit_whencloselast\" : \"1\",\
				\"tabset_newtab_whenclickbookmark\" : \"1\",\
				\"tabset_newtab_navigateaddress\" : \"1\",\
				\"tabset_newtab_position\" : \"0\",\
				\"tabset_activepos_whenclosetab\" : \"1\",\
				\"dnload_location\" : \"c:\\download\\\",\
				\"dnload_ask_befroe_newtask\" : \"1\",\
				\"dnload_info_whenover\" : \"1\",\
				\"shortcut_ext\" : \"png\",\
				\"location\" : \"D:\\shortcut\\\"\}}";
	}
	void AppConfig::setRegKey(const string& keyname)
	{
		regkey = keyname;
	}

	AppConfig::~AppConfig(){};


	void AppConfig::setAskBeforeClose(const string& value)
	{
		m_ask_before_close = value;
	}

	bool AppConfig::IfAskBeforeClose()
	{
		return m_ask_before_close == "yes";
	}
	string AppConfig::getAskBeforeClose()
	{
		return m_ask_before_close;
	}

	string AppConfig::getConfigFile()
	{
		return inipath;
	}




	string AppConfig::get_ui_show_homepage()
	{
		return m_ui_show_homepage;
	}

	string AppConfig::get_startup_page_policy()
	{
		return m_startup_page_policy;

	}




	string AppConfig::get_ui_show_bookmark()
	{
		return m_ui_show_bookmark;
	}

	string AppConfig::get_ui_show_shortcut()
	{
		return m_ui_show_shortcut;
	}

	string AppConfig::get_ui_show_restore_recent()
	{
		return m_ui_show_restore_recent;
	}

	void AppConfig::set_ui_show_homepage(const string& a)
	{
		m_ui_show_homepage = a;
	}


	void AppConfig::set_startup_page_policy(const string& a)
	{
		m_startup_page_policy = a;
	}

	void AppConfig::set_ui_show_bookmark(const string& a)
	{
		m_ui_show_bookmark = a;
	}

	void AppConfig::set_ui_show_shortcut(const string& a)
	{
		m_ui_show_shortcut = a;
	}

	void AppConfig::set_ui_show_restore_recent(const string& a)
	{
		m_ui_show_restore_recent = a;
	}

	AppConfig::SEARCHENGINE_CONFIG AppConfig::get_default_searchengine()
	{
		return m_default_search_engine;
	}

	vector<AppConfig::SEARCHENGINE_CONFIG> AppConfig::get_all_searchengine()
	{
		return m_search_engines;
	}

	string AppConfig::get_tabset_qiantaitiaozhuan()
	{
		return m_tabset_qiantaitiaozhuan;
	}

	string AppConfig::get_tabset_close_dblclick()
	{
		return m_tabset_close_dblclick;
	}

	string AppConfig::get_tabset_close_rightclick()
	{
		return m_tabset_close_rightclick;
	}



	string AppConfig::get_tabset_newtab_whenclickbookmark()
	{
		return m_tabset_newtab_whenclickbookmark;
	}

	string AppConfig::get_tabset_newtab_navigateaddress()
	{
		return m_tabset_newtab_navigateaddress;
	}

	string AppConfig::get_tabset_newtab_position()
	{
		return m_tabset_newtab_position;
	}

	string AppConfig::get_tabset_activepos_whenclosetab()
	{
		return m_tabset_activepos_whenclosetab;
	}

	void AppConfig::set_tabset_qiantaitiaozhuan(const string& a)
	{
		m_tabset_qiantaitiaozhuan = a;
	}

	void AppConfig::set_tabset_close_dblclick(const string& a)
	{
		m_tabset_close_dblclick = a;
	}

	void AppConfig::set_tabset_close_rightclick(const string& a)
	{
		m_tabset_close_rightclick = a;
	}

	void AppConfig::set_tabset_quit_whencloselast(const string& a)
	{
		m_tabset_quit_whencloselast = a;
	}

	void AppConfig::set_tabset_newtab_whenclickbookmark(const string& a)
	{
		m_tabset_newtab_whenclickbookmark = a;;
	}

	void AppConfig::set_tabset_newtab_navigateaddress(const string& a)
	{
		m_tabset_newtab_navigateaddress = a;
	}

	void AppConfig::set_tabset_newtab_position(const string& a)
	{
		m_tabset_newtab_position = a;;
	}

	void AppConfig::set_tabset_activepos_whenclosetab(const string& a)
	{
		m_tabset_activepos_whenclosetab = a;;
	}

	string AppConfig::get_dnload_location()
	{
		return m_dnload_location;
	}

	string AppConfig::get_dnload_ask_befroe_newtask()
	{
		return m_dnload_ask_befroe_newtask;
	}

	string AppConfig::get_dnload_info_whenover()
	{
		return m_dnload_info_whenover;
	}

	void AppConfig::set_dnload_location(const string& a)
	{
		m_dnload_location = a;
	}

	void AppConfig::set_dnload_ask_befroe_newtask(const string& a)
	{
		m_dnload_ask_befroe_newtask = a;
	}

	void AppConfig::set_dnload_info_whenover(const string& a)
	{
		m_dnload_info_whenover = a;
	}

	string AppConfig::get_shortcut_ext()
	{
		return m_shortcut_ext;
	}

	string AppConfig::get_location()
	{
		return m_location;
	}



	void AppConfig::set_shortcut_ext(const string& a)
	{
		m_shortcut_ext = a;;
	}

	void AppConfig::set_location(const string& a)
	{
		m_location = a;
	}


	BrowserApp::BrowserApp():AppConfig()
	{
		char szPath[MAX_PATH];
		ULONG nSize = _countof(szPath);
		::GetModuleFileNameA(NULL, szPath, _countof(szPath));
		::PathAppendA(szPath, "..\\");
		m_appdir.assign(szPath);
		::PathAppendA(szPath, "gmbrowser.db");

		db.open(szPath);

		inipath = m_appdir + inifile;

		m_favor = new CFavorManager(&db);
		m_history = new CHistoryMgr(&db);
		m_favorfolder = new CFavorFolder(&db);


		ifstream ifs;
		ifs.open(inipath.c_str());

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(ifs, m_jroot, false))
		{
			throw std::exception("imcomplete ini file");
			reader.parse(defaultconfig, m_jroot, true);
		}

		try
		{

			
			
			setAskBeforeClose(m_jroot["appconfig"]["ask_before_close"].asString());
			set_startup_page_policy(m_jroot["appconfig"]["startup_page_policy"].asString());
			set_ui_show_homepage(m_jroot["appconfig"]["ui_show_homepage"].asString());
			set_ui_show_bookmark(m_jroot["appconfig"]["ui_show_bookmark"].asString());
			set_ui_show_shortcut(m_jroot["appconfig"]["ui_show_shortcut"].asString());
			set_ui_show_restore_recent(m_jroot["appconfig"]["ui_show_restore_recent"].asString());
			set_tabset_qiantaitiaozhuan(m_jroot["appconfig"]["tabset_qiantaitiaozhuan"].asString());
			set_tabset_close_dblclick(m_jroot["appconfig"]["tabset_close_dblclick"].asString());
			set_tabset_close_rightclick(m_jroot["appconfig"]["tabset_close_rightclick"].asString());
			set_tabset_quit_whencloselast(m_jroot["appconfig"]["tabset_quit_whencloselast"].asString());
			set_tabset_newtab_whenclickbookmark(m_jroot["appconfig"]["tabset_newtab_whenclickbookmark"].asString());
			set_tabset_newtab_navigateaddress(m_jroot["appconfig"]["tabset_newtab_navigateaddress"].asString());
			set_tabset_newtab_position(m_jroot["appconfig"]["tabset_newtab_position"].asString());
			set_tabset_activepos_whenclosetab(m_jroot["appconfig"]["tabset_activepos_whenclosetab"].asString());
			set_dnload_location(m_jroot["appconfig"]["dnload_location"].asString());
			set_dnload_ask_befroe_newtask(m_jroot["appconfig"]["dnload_ask_befroe_newtask"].asString());
			set_dnload_info_whenover(m_jroot["appconfig"]["dnload_info_whenover"].asString());
			set_shortcut_ext(m_jroot["appconfig"]["shortcut_ext"].asString());
			set_location(m_jroot["appconfig"]["location"].asString());
			
			ifs.close();
			
		}
		catch (...)
		{
			throw std::exception("wrong ini file");
		}
		



	}
	BrowserApp::~BrowserApp()
	{
		db.close();
	}

	CHistoryMgr* BrowserApp::History()
	{
		return m_history;
	}
	CFavorManager* BrowserApp::Favor()
	{
		
		return m_favor;
	}


	CFavorFolder* BrowserApp::FavorFolder()
	{
		return m_favorfolder;
	}


	string BrowserApp::getAppdir()
	{
		return m_appdir;
	}

	Json::Value& BrowserApp::getJsonValue()
	{
		return m_jroot;
	}




BrowserApp theApp;

unsigned int __stdcall  sync_setting_proc(void* para)
{

	while (1)
	{

		

		try
		{
			
			Json::Reader reader;
			Json::Value  root;
			Json::StreamWriterBuilder b;



			root["appname"] = "gmbrowser";
			root["appconfig"]["ask_before_close"] = theApp.getAskBeforeClose();

			string result = Json::writeString(b, root);
			ofstream ofs;
			//ofs.open(theApp.getConfigFile().c_str());

			if (ofs.is_open())
			{
				ofs << result.c_str();
				ofs.close();
			}

		}
		catch (...)
		{
			
		}
		Sleep(100);
	}

	return 0;

}

int __stdcall _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)

{

	_beginthreadex(NULL, 0, sync_setting_proc, NULL, 0, NULL);


	string a;
	a.resize(79);


	//string tmp = theApp.Favor()->TestRoutine().c_str();
	//startproxy();
	
	CPaintManagerUI::SetInstance(hInstance);// 加载XML的时候，需要使用该句柄去定位EXE的路径，才能加载XML的路径
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	
	CFrameWindowWnd* pFrame = new CFrameWindowWnd; // 生成对象
	pFrame->Create(NULL, _T("极密浏览器"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow(); // 窗口居中
	pFrame->ShowWindow(true);
	pFrame->m_engine->Add(pFrame->m_engine->getIndexPage());

	
	// 显示
	CPaintManagerUI::MessageLoop(); // 消息循环

	//delete pFrame; // 删除对象


	return 0;
}