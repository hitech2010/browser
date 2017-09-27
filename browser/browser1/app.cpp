
#include "utilities.h"
#include <fstream>

#include "app_config.h"
#include "appframe.h"
//winhttp
#include <winhttp.h>
#pragma  comment(lib, "Winhttp.lib")
#pragma  comment(lib, "jsoncpp.lib")


// #pragma  comment(lib, "gmssld.lib")
// #pragma  comment(lib, "http_proxy_serverd.lib")
// #pragma  comment(lib, "libeay32mtd.lib")
// #pragma  comment(lib, "mdsmd.lib")
// #pragma  comment(lib, "smd.lib")

bool isGMSSL(string &strt, bool &b)
{
	return false;
}


string HttpWebShortcutEvent( std::map<std::string, string>& para)
{
	string para_hwnd = "hwnd";
	string para_file = "filename";
	
	HWND wnd = (HWND)atoi(para[para_hwnd].c_str());
	string file = para[para_file];
	
	HDC hDCSrc = GetDC(wnd);
	RECT rc;
	GetWindowRect(wnd, &rc);
	if (hDCSrc)
	{
		//// 得到当前显示设备的水平像素

		
		int nWidth = rc.right - rc.left;
		int nHeight = rc.bottom - rc.top;


		int nBitPerPixel = GetDeviceCaps(hDCSrc, BITSPIXEL);//获取到每个像素的bit数目
		
		
		CImage cImage;//使用CImage能省好多截图的代码  

		::CreateCompatibleBitmap(hDCSrc, nWidth, nHeight);
		cImage.Create(nWidth, nHeight, nBitPerPixel);

		BitBlt(cImage.GetDC(), 0, 0, nWidth, nHeight, hDCSrc, 0, 0, SRCCOPY);


// 		CImage smallImage;
// 		bool bRet = smallImage.Create(336, 200, nBitPerPixel);
// 		SetStretchBltMode(smallImage.GetDC(), COLORONCOLOR);
// 		 bRet = cImage.StretchBlt(smallImage.GetDC(), 0, 0, 336, 200, SRCCOPY);
// 
// 		//直接保存吧  
		HRESULT hr = cImage.Save(_encoding(file).a_utf16().getutf16().c_str(), Gdiplus::ImageFormatPNG);
		if (SUCCEEDED(hr))
		{
			theApp.History()->AddShotcut(para["id"], file);
		}


// 
// 		
// 		smallImage.ReleaseDC();
// 		smallImage.ReleaseDC();

		cImage.ReleaseDC();//截图的代码到这里就结束了  
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
			MessageBoxA(NULL, e.errorMessage(), "发生异常", MB_OK);

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