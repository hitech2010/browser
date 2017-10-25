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
	
	if (!url)
	{
		return;
	}

	
	

	string strt = wstring2string(wstring(url->bstrVal));
	if (TargetFrameName)
	{
		Log(L"void CWebEventHandler::BeforeNavigate2 %s TargetFrameName[%s]", url->bstrVal, TargetFrameName->bstrVal);
	}
	else
	{
		Log(L"void CWebEventHandler::BeforeNavigate2 %s ", url->bstrVal);
	}

	if (g_um.find(strt) != g_um.end())
	{
		strt = g_um[strt];
		unsigned char* puch = (unsigned char*)url->bstrVal;

		if (_tcslen(url->bstrVal))
		{
			url->bstrVal = SysAllocString(string2wstring(strt).c_str());
		}
	}





	CMdWebBrowserUI* pmdweb = dynamic_cast<CMdWebBrowserUI*>(pWeb);

	if (pWeb->GetWebBrowser2() == pDisp && pmdweb)
	{
		pmdweb->setUrl(strt);
		pmdweb->setNickUrl(L"");

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
	if (!url)
	{
		return;
	}

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

	CMdWebBrowserUI* ui = dynamic_cast<CMdWebBrowserUI*>(pWeb);


	if (pWeb->GetWebBrowser2() == pDisp && ui)
	{
		ui->setNickUrl(bstr);
		wstring nickname = ui->getNickUrl();


		if (g_um.find(strt) != g_um.end())
		{
			strt = g_um[strt];
			if(nickname.size())
			{
				pEdit->SetText(nickname.c_str());
			}
			else
			{
				pEdit->SetText(string2wstring(strt).c_str());
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
				pEdit->SetText(string2wstring(strt).c_str());
			}

		}

	}
	



	BSTR bstrname;
	m_webengine->m_crrentWebPage->GetWebBrowser2()->get_LocationName(&bstrname);
	
	

}


void CWebEventHandler::DocumentComplete( CWebBrowserUI* pWeb, IDispatch *pDisp,VARIANT *&url )
{
	if (!url || !url->bstrVal)
	{
		return;
	}
	Log(_T("void CWebEventHandler::DocumentComplete [%08x %08X]"), pWeb->GetWebBrowser2(), pDisp);

	

	if (pWeb->GetWebBrowser2() == pDisp)
	{
		CMdWebBrowserUI* pmdweb = dynamic_cast<CMdWebBrowserUI*>(pWeb);
		IWebBrowser2* wb = pmdweb->GetIWebBrowser();

		//wb->ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_PROMPTUSER, NULL, NULL);

		CComPtr<IDispatch> pDoc;
		HRESULT hr = wb->get_Document(&pDoc);
		if (FAILED(hr))
			return ;
		CComQIPtr<IHTMLDocument2> pDoc2 = pDoc;
		if (NULL == pDoc2)
			return ;
		CComPtr<IHTMLElementCollection> pBody = NULL;
		hr = pDoc2->get_all(&pBody);

		long len = 0;
		pBody->get_length(&len);

		CComPtr<IHTMLElement> elmt;
		for (int i = 0; i < len; ++i)
		{
			_variant_t index = i;
			IDispatch *pdisp;
			CComPtr<IHTMLElement> elem;
			HRESULT hResult = pBody->item(index, index, &pdisp);
			
			hResult = pdisp->QueryInterface(IID_IHTMLElement,
				(void **)&elem);

			pdisp->Release();
			BSTR p;
			hr = elem->get_innerHTML(&p);   //类似的还有put_innerTEXT //put_innerHTML
			if (p)
			{
				wstring strContext = p;
			}

		}

		//获得句柄
		IOleWindow *pOWin;
		HWND hBWnd;
		HWND hTar;

		HRESULT hRes = wb->QueryInterface(IID_IOleWindow, (void **)&pOWin);
		if (SUCCEEDED(hRes)) {
			hRes = pOWin->GetWindow(&hBWnd);
			if (SUCCEEDED(hRes)) {
				// Place hBWnd-manipulating code here

				HWND child =  GetWindow(hBWnd, GW_CHILD);

				if (child)
				{
					hTar = GetWindow(child, GW_CHILD);
				}

			}
		}


		
		string xurl = _encoding(url->bstrVal).astr().get();
		 	CHistoryMgr::RECORD record;
		 	record.folder = "";
		 	record.img = "";
			record.url = xurl;
			record.title = _encoding(pmdweb->getTitle()).utf8().get();

			if (xurl.find("history.html") == std::string::npos &&
				xurl.find("bookmark.html") == std::string::npos &&
				xurl.find("index.html") == std::string::npos)
			{
				CHistoryMgr::RECORD rd = theApp.History()->Add(record).Query(-1).at(0);


				xstring tmp;
				tmp.format("%u", hTar);
				xstring filepath;
				filepath.format("%s%s%d%s", theApp.getAppdir().c_str(),"tempfiles\\", rd.id,".png");
				
				map<string, string> para;
				para["hwnd"] = tmp.c_str();
				xstring idstr;
				idstr.format("%d", rd.id);
				para["id"] = idstr.c_str();


				para["filename"] = filepath.operator std::string();
				CGeeMeeEvent event(HttpWebShortcutEvent, para);
				theApp.Pool().AddPool(event);

			}
		
			
			if (xurl[xurl.length() - 1] != '/')
			{
				xurl.append("/");
			}
			xurl.append("favicon.ico");

			map<string, string> p;
			p["url"] = xurl;
			CGeeMeeEvent event = CGeeMeeEvent(HttpDownloadEvent, p);
			theApp.Pool().AddPool(event);
		 	

	}

	//CEditUI* pEdit = dynamic_cast<CEditUI*>(m_webengine->m_pm->FindControl(_T("ui_address")));
	//pEdit->SetText(url->bstrVal);
	//pWeb->SetUserData(url->bstrVal);
	
	//Log(_T("void CWebEventHandler::DocumentComplete [%s]"));


	

}


