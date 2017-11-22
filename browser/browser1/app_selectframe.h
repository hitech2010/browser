#pragma  once
#include <ObjBase.h>
#include <DuiLib\UIlib.h>
using namespace DuiLib;


class CChooseCertsDlg : public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:
	typedef void(*PFShowCertDetail)(int id, const char * title);

	CChooseCertsDlg();
	~CChooseCertsDlg();

//#define NOTIFY_ON_CHOOSECERT
#ifdef NOTIFY_ON_CHOOSECERT
#define LOGNOTIFY_CHOOSECERT wxstring tmp;\
	tmp.format(L"NOTIFY_ON_CHOOSECERT::psender[%s], sType[%s]", msg.pSender->GetName().GetData(), msg.sType);\
	Log(tmp);
#else
#define LOGNOTIFY_CHOOSECERT __noop
#endif

	void SetData(const vector<string>& data);
	void SetViewInfoCallback(PFShowCertDetail func);
	LPCTSTR GetWindowClassName() const;
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void OnFinalMessage(HWND /*hWnd*/);
	virtual UINT GetClassStyle() const;
	void Edit();



	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	CPaintManagerUI m_pm;
	PFShowCertDetail m_cb;
	CListUI* m_list;
	int exitid;
	vector<string> m_data;
};



