#include "utilities.h"
#include "resource.h"

#include "appframe.h"
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
		if (msg.pSender->GetName() == _T("ui_closetipdlg_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
		}
		else if(msg.pSender->GetName() == _T("ui_close_tip_ok") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			::PostMessage(m_fream->GetHWND(),WM_DESTROY,NULL,NULL);
		}
		else if(msg.pSender->GetName() == _T("ui_close_tip_cancel") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
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
		else if(msg.pSender->GetName() == _T("ui_close_tip_checkboxtext") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			COptionUI* option = static_cast<COptionUI*>(m_pm.FindControl(_T("ui_close_tip_checkbox")));
			if(option->IsSelected())
			{
				option->Selected(false);
				m_fream->SetNeedTip(true);
			}
			else
			{
				option->Selected(true);
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

class CMenu : public CMenuWnd, public INotifyUI
{
public:
	CMenu(CFrameWindowWnd* cfw):m_frame(cfw){}
	CFrameWindowWnd* m_frame;

	void Notify(TNotifyUI& msg)
	{
		if (msg.pSender->GetName() == _T("seting_homepage") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			m_frame->m_engine->m_crrentWebPage->SetHomePage(L"www.baidu.com");
			MessageBox(m_frame->GetHWND(), L"设为主页成功", tip, MB_OK);
		}
		else if (msg.pSender->GetName() == _T("ebank_safe") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			MessageBox(m_frame->GetHWND(), L"该功能暂不支持", tip, MB_OK);
		}
		else if (msg.pSender->GetName() == _T("clear") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			//WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 255", 9);
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 8", 9);
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 2", 9);
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 16", 9);
			WinExec("rundll32.exe Inetcpl.cpl, ClearMyTracksByProcess 32", 9);
			MessageBox(m_frame->GetHWND(), L"清理缓存成功", tip, MB_OK);
		}
		else if (msg.pSender->GetName() == _T("history") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			::SendMessage(this->GetHWND(),WM_KILLFOCUS,NULL,NULL);
			::PostMessage(m_frame->GetHWND(),WM_SHOW_HISTORY,NULL,NULL);
		}
		else if (msg.pSender->GetName() == _T("setings") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			WinExec("rundll32.exe shell32.dll, Control_RunDLL Inetcpl.cpl", 9);
		}
		else if (msg.pSender->GetName() == _T("about") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			m_frame->ShowAboutDlg();
		}
		else if (msg.pSender->GetName() == _T("go_homepage") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			m_frame->m_engine->m_crrentWebPage->NavigateHomePage();
		}
	}
};

class CFavorAddItemDlg : public CWindowWnd, public INotifyUI, public IMessageFilterUI  
{
public:

	LPCTSTR GetWindowClassName() const { return _T("UIFavorAddItemForm"); };
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)  
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
	void OnFinalMessage(HWND /*hWnd*/)  
	{  
		m_pm.RemovePreMessageFilter(this);  
		delete this;  
	}

	void AddItem()
	{
		CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("ui_favor_add_title")));

		favor_recode_item hri;
		LPCTSTR pTitle = pEdit->GetText().GetData();
		hri.title = Codec::wstring2string(pTitle);

		BSTR bstr = NULL;
		m_fream->m_engine->m_crrentWebPage->GetWebBrowser2()->get_LocationURL(&bstr);
		//hri.url = Codec::wstring2string(bstr);
		string strt = Codec::wstring2string(wstring(bstr));
		if (g_um.find(strt) != g_um.end())
		{
			hri.url  = g_um[strt];
		}
		else
		{
			hri.url  = strt;
		}

		CFavor::AddItem(hri);

		this->Close(IDCLOSE);
	}

	void Notify(TNotifyUI& msg)  
	{
		if (msg.pSender->GetName() == _T("ui_favor_add_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
		}
		if (msg.pSender->GetName() == _T("ui_favor_add_ok") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			AddItem();
		}
		if (msg.pSender->GetName() == _T("ui_favor_add_cancel") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
		}
		if (msg.pSender->GetName() == _T("ui_favor_add_title") && msg.sType == DUI_MSGTYPE_RETURN)
		{
			AddItem();
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
		CControlUI* pRoot = builder.Create(_T("skin//favor_add_item.xml"), (UINT)0, NULL, &m_pm);  
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);  
		m_pm.AddNotifier(this);

		LPCTSTR pTitle = m_fream->m_engine->GetContainer(m_fream->m_engine->m_crrentWebPage)->GetItemAt(0)->GetText().GetData();
		CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("ui_favor_add_title")));
		pEdit->SetText(pTitle);
		pEdit->SetFocus();

		return 0;  
	}  

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)  
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
		default:  
			bHandled = FALSE;  
		}  

		if (bHandled) return lRes;  
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;  
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
	} 

