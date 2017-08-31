#include "utilities.h"

 
std::wstring Codec::string2wstring(const std::string strIn, UINT CodePage)
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
std::string Codec::wstring2string(const std::wstring strIn, UINT CodePage)
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
