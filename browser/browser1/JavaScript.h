#pragma once
#include <atlcoll.h>
#include <atlstr.h>
#include <comutil.h>
//#include <mshtml.h>

class CJavaScript
{
private:
	CComPtr<IHTMLDocument2> m_pDoc2;
public:
	CJavaScript(){}
	CJavaScript(IHTMLDocument2* pDoc2) { SetDocument2(pDoc2); }
	void SetDocument2(IHTMLDocument2* pDoc2)
	{
		m_pDoc2.Release();
		m_pDoc2 = pDoc2;
	}
	bool IsNull()
	{
		return m_pDoc2 == NULL;
	}
	HRESULT ExecuteJavaScript(LPCTSTR szScript)
	{
		HRESULT hr = E_POINTER;
		if (m_pDoc2)
		{
			CComQIPtr<IHTMLWindow2> pWnd;
			if (SUCCEEDED(hr = m_pDoc2->get_parentWindow( &pWnd )))
			{
				VARIANT varResult;
				CComBSTR script = szScript;
				CComBSTR lang(L"javascript");
				hr = pWnd->execScript(script, lang, &varResult);
			}
		}
		return hr;
	}
	HRESULT CallJSFunction(LPCTSTR szFunctionName, DISPPARAMS * dispParams, VARIANT* varResult, EXCEPINFO* exceptInfo, UINT* nArgErr )
	{
		HRESULT hr = E_POINTER;
		if (m_pDoc2)
		{
			CComPtr<IDispatch> pDispScript;
			if (SUCCEEDED(hr = m_pDoc2->get_Script(&pDispScript)))
			{
				DISPID dispid;
				CComBSTR objbstrValue = szFunctionName;
				OLECHAR *pszFunct = objbstrValue;
				hr = pDispScript->GetIDsOfNames(IID_NULL, &pszFunct, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
				if (SUCCEEDED(hr))
				{
					hr = pDispScript->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, dispParams, varResult, exceptInfo, nArgErr);
				}
			}
		}
		return hr;
	}
	HRESULT SetJSVar(LPCTSTR szVarName, VARIANTARG *rgvarg)
	{
		HRESULT hr = E_POINTER;
		if (m_pDoc2)
		{
			CComPtr<IDispatch> pDispScript;
			if (SUCCEEDED(hr = m_pDoc2->get_Script(&pDispScript)))
			{
				DISPID dispid;
				CComBSTR objbstrValue = szVarName;
				OLECHAR *pszFunct = objbstrValue;
				hr = pDispScript->GetIDsOfNames(IID_NULL, &pszFunct, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
				if (SUCCEEDED(hr))
				{
					DISPPARAMS dispPara = {0};
					DISPID putdispid = DISPID_PROPERTYPUT;
					dispPara.cArgs = 1;
					dispPara.cNamedArgs = 1;
					dispPara.rgdispidNamedArgs = &putdispid;
					dispPara.rgvarg = rgvarg;
					hr = pDispScript->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dispPara, NULL, NULL, NULL);
				}
			}
		}
		return hr;
	}
	HRESULT GetJSVar(LPCTSTR szVarName, VARIANTARG *rgvarg)
	{
		HRESULT hr = E_POINTER;
		if (m_pDoc2)
		{
			CComPtr<IDispatch> pDispScript;
			if (SUCCEEDED(hr = m_pDoc2->get_Script(&pDispScript)))
			{
				DISPID dispid;
				CComBSTR objbstrValue = szVarName;
				OLECHAR *pszFunct = objbstrValue;
				hr = pDispScript->GetIDsOfNames(IID_NULL, &pszFunct, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
				if (SUCCEEDED(hr))
				{
					DISPPARAMS dispPara = {0};
					hr = pDispScript->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispPara, rgvarg, NULL, NULL);
				}
			}
		}
		return hr;
	}
	static HRESULT CallAnonymousJSFunction(IDispatch * pfn, DISPPARAMS * dispParams, VARIANT* varResult, EXCEPINFO* exceptInfo, UINT* nArgErr)
	{
		HRESULT hr = E_POINTER;
		if (pfn)
		{
			DISPID dispid;
			OLECHAR *pszFunct = L"call";
			HRESULT hr = pfn->GetIDsOfNames(IID_NULL, &pszFunct, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
			if (SUCCEEDED(hr))
			{
				hr = pfn->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, dispParams, varResult, exceptInfo, nArgErr);
			}
		}
		return hr;
	}
	HRESULT NewObject(LPOLESTR szObjectName, IDispatchEx ** p)
	{
		HRESULT hr = E_POINTER;
		if (p)
		{
			*p = NULL;
			if (m_pDoc2)
			{
				CComPtr<IDispatch> pDispScript;
				if (SUCCEEDED(hr = m_pDoc2->get_Script(&pDispScript)))
				{
					// get DISPID for szObjectName
					DISPID did = 0;
					LPOLESTR lpNames[] = {szObjectName};
					if (SUCCEEDED(hr = pDispScript->GetIDsOfNames(IID_NULL, lpNames, 1, LOCALE_USER_DEFAULT, &did)))
					{
						// invoke pDispScript with DISPATCH_PROPERTYGET for szObjectName
						CComVariant vtRet;
						DISPPARAMS params = {0};
						CComVariant vtResult;
						if (SUCCEEDED(hr = pDispScript->Invoke(did, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &vtResult, NULL, NULL)))
						{
							// check result: should be a VT_DISPATCH
							if ((VT_DISPATCH != vtResult.vt) || (NULL == vtResult.pdispVal))
								return DISP_E_TYPEMISMATCH;

							// get IDispatchEx on returned IDispatch
							CComQIPtr<IDispatchEx> prototype(vtResult.pdispVal);
							if (!prototype)
								return E_NOINTERFACE;

							// call InvokeEx with DISPID_VALUE and DISPATCH_CONSTRUCT to construct new object
							vtResult.Clear();
							if (FAILED(hr = prototype->InvokeEx(DISPID_VALUE, LOCALE_USER_DEFAULT, DISPATCH_CONSTRUCT, &params, &vtResult, NULL, NULL)))
								return hr;

							// vtresult should contain the new array now.
							if ((VT_DISPATCH != vtResult.vt) || (NULL == vtResult.pdispVal))
								return DISP_E_TYPEMISMATCH;

							// get IDispatchEx on returned IDispatch
							CComQIPtr<IDispatchEx> theObject(vtResult.pdispVal);
							if (!theObject)
								return E_NOINTERFACE;
							*p = theObject;
							(*p)->AddRef();
						}
					}
				}
			}
		}
		return hr;
	}
};