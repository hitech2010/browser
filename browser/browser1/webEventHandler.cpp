#include "utilities.h"
#include "appframe.h"
#include "webEventHandler.h"
#include "Favor.h"

static std::wstring string2wstring(const std::string strIn, UINT CodePage = CP_UTF8)
{
	std::wstring strOut;
	int nLen = ::MultiByteToWideChar(CodePage, 0, strIn.data(), strIn.size(), NULL, 0);
	if (nLen)
	{
		strOut.resize(nLen);
		::MultiByteToWideChar(CodePage, 0, strIn.data(), strIn.size(), const_cast<wchar_t*>(strOut.data()), nLen);
	}
	return strOut;
}
static std::string wstring2string(const std::wstring strIn, UINT CodePage = CP_UTF8)
{
	std::string strOut;
	int nLen = ::WideCharToMultiByte(CodePage, 0, strIn.data(), strIn.size(), NULL, 0, NULL, NULL);
	if (nLen)
	{
		strOut.resize(nLen);
		::WideCharToMultiByte(CodePage, 0, strIn.data(), strIn.size(), const_cast<char*>(strOut.data()), nLen, NULL, NULL);
	}
	return strOut;
}

void CWebEventHandler::BeforeNavigate2( CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	Log("void CWebEventHandler::BeforeNavigate2");

	string strt = wstring2string(wstring(url->bstrVal));
	if (g_um.find(strt) != g_um.end())
	{
		strt = g_um[strt];
		unsigned char* puch = (unsigned char*)url->bstrVal;

		if (_tcslen(url->bstrVal))
		{
			url->bstrVal = SysAllocString(string2wstring(strt).c_str());
		}
	}
	
	/*static string strUrl;
	strUrl.assign((_bstr_t)url->bstrVal);
	
	bool b = false;
	if (isGMSSL(strUrl, b))
	{
		setproxy(true);
	}
	
	url->bstrVal = _bstr_t(strUrl.c_str());*/
}


void CWebEventHandler::NavigateError( CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel )
{
	Log("void CWebEventHandler::NavigateError");
}



void CWebEventHandler::NavigateComplete2( CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT *&url )
{
	/*CEditUI* pEdit = dynamic_cast<CEditUI*>(m_webengine->m_pm->FindControl(_T("ui_address")));
	pEdit->SetText(url->bstrVal);*/
	
	Log(_T("void CWebEventHandler::NavigateComplete2 [%08x %08X]"), static_cast<IDispatch*>(pWeb), pDisp);
	/*setproxy(false);

	BSTR bstr;
	m_webengine->m_crrentWebPage->GetWebBrowser2()->get_LocationURL(&bstr);

	CEditUI* pEdit = dynamic_cast<CEditUI*>(m_webengine->m_pm->FindControl(_T("ui_address")));
	pEdit->SetText(bstr);

	static favor_recode_item fri;
	BSTR bstrname;
	m_webengine->m_crrentWebPage->GetWebBrowser2()->get_LocationName(&bstrname);
	fri.title = wstring2string(bstrname);
	fri.url = wstring2string(bstr);
	CHistory::AddItem(fri);*/
	//setproxy(false);
	BSTR bstr;
	m_webengine->m_crrentWebPage->GetWebBrowser2()->get_LocationURL(&bstr);

	
	CEditUI* pEdit = dynamic_cast<CEditUI*>(m_webengine->m_pm->FindControl(_T("ui_address")));
	string strt = wstring2string(wstring(bstr));
	if (g_um.find(strt) != g_um.end())
	{
		strt = g_um[strt];
		pEdit->SetText(string2wstring(strt).c_str());
	}
	else
	{
		pEdit->SetText(string2wstring(strt).c_str());
	}

	BSTR bstrname;
	m_webengine->m_crrentWebPage->GetWebBrowser2()->get_LocationName(&bstrname);
	
	static history_recode_item fri;
	fri.title = wstring2string(bstrname);
	fri.url = strt;
	fri.folder = "";
	fri.img = "";

	vector<history_recode_item> tmp = theApp.History()->Query(q_thismonth);

	
	

}


