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

class CMenu : public CMenuWnd, public INotifyUI
{
public:
	CMenu(CFrameWindowWnd* cfw):m_frame(cfw){}
	CFrameWindowWnd* m_frame;

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

	LRESULT CFavorAddItemDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{  
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);  
		styleValue &= ~WS_CAPTION;  
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);  

		m_pm.Init(m_hWnd);  
		m_pm.AddPreMessageFilter(this);  
		CDialogBuilder builder;  
		CControlUI* pRoot = builder.Create(_T("skin//bookmark_add.xml"), (UINT)0, NULL, &m_pm);  
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);  
		m_pm.AddNotifier(this);

		LPCTSTR pTitle = m_fream->m_engine->GetContainer(m_fream->m_engine->m_crrentWebPage)->GetItemAt(0)->GetText().GetData();
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
			wxstring folder = (const wchar_t*)_encoding(otherfolders[i].folder).u8_utf16().get().c_str();

			newlabel->SetText(folder);
			newlabel->SetFixedHeight(23);
			newlabel->SetFixedWidth(214);

			m_Combo->Add(newlabel);
		}

		CListLabelElementUI* newlabel = new CListLabelElementUI();

		newlabel->SetManager(&m_pm, NULL, false);
		newlabel->SetName(L"ui_favor_selfolder");
		newlabel->SetText(L"其它文件夹");
		newlabel->SetFixedHeight(23);
		newlabel->SetFixedWidth(214);
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
		btn->SetAttribute(_T("padding"), _T("5,7,0,0"));
		btn->SetName(_T("ui_folder_btn"));
		btn->SetBkImage(_T("skin/folder_close.png"));
		btn->SetFixedHeight(16);
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
		edit->SetText(tmp);
		elmt->Add(edit);
		

		m_folder->SelectItem(m_folder->GetCount() - 1, true);
		
		edit->SetFocus();
		SendMessage(WM_PAINT);
		m_folder->EndDown();
	}


	void CFavorEditDlg::AddExistingFolder(CFavorFolder::RECORD record)
	{

		wstring  foldername = (LPWSTR)_encoding(record.folder).u8_utf16().get().c_str();


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
		btn->SetAttribute(_T("padding"), _T("5,7,0,0"));
		btn->SetName(_T("ui_folder_btn"));
		btn->SetBkImage(_T("skin/folder_close.png"));
		btn->SetFixedHeight(16);
		btn->SetFixedWidth(16);
		elmt->Add(btn);


		label->SetManager(&m_pm, NULL, false);
// 		RECT rclable = { 5, 0, 0, 0 };
// 		label->SetPadding(rcbtn);
		label->SetAttribute(_T("padding"), _T("5,0,0,0"));
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
		default:
			bHandled = FALSE;
		}

		if (bHandled) return lRes;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}





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

	m_engine->Init(&m_pm);



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
		if(m_engine->m_bindings.size() > 1 && theApp.IfAskBeforeClose())
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
		m_engine->Add(m_engine->getBookmarkPage());
	}
	else if(msg.pSender->GetUserData().Find(_T("ui_closetab")) != -1)
	{
		m_engine->Remove(msg.pSender);
		if (m_engine->GetCount() < 2) /*UI_addtab btn left and quit */
		{
			Close();;
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
		BookmarkAdd();
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
	
	m_bookmark_page.format(_T("file:\\\\\\%ssetting\\html\\%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("bookmark.html"));
	m_index_page.format(_T("file:\\\\\\%ssetting\\%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("index.html"));
	m_history_page.format(_T("file:\\\\\\%ssetting\\html\\%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("history.html"));
	m_settings_page.format(_T("file:\\\\\\%ssetting\\html\\%s"), _encoding(theApp.getAppdir()).a_utf16().getutf16().c_str(), _T("settings.html"));

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
	pOpt->SetBkImage(_T("skin\\tab_bkgd.png"));
	pOpt->SetSelectedImage(_T("skin\\tab_focus.png"));
	pOpt->SetUserData(_T("ui_option"));
	pOpt->Selected(true);



	CWebBrowserUI* ie = new CMdWebBrowserUI();

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

int CMdWebEngine::GetCount()
{
	return m_tabcontainer->GetCount();
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

int CMdWebEngine::Reload(LPCTSTR url /*= NULL*/)
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
			Add(m_index_page);
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
	item.url = _encoding(m_engine->GetAddressBar()->GetText()).astr().get();
	item.img = "";

	_encoding ed(wxstring(m_engine->GetContainer(m_engine->m_crrentWebPage)->GetItemAt(0)->GetText().GetData()));
	item.title = ed.utf8().get();
	theApp.Favor()->Add(item);

	CFavorAddItemDlg* additemdlg = new CFavorAddItemDlg;
	additemdlg->m_fream = this;
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

	m_pMenu = new CMenu(this);
	m_pMenu->Init(NULL, _T("skin\\menu.xml"), point, &m_pm, NULL, eMenuAlignment_Right | eMenuAlignment_Top);
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
	if (need)
	{
		theApp.setAskBeforeClose("yes");
	}
	else
	{
		theApp.setAskBeforeClose("no");
	}
}