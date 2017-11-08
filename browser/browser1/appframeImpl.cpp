#include "utilities.h"
#include "resource.h"

#include "appframe.h"
#include "app_favorframe.h"
#include "webEventHandler.h"
//#include "../../module/WhiteEncrypt/Function.h"
#include "Favor.h"
#include <fstream>
#include <iostream>

#ifdef _DEBUG  
#   ifdef _UNICODE  
#       pragma comment(lib, "DuiLib_d.lib")  
#   else  
#       pragma comment(lib, "DuiLib_ad.lib")  
#   endif  
#else  
#   ifdef _UNICODE  
#       pragma comment(lib, "DuiLib_u.lib")  
#   else  
#       pragma comment(lib, "DuiLib_a.lib")  
#   endif  
#endif 

#define WM_SHOW_FAVOR_ADD	WM_USER+1000
#define WM_SHOW_HISTORY		WM_USER+1001

const wchar_t * tip = L"极密浏览器";
urlmap g_um;




class CAboutDlg : public CWindowWnd, public INotifyUI, public IMessageFilterUI  
{  
public:  
	CAboutDlg(){};  
	virtual ~CAboutDlg()  {};

	LPCTSTR GetWindowClassName() const { return _T("UIAbout"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };  

	void OnFinalMessage(HWND /*hWnd*/)  
	{  
		m_pm.RemovePreMessageFilter(this);  
		delete this;  
	};  

	void Init() {

	}  

	void Notify(TNotifyUI& msg)  
	{
		LOGNOTIFY;
		if (msg.pSender->GetName() == _T("ui_about_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
		}
	}  

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);  
		styleValue &= ~WS_CAPTION;  
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);  

		m_pm.Init(m_hWnd);  
		m_pm.AddPreMessageFilter(this);  
		CDialogBuilder builder;  
		CControlUI* pRoot = builder.Create(_T("skin//about.xml"), (UINT)0, NULL, &m_pm);  
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);  
		m_pm.AddNotifier(this);  

		Init();  
		return 0;  
	}  


	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		if (::IsIconic(*this)) bHandled = FALSE;  
		return (wParam == 0) ? TRUE : FALSE;  
	}  


	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		return 0;  
	}  

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		return 0;  
	}  

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		return HTCLIENT;  
	}  

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		SIZE szRoundCorner = m_pm.GetRoundCorner();  
		if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {  
			CDuiRect rcWnd;  
			::GetWindowRect(*this, &rcWnd);  
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);  
			rcWnd.right++; rcWnd.bottom++;  
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);  
			::SetWindowRgn(*this, hRgn, TRUE);  
			::DeleteObject(hRgn);  
		}  

		bHandled = FALSE;  
		return 0;  
	}  

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)  
	{  
		LRESULT lRes = 0;  
		BOOL bHandled = TRUE;  
		switch (uMsg) {  
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;  
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;

		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;  
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;  
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;  
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;  
		default:  
			bHandled = FALSE;  
		}  
		if (bHandled) return lRes;  
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;  
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
	}  

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)  
	{      
		if (uMsg == WM_KEYDOWN) {  
			if (wParam == VK_RETURN) {  
			}  
			else if (wParam == VK_ESCAPE) {
				this->Close(IDCLOSE);
				return true;
			}  

		}  
		return false; 
	}  

public:  
	CPaintManagerUI m_pm; 
}; 

class CCloseTipDlg : public CWindowWnd, public INotifyUI, public IMessageFilterUI  
{  
public:  
	CCloseTipDlg(){};  
	virtual ~CCloseTipDlg()  {};

	LPCTSTR GetWindowClassName() const { return _T("UICCloseTipDlg"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };  

	void OnFinalMessage(HWND /*hWnd*/)  
	{  
		m_pm.RemovePreMessageFilter(this);  
		delete this;  
	};  

	void Init() {
	}  

	void Notify(TNotifyUI& msg)  
	{
		//LOGNOTIFY;
		if (msg.pSender->GetName() == _T("ui_closetipdlg_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCANCEL);
		}
		else if(msg.pSender->GetName() == _T("ui_close_tip_ok") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			::PostMessage(m_fream->GetHWND(),WM_DESTROY,NULL,NULL);
		}
		else if(msg.pSender->GetName() == _T("ui_close_tip_cancel") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCANCEL);
		}
		else if(msg.pSender->GetName() == _T("ui_close_tip_checkbox") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			COptionUI* option = static_cast<COptionUI*>(m_pm.FindControl(_T("ui_close_tip_checkbox")));
			if(option->IsSelected())
			{
				m_fream->SetNeedTip(true);
			}
			else
			{
				m_fream->SetNeedTip(false);
			}
		}

		else if(msg.pSender->GetName() == _T("ui_close_tip_checkbox") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			COptionUI* option = static_cast<COptionUI*>(m_pm.FindControl(_T("ui_close_tip_checkbox")));
			if(option->IsSelected())
			{
				m_fream->SetNeedTip(true);
			}
			else
			{
				m_fream->SetNeedTip(false);
			}
		}


	}  

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);  
		styleValue &= ~WS_CAPTION;  
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);  

		m_pm.Init(m_hWnd);  
		m_pm.AddPreMessageFilter(this);  
		CDialogBuilder builder;  
		CControlUI* pRoot = builder.Create(_T("skin//closetip.xml"), (UINT)0, NULL, &m_pm);  
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);  
		m_pm.AddNotifier(this);  

		Init();  
		return 0;  
	}  

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		if (::IsIconic(*this)) bHandled = FALSE;  
		return (wParam == 0) ? TRUE : FALSE;  
	}  

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		return 0;  
	}  

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		return 0;  
	}  

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		return HTCLIENT;  
	}  

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
	{  
		SIZE szRoundCorner = m_pm.GetRoundCorner();  
		if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {  
			CDuiRect rcWnd;  
			::GetWindowRect(*this, &rcWnd);  
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);  
			rcWnd.right++; rcWnd.bottom++;  
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);  
			::SetWindowRgn(*this, hRgn, TRUE);  
			::DeleteObject(hRgn);  
		}  

		bHandled = FALSE;  
		return 0;  
	}  

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)  
	{  
		LRESULT lRes = 0;  
		BOOL bHandled = TRUE;  
		switch (uMsg) {  
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;  
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;  
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;  
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;  
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;  
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;  
		default:  
			bHandled = FALSE;  
		}  
		if (bHandled) return lRes;  
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;  
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
	}  

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)  
	{      
		if (uMsg == WM_KEYDOWN) {  
			if (wParam == VK_RETURN) {  
			}  
			else if (wParam == VK_ESCAPE) {
				this->Close(IDCLOSE);
				return true;
			}  

		}  
		return false; 
	}  

public:  
	CPaintManagerUI m_pm; 
	CFrameWindowWnd* m_fream;
}; 


class CFavorEditDlgMenu : public CMenuWnd, public INotifyUI
{
public:
	CFavorEditDlgMenu(CFavorEditDlg* frame) :m_frame(frame){}
	CFavorEditDlg* m_frame;

#define NOTIFY_ON
#ifdef NOTIFY_ON
#define LOGNOTIFY wxstring tmp;\
	tmp.format(L"CFavorEditDlgMenu::NOTIFY-psender[%s], sType[%s]", msg.pSender->GetName().GetData(), msg.sType);\
	Log(tmp);
#else
#define LOGNOTIFY __noop
#endif
	void Notify(TNotifyUI& msg)
	{
		
		if (msg.pSender->GetName() == _T("menu_rename") && msg.sType == DUI_MSGTYPE_ITEMCLICK)
		{
			this->Close();
		}
		if (msg.pSender->GetName() == _T("menu_delete") && msg.sType == DUI_MSGTYPE_ITEMCLICK)
		{
			this->Close();
			::PostMessage(m_frame->GetHWND(), 1235, NULL, NULL);
		}
		if (msg.pSender->GetName() == _T("menu_newfolder") && msg.sType == DUI_MSGTYPE_ITEMCLICK)
		{

			this->Close();
			::PostMessage(m_frame->GetHWND(), 1234, NULL, NULL);
	
			
			
		}
	}
};

class CSubMenu : public CMenuWnd, public INotifyUI
{
public:
	CSubMenu(CFrameWindowWnd* cfw):m_frame(cfw){}
	CFrameWindowWnd* m_frame;


#define NOTIFY_ON
#ifdef NOTIFY_ON
#define LOGNOTIFY wxstring tmp;\
	tmp.format(L"SubCMenu::NOTIFY-psender[%s], sType[%s]", msg.pSender->GetName().GetData(), msg.sType);\
	Log(tmp);
#else
#define LOGNOTIFY __noop
#endif

	void Notify(TNotifyUI& msg)
	{
		LOGNOTIFY;



	}
	// 
	// 	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// 	{
	// 		LRESULT lRes = 0;
	// 		BOOL bHandled = TRUE;
	// 		switch (uMsg)
	// 		{
	// 
	// 		case WM_KEYDOWN:
	// 			if (wParam == VK_ESCAPE || wParam == VK_LEFT)
	// 				Close();
	// 			break;
	// 
	// 		default:
	// 			
	// 			break;
	// 		}
	// 
	// 		return CMenu::HandleMessage(uMsg, wParam, lParam);
	// 	}


};


