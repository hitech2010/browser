
#include "utilities.h"
#include "app_config.h"
#include "appframe.h"


bool isGMSSL(string &strt, bool &b)
{
	return false;
}


	AppConfig::AppConfig()
	{
		regkey = "GMbrowser";
	}
	void AppConfig::setRegKey(const string& keyname)
	{
		regkey = keyname;
	}

	AppConfig::~AppConfig(){};



	BrowserApp::BrowserApp():AppConfig()
	{
		char szPath[MAX_PATH];
		ULONG nSize = _countof(szPath);
		::GetModuleFileNameA(NULL, szPath, _countof(szPath));
		::PathAppendA(szPath, "..\\");
		m_appdir.assign(szPath);
		::PathAppendA(szPath, "gmbrowser.db");

		db.open(szPath);

		m_favor = new CFavorManager(&db);
		m_history = new CHistoryMgr(&db);
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



BrowserApp theApp;


int __stdcall _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)

{


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