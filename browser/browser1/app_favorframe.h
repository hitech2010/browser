class CFavorAddItemDlg : public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:

#define NOTIFY_ON
#ifdef NOTIFY_ON
#define LOGNOTIFY wxstring tmp;\
	tmp.format(L"CFavorAddItemDlg::NOTIFY-psender[%s], sType[%s]", msg.pSender->GetName().GetData(), msg.sType);\
	Log(tmp);
#else
#define LOGNOTIFY __noop
#endif

	LPCTSTR GetWindowClassName() const;
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void OnFinalMessage(HWND /*hWnd*/);

	void Edit();

	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CPaintManagerUI m_pm;
	CEditUI*        m_title;
	CComboUI*       m_Combo;
	CFrameWindowWnd* m_frame;
};


class CFavorEditDlg : public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:

	LPCTSTR GetWindowClassName() const ;
	LRESULT CFavorEditDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void OnFinalMessage(HWND /*hWnd*/);

	void NewFolder();

	void DeleteSelect();

	void AddExistingFolder(CFavorFolder::RECORD record);

	void Save();

	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CPaintManagerUI m_pm;
	CFrameWindowWnd* m_frame;

	CEditUI*           m_captain;
	CEditUI*		 m_site;
	CListUI*        m_folder;
};



class CCacheClearDlg : public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:

	LPCTSTR GetWindowClassName() const ;
	LRESULT CCacheClearDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void OnFinalMessage(HWND /*hWnd*/);


	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CPaintManagerUI m_pm;
	CFrameWindowWnd* m_frame;

	CEditUI*           m_captain;
	CEditUI*		 m_site;
	CListUI*        m_folder;


	bool  m_linshiwenjian;
	bool  m_lish;
	bool  m_password;
	bool  m_download;
	bool m_changyong;
	bool m_cookies;
	bool m_recentclose;
	bool m_biaodan;
	bool m_autoclean;

};