class COptionMenu : public CWindowWnd, public INotifyUI ,public IMessageFilterUI
{
public:
	COptionMenu(CFrameWindowWnd* cfw):m_frame(cfw),m_submenu(NULL){}
	CFrameWindowWnd* m_frame;
	CSubMenu* m_submenu;
	CPaintManagerUI m_pm;
	CDuiPoint m_pt;



#define NOTIFY_OFF
#ifdef NOTIFY_ON
#define LOGNOTIFY wxstring tmp;\
	tmp.format(L"CMenu::NOTIFY-psender[%s], sType[%s]", msg.pSender->GetName().GetData(), msg.sType);\
	Log(tmp);
#else
#define LOGNOTIFY __noop
#endif


	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		;
		return S_OK;
	}

	LPCTSTR GetWindowClassName() const { return _T("UIOptionMenu"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };  

	void OnFinalMessage(HWND /*hWnd*/)  
	{  
		m_pm.RemovePreMessageFilter(this);
		delete this;  
	};  

	void Notify(TNotifyUI& msg)
	{
		LOGNOTIFY;

		if (msg.pSender->GetName() == _T("menu_bookmark") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			m_frame->m_engine->Add(m_frame->m_engine->getBookmarkPage());
			Close();
		}
		else if (msg.pSender->GetName() == _T("menu_history") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			m_frame->m_engine->Add(m_frame->m_engine->getHistoryPage());
			Close();
		}
		else if (msg.pSender->GetName() == _T("menu_setting") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			m_frame->m_engine->Add(m_frame->m_engine->getSettingsPage());
			Close();
		}
		else if (msg.pSender->GetName() == _T("menu_clearcache") && msg.sType == DUI_MSGTYPE_CLICK)
		{


			MenuCmd pMenuCmd;
			lstrcpy(pMenuCmd.szName, msg.pSender->GetName().GetData());
			lstrcpy(pMenuCmd.szUserData, msg.pSender->GetUserData().GetData());
			lstrcpy(pMenuCmd.szText, msg.pSender->GetText().GetData());
			if (!::PostMessage(m_frame->GetHWND(), WM_MENUCLICK, (WPARAM)WM_USER_OPTIONMENU_CLEARCHCHE, (LPARAM)this))
			{

			}



			//WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 255", 9);
// 			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 8", 9);
// 			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 2", 9);
// 			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 16", 9);
// 			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 32", 9);
// 			MessageBox(m_frame->GetHWND(), L"清理缓存成功", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
		}
		else if (msg.pSender->GetName() == _T("menu_print") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			

			if (!::PostMessage(m_frame->GetHWND(), WM_MENUCLICK, (WPARAM)WM_USER_OPTIONMENU_PRINT, (LPARAM)this))
			{

			}
			

		}
		else if (msg.pSender->GetName() == _T("menu_download") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (!::PostMessage(m_frame->GetHWND(), WM_MENUCLICK, (WPARAM)WM_USER_OPTIONMENU_DOWNLOAD, (LPARAM)this))
			{

			}
		}
		else if (msg.pSender->GetName() == _T("putong") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			theApp.RunSelf();
		}
		else if (msg.pSender->GetName() == _T("xiaohao") && msg.sType == DUI_MSGTYPE_CLICK)
		{
// 			theApp.RunSelf();
// 			Close();
		}
		else if (msg.pSender->GetName() == _T("wuhen") && msg.sType == DUI_MSGTYPE_CLICK)
		{
// 			theApp.RunSelf();
// 			Close();
		}
		else if (msg.pSender->GetName() == _T("suofang_minus") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			//MessageBox(NULL, L"暂不支持", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
			CButtonUI* btn = dynamic_cast<CButtonUI*>(m_pm.FindControl(_T("suofang_number")));
			wstring tmp = btn->GetText().GetData();
			wstring radio;
			int		nradio;
			if (tmp == L"400%") { radio = L"200%", nradio = 200; };
			if (tmp == L"200%") { radio = L"150%", nradio = 150; };
			if (tmp == L"150%") { radio = L"100%", nradio = 100; };
			if (tmp == L"100%") { radio = L"75%", nradio = 75; };
			if (tmp == L"75%") { radio = L"50%", nradio = 50; };
			if (tmp == L"50%") return;


			btn->SetText(radio.c_str());

			btn->SetText(radio.c_str());
			CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_frame->m_engine->m_crrentWebPage);
			IWebBrowser2* wb = ui->GetWebBrowser2();
			CComVariant varZoom((int)nradio);
			wb->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, &varZoom, NULL);
		}

		else if (msg.pSender->GetName() == _T("tupian") && msg.sType == DUI_MSGTYPE_CLICK)
		{
// 			CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_frame->m_engine->m_crrentWebPage);
// 			IWebBrowser2* wb = ui->GetWebBrowser2();
// 			wb->ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
// 			Close();
		}
		else if (msg.pSender->GetName() == _T("wenjian") && msg.sType == DUI_MSGTYPE_CLICK)
		{


			if (!::PostMessage(m_frame->GetHWND(), WM_MENUCLICK, (WPARAM)WM_USER_OPTIONMENU_SAVEASFILE, (LPARAM)this))
			{

			}

		}

		else if (msg.pSender->GetName() == _T("suofang_plus") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			CButtonUI* btn = dynamic_cast<CButtonUI*>(m_pm.FindControl(_T("suofang_number")));
			wstring tmp = btn->GetText().GetData();
			wstring radio;

			int		nradio;


			if (tmp == L"200%") { radio = L"400%", nradio = 400; };
			if (tmp == L"150%") { radio = L"200%", nradio = 200; };
			if (tmp == L"100%") { radio = L"150%", nradio = 150; };
			if (tmp == L"75%")  { radio = L"100%", nradio = 100; };
			if (tmp == L"50%")  { radio = L"75%", nradio = 75; };
			if (tmp == L"400%") return;


			btn->SetText(radio.c_str());
			CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_frame->m_engine->m_crrentWebPage);
			IWebBrowser2* wb = ui->GetWebBrowser2();
			CComVariant varZoom((int)nradio);
			wb->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, &varZoom, NULL);

			//MessageBox(NULL, L"暂不支持", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
		}
		else if (msg.pSender->GetName() == _T("suofang_restore") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			CButtonUI* btn = dynamic_cast<CButtonUI*>(m_pm.FindControl(_T("suofang_number")));
			wstring tmp = btn->GetText().GetData();
			wstring radio;
			radio = L"100%";


			btn->SetText(radio.c_str());

			CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_frame->m_engine->m_crrentWebPage);
			IWebBrowser2* wb = ui->GetWebBrowser2();
			CComVariant varZoom((int)100);

			wb->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, &varZoom, NULL);

			//MessageBox(NULL, L"暂不支持", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
		}

		else if (msg.pSender->GetName() == _T("suofang_max") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			CButtonUI* btn = dynamic_cast<CButtonUI*>(m_pm.FindControl(_T("suofang_number")));
			wstring tmp = btn->GetText().GetData();
			wstring radio = L"400%";


			btn->SetText(radio.c_str());


			CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_frame->m_engine->m_crrentWebPage);
			IWebBrowser2* wb = ui->GetWebBrowser2();
			CComVariant varZoom((int)400);

			wb->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, &varZoom, NULL);

			//MessageBox(NULL, L"暂不支持", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
		}

		else if (msg.pSender->GetName() == _T("ui_internet_option") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			ShellExecuteA(NULL, "open", "cmd.exe", "/c inetcpl.cpl", NULL, SW_HIDE);

			//MessageBox(NULL, L"暂不支持", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
		}

		else if (msg.pSender->GetName() == _T("ui_about_geemee") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (!::PostMessage(m_frame->GetHWND(), WM_MENUCLICK, (WPARAM)WM_USER_OPTIONMENU_ABOUT, (LPARAM)this))
			{

			}

			//MessageBox(NULL, L"暂不支持", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
		}


		else if (msg.pSender->GetName() == _T("submenu_toobar") && msg.sType == DUI_MSGTYPE_MOUSEHOVER)
		{
// 			CDuiPoint point;
// 			CControlUI* pCon = m_pm.FindControl(L"submenu_toobar");
// 			RECT rc = pCon->GetPos();
// 			point.x = rc.left;
// 			point.y = rc.top;
// 			ClientToScreen(GetHWND(), &point);
// 
// 			xstring tmp;
// 			tmp.format("before[%d,%d], after [%d,%d]", rc.left,rc.top, point.x, point.y).log();
// 
// 			
// 
// 			if(!m_submenu)
// 			{
// 				m_submenu = new CSubMenu(m_frame);
// 				m_submenu->Init(NULL, _T("skin\\submenu.xml"), point, &m_pm, NULL, eMenuAlignment_Right | eMenuAlignment_Top);
// 
// 			}
			
		}
		else if (msg.pSender->GetName() == _T("submenu_toobar") && msg.sType == DUI_MSGTYPE_MOUSELEAVE)
		{
			if(m_submenu)
			{
				POINT pt = { 0 };
				
				::GetCursorPos(&pt);

				RECT rc;
				::GetWindowRect(m_submenu->GetHWND(), &rc);
				xstring().format("pt(%d,%d), rect(%d,%d,%d,%d)", pt.x, pt.y,rc.left,rc.right, rc.top, rc.bottom).log();

				if(!::PtInRect(&rc, pt))
				{
					m_submenu->Close();
					m_submenu = NULL;
				}

				
			}


		}

	}

	void setCursorPos(CDuiPoint pt)
	{
		m_pt = pt;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		m_pm.AddPreMessageFilter(this);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("skin//menu.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);



		SIZE sz;
		sz.cx = pRoot->GetFixedWidth();
		sz.cy = pRoot->GetFixedHeight();

		POINT newPt;
		newPt.x = m_pt.x - sz.cx;
		newPt.y = m_pt.y;

		//MoveWindow(m_hWnd, newPt.x, newPt.y, sz.cx, sz.cy, TRUE);

		SetForegroundWindow(m_hWnd);
		MoveWindow(m_hWnd, newPt.x, newPt.y, sz.cx, sz.cy ,FALSE);



		CButtonUI* btn = dynamic_cast<CButtonUI*>(m_pm.FindControl(_T("suofang_number")));


		CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_frame->m_engine->m_crrentWebPage);
		IWebBrowser2* wb = ui->GetWebBrowser2();

		CComVariant varZoom;
		wb->ExecWB(OLECMDID_OPTICAL_ZOOM, OLECMDEXECOPT_DODEFAULT, NULL, &varZoom);

		ASSERT(V_VT(&varZoom) == VT_I4);
		ULONG ulZoom = V_I4(&varZoom);

		wstring radio;
		if (ulZoom == 400) radio = L"400%";
		if (ulZoom == 200) radio = L"200%";
		if (ulZoom == 150) radio = L"150%";
		if (ulZoom == 100) radio = L"100%";
		if (ulZoom == 75)  radio = L"75%";
		if (ulZoom == 50)  radio = L"50%";

		btn->SetText(radio.c_str());

		bHandled = true;

		return S_OK;
	}
