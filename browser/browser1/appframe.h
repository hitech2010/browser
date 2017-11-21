#pragma  once
#include <ObjBase.h>
#include <DuiLib\UIlib.h>
using namespace DuiLib;

typedef map<string, string> urlmap;
extern urlmap g_um;

#include <stack>
using namespace std;

namespace THEAPP
{
#define favor_hot L"file='skin\\favor.png' source='24,0,48,24'"
#define favor_nor L"file='skin\\favor.png' source='0,0,24,24'"
}

#define WM_USER_OPTIONMENU_ABOUT  (WM_APP + 1500L)
#define WM_USER_OPTIONMENU_DOWNLOAD (WM_USER_OPTIONMENU_ABOUT + 1)
#define WM_USER_OPTIONMENU_SAVEASFILE (WM_USER_OPTIONMENU_ABOUT + 2)
#define WM_USER_OPTIONMENU_SAVEASIMG  (WM_USER_OPTIONMENU_ABOUT + 3)
#define WM_USER_OPTIONMENU_CLEARCHCHE  (WM_USER_OPTIONMENU_ABOUT + 4)
#define WM_USER_OPTIONMENU_PRINT		(WM_USER_OPTIONMENU_ABOUT+ 5)
#define WM_USER_SELECT_CERTS		    (WM_USER_OPTIONMENU_ABOUT+ 6)


class CMdWebEngine
{
	
public:

static CMdWebEngine* thisobj;
	CMdWebEngine();
	void Init(CPaintManagerUI* pm , CWindowWnd* wnd);

	~CMdWebEngine();
	static CMdWebEngine* Get();
// 	int Add(LPCTSTR url = L"file:\\\\\\d:\\svn\\GMBrowser\\trunk\\browser\\Debug\\setting\\index.html");
// 	int Add(LPCTSTR url = L"file:\\\\\\d:\\svn\\GMBrowser\\trunk\\browser\\Debug\\setting\\settings.html");
// 	int Add(LPCTSTR url = L"file:\\\\\\d:\\svn\\GMBrowser\\trunk\\browser\\Debug\\setting\\history.html");
	int Add(LPCTSTR url , bool background = false);
	int Add(int ipage);
	int GetCount();
	int Remove(CControlUI* btnCloseTab);
	int Reload(LPCTSTR url = NULL);
	int Switch(CControlUI* pOption);

	void ClearTemps();

	void CloseAll();

	stack<string> GetDeleted();

	CWebBrowserUI* GetWebPage(UINT_PTR tab = NULL);

	LPCTSTR getHistoryPage();
	LPCTSTR getIndexPage();
	LPCTSTR getBookmarkPage();
	LPCTSTR getSettingsPage();
	void RestorePage();
protected:

	void Bind(CContainerUI* tab, CWebBrowserUI* page);
	void UnBind(CContainerUI* tab);
	void Push(string & url);
	string Pop();


	stack<string> m_deleted;
	
public:
	CEditUI* GetAddressBar();

protected:
	wxstring m_history_page;
	wxstring m_settings_page;
	wxstring m_index_page;
	wxstring m_bookmark_page;

	enum
	{
		page_bookmark,
		page_history,
		page_settings,
		page_index
		
	};



public:
	map<UINT_PTR, UINT_PTR> m_bindings;
	::CPaintManagerUI* m_pm;
	


	CTabLayoutUI*         m_webcontainer;
	CContainerUI*		  m_tabcontainer;
	CWindowWnd*			  m_pWnd;

	CWebBrowserUI* m_crrentWebPage;
	CContainerUI* GetContainer(CWebBrowserUI*pWeb)
	{
		UINT_PTR ptr = (UINT_PTR)pWeb;
		for (auto it = this->m_bindings.begin(); it != this->m_bindings.end(); it++)
		{
			if ((UINT_PTR)(it->second) == ptr)
			{
				return (CContainerUI*) it->first;
			}
		}
		return NULL;
	}

	CWebBrowserUI* GetCurrentWebBrowserUI()
	{
		

		return m_crrentWebPage;
	}

	
};


class CFrameWindowWnd : public CWindowWnd, public INotifyUI
{
public:

	CFrameWindowWnd();

	~CFrameWindowWnd();


	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);
	void OnClick(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void OnAddressNotify(TNotifyUI&msg);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL init();
	void OnSelectChanged(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnWebBrowserNotify(TNotifyUI& msg);
	
	void ShowAboutDlg();
	void ShowAddFavorDlg();
	void ShowHistoryForm();
	void ShowSelectCertDlg();
	void BookmarkAdd(void);
	void ShowMenu(void);
	int ShowCloseTipDlg();
	void SetNeedTip(bool need);

	



//#define NOTIFY_ON
#ifdef NOTIFY_ON
#define LOGNOTIFY wxstring tmp;\
	tmp.format(L"CFrameWindowWnd::NOTIFY-psender[%s], sType[%s]", msg.pSender->GetName().GetData(), msg.sType);\
	Log(tmp);
#else
#define LOGNOTIFY __noop
#endif

public:
	::CPaintManagerUI m_pm;
	CMdWebEngine*   m_engine;
	CControlUI* m_pRoot;
	CHorizontalLayoutUI*  m_webtags;

	vector<string>      m_data;

	CWindowWnd* m_pMenu;

	//最大化最小化
	CControlUI* m_pMaxButton;
	CControlUI* m_pResotreButton;
	
};

extern CFrameWindowWnd* g_frame;