void CWebEventHandler::DocumentComplete( CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT *&url )
{

	//CEditUI* pEdit = dynamic_cast<CEditUI*>(m_webengine->m_pm->FindControl(_T("ui_address")));
	//pEdit->SetText(url->bstrVal);
	//pWeb->SetUserData(url->bstrVal);
	
	//Log(_T("void CWebEventHandler::DocumentComplete [%s]"));
	

}


void CWebEventHandler::ProgressChange( CWebBrowserUI* pWeb, LONG nProgress, LONG nProgressMax )
{
	Log("void CWebEventHandler::ProgressChange");
}



void CWebEventHandler::NewWindow3( CWebBrowserUI* pWeb, IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl )
{
	Log("void CWebEventHandler::NewWindow3");
	
	m_webengine->Add(bstrUrl);
	(*Cancel) = -1;
}



void CWebEventHandler::CommandStateChange( CWebBrowserUI* pWeb, long Command,VARIANT_BOOL Enable )
{
	Log("void CWebEventHandler::CommandStateChange");
}



CWebEventHandler::CWebEventHandler( CMdWebEngine* webcore /*= NULL*/ )
{
	
m_webengine = webcore;
}


void CWebEventHandler::TitleChange(CWebBrowserUI* pWeb, BSTR bstrTitle)
{
	UINT_PTR ptr = (UINT_PTR)pWeb;
	for (auto it = this->m_webengine->m_bindings.begin(); it != this->m_webengine->m_bindings.end(); it++)
	{
		if ((UINT_PTR)(it->second) == ptr)
		{
			CContainerUI* pCon = (CContainerUI*)it->first;
			COptionUI* opt = (COptionUI*)pCon->GetItemAt(0);
			opt->SetAttribute(L"textpadding", L"15,0,15,0");
			opt->SetAttribute(L"align", L"left");
			opt->SetAttribute(L"valign", L"vcenter");
			opt->SetAttribute(L"endellipsis", L"true");
			wstring title = bstrTitle;
			opt->SetText(title.c_str());
		}
	}

}