// 
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;  
		BOOL bHandled = TRUE;  

		
		switch (uMsg) {  
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;  

		case WM_KEYDOWN:
			{
				if(wParam == VK_ESCAPE || wParam == VK_LEFT)
				{
					Close();break;
				}
			}
		case WM_KILLFOCUS:
			{
				Close();break;
			}

		case WM_DESTROY:
			{
				break;
			}


		default:  
			bHandled = FALSE;  
		}  
		if (bHandled) return lRes;  
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;  
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
	}


};

//CFavorAddItemDlg
	LPCTSTR CFavorAddItemDlg::GetWindowClassName() const { return _T("UIFavorAddItemForm"); };
	LRESULT CFavorAddItemDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if(uMsg == WM_KEYDOWN)
		{
			if(wParam == VK_ESCAPE)
			{
				Close();
				return true;
			}
		}

		return false;
	}
	void CFavorAddItemDlg::OnFinalMessage(HWND /*hWnd*/)
	{  
		m_pm.RemovePreMessageFilter(this);  
		delete this;  
	}

	void CFavorAddItemDlg::Edit()
	{

		
		wxstring title = m_title->GetText();

		string utf8title = _encoding(title).utf8().get();

		favor_recode_item res = theApp.Favor()->QueryById().GetResult().at(0);

		if (res.title != utf8title)
		{
			res.title = utf8title;

			theApp.Favor()->Edit(res);
		}

		

		


	}

	void CFavorAddItemDlg::Notify(TNotifyUI& msg)
	{
		LOGNOTIFY;

		if (msg.pSender->GetName() == _T("ui_favor_add_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
		}
		if (msg.pSender->GetName() == _T("ui_favor_add_ok") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			
			try
			{
				Edit();
			}
			catch (std::exception& e)
			{
				MessageBoxA(NULL, e.what(), "发生异常", MB_OK);
			}
			
			Close();
		}
		if (msg.pSender->GetName() == _T("ui_favor_add_edit") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			m_Combo->RemoveAll();
			Close(1000);
		}
		if (msg.pSender->GetName() == _T("ui_favor_add_delete") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			favor_recode_item record = theApp.Favor()->QueryById().GetResult().at(0);
			theApp.Favor()->Delete(record.id, record.id);
			Close();
		}
		if (msg.pSender->GetName() == _T("ui_favor_add_title") && msg.sType == DUI_MSGTYPE_RETURN)
		{
			try
			{
				Edit();
			}
			catch (std::exception& e)
			{
				MessageBoxA(NULL, e.what(), "发生异常", MB_OK);
			}
			
			Close(IDCLOSE);

		}
		if ( msg.sType == DUI_MSGTYPE_ITEMSELECT)
		{
			

			if (m_Combo && m_Combo->GetCurSel() == m_Combo->GetCount() - 1)
			{
				m_Combo->RemoveAll();
				Close(1000);
			}

			
		}

		
	}

	LRESULT CFavorAddItemDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		Log("OnNcHitTest"
			);

		if(::IsZoomed(*this))
		{
			return HTCLIENT;
		}




		int nPos = 0;

		RECT rcSizeBox = m_pm.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;



		RECT rcCaption = m_pm.GetCaptionRect();


		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));

			if( pControl && (_tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("ControlUI")) !=  0 &&
				_tcscmp(pControl->GetClass(), _T("ContainerUI")) !=  0 )
				)
			{
				Log("OnNcHitTest() HTCAPTION ");
				return HTCAPTION;  //如果鼠标在CAPTION区域中按钮容器控件上面，则不允许拖动
			}


		}
		else
		{
			;
		}





		return HTCLIENT;
	}

	LRESULT CFavorAddItemDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{  
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);  
		styleValue &= ~WS_CAPTION;  
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);  
		this->SetIcon(IDI_APP);
		m_pm.Init(m_hWnd);  
		m_pm.AddPreMessageFilter(this);  
		CDialogBuilder builder;  
		CControlUI* pRoot = builder.Create(_T("skin//bookmark_add.xml"), (UINT)0, NULL, &m_pm);  
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);  
		m_pm.AddNotifier(this);

		LPCTSTR pTitle = m_frame->m_engine->GetContainer(m_frame->m_engine->m_crrentWebPage)->GetItemAt(0)->GetText().GetData();
		m_title = static_cast<CEditUI*>(m_pm.FindControl(_T("ui_favor_add_title")));
		m_Combo = static_cast<CComboUI*>(m_pm.FindControl(_T("ui_favor_folderlist")));
		assert(m_title);
		assert(m_Combo);

		vector<CFavorFolder::RECORD> otherfolders = theApp.FavorFolder()->Query();

		for (int i = 0; i < otherfolders.size(); ++i)
		{
			CListLabelElementUI* newlabel = new CListLabelElementUI();

			newlabel->SetManager(&m_pm, NULL, false);
			newlabel->SetName(L"ui_favor_customfolder");
			wxstring folder = (const wchar_t*)_encoding(otherfolders[i].folder).u8_utf16().getutf16().c_str();

			newlabel->SetText(folder);
			newlabel->SetFixedHeight(23);
			newlabel->SetBkColor(0xFFF40000);
			newlabel->SetFixedWidth(214);

			m_Combo->Add(newlabel);
		}

		CListLabelElementUI* newlabel = new CListLabelElementUI();

		newlabel->SetManager(&m_pm, NULL, false);
		newlabel->SetName(L"ui_favor_selfolder");
		newlabel->SetText(L"其它文件夹");
		newlabel->SetFixedHeight(23);
		newlabel->SetFixedWidth(214);
		newlabel->SetBkColor(0xFFF40000);
		m_Combo->Add(newlabel);







		//<ListLabelElement name = "ui_favor_selfolder" text = "其它文件夹" height = "23" width = "214" / >

		m_title->SetText(pTitle);
		m_title->SetFocus();

		return 0;  
	}  

	LRESULT CFavorAddItemDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{  
		LRESULT lRes = 0;  
		BOOL bHandled = TRUE;

		switch (uMsg) 
		{
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;  
		case WM_KEYDOWN:
			{
				if (wParam == VK_RETURN) 
				{
					//AddItem();
				}  
				else if (wParam == VK_ESCAPE) 
				{
					this->Close(IDCLOSE);
					return true;
				} 
			}
			break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); Log("OnNcHitTest %d", lRes); break;
		default:  
			bHandled = FALSE;  
		}  

		if (bHandled) return lRes;  
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;  
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
	} 


