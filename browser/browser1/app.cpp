
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
	
	CPaintManagerUI::SetInstance(hInstance);// ����XML��ʱ����Ҫʹ�øþ��ȥ��λEXE��·�������ܼ���XML��·��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	
	CFrameWindowWnd* pFrame = new CFrameWindowWnd; // ���ɶ���
	pFrame->Create(NULL, _T("���������"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow(); // ���ھ���
	pFrame->ShowWindow(true);
	pFrame->m_engine->Add();

	
	// ��ʾ
	CPaintManagerUI::MessageLoop(); // ��Ϣѭ��

	//delete pFrame; // ɾ������


	return 0;
}