// interface IDocHostUIHandler
HRESULT STDMETHODCALLTYPE CWebEventHandler::ShowContextMenu(CWebBrowserUI* pWeb,
	/* [in] */ DWORD dwID,
	/* [in] */ POINT __RPC_FAR *ppt,
	/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
	/* [in] */ IDispatch __RPC_FAR *pdispReserved)
{
	return E_NOTIMPL;
	//返回 E_NOTIMPL 正常弹出系统右键菜单
	//return S_OK;
	//返回S_OK 则可屏蔽系统右键菜单
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::GetHostInfo(CWebBrowserUI* pWeb,
	/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::ShowUI(CWebBrowserUI* pWeb,
	/* [in] */ DWORD dwID,
	/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
	/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
	/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
	/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::HideUI(CWebBrowserUI* pWeb)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::UpdateUI(CWebBrowserUI* pWeb)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::EnableModeless(CWebBrowserUI* pWeb,
	/* [in] */ BOOL fEnable)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::OnDocWindowActivate(CWebBrowserUI* pWeb,
	/* [in] */ BOOL fActivate)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::OnFrameWindowActivate(CWebBrowserUI* pWeb,
	/* [in] */ BOOL fActivate)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::ResizeBorder(CWebBrowserUI* pWeb,
	/* [in] */ LPCRECT prcBorder,
	/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
	/* [in] */ BOOL fRameWindow)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::TranslateAccelerator(CWebBrowserUI* pWeb,
	/* [in] */ LPMSG lpMsg,
	/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
	/* [in] */ DWORD nCmdID)
{
	return S_FALSE;
}

STDMETHODIMP CWebEventHandler::QueryInterface(REFIID riid, LPVOID *ppvObject)
{
	if(riid == IID_IUnknown)
	{
		*ppvObject = (LPVOID)static_cast<IUnknown*>(this);
		this->AddRef();
		return S_OK;
	}
	if (riid == IID_IDispatch)
	{
		*ppvObject = (LPVOID)static_cast<IDispatch*>(this);
		this->AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP CWebEventHandler::GetIDsOfNames(
	_In_ REFIID riid,
	_In_count_(cNames) _Deref_pre_z_ LPOLESTR* rgszNames,
	_In_ UINT cNames,
	_In_ LCID lcid,
	_Out_ DISPID* rgdispid)
{
	;
	
	if (_tcscmp((const wchar_t*)*rgszNames, L"test") == 0)
	{
		*rgdispid = FUN_TEST;
		return S_OK;
	}
	else if (_tcscmp((const wchar_t*)*rgszNames, L"getHistory") == 0)
	{
		*rgdispid = FUN_GET_HISTORY;
		return S_OK;
	}
	else if (_tcscmp((const wchar_t*)*rgszNames, L"getFavor") == 0)
	{
		*rgdispid = FUN_GET_FAVOR;
		
		return S_OK;
	}
	*rgdispid = -1;
	return E_FAIL;
}



STDMETHODIMP CWebEventHandler::Invoke(
	_In_ DISPID dispidMember,
	_In_ REFIID riid,
	_In_ LCID lcid,
	_In_ WORD wFlags,
	_In_ DISPPARAMS* pdispparams,
	_Out_opt_ VARIANT* pvarResult,
	_Out_opt_ EXCEPINFO* pexcepinfo,
	_Out_opt_ UINT* puArgErr)
{
	if (dispidMember == -1)
	{
		return E_FAIL;
	}

	
	
	switch (dispidMember)
	{
	case FUN_TEST:		pvarResult->vt = VT_I4; V_I4(pvarResult) = 5;	break;
	case FUN_GET_HISTORY:
	{
		vector<history_recode_item> res = theApp.History()->Query(V_I4(&pdispparams->rgvarg[0])); break;
	}
	case FUN_GET_FAVOR:
	{
		vector<favor_recode_item> res = theApp.Favor()->Query(V_I4(&pdispparams->rgvarg[0])); break;
	}
	default:;
	}


	return S_OK;
}



STDMETHODIMP_(ULONG) CWebEventHandler::Release()
{
	InterlockedIncrement(&m_dwRef);
	return m_dwRef;
}

STDMETHODIMP_(ULONG) CWebEventHandler::AddRef()
{
	ULONG ulRefCount = InterlockedDecrement(&m_dwRef);
	return ulRefCount;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::GetOptionKeyPath(CWebBrowserUI* pWeb,
	/* [out] */ LPOLESTR __RPC_FAR *pchKey,
	/* [in] */ DWORD dw)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::GetDropTarget(CWebBrowserUI* pWeb, 
	/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
	/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::GetExternal(CWebBrowserUI* pWeb, 
	/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	*ppDispatch = this;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::TranslateUrl(CWebBrowserUI* pWeb, 
	/* [in] */ DWORD dwTranslate,
	/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
	/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::FilterDataObject(CWebBrowserUI* pWeb, 
	/* [in] */ IDataObject __RPC_FAR *pDO,
	/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	return S_OK;
}

// 	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( 
// 		/* [annotation][out] */ 
// 		__deref_out  LPOLESTR *pchKey,
// 		/* [in] */ DWORD dw)
// 	{
// 		return E_NOTIMPL;
// 	}

// IDownloadManager
HRESULT STDMETHODCALLTYPE CWebEventHandler::Download( CWebBrowserUI* pWeb, 
	/* [in] */ IMoniker *pmk,
	/* [in] */ IBindCtx *pbc,
	/* [in] */ DWORD dwBindVerb,
	/* [in] */ LONG grfBINDF,
	/* [in] */ BINDINFO *pBindInfo,
	/* [in] */ LPCOLESTR pszHeaders,
	/* [in] */ LPCOLESTR pszRedir,
	/* [in] */ UINT uiCP)
{
	return S_OK;
}