//CFavorEditDlg



	LPCTSTR CFavorEditDlg::GetWindowClassName() const { return _T("UIFavorEditDlg"); };
	LRESULT CFavorEditDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if (uMsg == WM_KEYDOWN)
		{
			if (wParam == VK_ESCAPE)
			{
				Close();
				return true;
			}
		}

		return false;
	}
	void CFavorEditDlg::OnFinalMessage(HWND /*hWnd*/)
	{
		m_pm.RemovePreMessageFilter(this);
		delete this;
	}


	void CFavorEditDlg::DeleteSelect()
	{
		if (m_folder->GetSelectItemCount() == 0)
		{
			return;
		}

		
		CListContainerElementUI* elmt = static_cast<CListContainerElementUI*>(m_folder->GetItemAt(m_folder->GetCurSel()));

		if (!elmt)
		{
			return;
		}
		
		CLabelUI* lable = static_cast<CLabelUI*>(elmt->GetItemAt(1));

		if (!lable)
		{
			return;
		}

		theApp.FavorFolder()->Delete(_tcstol(lable->GetName(), NULL, 10), _tcstol(lable->GetName(), NULL, 10));
		m_folder->Remove(elmt);


		


	}


	void CFavorEditDlg::NewFolder()
	{
		CListContainerElementUI* elmt = new CListContainerElementUI();
		CButtonUI*               btn = new CButtonUI();
		CLabelUI*                label = new CLabelUI();
		CEditUI*                edit = new CEditUI();
		elmt->SetChildAlign(DT_CENTER);
		elmt->SetManager(&m_pm, NULL, false);
		elmt->SetFixedHeight(30);
		RECT rc = { 1, 1, 1, 1 };
		elmt->SetPadding(rc);
		m_folder->Add(elmt);


		btn->SetManager(&m_pm, NULL, false);
		btn->SetAttribute(_T("padding"), _T("5,8,0,0"));
		btn->SetName(_T("ui_folder_btn"));
		btn->SetBkImage(_T("skin/folder_.png"));
		btn->SetFixedHeight(14);
		btn->SetFixedWidth(16);
		elmt->Add(btn);


		label->SetManager(&m_pm, NULL, false);
		label->SetAttribute(_T("padding"), _T("5,0,0,0"));
		label->SetName(_T("ui_folder_label"));
		label->SetFixedHeight(30);
		label->SetVisible(false);
		wxstring tmp;
		tmp.format(L"新建文件夹%d", m_folder->GetCount());

		label->SetText(tmp);
		elmt->Add(label);


		edit->SetManager(&m_pm, NULL, false);
		
		edit->SetAttribute(_T("padding"), _T("1,1,1,1"));
		edit->SetName(_T("ui_folder_edit"));
		edit->SetBorderSize(1);
		edit->SetBorderColor(0xFFCBD7DE);
		edit->SetFixedHeight(30);
		label->SetFont(12);
		label->SetTextColor(0xFF5C5C5C);
		edit->SetText(L"ui_add");
		elmt->Add(edit);
		

		m_folder->SelectItem(m_folder->GetCount() - 1, true);
		
		edit->SetFocus();
		SendMessage(WM_PAINT);
		m_folder->EndDown();
	}


	void CFavorEditDlg::AddExistingFolder(CFavorFolder::RECORD record)
	{

		wstring  foldername = (LPWSTR)_encoding(record.folder).u8_utf16().getutf16().c_str();


		CListContainerElementUI* elmt = new CListContainerElementUI();
		CButtonUI*               btn = new CButtonUI();
		CLabelUI*                label = new CLabelUI();
		CEditUI*                edit = new CEditUI();
		elmt->SetChildAlign(DT_CENTER);
		elmt->SetManager(&m_pm, NULL, false);
		elmt->SetFixedHeight(30);
		RECT rc = { 1, 1, 1, 1 };
		elmt->SetPadding(rc);
		m_folder->Add(elmt);


		btn->SetManager(&m_pm, NULL, false);
// 		RECT rcbtn = { 5, 7, 0, 0 };
// 		btn->SetPadding(rcbtn);
		btn->SetAttribute(_T("padding"), _T("5,8,0,0"));
		btn->SetName(_T("ui_folder_btn"));
		btn->SetBkImage(_T("skin/folder_.png"));
		btn->SetFixedHeight(14);
		btn->SetFixedWidth(16);
		
		elmt->Add(btn);


		label->SetManager(&m_pm, NULL, false);
// 		RECT rclable = { 5, 0, 0, 0 };
// 		label->SetPadding(rcbtn);
		label->SetAttribute(_T("padding"), _T("5,0,0,0"));
		label->SetFont(12);
		label->SetTextColor(0xFF5C5C5C);
		wxstring id;
		id.format(L"%d", record.id);
		label->SetName(id);
		label->SetFixedHeight(30);
		wxstring tmp = foldername;

		label->SetText(tmp);
		elmt->Add(label);


// 		edit->SetManager(&m_pm, NULL, false);
// 		RECT rcedit = { 1, 1, 1, 1 };
// 		edit->SetPadding(rcbtn);
// 		edit->SetName(_T("ui_folder_edit"));
// 		edit->SetBorderSize(1);
// 		edit->SetBorderColor(0xFFCBD7DE);
// 		edit->SetFixedHeight(30);
// 		edit->SetText(tmp);
// 		elmt->Add(edit);
// 
// 
// 		m_folder->SelectItem(m_folder->GetCount() - 1, true);
// 
// 		edit->SetFocus();
// 		SendMessage(WM_PAINT);
// 		m_folder->EndDown();
	}

	void CFavorEditDlg::Save()
	{
		favor_recode_item res = theApp.Favor()->QueryById(-1).GetResult().at(0);


		wxstring captain		= m_captain->GetText().GetData();
		wxstring site			= m_site->GetText().GetData();

		res.title				= _encoding(captain).utf8().get();
		res.url					= _encoding(site).utf8().get();
		

		CListContainerElementUI* elmt	= static_cast<CListContainerElementUI*>(m_folder->GetItemAt(m_folder->GetCurSel()));
		assert(elmt);
		CLabelUI* label					= static_cast<CLabelUI*>(elmt->GetItemAt(1));
		wxstring  folder				= label->GetText().GetData();

		res.folder						= _encoding(folder).utf8().get();

		theApp.Favor()->Edit(res);


	}

	void CFavorEditDlg::Notify(TNotifyUI& msg)
	{
		wxstring tmp;

		tmp.format(L"Notify:psender(%s): msgtype(%s)", msg.pSender->GetName().GetData(), msg.sType);

		Log(tmp);
		if (msg.pSender->GetName() == _T("ui_favor_edit_newfolder") && msg.sType == DUI_MSGTYPE_CLICK)
		{	
			try
			{
				NewFolder();
			}
			
			catch (...)
			{

			}

		}


		if (msg.pSender->GetName() == _T("ui_favor_edit_cancel") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			Close(IDCLOSE);
		}
		if (msg.pSender->GetName() == _T("ui_favor_edit_save") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			Save();
			Close(IDCLOSE);
		}
		if (msg.pSender->GetName() == _T("ui_favor_edit_list") && msg.sType == DUI_MSGTYPE_MENU)
		{
			
			CFavorEditDlgMenu* menu = new CFavorEditDlgMenu(this);

			CDuiPoint point = msg.ptMouse;
			
			ClientToScreen(m_hWnd, &point);

			
			menu->Init(NULL, _T("skin\\favormenu.xml"), point, &m_pm, NULL, eMenuAlignment_Right | eMenuAlignment_Top);

		}

		

		if (msg.pSender->GetName() == _T("ui_folder_edit") && msg.sType == DUI_MSGTYPE_KILLFOCUS)
		{
			CEditUI* tmp = static_cast<CEditUI*>(msg.pSender);
			wxstring text = tmp->GetText().GetData();
			
			CListContainerElementUI* elmt = static_cast<CListContainerElementUI*>(m_folder->GetItemAt(m_folder->GetCurSel()));
			assert(elmt);
			CLabelUI* label  = static_cast<CLabelUI*>(elmt->GetItemAt(1));
			label->SetText(text);

			string foldername = _encoding(text).utf8().get();
			CFavorFolder::RECORD item;
			item.folder = foldername;
			theApp.FavorFolder()->Add(item);
			item = theApp.FavorFolder()->QueryById(-1).GetResult().at(0);
			//label->SetName()
			wxstring fmt;
			fmt.format(L"%d", item.id);
			label->SetName(fmt);
			label->SetVisible(true);
			elmt->RemoveAt(2);

		}
		

	}

	LRESULT CFavorEditDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		this->SetIcon(IDI_APP);

		m_pm.Init(m_hWnd);
		m_pm.AddPreMessageFilter(this);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("skin//bookmark_edit.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		LPCTSTR pTitle = m_frame->m_engine->GetAddressBar()->GetText();

		m_folder = static_cast<CListUI*>(m_pm.FindControl(_T("ui_favor_edit_list")));
		assert(m_folder);

		m_captain = static_cast<CEditUI*>(m_pm.FindControl(_T("ui_favor_edit_nameedit")));
		m_site = static_cast<CEditUI*>(m_pm.FindControl(_T("ui_favor_edit_siteedit")));
		assert(m_captain);
		assert(m_site);

		wxstring sitename = m_frame->m_engine->GetContainer(m_frame->m_engine->m_crrentWebPage)->GetItemAt(0)->GetText().GetData();
		wxstring siteaddress = m_frame->m_engine->GetAddressBar()->GetText().GetData();

		m_captain->SetText(sitename);
		m_site->SetText(siteaddress);


		vector<CFavorFolder::RECORD> otherfolders = theApp.FavorFolder()->Query();

		for (int i = 0; i < otherfolders.size(); ++i)
		{
			
			AddExistingFolder(otherfolders[i]);
		}

		//AddExistingFolder();

		return 0;
	}
	LRESULT CFavorEditDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		Log("OnNcHitTest"
			);

		if(::IsZoomed(*this))
		{
			return HTCLIENT;
		}




		int nPos = 0;

		RECT rcSizeBox = m_pm.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;



		RECT rcCaption = m_pm.GetCaptionRect();


		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));

			if( pControl && (_tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("ControlUI")) !=  0 &&
				_tcscmp(pControl->GetClass(), _T("ContainerUI")) !=  0 )
				)
			{
				Log("OnNcHitTest() HTCAPTION ");
				return HTCAPTION;  //如果鼠标在CAPTION区域中按钮容器控件上面，则不允许拖动
			}


		}
		else
		{
			;
		}





		return HTCLIENT;
	}

	LRESULT CFavorEditDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;

		switch (uMsg)
		{
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:
		{
			if (wParam == VK_RETURN)
			{
				//AddItem();
			}
			else if (wParam == VK_ESCAPE)
			{
				this->Close(IDCLOSE);
				return true;
			}
		}
		break;
		case 1234:
			NewFolder();
			break;
		case 1235:
			DeleteSelect();
			break;

		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); Log("OnNcHitTest %d", lRes); break;
		
		default:
			bHandled = FALSE;
		}

		if (bHandled) return lRes;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}


	//CCacheClearDlg

	LPCTSTR CCacheClearDlg::GetWindowClassName() const { return _T("UICacheClearDlg"); };
	LRESULT CCacheClearDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if (uMsg == WM_KEYDOWN)
		{
			if (wParam == VK_ESCAPE)
			{
				Close();
				return true;
			}
		}

		return false;
	}
	void CCacheClearDlg::OnFinalMessage(HWND /*hWnd*/)
	{
		m_pm.RemovePreMessageFilter(this);
		delete this;
	}





	void CCacheClearDlg::Notify(TNotifyUI& msg)
	{
		wxstring tmp;

		tmp.format(L"CCacheClearDlg::Notify:psender(%s): msgtype(%s)", msg.pSender->GetName().GetData(), msg.sType);

		Log(tmp);
		if (msg.pSender->GetName() == _T("ui_clearcache_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{	
			Close(IDCLOSE);
		}

		if (msg.pSender->GetName() == _T("ui_clearcache_doclean") && msg.sType == DUI_MSGTYPE_CLICK)
		{	

			//Temporary Internet Files  (删除Internet临时文件)  
			//RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 8  
			//Cookies (删除cookie)  
			//RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 2  
			//History (删除历史记录)  
			//RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 1  
			//Form Data (删除表单数据)  
			//RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 16  
			//Passwords (删除密码)  
			//RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 32  
			//Delete All  (全部删除)  
			//RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 255  
			//Delete All (全部删除，包括插件的设置和文件)  
			//RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 4351  

			if(m_linshiwenjian)
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 8", 9);
			if(m_cookies)
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 2", 9);
			if(m_biaodan)
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 16", 9);
			if(m_password)
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 32", 9);
			if(m_lish)
			{
				WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 1", 9);
				theApp.History()->Clear();
				m_frame->m_engine->ClearTemps();
			}
			

			Close(IDCLOSE);
		}



		if (msg.pSender->GetName() == _T("ui_clearcache_linshiwenjian_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_linshiwenjian =  pCheck->GetCheck();
			}
			//
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_cookies_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_cookies =  pCheck->GetCheck();
			}
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_lishi_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_lish =  pCheck->GetCheck();
			}
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_donwload_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_download =  pCheck->GetCheck();
			}
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_recentclose_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_recentclose =  pCheck->GetCheck();
			}
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_changyongwangzhan_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_changyong =  pCheck->GetCheck();
			}
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_password_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_password =  pCheck->GetCheck();
			}
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_biaodanjilu_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_biaodan =  pCheck->GetCheck();
			}
		}
		if (msg.pSender->GetName() == _T("ui_clearcache_autoclean_checkbox") && msg.sType == DUI_MSGTYPE_SELECTCHANGED)
		{
			CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
			if(pCheck)
			{
				m_autoclean =  pCheck->GetCheck();
			}
		}


	}

	LRESULT CCacheClearDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		this->SetIcon(IDI_APP);

		m_pm.Init(m_hWnd);
		m_pm.AddPreMessageFilter(this);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("skin/cacheclear.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);


		m_lish = false;
		m_linshiwenjian = false;
		m_autoclean = false;
		m_biaodan = false;
		m_changyong = false;
		m_download = false;
		m_password =false;
		m_recentclose =false;
		m_cookies = false;


		//AddExistingFolder();

		return 0;
	}

	LRESULT CCacheClearDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;

		Log("CCacheClearDlg::HandleMessage %d",uMsg);

		switch (uMsg)
		{
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:
			{
				if (wParam == VK_RETURN)
				{
					//AddItem();
				}
				else if (wParam == VK_ESCAPE)
				{
					this->Close(IDCLOSE);
					return true;
				}
			}
			break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); Log("OnNcHitTest %d", lRes); break;
		default:
			bHandled = FALSE;
		}

		if (bHandled) return lRes;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	LRESULT CCacheClearDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		Log("OnNcHitTest"
			);

		if(::IsZoomed(*this))
		{
			return HTCLIENT;
		}

		


		int nPos = 0;

		RECT rcSizeBox = m_pm.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;



		RECT rcCaption = m_pm.GetCaptionRect();


		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));

			if( pControl && (_tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("ControlUI")) !=  0 &&
				_tcscmp(pControl->GetClass(), _T("ContainerUI")) !=  0 )
				)
			{
				Log("OnNcHitTest() HTCAPTION ");
				return HTCAPTION;  //如果鼠标在CAPTION区域中按钮容器控件上面，则不允许拖动
			}


		}
		else
		{
			;
		}





		return HTCLIENT;
	}