void CWebEventHandler::ProgressChange( CWebBrowserUI* pWeb, LONG nProgress, LONG nProgressMax )
{
	CWebBrowserEventHandler::ProgressChange(pWeb, nProgress, nProgressMax);
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
	wstring title;
	for (auto it = this->m_webengine->m_bindings.begin(); it != this->m_webengine->m_bindings.end(); it++)
	{
		if ((UINT_PTR)(it->second) == ptr)
		{
			CContainerUI* pCon = (CContainerUI*)it->first;
			COptionUI* opt = (COptionUI*)pCon->GetItemAt(0);
			opt->SetAttribute(L"textpadding", L"33,0,15,0");
			opt->SetAttribute(L"align", L"left");
			opt->SetAttribute(L"valign", L"vcenter");
			opt->SetAttribute(L"endellipsis", L"true");
			 title = bstrTitle;
			opt->SetText(title.c_str());



			CMdWebBrowserUI* pmdweb = dynamic_cast<CMdWebBrowserUI*>(pWeb);

			pmdweb->setTitle(title);
			
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
	return CWebBrowserEventHandler::ShowContextMenu(pWeb, dwID, ppt, pcmdtReserved, pdispReserved);
	//返回 E_NOTIMPL 正常弹出系统右键菜单
	//return S_OK;
	//返回S_OK 则可屏蔽系统右键菜单
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::GetHostInfo(CWebBrowserUI* pWeb,
	/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	return CWebBrowserEventHandler::GetHostInfo(pWeb, pInfo);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::ShowUI(CWebBrowserUI* pWeb,
	/* [in] */ DWORD dwID,
	/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
	/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
	/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
	/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc)
{
	return CWebBrowserEventHandler::ShowUI(pWeb, dwID,pActiveObject, pCommandTarget, pFrame, pDoc);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::HideUI(CWebBrowserUI* pWeb)
{
	return CWebBrowserEventHandler::HideUI(pWeb);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::UpdateUI(CWebBrowserUI* pWeb)
{
	return CWebBrowserEventHandler::UpdateUI(pWeb);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::EnableModeless(CWebBrowserUI* pWeb,
	/* [in] */ BOOL fEnable)
{
	return CWebBrowserEventHandler::EnableModeless(pWeb, fEnable);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::OnDocWindowActivate(CWebBrowserUI* pWeb,
	/* [in] */ BOOL fActivate)
{
	return CWebBrowserEventHandler::OnDocWindowActivate(pWeb, fActivate);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::OnFrameWindowActivate(CWebBrowserUI* pWeb,
	/* [in] */ BOOL fActivate)
{
	return CWebBrowserEventHandler::OnFrameWindowActivate(pWeb, fActivate);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::ResizeBorder(CWebBrowserUI* pWeb,
	/* [in] */ LPCRECT prcBorder,
	/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
	/* [in] */ BOOL fRameWindow)
{
	return  CWebBrowserEventHandler::ResizeBorder(pWeb, prcBorder, pUIWindow, fRameWindow);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::TranslateAccelerator(CWebBrowserUI* pWeb,
	/* [in] */ LPMSG lpMsg,
	/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
	/* [in] */ DWORD nCmdID)
{
	return CWebBrowserEventHandler::TranslateAccelerator(pWeb, lpMsg, pguidCmdGroup, nCmdID);
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
	else if (_tcscmp((const wchar_t*)*rgszNames, L"jnotify_asyn") == 0)
	{
		*rgdispid = FUN_GET_ASYNCALL;

		return S_OK;
	}
	else if (_tcscmp((const wchar_t*)*rgszNames, L"jnotify") == 0)
	{
		*rgdispid = FUN_GET_CALL;

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

	xstring loginfo;
	loginfo.format("CWebEventHandler::Invoke %d", dispidMember).log();
	
	
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
	case FUN_GET_ASYNCALL:
	{

		pvarResult->vt = VT_I4; V_I4(pvarResult) = 5;	break;

				
		break;
	}
	case FUN_GET_CALL:
	{

		VARIANT p0 = pdispparams->rgvarg[4];
		VARIANT p1 = pdispparams->rgvarg[3];
		VARIANT p2 = pdispparams->rgvarg[2];
		VARIANT p3 = pdispparams->rgvarg[1];
		VARIANT p4 = pdispparams->rgvarg[0];

		try
		{
			wxstring fun = p0.bstrVal;
			if (fun == L"history_query_today")
			{
				CHistoryMgr::VRECORD res = theApp.History()->Query(q_today);

				xstring querydata;

				for (int i = 0; i < res.size(); ++i)
				{
					CHistoryMgr::RECORD record = res[i];
					xstring tmp;
					tmp.format("<label for=\"history%d\" class=\"history-info\" data-index=\"%d\">"
						"<p class = \"span-check\"><input type = \"checkbox\" name = \"min-history\" data-index = \"%d\"></p>"
						"<a href = \"%s\">%s</a>"
						"<em class = \"cut\"><span data-index=\"%d\" style = \"display: none;\"></span></em>"
						"<span style = \"display: none;\">%s</span>"
						"</label>\r\n", record.id, record.id, record.id, record.url.c_str(), record.title.c_str(), record.id, record.url.c_str());
					querydata.append(tmp);
				}


				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(querydata).u8_utf16().getutf16().c_str());
				break;
				



			}
			if (fun == L"history_query_yesterday")
			{
				CHistoryMgr::VRECORD res = theApp.History()->Query(q_yesterday);

				xstring querydata;

				for (int i = 0; i < res.size(); ++i)
				{
					CHistoryMgr::RECORD record = res[i];
					xstring tmp;
					tmp.format("<label for=\"history%d\" class=\"history-info\" data-index=\"%d\">"
						"<p class = \"span-check\"><input type = \"checkbox\" name = \"min-history\" data-index = \"%d\"></p>"
						"<a href = \"%s\">%s</a>"
						"<em class = \"cut\"><span data-index=\"%d\" style = \"display: none;\"></span></em>"
						"<span style = \"display: none;\">%s</span>"
						"</label>\r\n", record.id, record.id, record.id, record.url.c_str(), record.title.c_str(), record.id, record.url.c_str());
					querydata.append(tmp);

				}


				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(querydata).u8_utf16().getutf16().c_str());
				break;
				break;
			}
			if (fun == L"history_query_thisweek")
			{
				CHistoryMgr::VRECORD res = theApp.History()->Query(q_thisweek);

				xstring querydata;

				for (int i = 0; i < res.size(); ++i)
				{
					CHistoryMgr::RECORD record = res[i];
					xstring tmp;
					tmp.format("<label for=\"history%d\" class=\"history-info\" data-index=\"%d\">"
						"<p class = \"span-check\"><input type = \"checkbox\" name = \"min-history\" data-index = \"%d\"></p>"
						"<a href = \"%s\">%s</a>"
						"<em class = \"cut\"><span data-index=\"%d\" style = \"display: none;\"></span></em>"
						"<span style = \"display: none;\">%s</span>"
						"</label>\r\n", record.id, record.id, record.id, record.url.c_str(), record.title.c_str(), record.id, record.url.c_str());
					querydata.append(tmp);
				}


				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(querydata).u8_utf16().getutf16().c_str());
				break;
				break;
			}
			if (fun == L"history_query_thismonth")
			{
				CHistoryMgr::VRECORD res = theApp.History()->Query(q_thismonth);

				xstring querydata;

				for (int i = 0; i < res.size(); ++i)
				{
					CHistoryMgr::RECORD record = res[i];
					xstring tmp;
					tmp.format("<label for=\"history%d\" class=\"history-info\" data-index=\"%d\">"
						"<p class = \"span-check\"><input type = \"checkbox\" name = \"min-history\" data-index = \"%d\"></p>"
						"<a href = \"%s\">%s</a>"
						"<em class = \"cut\"><span data-index=\"%d\" style = \"display: none;\"></span></em>"
						"<span style = \"display: none;\">%s</span>"
						"</label>\r\n", record.id, record.id, record.id, record.url.c_str(), record.title.c_str(), record.id, record.url.c_str());
					querydata.append(tmp);

				}


				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(querydata).u8_utf16().getutf16().c_str());
				break;
			}


			if (fun == L"history_delete")
			{
				

				wxstring tuple = p1.bstrVal;

				theApp.History()->Delete(_encoding(tuple).utf8().get());

				break;
			}
			if (fun == L"history_clear")
			{

				theApp.History()->Clear();

				break;
			}

			if (fun == L"history_search")
			{
				wxstring keyword = p1.bstrVal;

				xstring querydata;

				CHistoryMgr::VRECORD res = theApp.History()->Query(_encoding(keyword).utf8().get());


				for (int i = 0; i < res.size(); ++i)
				{
					CHistoryMgr::RECORD record = res[i];
					xstring tmp;
					tmp.format(
						"<label class = \"history-info\" for = \"history_2\">"
							"<p class = \"span-check\"><input type = \"checkbox\" name = \"min-history\" data-index = \"%d\" /></p>"
							"<a href = \"%s\">%s</a>"
							"<em class = \"cut\"><span></span></em>"
							"<span>%s</span>"
						"</label>\r\n", record.id, record.url.c_str(), record.title.c_str(), record.url.c_str()
						);
					querydata.append(tmp);

				}


				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(querydata).u8_utf16().getutf16().c_str());

				break;
			}
			if (fun == L"bookmark_search")
			{
				wxstring keyword = p1.bstrVal;

				xstring querydata;

				CFavorManager::VRECORD res = theApp.Favor()->Query(_encoding(keyword).utf8().get());


				for (int i = 0; i < res.size(); ++i)
				{
					CFavorManager::RECORD record = res[i];
					xstring tmp;
					tmp.format(
						"<div class = \"history-info\">"
							"<div class = \"bookmark-name-div\">"
								"%s"
							"</div>"
							"<div class = \"bookmark-src-div\">"
								"%s"
							"</div>"
						"</div>", record.title.c_str(), record.url.c_str()
						);
					querydata.append(tmp);

				}


				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(querydata).u8_utf16().getutf16().c_str());

				break;
			}
			if (fun == L"bookmark_query")
			{


				xstring querydata;

				vector<string> folders = theApp.Favor()->QueryFolders();

				for (int i = 0; i < folders.size(); ++i)
				{
					CFavorManager::VRECORD folder_record = theApp.Favor()->QueryByFolder(folders[i]).GetResult();

					xstring part;;
					part.format(
						"<div class=\"part\">"
						"<div class=\"part-up\">"
						"<h4 class = \"bookmarksfolder1\"><span>%s</span><i></i></h4>"
						"</div>", folders[i].c_str());

					xstring part_down = "<div class=\"part-down\"><div class=\"part-box\">";
					

					for (int i = 0; i < folder_record.size(); ++i)
					{
						CFavorManager::RECORD record = folder_record[i];
						xstring tmp;
						tmp.format(
							"<div class = \"history-info\">"
							"<div class = \"bookmark-name-div\">"
							"%s"
							"</div>"
							"<div class = \"bookmark-src-div\">"
							"%s"
							"</div>"
							"</div>",record.title.c_str(), record.url.c_str());

						part_down.append(tmp);

					}
					part_down.append("</div></div>");
					part.append(part_down).append("</div>");
					querydata.append(part);

				}


				


				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(querydata).u8_utf16().getutf16().c_str());

				break;
			}

			if (fun == L"setting_query")
			{
				Json::StreamWriterBuilder b;
				string content = Json::writeString(b, theApp.getJsonValue());

				xstring loginfo;
				loginfo.format("setting_query %s", content.c_str()).log();
 

				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(content).u8_utf16().getutf16().c_str());

				break;
			}

			if (fun == L"setting_update")
			{
				wxstring json = p1.bstrVal;

				Json::Reader reader;


				wxstring loginfo;
				loginfo.format(L"setting_update before %s", json.c_str()).log();

				reader.parse(_encoding(json).utf8().get(), theApp.getJsonValue(), false);


				Json::StreamWriterBuilder b;
				string content = Json::writeString(b, theApp.getJsonValue());
				xstring xctnt;
				xctnt.format("setting_update after %s",content.c_str() ).log();

				sync_setting_proc(NULL);


				
				
				break;
			}


			if (fun == L"index_delete_history")
			{
				

				break;
			}

			if (fun == L"index_query")
			{
				
				Json::StreamWriterBuilder b;
				Json::Value  root;

				map<int, string> mp;
				map<int, string> mpHotSites;
				theApp.History()->GetTop4ShortCut(mp);
				theApp.History()->GetTop4MostRrequentShortCut(mpHotSites);
				int sz = mp.size();

				xstring filepath;
				filepath.format("%s%s%d%s", theApp.getAppdir().c_str(), "tempfiles\\", 312, ".png");



				root["hotcount"] = mpHotSites.size() / 3 ;
				for(int i = 0; i < mpHotSites.size(); i+=3)
				{
					root["hotarray"][i]		= mpHotSites[i];
					root["hotarray"][i+1]	= mpHotSites[i+1];
					root["hotarray"][i+2]	= mpHotSites[i+2];
				}


				root["hiscount"] = mp.size() / 3 ;
				for(int i = 0; i < mp.size(); i+=3)
				{
					root["hisarray"][i]		= mp[i];
					root["hisarray"][i+1]	= mp[i+1];
					root["hisarray"][i+2]	= mp[i+2];
				}




				string content = Json::writeString(b, root);

				pvarResult->vt = VT_BSTR;
				pvarResult->bstrVal = ::SysAllocString(_encoding(content).u8_utf16().getutf16().c_str());

				break;
			}

			
		}
		catch (std::exception& e)
		{
			pvarResult->vt = VT_I4; V_I4(pvarResult) = -1;	break;
		}
		
		break;
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
	return CWebBrowserEventHandler::GetOptionKeyPath(pWeb,pchKey, dw);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::GetDropTarget(CWebBrowserUI* pWeb, 
	/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
	/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	return CWebBrowserEventHandler::GetDropTarget(pWeb, pDropTarget, ppDropTarget);
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
	return CWebBrowserEventHandler::TranslateUrl(pWeb, dwTranslate, pchURLIn, ppchURLOut);
}

HRESULT STDMETHODCALLTYPE CWebEventHandler::FilterDataObject(CWebBrowserUI* pWeb, 
	/* [in] */ IDataObject __RPC_FAR *pDO,
	/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	return CWebBrowserEventHandler::FilterDataObject(pWeb, pDO, ppDORet);
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
	return CWebBrowserEventHandler::Download(pWeb, pmk, pbc, dwBindVerb, grfBINDF, pBindInfo, pszHeaders, pszRedir, uiCP);
}







CMdWebBrowserUI::CMdWebBrowserUI()
{
}

CMdWebBrowserUI::~CMdWebBrowserUI()
{
}

void CMdWebBrowserUI::setTitle(const wstring& title)
{
	m_title = title;
}

void CMdWebBrowserUI::setUrl(const string& url)
{
	m_url = url;
}

string CMdWebBrowserUI::getUrl()
{
	return m_url;
}

wstring CMdWebBrowserUI::getNickUrl()
{
	return m_nickurl;
}

void CMdWebBrowserUI::setNickUrl(const wstring& url)
{
	if(url.find(L"index.html") != wstring::npos)
	{
		m_nickurl = L"geemee://tag";
	}
	else if(url.find(L"settings.html") != wstring::npos)
	{
		m_nickurl = L"geemee://settings";
	}
	else if(url.find(L"history.html") != wstring::npos)
	{
		m_nickurl = L"geemee://history";
	}
	else if(url.find(L"bookmark.html") != wstring::npos)
	{
		m_nickurl = L"geemee://bookmark";
	}
	else
	{
		m_nickurl = L"";
	}
	
	
}

std::wstring CMdWebBrowserUI::getTitle()
{
	return m_title;
}

IWebBrowser2* CMdWebBrowserUI::GetIWebBrowser()
{
	return m_pWebBrowser2;
}

bool CMdWebBrowserUI::IsGeemeeScheme(const wstring& url)
{
	return true;


}