public:
	CPaintManagerUI m_pm;
	CFrameWindowWnd* m_fream;
};

class CFavorForm : public CWindowWnd, public INotifyUI, public IMessageFilterUI  
{
public:

	LPCTSTR GetWindowClassName() const { return _T("UIFavorForm"); };
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)  
	{
		return false;
	}
	void OnFinalMessage(HWND /*hWnd*/)  
	{  
		m_pm.RemovePreMessageFilter(this);  
		delete this;  
	};

	void RemoveItem(CControlUI* item)
	{
		CListUI *pList = static_cast<CListUI*>(m_pm.FindControl(_T("ui_favor_list")));

		//处理数据

		CButtonUI * pBtnTitle = static_cast<CButtonUI*>(((CListContainerElementUI*)item)->GetItemAt(1));
		string title = Codec::wstring2string(pBtnTitle->GetText().GetData());
		CFavor::DeleteItem(title);

		//处理界面
		pList->Remove(item);
	}

	void OpenItem(CControlUI* item)
	{
		static favor_recode_item fri;
		CButtonUI * pBtnTitle = static_cast<CButtonUI*>(item);
		string title = Codec::wstring2string(pBtnTitle->GetText().GetData());
		CFavor::GetItem(title,fri);
		m_fream->m_engine->Add(Codec::string2wstring(fri.url).c_str());
	}

	void Init()
	{
		static FAVOR_LIST hl;
		hl.clear();

		CFavor::GetAllItem(hl);
		int listsize = hl.size();
		auto & fs_list = hl;

		CListUI *pList = static_cast<CListUI*>(m_pm.FindControl(_T("ui_favor_list")));
		for(int i = 0; i < listsize; ++i)
		{
			wstring strTitle = Codec::string2wstring(fs_list[i].title);

			wstring strImagePath = _T("skin//image.png");

			CListContainerElementUI* item = new CListContainerElementUI;
			item->SetMaxWidth(216);
			item->SetMaxHeight(20);
			RECT rc;
			rc.left = 27;
			rc.top = 0;
			rc.right = 27;
			rc.bottom = 0;
			item->SetPadding(rc);

			CLabelUI* lableImage = new CLabelUI;
			lableImage->SetBkImage(strImagePath.c_str());
			lableImage->SetMaxWidth(16);
			lableImage->SetMaxHeight(16);
			item->Add(lableImage);


			CButtonUI* btn_title = new CButtonUI;
			btn_title->SetText(strTitle.c_str());
			btn_title->SetMinWidth(180);
			btn_title->SetMinHeight(20);
			btn_title->SetName(_T("ui_favor_item_open"));
			item->Add(btn_title);

			CButtonUI* btn_close = new CButtonUI;
			btn_close->SetBkImage(_T("file='skin\\tab_close.png' source='16,0,32,16'"));
			btn_close->SetHotImage(_T("file='skin\\tab_close.png' source='32,0,48,16'"));
			btn_close->SetMaxWidth(16);
			btn_close->SetMaxHeight(16);
			btn_close->SetName(_T("ui_favor_item_close"));
			item->Add(btn_close);

			pList->Add(item);
		}
	}

	void Notify(TNotifyUI& msg)  
	{
		Log(_T("msg.pSender->GetClass() [%s] msg[%s]"), msg.pSender->GetClass(), msg.sType);
		if (msg.pSender->GetName() == _T("ui_favor_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
		}
		else if (msg.pSender->GetName() == _T("ui_favor_add") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			::PostMessage(m_fream->GetHWND(),WM_SHOW_FAVOR_ADD,NULL,NULL);
		}
		else if (msg.pSender->GetName() == _T("ui_favor_item_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->RemoveItem(msg.pSender->GetParent());
		}
		else if (msg.pSender->GetName() == _T("ui_favor_item_open") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			OpenItem(msg.pSender);
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
		CControlUI* pRoot = builder.Create(_T("skin//favor.xml"), (UINT)0, NULL, &m_pm);  
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);  
		m_pm.AddNotifier(this);

		this->Init();

		return 0;  
	}  

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)  
	{  
		LRESULT lRes = 0;  
		BOOL bHandled = TRUE;  
		switch (uMsg) 
		{  
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:	  
			{
				this->Close(IDCLOSE);
				return true;
			}
			break;
		default:  
			bHandled = FALSE;  
		}  
		if (bHandled) return lRes;  
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;  
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
	}
public:
	CPaintManagerUI m_pm;
	CFrameWindowWnd* m_fream;
};