CMdWebEngine* CMdWebEngine::thisobj = new CMdWebEngine();

CFrameWindowWnd::CFrameWindowWnd(){m_pMenu = NULL;};

CFrameWindowWnd::~CFrameWindowWnd()
{
	if(m_engine)
	{
		delete m_engine;
		m_engine = NULL;
	}
}

void CFrameWindowWnd::ShowAddFavorDlg()
{
	CFavorAddItemDlg* additemdlg = new CFavorAddItemDlg;
	additemdlg->m_frame = this;
	additemdlg->Create(GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 386, 254, NULL);  
	additemdlg->CenterWindow();
	additemdlg->ShowModal();
}

void CFrameWindowWnd::ShowHistoryForm()
{
	;
}

BOOL CFrameWindowWnd::init()
{
	m_pMenu = NULL;
	m_pMaxButton = m_pm.FindControl(_T("ui_maximize"));

	assert(m_pMaxButton);
	assert(m_pResotreButton);
	m_pResotreButton = m_pm.FindControl(_T("ui_restore"));

	m_pMaxButton->SetVisible(true);
	m_pResotreButton->SetVisible(false);

	m_engine = CMdWebEngine::Get();

	m_engine->Init(&m_pm,this);



	CButtonUI* btn_favor = static_cast<CButtonUI*>(m_pm.FindControl(L"ui_favor"));
	m_pm.SetTimer(btn_favor, 1000, 100);
	m_pm.SetTimer(m_pm.FindControl(L"ui_home"), 1001, 300);
	m_pm.SetTimer(m_pm.FindControl(L"ui_restorepage"), 1002, 300);

	m_pm.GetDPIObj()->SetDPIAwareness(PROCESS_DPI_UNAWARE);


	//SetIcon(IDR_MAINFRAME);

	return TRUE;
}
LPCTSTR CFrameWindowWnd::GetWindowClassName() const { return _T("UIMainFrame"); };
UINT CFrameWindowWnd::GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
void CFrameWindowWnd::OnFinalMessage(HWND /*hWnd*/) { ; };


void CFrameWindowWnd::OnWebBrowserNotify(TNotifyUI& msg)
{
	if(msg.sType == _T("setfocus"))
	{
		//Log(L"control %s Messagetype  [%s] %08X",msg.pSender->GetClass(), msg.sType, msg.pSender);

		msg.pSender->NeedUpdate();
	}

}

void CFrameWindowWnd::ShowAboutDlg()
{
	auto aboutdlg = new CAboutDlg;
	aboutdlg->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 386, 254, NULL);  
	aboutdlg->CenterWindow();
	aboutdlg->ShowModal();
}

void CFrameWindowWnd::OnSelectChanged(TNotifyUI& msg)
{
	Log("OnSelectChanged %08X", msg.pSender);

	if(msg.pSender->GetUserData().Find(_T("ui_option")) != -1)
	{
		Log("OnSelectChanged %08X", msg.pSender);
		COptionUI* pOption = dynamic_cast<COptionUI*>(msg.pSender);
		m_engine->Switch(pOption);
	}



}

void CFrameWindowWnd::OnTimer(TNotifyUI& msg)
{

	if( msg.pSender->GetName() == _T("ui_home") ) 
	{
		msg.pSender->SetVisible(theApp.get_ui_show_homepage() == "1");

	}


	if( msg.pSender->GetName() == _T("ui_restorepage") ) 
	{
		msg.pSender->SetVisible(theApp.get_ui_show_restore_recent() == "1");
	}

	if( msg.pSender->GetName() == _T("ui_favor") ) 
	{

		CButtonUI* btn_favor = static_cast<CButtonUI*>(m_engine->m_pm->FindControl(_T("ui_favor")));

		wstring addrtext = m_engine->GetAddressBar()->GetText();

		if(addrtext.find(L"geemee:") != wstring::npos)
		{
			btn_favor->SetVisible(false);
		}
		else
		{
			btn_favor->SetVisible(TRUE);
		}


		CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_engine->GetCurrentWebBrowserUI());
		if(ui )
		{
			string url = ui->getUrl();

			
			if(theApp.Favor()->CountOf(url))
			{
				btn_favor->SetAttribute(L"bkimage", favor_hot);
				wstring tmp = btn_favor->GetBkImage();

			}
			else
			{
				btn_favor->SetAttribute(L"bkimage", favor_nor);
				wstring tmp = btn_favor->GetBkImage();

			}


		}
	}


}

