#include "utilities.h"
#include <WinInet.h>
#include <urlmon.h>

void setproxy(bool bflag)
{
	static bool s_bset = false;

	if (!bflag)
	{
		if (s_bset)
		{
			INTERNET_PROXY_INFO ipi;
			ipi.dwAccessType = INTERNET_OPEN_TYPE_DIRECT;
			ipi.lpszProxy = NULL;
			ipi.lpszProxyBypass = NULL;
			HRESULT hr = UrlMkSetSessionOption(INTERNET_OPTION_PROXY, &ipi, sizeof(ipi), 0);

			Log("setproxy close %08X", hr);
			s_bset = false;
		}
	}
	else
	{
		if (!s_bset)
		{
			INTERNET_PROXY_INFO ipi;
			ipi.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
			ipi.lpszProxy = (LPCTSTR)"127.0.0.1:8899"; // (LPCTSTR)"127.0.0.1:60006";
			ipi.lpszProxyBypass = NULL;
			HRESULT hr = UrlMkSetSessionOption(INTERNET_OPTION_PROXY, &ipi, sizeof(ipi), 0);


			Log("setproxy open %08X", hr);
			s_bset = true;
		}
	}
}