class CHistoryForm : public CWindowWnd, public INotifyUI, public IMessageFilterUI  
{
public:
	static CHistoryForm* s_historyform;
	static void Show(CFrameWindowWnd* fream)
	{
		CDuiPoint point;
		CControlUI* pCon = fream->m_pm.FindControl(L"toolbar");
		RECT rc = pCon->GetClientPos();
		point.x = rc.right;
		point.y = rc.bottom;
		ClientToScreen(fream->GetHWND(), &point);

		if(s_historyform == NULL)
		{
			s_historyform = new CHistoryForm;
			s_historyform->m_fream = fream;
			s_historyform->Create(fream->GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, 0, point.x - 270, point.y, 270, 355, NULL);
		}
		s_historyform->ShowWindow();
	}

	LPCTSTR GetWindowClassName() const { return _T("UIHistoryForm"); };
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)  
	{
		if(uMsg == WM_CLOSE)
		{
			s_historyform = NULL;
		}

		return false;
	}
	void OnFinalMessage(HWND /*hWnd*/)  
	{  
		//m_pm.RemovePreMessageFilter(this);  
		//delete this;  
	};

	void RemoveItem(CControlUI* item)
	{
		CListUI *pList = static_cast<CListUI*>(m_pm.FindControl(_T("ui_history_list")));

		//处理数据
		CHistory::DeleteItem(m_mapindex[item]);

		pList->Remove(item);
	}

	void Init()
	{
		CHistory::GetAllItem();
		int listsize = CHistory::s_hl.size();
		auto & fs_list = CHistory::s_hl;

		CListUI *pList = static_cast<CListUI*>(m_pm.FindControl(_T("ui_history_list")));
		for(int i = 0; i < listsize; ++i)
		{
			wstring strTitle = Codec::string2wstring(fs_list[i].title);
			wstring strImagePath = _T("skin//image.png");

			CListContainerElementUI* item = new CListContainerElementUI;
			item->SetMinWidth(216);
			item->SetMinHeight(20);
			item->SetMaxHeight(20);
			RECT rc;
			rc.left = 27;
			rc.top = 0;
			rc.right = 27;
			rc.bottom = 0;
			item->SetPadding(rc);

			CLabelUI* lableImage = new CLabelUI;
			lableImage->SetBkImage(strImagePath.c_str());
			lableImage->SetMaxWidth(16);
			lableImage->SetMaxHeight(16);
			item->Add(lableImage);


			CButtonUI* btn_title = new CButtonUI;
			btn_title->SetText(strTitle.c_str());
			btn_title->SetMinWidth(180);
			btn_title->SetMinHeight(20);
			btn_title->SetName(_T("ui_history_item_open"));
			item->Add(btn_title);

			CButtonUI* btn_close = new CButtonUI;
			btn_close->SetBkImage(_T("file='skin\\tab_close.png' source='16,0,32,16'"));
			btn_close->SetHotImage(_T("file='skin\\tab_close.png' source='32,0,48,16'"));
			btn_close->SetMaxWidth(16);
			btn_close->SetMaxHeight(16);
			btn_close->SetName(_T("ui_history_item_close"));
			item->Add(btn_close);

			pList->Add(item);
			m_mapindex[item] = i;
		}
	}

	void Notify(TNotifyUI& msg)  
	{
		if (msg.pSender->GetName() == _T("ui_history_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->Close(IDCLOSE);
		}
		else if (msg.pSender->GetName() == _T("ui_history_item_close") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			this->RemoveItem(msg.pSender->GetParent());
		}
		else if(msg.pSender->GetName() == _T("ui_history_item_open") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			static favor_recode_item fri;
			CListUI *pList = static_cast<CListUI*>(m_pm.FindControl(_T("ui_history_list")));
			CHistory::GetItem(pList->GetItemIndex(msg.pSender->GetParent()),fri);
			m_fream->m_engine->Add(Codec::string2wstring(fri.url).c_str());
		}
		else if(msg.pSender->GetName() == _T("ui_history_clear") && msg.sType == DUI_MSGTYPE_CLICK)
		{
			CListUI *pList = static_cast<CListUI*>(m_pm.FindControl(_T("ui_history_list")));
			CHistory::DeleteAllItem();
			pList->RemoveAll();
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
		CControlUI* pRoot = builder.Create(_T("skin//history.xml"), (UINT)0, NULL, &m_pm);  
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);  
		m_pm.AddNotifier(this);

		this->Init();

		return 0;  
	}  

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)  
	{  
		LRESULT lRes = 0;  
		BOOL bHandled = TRUE;  
		switch (uMsg) 
		{  
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS: Close(); bHandled = False; break;
		default:  
			bHandled = FALSE;  
		}  

		if (bHandled) return lRes;  
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;  
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
	}