void CFrameWindowWnd::OnClick(TNotifyUI& msg)
{

	if( msg.pSender->GetName() == _T("ui_close") ) {

		int cmd = IDCLOSE;
		if(m_engine->m_bindings.size() > 1 && theApp.IfAskBeforeClose())
		{
			cmd = ShowCloseTipDlg();
		}
		
		if(cmd == IDCLOSE)
		{
			
			Close();
		}
	}
	else if(msg.pSender->GetName() == _T("ui_addtab"))
	{

		m_engine->Add(m_engine->getIndexPage());
	}
	else if(msg.pSender->GetUserData().Find(_T("ui_closetab")) != -1)
	{
		int cnt = m_engine->GetCount();

		Log("ui_closetab cnt %d", cnt);
		if (m_engine->GetCount() < 2) /*UI_addtab btn left and quit */
		{

			Close();
		}
		else
		{
			Log("ui_closetab remove ");
			m_engine->Remove(msg.pSender);
			Log("ui_closetab remove %d ", m_engine->GetCount());
		}
		
	}

	else if( msg.pSender->GetName() == _T("ui_maximize") ) {

		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		m_pMaxButton->SetVisible(false);
		m_pResotreButton->SetVisible();

	}
	else if( msg.pSender->GetName() == _T("ui_restore") ) {
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
		m_pMaxButton->SetVisible(true);
		m_pResotreButton->SetVisible(false);
	}
	else if( msg.pSender->GetName() == _T("ui_minimize") ) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
	}


	else if (msg.pSender->GetName() == _T("ui_restorepage")) {
		
		m_engine->RestorePage();

	}

	else if (msg.pSender->GetName() == _T("ui_home")) {

		

		m_engine->Add(_encoding(theApp.get_home_page()).u8_utf16().getutf16().c_str());

	}


}
void CFrameWindowWnd::Notify(TNotifyUI& msg)
{
	LOGNOTIFY;

	if (msg.pSender->GetName() == _T("ui_settings") && msg.sType == DUI_MSGTYPE_CLICK)
	{
		ShowMenu();
	}
	else if (msg.pSender->GetName() == _T("ui_refreshed") && msg.sType == DUI_MSGTYPE_CLICK)
	{
		m_engine->m_crrentWebPage->Refresh();
		/*CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("ui_favor_add_title")));
		BSTR bstr = NULL;
		m_engine->m_crrentWebPage->GetWebBrowser2()->get_LocationURL(&bstr);
		m_engine->Reload(bstr);*/
	}
	else if (msg.pSender->GetName() == _T("ui_forward") && msg.sType == DUI_MSGTYPE_CLICK)
	{
		m_engine->m_crrentWebPage->GoForward();
	}
	else if (msg.pSender->GetName() == _T("ui_back") && msg.sType == DUI_MSGTYPE_CLICK)
	{
		m_engine->m_crrentWebPage->GoBack();
	}
	else if (msg.pSender->GetName() == _T("ui_address"))
	{
		OnAddressNotify(msg);
	}
	else if (msg.pSender->GetName() == _T("ui_favor") && msg.sType == DUI_MSGTYPE_CLICK)
	{
		BookmarkAdd();
	}
	else if (msg.pSender->GetName() == _T("ui_download") && msg.sType == DUI_MSGTYPE_CLICK)
	{
		;
	}

	
	else if (CDuiString(msg.pSender->GetClass()) == _T("WebBrowser"))
	{

	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		OnSelectChanged(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		OnClick(msg);
	}

	else if(msg.sType == DUI_MSGTYPE_TIMER)
	{
		OnTimer(msg);
	}

	else if(msg.pSender->GetName() == _T("popt") && msg.sType == DUI_MSGTYPE_RBUTTONDOWN)
	{
		COptionUI* option = static_cast<COptionUI*>(msg.pSender);
		CContainerUI* parent = static_cast<CContainerUI*>(option->GetParent());
		CButtonUI* pclose = static_cast<COptionUI*>(parent->GetItemAt(1));
		if(option && option->IsSelected() && theApp.get_tabset_close_rightclick() == "1")
		{
			CControlUI* paraent = option->GetParent();
			m_pm.SendNotify(pclose, DUI_MSGTYPE_CLICK);

		}

	}

	else if(msg.pSender->GetName() == _T("popt") && msg.sType == DUI_MSGTYPE_DBCLICK)
	{
		COptionUI* option = static_cast<COptionUI*>(msg.pSender);
		CContainerUI* parent = static_cast<CContainerUI*>(option->GetParent());
		CButtonUI* pclose = static_cast<COptionUI*>(parent->GetItemAt(1));
		if(option && option->IsSelected() && theApp.get_tabset_close_dblclick() == "1")
		{
			CControlUI* paraent = option->GetParent();
			m_pm.SendNotify(pclose, DUI_MSGTYPE_CLICK);

		}

	}

}

void CFrameWindowWnd::OnAddressNotify(TNotifyUI&msg)
{
	if(msg.sType == DUI_MSGTYPE_RETURN)
	{
		CEditUI* pEdit = dynamic_cast<CEditUI*>(msg.pSender);
		assert(pEdit);

		CWebBrowserUI* ui = m_engine->GetCurrentWebBrowserUI();

		wstring addrtext = pEdit->GetText().GetData();
		string utf8url = _encoding(addrtext).utf8().get();

		_re reg("((https?|ftp|file):\\/\\/)?[\\-_a-zA-Za0-9]+(\\.[\\-_a-zA-z0-9]+)+(:\\d+)?(\\/([\\-_a-zA-Z;%?:@%$&+=~#0-9\\.])*)*");

		if(reg.match(utf8url))//是URL
		{
			
			if(ui && theApp.get_tabset_newtab_navigateaddress() == "0")
			{
				m_engine->Reload((LPCTSTR)(pEdit->GetText()));
			}
			else
			{
				m_engine->Add(pEdit->GetText().GetData());
			}

		}
		else
		{
			AppConfig::SEARCHENGINE_CONFIG engin = theApp.get_default_searchengine();

			string queryurl = engin[2];
			queryurl.append(utf8url);

			if(ui && theApp.get_tabset_newtab_navigateaddress() == "0")
			{
				m_engine->Reload(_encoding(queryurl).u8_utf16().getutf16().c_str());
			}
			else
			{
				m_engine->Add(_encoding(queryurl).u8_utf16().getutf16().c_str());
			}

		}




		


	}
}

LRESULT CFrameWindowWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);


	if(::IsZoomed(*this))
	{
		return HTCLIENT;
	}

	Log("OnNcHitTest"
		);


	int nPos = 0;

	RECT rcSizeBox = m_pm.GetSizeBox();
	if (pt.y < rcClient.top + rcSizeBox.top)
	{
		if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
		return HTTOP;
	}
	else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
	{
		if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
		return HTBOTTOM;
	}

	if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
	if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;



	RECT rcCaption = m_pm.GetCaptionRect();


	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));

		if( pControl && (_tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("ControlUI")) !=  0 &&
			_tcscmp(pControl->GetClass(), _T("ContainerUI")) !=  0 )
			)
		{
			Log("OnNcHitTest() HTCAPTION ");
			return HTCAPTION;  //如果鼠标在CAPTION区域中按钮容器控件上面，则不允许拖动
		}
		

	}
	else
	{
		;
	}





	return HTCLIENT;
}
LRESULT CFrameWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = TRUE;
	LRESULT lRes = 0;
	switch (uMsg)
	{


	case WM_CREATE:
		{
			m_pm.Init(m_hWnd);
			CDialogBuilder dlg;
			m_pRoot = dlg.Create(L"frame.xml",(UINT)0,NULL,&m_pm);
			

			m_pm.AttachDialog(m_pRoot); 
			m_pm.AddNotifier(this);
			this->SetIcon(IDI_APP);

			init();

			break;
		}
	case WM_DESTROY:
		{
			m_pm.KillTimer(m_pm.FindControl(L"ui_favor"));
			m_pm.KillTimer(m_pm.FindControl(L"ui_home"));
			m_pm.KillTimer(m_pm.FindControl(L"ui_restorepage"));


			m_engine->CloseAll();
			
			stack<string> deleted = m_engine->GetDeleted();
			while (!deleted.empty())
			{
				string url = deleted.top();
				theApp.Lock();
				
				if(url.size())
				{	
					theApp.getJsonValue()["cache"].append(_encoding(url).en_base64().get());
					deleted.pop();
				}
				else
				{
					theApp.getJsonValue()["cache"].append(Json::Value());
					break;
				}
				sync_setting_proc(NULL);

				theApp.Unlock();


			};

			theApp.Pool().OnExit();
			
			::PostQuitMessage(0);
			break;
		}
	case WM_NCACTIVATE:
		{
			if( !::IsIconic(m_hWnd) ) 
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}

	case WM_NCLBUTTONDBLCLK:
		{
			bHandled = FALSE;
			if(m_pMaxButton->IsVisible())
			{

				m_pMaxButton->SetVisible(false);
				m_pResotreButton->SetVisible();

			}
			else {

				m_pMaxButton->SetVisible(true);
				m_pResotreButton->SetVisible(false);
			}
			break;
		}
	case WM_NCCALCSIZE:
	case WM_NCPAINT:
		{
			break;
		}
	case WM_NCHITTEST:
		{
			lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
			break;
		}
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				{
					bHandled = TRUE;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_SHOW_FAVOR_ADD:
		{
			ShowAddFavorDlg();
		}
		break;
	case WM_SHOW_HISTORY:
		{
			ShowHistoryForm();
		}
		break;

	case WM_MENUCLICK:
		{
			if(wParam == WM_USER_OPTIONMENU_CLEARCHCHE)
			{
				CCacheClearDlg* additemdlg = new CCacheClearDlg;
				additemdlg->m_frame = this;
				additemdlg->Create(GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, 0);
				additemdlg->CenterWindow();
				int nRet = additemdlg->ShowModal();
			}
			if(wParam == WM_USER_OPTIONMENU_SAVEASFILE)
			{
				CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_engine->m_crrentWebPage);
				IWebBrowser2* wb = ui->GetWebBrowser2();
				wb->ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
			}

			if(wParam == WM_USER_OPTIONMENU_DOWNLOAD)
			{
				MessageBox(NULL, L"暂不支持", tip, MB_OK | MB_APPLMODAL | MB_TOPMOST);
			}

			if(wParam == WM_USER_OPTIONMENU_ABOUT)
			{
				ShowAboutDlg();
			}

			if(wParam == WM_USER_OPTIONMENU_PRINT)
			{
				CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_engine->m_crrentWebPage);
				IWebBrowser2* wb = ui->GetWebBrowser2();
				CComVariant var((int)0);
				wb->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
			}


			break;

			
		}
	case 1236:
		{
			wstring url = (LPCTSTR)wParam;
			CWebBrowserUI* ui = m_engine->GetCurrentWebBrowserUI();
			if(ui && theApp.get_tabset_newtab_whenclickbookmark() == "0")
			{
				ui->Navigate2(url.c_str());
			}
			else
			{
				m_engine->Add(url.c_str());
			}
			
			
		}
	default:
		{
			bHandled = FALSE;
		}
	}

	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

}

