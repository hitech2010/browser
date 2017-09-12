#pragma  once
#include <ObjBase.h>
#include <DuiLib\UIlib.h>
using namespace DuiLib;

typedef map<string, string> urlmap;
extern urlmap g_um;

class CMdWebEngine
{
	
public:
static CMdWebEngine* thisobj;
	CMdWebEngine();
	void Init(CPaintManagerUI* pm);

	~CMdWebEngine();
	static CMdWebEngine* Get();
// 	int Add(LPCTSTR url = L"file:\\\\\\d:\\svn\\GMBrowser\\trunk\\browser\\Debug\\setting\\index.html");
// 	int Add(LPCTSTR url = L"file:\\\\\\d:\\svn\\GMBrowser\\trunk\\browser\\Debug\\setting\\settings.html");
// 	int Add(LPCTSTR url = L"file:\\\\\\d:\\svn\\GMBrowser\\trunk\\browser\\Debug\\setting\\history.html");
	int Add(LPCTSTR url );
	int GetCount();
	int Remove(CControlUI* btnCloseTab);
	int Reload(LPCTSTR url = NULL);
	int Switch(CControlUI* pOption);
	CWebBrowserUI* GetWebPage(UINT_PTR tab = NULL);

	LPCTSTR getHistoryPage();
	LPCTSTR getIndexPage();
	LPCTSTR getBookmarkPage();
	LPCTSTR getSettingsPage();
protected:

	void Bind(CContainerUI* tab, CWebBrowserUI* page);
	void UnBind(CContainerUI* tab);
	
public:
	CEditUI* GetAddressBar();

protected:
	wxstring m_history_page;
	wxstring m_settings_page;
	wxstring m_index_page;
	wxstring m_bookmark_page;



public:
	map<UINT_PTR, UINT_PTR> m_bindings;
	::CPaintManagerUI* m_pm;
	


	CTabLayoutUI*         m_webcontainer;
	CContainerUI*		  m_tabcontainer;

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
	
};

class CFrameWindowWnd : public CWindowWnd, public INotifyUI
{
public:

	CFrameWindowWnd();
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle();
	void OnFinalMessage(HWND /*hWnd*/);
	void OnClick(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void OnAddressNotify(TNotifyUI&msg);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL init();
	void OnSelectChanged(TNotifyUI& msg);
	void OnWebBrowserNotify(TNotifyUI& msg);
	
	void ShowAboutDlg();
	void ShowAddFavorDlg();
	void ShowHistoryForm();
	void BookmarkAdd(void);
	void ShowMenu(void);
	void ShowCloseTipDlg();
	void SetNeedTip(bool need);

public:
	::CPaintManagerUI m_pm;
	CMdWebEngine*   m_engine;
	CControlUI* m_pRoot;
	CHorizontalLayoutUI*  m_webtags;

	CMenuWnd* m_pMenu;

	//最大化最小化
	CControlUI* m_pMaxButton;
	CControlUI* m_pResotreButton;
	
};