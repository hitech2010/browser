
#include "utilities.h"
#include <fstream>
#include "json/json.h"
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
		if (!reader.parse(ifs, root, false))
		{
			throw std::exception("imcomplete ini file");
		}

		try
		{
			setAskBeforeClose(root["appconfig"]["ask_before_close"].asString());
			
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
			ofs.open(theApp.getConfigFile().c_str());

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
	pFrame->m_engine->Add();

	
	// 显示
	CPaintManagerUI::MessageLoop(); // 消息循环

	//delete pFrame; // 删除对象


	return 0;
}