CMdWebEngine* CMdWebEngine::Get()
{

	if(thisobj)
	{
		return thisobj;
	}
	else
	{
		thisobj =  new CMdWebEngine();
		assert(thisobj);
		return thisobj;
	}

}

CMdWebEngine::CMdWebEngine()
{
	thisobj = NULL;
	
	m_bookmark_page.format(_T("file:///%ssetting/html/%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("geemee_bookmark.html"));
	m_index_page.format(_T("file:///%ssetting/%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("geemee_index.html"));
	m_history_page.format(_T("file:///%ssetting/html/%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("geemee_history.html"));
	m_settings_page.format(_T("file:///%ssetting/html/%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("geemee_settings.html"));

	string a1 = _encoding(m_bookmark_page).utf8().get();
	string a2 = _encoding(m_index_page).utf8().get();
	string a3 = _encoding(m_history_page).utf8().get();
	string a4 = _encoding(m_settings_page).utf8().get();


	m_crrentWebPage = NULL;

	try
	{
		_re re("\\\\") ;

		a1 = re.replace(a1, "/").replaceresult();
		a2 = re.replace(a2, "/").replaceresult();
		a3 = re.replace(a3, "/").replaceresult();
		a4 = re.replace(a4, "/").replaceresult();

		m_bookmark_page = _encoding(a1).u8_utf16().getutf16();
		m_index_page = _encoding(a2).u8_utf16().getutf16();
		m_history_page = _encoding(a3).u8_utf16().getutf16();
		m_settings_page = _encoding(a4).u8_utf16().getutf16();

	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "发生异常", MB_OK);
	}
	

}


CMdWebEngine::~CMdWebEngine()
{
	m_bindings.clear();
		
	
	
	

}

void CMdWebEngine::Init(CPaintManagerUI* pm, CWindowWnd* wnd)
{
	m_pm = pm;
	m_pWnd = wnd;
	m_webcontainer = dynamic_cast<CTabLayoutUI*>(m_pm->FindControl(_T("ui_webcontainer")));
	assert(m_webcontainer);
	m_tabcontainer = dynamic_cast<CContainerUI*>(m_pm->FindControl(_T("htab")));
	assert(m_tabcontainer);
}

int CMdWebEngine::Add(LPCTSTR url, bool background)
{
	Log("COptionUIMgr::Add");


	int cntTabCon = m_webcontainer->GetCount();
	for(int i = 0 ; i < cntTabCon; ++i)
	{
		CMdWebBrowserUI* _ie = dynamic_cast<CMdWebBrowserUI*>(m_webcontainer->GetItemAt(i));

		wstring _wurl = url ;


		
		if( (_wurl.find(L"geemee_bookmark.html") != -1  && _ie->getNickUrl() == L"geemee://bookmark") ||
			(_wurl.find(L"geemee_history.html") != -1   && _ie->getNickUrl() == L"geemee://history") ||
			(_wurl.find(L"geemee_settings.html") != -1   && _ie->getNickUrl() == L"geemee://settings") )
		{
			;
			CContainerUI* con = GetContainer(_ie);
			if(con)
			{
				CControlUI* ptmp = con->GetItemAt(0);
				COptionUI* optionUI = dynamic_cast<COptionUI*>(ptmp);
				optionUI->Selected(true);

				/*			Switch(contaner);*/
				return 0;

			}



		}




	}






	CButtonUI* pBtn = new CButtonUI();
	CButtonUI* pBtnClose = new CButtonUI();
	COptionUI* pOpt  = new COptionUI();
	CContainerUI* pContainer = new CContainerUI();


	m_tabcontainer->AddAt(pContainer, m_tabcontainer->GetCount() - 1);
	pContainer->SetName(_T("123421341234"));
	pContainer->SetFixedWidth(130);
	pContainer->SetFixedHeight(30);
	RECT rc0 = {1,1,1,1};
	pContainer->SetBorderSize(rc0);
	pContainer->SetBorderColor(0xFFE8ECF2);
	m_pWnd->SendMessageW(WM_PAINT);
	
	

	pContainer->Add(pOpt);
	pContainer->Add(pBtnClose);
	pContainer->Add(pBtn);

	m_pWnd->SendMessageW(WM_PAINT);

	pBtn->SetName(L"pbtn");
	pBtn->SetFloat(true);
	pBtn->SetAttribute(L"pos", L"13,6,29,22");
	pBtn->SetBkImage(L"skin\\webicon.png");




	pOpt->SetName(L"popt");
	pOpt->SetBkImage(_T("skin\\tab_bkgd.png"));
	pOpt->SetSelectedImage(_T("skin\\tab_focus.png"));
	pOpt->SetFont(4);

// 	pOpt->SetBkColor(0xFFFF0000);
// 	pOpt->SetSelectedBkColor(0xFF00FFFF);
	pOpt->SetGroup(_T("webpage"));
	pOpt->SetUserData(_T("ui_option"));
	pOpt->SetTag((UINT_PTR)pOpt);

	if(!background)
	{
		pOpt->Selected(true);
	}
	
	

	pBtnClose->SetName(L"pbtnclose");
	pBtnClose->SetUserData(_T("ui_closetab"));
	pBtnClose->SetFloat(true);
	pBtnClose->SetAttribute(L"pos", L"114,11,122,19");
	pBtnClose->SetUserData(_T("ui_closetab"));
	pBtnClose->SetBkImage(L"skin\\tab_close_new.png");



	CWebBrowserUI* ie = new CMdWebBrowserUI();
	CMdWebBrowserUI* _ie = dynamic_cast<CMdWebBrowserUI*>(ie);

	m_webcontainer->Add(ie);

	ie->CActiveXUI::SetAttribute(_T("clsid"), _T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
	ie->SetDelayCreate(true);

	static CWebEventHandler handler(this);; 
	ie->SetWebBrowserEventHandler(&handler);
	ie->DoCreateControl();

	if(!background)
	{
		m_webcontainer->SelectItem(ie);
	}
	else
	{
		ie->SetVisible(FALSE);
	}
	
	ie->SetFloat(false);
	ie->SetUserData(url);
	_ie->setNickUrl(url);
	

	//检测是否是国密连接，是则设置代理
	string strUrl;
	strUrl.assign((_bstr_t)url);
	bool b = false;
	string strback = strUrl;

	if (isGMSSL(strUrl, b))
	{
		g_um[strUrl] = strback;
		setproxy(true);
	}
	else
	{
		setproxy(false);
	}
	url = _bstr_t(strUrl.c_str());

	//请求网页
	Bind(pContainer, ie);
	ie->Navigate2(url);

	//设置当前页面
	if(!background)
	{
		m_crrentWebPage = ie;
	}

	//ie->GetWebBrowser2()->put_RegisterAsBrowser(VARIANT_TRUE);
	//ie->GetWebBrowser2()->get_Application(ppDisp); //这一句是关键，关联我们新建的窗口的browser控件到点击的目标链接

	////设置地址栏
	//CEditUI* pEdit = dynamic_cast<CEditUI*>(this->m_pm->FindControl(_T("ui_address")));
	//pEdit->SetText(url);


	return true;
}





int CMdWebEngine::GetCount()
{
	return m_webcontainer->GetCount();
}

int CMdWebEngine::Remove( CControlUI* btnCloseTab )
{
	CContainerUI* pContainer = dynamic_cast<CContainerUI*>(btnCloseTab->GetParent());
	assert(pContainer);

	COptionUI* pOpt = dynamic_cast<COptionUI*>(m_pm->FindSubControlByClass(pContainer, _T("OptionUI")));
	//pOpt->Selected(false);
	CWebBrowserUI* pNewSelected = NULL;

	if(1)//删除当前选中的tab前，需要重新设置选中的tab
	{
		int pos = m_tabcontainer->GetItemIndex(pContainer);
		int nCount = m_tabcontainer->GetCount();
		assert(nCount > 1);

		if(nCount == 2)
		{
			//do nothing; 最左边
		}
		else if(pos == nCount - 2)
		{
			CContainerUI* pContainerPrev = dynamic_cast<CContainerUI*>(m_tabcontainer->GetItemAt(pos-1));
			COptionUI* pOptPrev = dynamic_cast<COptionUI*>(m_pm->FindSubControlByClass(pContainerPrev, _T("OptionUI")));
			if(pOpt->IsSelected())
			{
				pOptPrev->Selected(true);
				pNewSelected = GetWebPage((UINT_PTR)pContainerPrev);

			}
		}
		else
		{
			CContainerUI* pContainerNext = dynamic_cast<CContainerUI*>(m_tabcontainer->GetItemAt(pos+1));
			COptionUI* pOptNext = dynamic_cast<COptionUI*>(m_pm->FindSubControlByClass(pContainerNext, _T("OptionUI")));

			if(pOpt->IsSelected())
			{
				pOptNext->Selected(true);
				pNewSelected = GetWebPage((UINT_PTR)pContainerNext);
			}


			
		}
	}


	CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(GetWebPage((UINT_PTR)pContainer));
	if (ui)
	{
		
		string url = ui->getUrl();
		Push(url);
	}


	

	m_webcontainer->Remove(GetWebPage((UINT_PTR)pContainer));
	m_tabcontainer->Remove(pContainer);
	UnBind(pContainer);


	if(pNewSelected)
	{
		m_webcontainer->SelectItem(pNewSelected);
		m_crrentWebPage = pNewSelected;
	}

	

	return true;
}

void CMdWebEngine::Bind( CContainerUI* tab, CWebBrowserUI* page )
{
	Log("Bind CONTAINER[%08X], WEBBROWSER[%08X]", tab, page);
	m_bindings[(UINT_PTR)tab] = (UINT_PTR)page;

}

void CMdWebEngine::UnBind( CContainerUI* tab)
{
	std::map<UINT_PTR,UINT_PTR>::iterator iter;

	for(iter = m_bindings.begin(); iter != m_bindings.end(); )
	{
		if(iter->first == (UINT_PTR)tab)
		{
			m_bindings.erase(iter++);
		}
		else
		{
			iter++;


		}



	}

}

void CMdWebEngine::Push(string & url)
{
	m_deleted.push(url);
}

string CMdWebEngine::Pop()
{
	if (m_deleted.size())
	{
		string tmp = m_deleted.top();
		m_deleted.pop();
		return tmp;
	}
	return "";
}

CEditUI* CMdWebEngine::GetAddressBar()
{
	CEditUI* pEdit = dynamic_cast<CEditUI*>(m_pm->FindControl(_T("ui_address")));

	return pEdit;

}

CWebBrowserUI* CMdWebEngine::GetWebPage(UINT_PTR tab)
{
	if(tab)
	{
		return (CWebBrowserUI*)m_bindings[tab];
	}
	else
	{
		CStdPtrArray* group = m_pm->GetOptionGroup(_T("webpage"));

		for( int i = 0; i < group->GetSize(); i++ )
		{
			COptionUI* pControl = static_cast<COptionUI*>(group->GetAt(i));
			if( pControl->IsSelected())
			{
				CContainerUI* container = dynamic_cast<CContainerUI*>(pControl->GetParent());
				return (CWebBrowserUI*)m_bindings[(UINT_PTR)container];

			}
		}

	}
}

LPCTSTR CMdWebEngine::getHistoryPage()
{
	return m_history_page;
}

LPCTSTR CMdWebEngine::getIndexPage()
{
	return m_index_page;
}

LPCTSTR CMdWebEngine::getBookmarkPage()
{
	return m_bookmark_page;
}

LPCTSTR CMdWebEngine::getSettingsPage()
{
	return m_settings_page;
}

void CMdWebEngine::RestorePage()
{
	string tmp = Pop();
	if (tmp.size())
	{
		Add(_encoding(tmp).u8_utf16().getutf16().c_str());
	}
}

int CMdWebEngine::Reload(LPCTSTR url /*= NULL*/)
{
	CMdWebBrowserUI* pWebBrowserUI  = dynamic_cast<CMdWebBrowserUI*>(
		m_webcontainer->GetItemAt(m_webcontainer->GetCurSel()) );

	//检测是否是国密连接，是则设置代理
	string strUrl;

	if(url == NULL)
	{
		strUrl = pWebBrowserUI->getUrl();
	}
	else
	{
		strUrl.assign((_bstr_t)url);
	}

	
	bool b = false;
	string strback = strUrl;
	if (isGMSSL(strUrl, b))
	{
		g_um[strUrl] = strback;
		setproxy(true);
	}
	else
	{
		setproxy(false);
	}
	url = _bstr_t(strUrl.c_str());

	if(m_webcontainer->GetCount() == 0)
	{
		if(url)
		{
			Add(url);
		}
		else
		{
			Add(m_index_page);
		}


	}
	else if(url == NULL)
	{
		//reloading current page
		pWebBrowserUI->Refresh2(REFRESH_IFEXPIRED );
	}
	else
	{
		pWebBrowserUI->Navigate2(url);
		pWebBrowserUI->SetUserData(url);
	}

	return 0;

}

int CMdWebEngine::Switch( CControlUI* pOption )
{

	Log("CMdWebEngine::Switch");
	CContainerUI* pContainer = dynamic_cast<CContainerUI*>(pOption->GetParent());

	if(pContainer)
	{

		if(m_bindings.find((UINT_PTR)pContainer) != m_bindings.end())
		{

			CWebBrowserUI* ie = reinterpret_cast<CWebBrowserUI*>(m_bindings[(UINT_PTR)pContainer]);
			CMdWebBrowserUI*_ie  = dynamic_cast<CMdWebBrowserUI*>(ie);

			if (!ie)
			{
				return 0;
			}

			wstring nickname = _ie->getNickUrl();

			Log(_T("SWITCHTO CWEBBROWSER[%s]"),ie->GetUserData());

			m_webcontainer->SelectItem(ie);
			m_crrentWebPage = ie;

			CEditUI* pEdit = dynamic_cast<CEditUI*>(m_pm->FindControl(_T("ui_address")));
			//pEdit->SetText(ie->GetUserData());
			string strt = _ie->getUrl();
			if (g_um.find(strt) != g_um.end())
			{
				strt = g_um[strt];
				if(nickname.size())
				{
					pEdit->SetText(nickname.c_str());
				}
				else
				{
					pEdit->SetText(_encoding(strt).u8_utf16().getutf16().c_str());
				}
			}
			else
			{
				if(nickname.size())
				{
					pEdit->SetText(nickname.c_str());
				}
				else
				{
					pEdit->SetText(_encoding(strt).u8_utf16().getutf16().c_str());
				}
			}
		}


	}


	return 0;
}

void CMdWebEngine::ClearTemps()
{
	xstring cmd;
	cmd.format("/c del /f /s /q %s%s", theApp.getAppdir().c_str(),"tempfiles\\");

	ShellExecuteA(NULL, "open", "cmd.exe", cmd.c_str(), NULL, SW_HIDE);

}

void CMdWebEngine::CloseAll()
{



	map<UINT_PTR, UINT_PTR>::iterator iter;


	while(!m_deleted.empty()) m_deleted.pop();

	iter = m_bindings.begin();

	while(iter != m_bindings.end())
	{
		CMdWebBrowserUI* ie = static_cast<CMdWebBrowserUI*>((void*)(iter->second));

		if(ie)
		{
			Push(ie->getUrl());
			ie->GetWebBrowser2()->ExecWB(OLECMDID_CLOSE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);

		}


		iter ++;         
	}

}

stack<string> CMdWebEngine::GetDeleted()
{
	return m_deleted;
}

void CFrameWindowWnd::BookmarkAdd(void)
{
	CDuiPoint point;
	CControlUI* pCon = m_pm.FindControl(L"ui_address");
	RECT rc = pCon->GetClientPos();
	point.x = rc.right;
	point.y = rc.bottom;
	ClientToScreen(m_hWnd, &point);

	favor_recode_item item;
	item.folder = _encoding(L"书签栏").utf8().get();

	CMdWebBrowserUI* ui = static_cast<CMdWebBrowserUI*>(m_engine->m_crrentWebPage);
	item.url = ui->getUrl();
	item.img = "";





	_encoding ed(wxstring(m_engine->GetContainer(m_engine->m_crrentWebPage)->GetItemAt(0)->GetText().GetData()));
	item.title = ed.utf8().get();
	theApp.Favor()->Add(item);

	CFavorAddItemDlg* additemdlg = new CFavorAddItemDlg;
	additemdlg->m_frame = this;
	additemdlg->Create(GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 386, 254, NULL);
	additemdlg->CenterWindow();
	int nRet = additemdlg->ShowModal();

	if (nRet == 1000)
	{
		CFavorEditDlg* additemdlg = new CFavorEditDlg;
		additemdlg->m_frame = this;
		additemdlg->Create(GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 386, 254, NULL);
		additemdlg->CenterWindow();
		int nRet = additemdlg->ShowModal();
	}
}


void CFrameWindowWnd::ShowMenu(void)
{
		CDuiPoint point;
		CControlUI* pCon = m_pm.FindControl(L"toolbar");
		RECT rc = pCon->GetClientPos();
		point.x = rc.right;
		point.y = rc.bottom;
		ClientToScreen(m_hWnd, &point);

		m_pMenu = new COptionMenu(this);
		static_cast<COptionMenu*>(m_pMenu)->setCursorPos(point);

		m_pMenu->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 270, 456, NULL); 
		m_pMenu->ShowWindow(true);

		//m_pMenu->Create(m_hWnd, _T("skin\\menu.xml"), point, &m_pm, NULL, eMenuAlignment_Right | eMenuAlignment_Top);
}

int CFrameWindowWnd::ShowCloseTipDlg()
{
	auto dlg = new CCloseTipDlg;
	dlg->m_fream = this;
	dlg->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 386, 254, NULL);  
	dlg->CenterWindow();
	return dlg->ShowModal();	
}

void CFrameWindowWnd::SetNeedTip(bool need)
{
	if (need)
	{
		theApp.config("ask_before_close","yes");
	}
	else
	{
		theApp.config("ask_before_close","no");
	}

	theApp.Lock();
	sync_setting_proc(NULL);
	theApp.Unlock();
}

