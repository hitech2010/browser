
#include "utilities.h"
#include <fstream>

#include "app_config.h"
#include "appframe.h"
//winhttp
#include <winhttp.h>
#pragma  comment(lib, "Winhttp.lib")
#pragma  comment(lib, "jsoncpp.lib")


 #pragma  comment(lib, "gmssld.lib")
 #pragma  comment(lib, "http_proxy_serverd.lib")
 #pragma  comment(lib, "libeay32mtd.lib")
 #pragma  comment(lib, "mdsmd.lib")
 #pragma  comment(lib, "smd.lib")
//
//bool isGMSSL(string &strt, bool &b)
//{
//	return false;
//}


string HttpWebShortcutEvent( std::map<std::string, string>& para)
{
	
	Sleep(100);
	string para_hwnd = "hwnd";
	string para_file = "filename";
	
	HWND wnd = (HWND)atoi(para[para_hwnd].c_str());
	string file = para[para_file];
	
	
	if(!IsWindowVisible(wnd))
	{
		return "continue";
	}

	RECT rc;
	GetWindowRect(wnd, &rc);

	HDC hDCSrc = GetDC(wnd);

	if (hDCSrc)
	{
		//// �õ���ǰ��ʾ�豸��ˮƽ����

		
		int nWidth = rc.right - rc.left;
		int nHeight = rc.bottom - rc.top;

		int nBitPerPixel = GetDeviceCaps(hDCSrc, BITSPIXEL);//��ȡ��ÿ�����ص�bit��Ŀ
		
		
		CImage cImage;//ʹ��CImage��ʡ�ö��ͼ�Ĵ���  


		cImage.Create(nWidth, nHeight, nBitPerPixel);
		BitBlt(cImage.GetDC(), 0, 0, nWidth, nHeight, hDCSrc, 0, 0, SRCCOPY);


// 		CImage smallImage;
// 		bool bRet = smallImage.Create(336, 200, nBitPerPixel);
// 		SetStretchBltMode(smallImage.GetDC(), COLORONCOLOR);
// 		 bRet = cImage.StretchBlt(smallImage.GetDC(), 0, 0, 336, 200, SRCCOPY);
// 
// 		//ֱ�ӱ����  
		HRESULT hr = cImage.Save(_encoding(file).a_utf16().getutf16().c_str(), Gdiplus::ImageFormatPNG);
		if (SUCCEEDED(hr))
		{
			theApp.History()->AddShotcut(para["id"], file);
		}


// 
// 		
// 		smallImage.ReleaseDC();
// 		smallImage.ReleaseDC();

		cImage.ReleaseDC();//��ͼ�Ĵ��뵽����ͽ�����  
		::ReleaseDC(NULL, hDCSrc);
	}

	return "ok";
}

string HttpDownloadEvent( std::map<string, string>& para)
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	string para_url = "url";
	string tmp = para[para_url];
	tmp = tmp.substr(tmp.find_first_not_of("http://"));
	tmp = tmp.substr(tmp.find_first_not_of("https://"));
	string getpara;

	if (-1 != tmp.find_first_of("/"))
	{
		getpara = tmp.substr(tmp.find_first_of("/"));
		tmp = tmp.substr(0, tmp.find_first_of("/"));
	}
	else
	{
		getpara = "/";
	}
	




	string response;

	do
	{
		// Use WinHttpOpen to obtain a session handle.
		hSession = WinHttpOpen(L"WinHTTP Example/1.0",
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS, 0);

		// Specify an HTTP server.
		if (hSession)
			hConnect = WinHttpConnect(hSession, _encoding(tmp).u8_utf16().getutf16().c_str(),
			INTERNET_DEFAULT_HTTPS_PORT, 0);
		else break;

		// Create an HTTP request handle.
		if (hConnect)
			hRequest = WinHttpOpenRequest(hConnect, L"GET", _encoding(getpara).u8_utf16().getutf16().c_str(),
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE);
		else break;

		// Send a request.
		if (hRequest)
			bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS, 0,
			WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);
		else break;


		// End the request.
		if (bResults)
			bResults = WinHttpReceiveResponse(hRequest, NULL);
		else break;

		// Keep checking for data until there is nothing left.
		if (bResults)
		{
			do
			{
				// Check for available data.
				dwSize = 0;
				if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				{
					break;
				}

				// Allocate space for the buffer.
				pszOutBuffer = new char[dwSize + 1];
				if (!pszOutBuffer)
				{
					printf("Out of memory\n");
					response = "";
					dwSize = 0;
				}
				else
				{
					// Read the data.
					ZeroMemory(pszOutBuffer, dwSize + 1);

					if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
						dwSize, &dwDownloaded))
					{
						response = "";
					}
						
					string tmp;
					tmp.assign(pszOutBuffer, dwDownloaded);
					response.append(tmp);
					// Free the memory allocated to the buffer.
					delete[] pszOutBuffer;
				}
			} while (dwSize > 0);
		}


		// Report any errors.
		if (!bResults)
			printf("Error %d has occurred.\n", GetLastError());




	} while (false);

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	const char* data = response.c_str();
	unsigned int sz = response.size();

	ofstream of;
	of.open("b.ico", ios::trunc);
	of.write(data, sz);
	of.close();
	return response;

}



