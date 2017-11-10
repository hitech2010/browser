
#include "appframe.h"


class CMdWebBrowserUI :public CWebBrowserUI 
{
public:
	CMdWebBrowserUI();
	~CMdWebBrowserUI();

	void setTitle(const wstring& title);
	void setUrl(const string& url);
	string getUrl();
	wstring getNickUrl();
	void setNickUrl(const wstring& url);
	wstring getTitle();
	IWebBrowser2* GetIWebBrowser();
	bool IsGeemeeScheme(const wstring& lpszUrl);

	CMdWebBrowserUI& SetFavored(bool bSet);
	CMdWebBrowserUI& SetFavorRecord(CFavorManager::RECORD& rc);
	CFavorManager::RECORD& GetFavorRecord();
	bool IsFavored();

private:
	int		m_favored;
	CFavorManager::RECORD m_record;
	wstring m_title;
	string  m_url;
	wstring  m_nickurl;

};




class CWebEventHandler : public IDispatch, public CWebBrowserEventHandler
{

public:
	enum 
	{
		FUN_TEST = 1000,
		FUN_GET_HISTORY,
		FUN_GET_FAVOR,
		FUN_GET_ASYNCALL,
		FUN_GET_CALL
	};

public:
	CWebEventHandler(CMdWebEngine* webcore = NULL);


	virtual void BeforeNavigate2(CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel );
	virtual void NavigateError(CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel) ;
	virtual void NavigateComplete2(CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT *&url);
	virtual void ProgressChange(CWebBrowserUI* pWeb, LONG nProgress, LONG nProgressMax);
	virtual void NewWindow3(CWebBrowserUI* pWeb, IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
	virtual void CommandStateChange(CWebBrowserUI* pWeb, long Command,VARIANT_BOOL Enable);
	virtual void TitleChange(CWebBrowserUI* pWeb, BSTR bstrTitle);
	virtual void DocumentComplete(CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT *&url);


	// interface IDocHostUIHandler
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(CWebBrowserUI* pWeb, 
		/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved);
	virtual HRESULT STDMETHODCALLTYPE ResizeBorder(CWebBrowserUI* pWeb, /* [in] */ LPCRECT prcBorder, /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow, /* [in] */ BOOL fRameWindow);
	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(CWebBrowserUI* pWeb, /* [in] */ BOOL fActivate);
	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(CWebBrowserUI* pWeb, /* [in] */ BOOL fActivate);
	virtual HRESULT STDMETHODCALLTYPE EnableModeless(CWebBrowserUI* pWeb, /* [in] */ BOOL fEnable);
	virtual HRESULT STDMETHODCALLTYPE UpdateUI( CWebBrowserUI* pWeb);
	virtual HRESULT STDMETHODCALLTYPE HideUI( CWebBrowserUI* pWeb);
	virtual HRESULT STDMETHODCALLTYPE ShowUI(CWebBrowserUI* pWeb, /* [in] */ DWORD dwID, /* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject, /* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget, /* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame, /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(CWebBrowserUI* pWeb, /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
	virtual HRESULT STDMETHODCALLTYPE Download( CWebBrowserUI* pWeb, /* [in] */ IMoniker *pmk, /* [in] */ IBindCtx *pbc, /* [in] */ DWORD dwBindVerb, /* [in] */ LONG grfBINDF, /* [in] */ BINDINFO *pBindInfo, /* [in] */ LPCOLESTR pszHeaders, /* [in] */ LPCOLESTR pszRedir, /* [in] */ UINT uiCP);
	virtual HRESULT STDMETHODCALLTYPE FilterDataObject(CWebBrowserUI* pWeb, /* [in] */ IDataObject __RPC_FAR *pDO, /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
	virtual HRESULT STDMETHODCALLTYPE TranslateUrl(CWebBrowserUI* pWeb, /* [in] */ DWORD dwTranslate, /* [in] */ OLECHAR __RPC_FAR *pchURLIn, /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
	virtual HRESULT STDMETHODCALLTYPE GetExternal(CWebBrowserUI* pWeb, /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
	virtual HRESULT STDMETHODCALLTYPE GetDropTarget(CWebBrowserUI* pWeb, /* [in] */ IDropTarget __RPC_FAR *pDropTarget, /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(CWebBrowserUI* pWeb, /* [out] */ LPOLESTR __RPC_FAR *pchKey, /* [in] */ DWORD dw);
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(CWebBrowserUI* pWeb, /* [in] */ LPMSG lpMsg, /* [in] */ const GUID __RPC_FAR *pguidCmdGroup, /* [in] */ DWORD nCmdID);


	//interface IUnknown
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);


	//interface IDispatch

	STDMETHOD(GetTypeInfoCount)(_Out_ UINT* pctinfo)
	{
		if (pctinfo == NULL)
			return E_POINTER;
		*pctinfo = 0;
		return S_OK;
	}
	STDMETHOD(GetTypeInfo)(
		_In_ UINT itinfo,
		_In_ LCID lcid,
		_Deref_out_ ITypeInfo** pptinfo)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetIDsOfNames)(
		_In_ REFIID riid,
		_In_count_(cNames) _Deref_pre_z_ LPOLESTR* rgszNames,
		_In_ UINT cNames,
		_In_ LCID lcid,
		_Out_ DISPID* rgdispid);


	STDMETHOD(Invoke)(
		_In_ DISPID dispidMember,
		_In_ REFIID riid,
		_In_ LCID lcid,
		_In_ WORD wFlags,
		_In_ DISPPARAMS* pdispparams,
		_Out_opt_ VARIANT* pvarResult,
		_Out_opt_ EXCEPINFO* pexcepinfo,
		_Out_opt_ UINT* puArgErr);

protected:

	CMdWebEngine* m_webengine;
	ULONG m_dwRef;


};