public:
	CPaintManagerUI m_pm;
	CFrameWindowWnd* m_fream;
	map<void*,int> m_mapindex;
};
CHistoryForm* CHistoryForm::s_historyform = NULL;

CMdWebEngine* CMdWebEngine::thisobj = new CMdWebEngine();

CFrameWindowWnd::CFrameWindowWnd(){m_pMenu = NULL;};

void CFrameWindowWnd::ShowAddFavorDlg()
{
	CFavorAddItemDlg* additemdlg = new CFavorAddItemDlg;
	additemdlg->m_fream = this;
	additemdlg->Create(GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 386, 254, NULL);  
	additemdlg->CenterWindow();
	additemdlg->ShowModal();
}

void CFrameWindowWnd::ShowHistoryForm()
{
	CHistoryForm::Show(this);
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

	m_engine->Init(&m_pm);

	CHistory::GetAllItem();

	m_bTip = true;
	ifstream f("gmbrowserseting.ini");
	if(f.is_open())
	{
		m_bTip = false;
		f.close();
	}

	//SetIcon(IDR_MAINFRAME);

	return TRUE;
}
LPCTSTR CFrameWindowWnd::GetWindowClassName() const { return _T("UIMainFrame"); };
UINT CFrameWindowWnd::GetClassStyle() { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
void CFrameWindowWnd::OnFinalMessage(HWND /*hWnd*/) { delete this; };


void CFrameWindowWnd::OnWebBrowserNotify(TNotifyUI& msg)
{
	if(msg.sType == _T("setfocus"))
	{
		Log(L"control %s Messagetype  [%s] %08X",msg.pSender->GetClass(), msg.sType, msg.pSender);

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


	if(msg.pSender->GetUserData().Find(_T("ui_option")) != -1)
	{
		Log("OnSelectChanged %08X", msg.pSender);
		COptionUI* pOption = dynamic_cast<COptionUI*>(msg.pSender);
		m_engine->Switch(pOption);
	}

}
void CFrameWindowWnd::OnClick(TNotifyUI& msg)
{

	if( msg.pSender->GetName() == _T("ui_close") ) {
		if(m_engine->m_bindings.size() > 1 && m_bTip)
		{
			ShowCloseTipDlg();
		}
		else
		{
			Close();
		}
	}
	else if(msg.pSender->GetName() == _T("ui_addtab"))
	{
		m_engine->Add();
	}
	else if(msg.pSender->GetUserData().Find(_T("ui_closetab")) != -1)
	{
		m_engine->Remove(msg.pSender);
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



}
void CFrameWindowWnd::Notify(TNotifyUI& msg)
{
	Log(_T("msg.pSender->GetClass() [%s] msg[%s]"), msg.pSender->GetClass(), msg.sType);

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
		ShowFavorForm();
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
}

void CFrameWindowWnd::OnAddressNotify(TNotifyUI&msg)
{
	if(msg.sType == DUI_MSGTYPE_RETURN)
	{
		CEditUI* pEdit = dynamic_cast<CEditUI*>(msg.pSender);
		assert(pEdit);
		m_engine->Reload((LPCTSTR)(pEdit->GetText()));


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

			init();

			break;
		}
	case WM_DESTROY:
		{
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
}

CMdWebEngine::~CMdWebEngine()
{
	thisobj = NULL;
}

void CMdWebEngine::Init( CPaintManagerUI* pm )
{
	m_pm = pm;
	m_webcontainer = dynamic_cast<CTabLayoutUI*>(m_pm->FindControl(_T("ui_webcontainer")));
	assert(m_webcontainer);
	m_tabcontainer = dynamic_cast<CContainerUI*>(m_pm->FindControl(_T("htab")));
	assert(m_tabcontainer);
}

int CMdWebEngine::Add(LPCTSTR url)
{
	Log("COptionUIMgr::Add");

	CButtonUI* pBtn = new CButtonUI();
	COptionUI* pOpt  = new COptionUI();
	CContainerUI* pContainer = new CContainerUI();


	if(pBtn == NULL ||
		pOpt == NULL ||
		pContainer == NULL)
	{
		if(pBtn) delete pBtn;
		if(pOpt) delete pOpt;
		if(pContainer) delete pContainer;
		return -1;
	}

	pContainer->SetFixedWidth(132);
	m_tabcontainer->AddAt(pContainer,m_tabcontainer->GetCount()-1);

	Log("COptionUIMgr::New Option %08X", pOpt);




	pContainer->Add(pOpt);
	pContainer->Add(pBtn);

	pBtn->SetFloat(true);
	pBtn->SetFixedWidth(16);
	pBtn->SetFixedHeight(16);

	pBtn->SetBkImage(_T("file='skin\\tab_close.png' source='16,0,32,16'"));
	pBtn->SetHotImage(_T("file='skin\\tab_close.png' source='32,0,48,16'"));
	pBtn->SetUserData(_T("ui_closetab"));
	pBtn->SetAttribute(_T("pos"), _T("-16,0,0,16"));
	pBtn->SetAttribute(_T("floatalign"), _T("right"));
	pBtn->SetAttribute(_T("floatalign"), _T("vcenter"));




	pOpt->SetBkImage(_T("skin\\tab.png"));
	pOpt->SetGroup(_T("webpage"));
	pOpt->SetTag((UINT_PTR)pOpt);
	pOpt->SetSelectedImage(_T("skin\\tab_focus.png"));
	pOpt->SetUserData(_T("ui_option"));
	pOpt->Selected(true);



	CWebBrowserUI* ie = new CWebBrowserUI;

	m_webcontainer->Add(ie);

	ie->CActiveXUI::SetAttribute(_T("clsid"), _T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
	ie->SetDelayCreate(true);

	static CWebEventHandler handler(this); 
	ie->SetWebBrowserEventHandler(&handler);
	ie->DoCreateControl();

	m_webcontainer->SelectItem(ie);
	ie->SetFloat(false);
	ie->SetUserData(url);

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
	m_crrentWebPage = ie;

	//ie->GetWebBrowser2()->put_RegisterAsBrowser(VARIANT_TRUE);
	//ie->GetWebBrowser2()->get_Application(ppDisp); //这一句是关键，关联我们新建的窗口的browser控件到点击的目标链接

	////设置地址栏
	//CEditUI* pEdit = dynamic_cast<CEditUI*>(this->m_pm->FindControl(_T("ui_address")));
	//pEdit->SetText(url);


	return true;
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

			}

			pNewSelected = GetWebPage((UINT_PTR)pContainerPrev);



		}
		else
		{
			CContainerUI* pContainerNext = dynamic_cast<CContainerUI*>(m_tabcontainer->GetItemAt(pos+1));
			COptionUI* pOptNext = dynamic_cast<COptionUI*>(m_pm->FindSubControlByClass(pContainerNext, _T("OptionUI")));

			if(pOpt->IsSelected())
			{
				pOptNext->Selected(true);

			}

			pNewSelected = GetWebPage((UINT_PTR)pContainerNext);
		}
	}


	m_tabcontainer->Remove(pContainer);
	m_webcontainer->Remove(GetWebPage((UINT_PTR)pContainer));

	if(pNewSelected)
	{

		//m_webcontainer->SelectItem(pNewSelected);
		m_webcontainer->NeedParentUpdate();
		m_crrentWebPage = pNewSelected;
	}

	UnBind(pContainer);


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

int CMdWebEngine::Reload( LPCTSTR url /*= NULL*/ )
{
	CWebBrowserUI* pWebBrowserUI  = dynamic_cast<CWebBrowserUI*>(
		m_webcontainer->GetItemAt(m_webcontainer->GetCurSel()) );

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

	if(m_webcontainer->GetCount() == 0)
	{
		if(url)
		{
			Add(url);
		}
		else
		{
			Add();
		}


	}
	else if(url == NULL)
	{
		//reloading current page
		pWebBrowserUI->Refresh2(REFRESH_COMPLETELY );
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
	CContainerUI* pContainer = dynamic_cast<CContainerUI*>(pOption->GetParent());

	if(pContainer)
	{

		if(m_bindings.find((UINT_PTR)pContainer) != m_bindings.end())
		{

			CWebBrowserUI* ie = reinterpret_cast<CWebBrowserUI*>(m_bindings[(UINT_PTR)pContainer]);

			Log(_T("SWITCHTO CWEBBROWSER[%s]"),ie->GetUserData());

			m_webcontainer->SelectItem(ie);
			m_crrentWebPage = ie;

			CEditUI* pEdit = dynamic_cast<CEditUI*>(m_pm->FindControl(_T("ui_address")));
			//pEdit->SetText(ie->GetUserData());
			string strt = Codec::wstring2string(ie->GetUserData().GetData());
			if (g_um.find(strt) != g_um.end())
			{
				strt = g_um[strt];
				pEdit->SetText(Codec::string2wstring(strt).c_str());
			}
			else
			{
				pEdit->SetText(Codec::string2wstring(strt).c_str());
			}
		}


	}


	return 0;
}





void CFrameWindowWnd::ShowFavorForm(void)
{
	CDuiPoint point;
	CControlUI* pCon = m_pm.FindControl(L"ui_address");
	RECT rc = pCon->GetClientPos();
	point.x = rc.right;
	point.y = rc.bottom;
	ClientToScreen(m_hWnd, &point);

	CFavorForm* cff = new CFavorForm;
	cff->m_fream = this;
	cff->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0, point.x - 270, point.y, 270, 355, NULL);  
	cff->ShowWindow();
}


void CFrameWindowWnd::ShowMenu(void)
{
	CDuiPoint point;
	CControlUI* pCon = m_pm.FindControl(L"toolbar");
	RECT rc = pCon->GetClientPos();
	point.x = rc.right;
	point.y = rc.bottom;
	ClientToScreen(m_hWnd, &point);

	m_pMenu = new CMenu(this);
	m_pMenu->Init(NULL, _T("skin\\menu.xml"), point, &m_pm, NULL, eMenuAlignment_Right | eMenuAlignment_Top);
	CMenuWnd::SetMenuItemInfo(_T("qianting"), true);	//设置状态
}

void CFrameWindowWnd::ShowCloseTipDlg()
{
	auto dlg = new CCloseTipDlg;
	dlg->m_fream = this;
	dlg->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 386, 254, NULL);  
	dlg->CenterWindow();
	dlg->ShowModal();	
}

void CFrameWindowWnd::SetNeedTip(bool need)
{
	if(!need)
	{
		ofstream f("gmbrowserseting.ini");
		if(f.is_open())
		{
			m_bTip = false;
			f.close();
		}
	}
	else
	{
		remove("gmbrowserseting.ini");
	}

}