int nExitFlag;




	AppConfig::AppConfig()
	{
		InitializeCriticalSection(&m_cs);


		inifile = "gmbrowser.ini";
		regkey = "GMbrowser";
		m_ask_before_close = "yes";
		m_startup_page_policy="1"; // {"0":"�ϴ�ҳ��"��"1","��ҳ"�� "2","һ��ҳ��"}
		//m_search_engines;
		m_default_search_engine["�ٶ�"] = "http://baidu.com";
		m_ui_show_homepage="0";//{"0","����"��"1","��ʾ"}
		m_ui_show_bookmark="1";//{"0","����"��"1","��ʾ"}
		m_ui_show_shortcut="0"; //{"0", "����"��"1", "��ʾ"}
		m_ui_show_restore_recent="1";//{"0","����"��"1","��ʾ"}
		//��ǩ����
		m_tabset_qiantaitiaozhuan="0";//{"0","�������ʱǰ̨��ʾ"��"1","�������ʱ��̨��ʾ"}
		m_tabset_close_dblclick="0";		//{"0","��","1","˫���ر�",
		m_tabset_close_rightclick="0";		//"0","��"��"1","˫���ر�"}
		m_tabset_quit_whencloselast="1";   //"0","�ر�����ǩ���ر�"��"1","�ر�����ǩ�ر������"}
		m_tabset_newtab_whenclickbookmark="1";//"0","�����ǩ���ǵ�ǰҳ","1","�����ǩ�´��ڴ�"}
		m_tabset_newtab_navigateaddress="1"; //"0","��ַ���򿪵�ǰҳ","1","��ַ�����±�ǩҳ"}
		m_tabset_newtab_position="0";			//"0","�Ҳཨ���±�ǩ/Ĭ�����","1","��ཨ���±�ǩ"}
		m_tabset_activepos_whenclosetab="1";		//"0","�������","1","�����Ҳ�"}
		//��������	
		m_dnload_location="C:\\downloads\\";					//����λ��
		m_dnload_ask_befroe_newtask="1";			//��������ʾ
		m_dnload_info_whenover="1";				//��ɺ���ʾ

		//��ͼ����
		m_shortcut_ext="png";						//��ͼ��ʽ
		m_location="D:\\downloads\\";							//��ͼλ��


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
				\"location\" : \"D:\\shortcut\\\"}}";
	}
	void AppConfig::setRegKey(const string& keyname)
	{
		regkey = keyname;
	}

	AppConfig::~AppConfig()
	{
		DeleteCriticalSection(&m_cs);
	};

	void AppConfig::Lock()
	{
		EnterCriticalSection(&m_cs);
	}
	void AppConfig::Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

	void AppConfig::config(const string & key,const string& value)
	{
		
		m_jroot["appconfig"][key.c_str()] = value;
	}


	string AppConfig::config(const string& key)
	{
		return m_jroot["appconfig"][key.c_str()].asString();
	}




	bool AppConfig::IfAskBeforeClose()
	{
		bool bret =  config("ask_before_close") == "yes";

		return bret;
	}
	string AppConfig::getAskBeforeClose()
	{
		return config("ask_before_close");
	}

	string AppConfig::getConfigFile()
	{
		return inipath;
	}




	string AppConfig::get_ui_show_homepage()
	{
		return config("ui_show_homepage");
	}

	string AppConfig::get_startup_page_policy()
	{
		return config("startup_page_policy");

	}

	string AppConfig::get_ui_show_bookmark()
	{
		return config("ui_show_bookmark"); 
	}

	string AppConfig::get_ui_show_shortcut()
	{
		return config("ui_show_shortcut");
;
	}

	string AppConfig::get_ui_show_restore_recent()
	{
		return config("ui_show_restore_recent");

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
		return config("tabset_qiantaitiaozhuan");
	}

	string AppConfig::get_tabset_close_dblclick()
	{
		return config("tabset_close_dblclick");
;
	}

	string AppConfig::get_tabset_close_rightclick()
	{
		return config("tabset_close_rightclick");

	}



	string AppConfig::get_tabset_newtab_whenclickbookmark()
	{
		return config("tabset_newtab_whenclickbookmark");

	}

	string AppConfig::get_tabset_newtab_navigateaddress()
	{
		return config("tabset_newtab_navigateaddress");

	}

	string AppConfig::get_tabset_newtab_position()
	{
		return config("tabset_newtab_position");

	}

	string AppConfig::get_tabset_activepos_whenclosetab()
	{
		return config("tabset_activepos_whenclosetab");

	}


	string AppConfig::get_dnload_location()
	{
		return config("dnload_location");

	}

	string AppConfig::get_home_page()
	{
		return config("home_page");
	}

	string AppConfig::get_user_startpage()
	{
		return config("user_startpage");
	}

	string AppConfig::get_dnload_ask_befroe_newtask()
	{
		return config("dnload_ask_befroe_newtask");
	}

	string AppConfig::get_dnload_info_whenover()
	{
		return config("dnload_ask_befroe_newtask");
	}



	string AppConfig::get_shortcut_ext()
	{
		return config("shortcut_ext");
	}

	string AppConfig::get_location()
	{
		return config("location");
	}





	BrowserApp::BrowserApp():AppConfig()
	{

		Log("BrowserApp::1");
		char szPath[MAX_PATH];
		ULONG nSize = _countof(szPath);
		::GetModuleFileNameA(NULL, szPath, _countof(szPath));
		::PathAppendA(szPath, "..\\");
		m_appdir.assign(szPath);
		::PathAppendA(szPath, "gmbrowser.db");

		Log("BrowserApp::2 %s", szPath);
		xstring tmp = szPath;

		string utf8path = _encoding(tmp).a_utf16().utf8().get();

		

		try
		{
			db.open(utf8path.c_str());
		}
		catch (CppSQLite3Exception& e)
		{
			MessageBoxA(NULL, e.errorMessage(), "�����쳣", MB_OK);

		}
		
			



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

			

			
			
			ifs.close();
			
		}
		catch (...)
		{
			MessageBoxA(NULL, "wrong ini file", NULL, MB_OK);
			// throw std::exception("wrong ini file");
		}
		

		Log("BrowserApp::7");

	}

	CGeeMeeEventPool& BrowserApp::Pool()
	{
		return m_pool;
	}

	BrowserApp::~BrowserApp()
	{
		if(m_favor) delete m_favor;m_favor = NULL;
		if(m_history) delete m_history;m_history = NULL;
		if(m_favorfolder) delete m_favorfolder;m_favorfolder = NULL;

		db.close();




		Log("~BrowserApp()");
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

	void BrowserApp::RunSelf()
	{

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		TCHAR pathfile[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, pathfile, MAX_PATH);

		// Start the child process. 
		if (!CreateProcess(NULL,   // No module name (use command line)
			pathfile,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{

			return;
		}



	}




BrowserApp theApp;

unsigned int __stdcall  sync_setting_proc(void* para)
{

	if (1)
	{

		

		try
		{
			
			Json::Reader reader;

			Json::StreamWriterBuilder b;


			string result = Json::writeString(b, theApp.getJsonValue());
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

	}

	return 0;

}

int __stdcall _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)

{

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	//_beginthreadex(NULL, 0, sync_setting_proc, NULL, 0, NULL);
	map<string, string> para;

	Log("_tWinMain(");

// 	string a;
// 	a.resize(79);


	//string tmp = theApp.Favor()->TestRoutine().c_str();
	startproxy();
	
	CPaintManagerUI::SetInstance(hInstance);// ����XML��ʱ����Ҫʹ�øþ��ȥ��λEXE��·�������ܼ���XML��·��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	
	CFrameWindowWnd* pFrame = new CFrameWindowWnd; // ���ɶ���
	pFrame->Create(NULL, _T("���������"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);


	pFrame->CenterWindow(); // ���ھ���
	pFrame->ShowWindow(true);
	pFrame->m_engine->Add(pFrame->m_engine->getIndexPage());

	
	// ��ʾ
	CPaintManagerUI::MessageLoop(); // ��Ϣѭ��

	delete pFrame; // ɾ������


	return 0;
}