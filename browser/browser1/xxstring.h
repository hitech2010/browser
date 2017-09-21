#pragma once

#include <stdio.h>
#include <varargs.h>
#include "encoding.h"
#include <assert.h>
#include <atlstr.h>


class _xstring
{
private:
	std::string m_buffer;
	typedef unsigned long size_t;
public:
	_xstring() :m_buffer(){ ; }
	_xstring(const char* src, size_t sz){ m_buffer.assign(src, sz); }
	_xstring(const char* src){ m_buffer.assign(src); }
	_xstring(const std::string& src) :m_buffer(src){ ; }
	operator std::string()
	{
		return m_buffer;
	}

	operator char*()
	{
		return m_buffer._Myptr();
	}

	operator int()
	{
		int tmp = 0;

		return atoi(m_buffer.c_str());

	}

	operator double()
	{
		return atof(m_buffer.c_str());

	}

	operator float()
	{
		return atof(m_buffer.c_str());

	}



	size_t size()
	{
		return m_buffer.size();
	}

	void resize(size_t sz)
	{
		return m_buffer.resize(sz);
	}


	const  char* c_str()
	{
		return m_buffer.c_str();
	}

	_xstring& append(_xstring& a)
	{
		m_buffer.append(a.c_str());
		return *this;
	}

	_xstring& append(const char* src)
	{
		m_buffer.append(src);
		return *this;
	}

	_xstring& append(const char* src, size_t sz)
	{
		m_buffer.append(src,sz);
		return *this;
	}



	_xstring& assign(_xstring& a)
	{
		m_buffer.assign(a.c_str());
		return *this;
	}

	_xstring& assign(std::string& a)
	{
		m_buffer.assign(a);
		return *this;
	}

	_xstring& assign(const char* src)
	{
		m_buffer.assign(src);
		return *this;
	}

	_xstring& assign(const char* src, size_t sz)
	{
		m_buffer.assign(src, sz);
		return *this;
	}

	_xstring& operator=(_xstring& a)
	{
		m_buffer.assign(a.c_str());
		return *this;
	}

	_xstring& operator=(std::string& a)
	{
		m_buffer.assign(a.c_str());
		return *this;
	}

	_xstring& operator=(const char*& src)
	{
		m_buffer.assign(src);
		return *this;
	}

	_xstring& operator=(int tmp)
	{
		format("%d", tmp);
	}
	_xstring& operator=(unsigned int tmp)
	{
		format("%u", tmp);
	}

	_xstring& operator=(const float& tmp)
	{
		format("%f", tmp);
	}

	_xstring& operator=(const double& tmp)
	{
		format("%f", tmp);
	}


	_xstring substr(int iStart, size_t cnt = -1)
	{
		_xstring tmp = m_buffer.substr(iStart, cnt);
		return tmp;
	}

	_xstring left(size_t cnt )
	{
		_xstring tmp = m_buffer.substr(0, cnt);
		return tmp;
	}

	_xstring right(size_t cnt)
	{
		size_t sz = m_buffer.size();
		_xstring tmp = m_buffer.substr(sz-cnt, cnt);
		return tmp;
	}

	bool Find(const string& src)
	{
		return (m_buffer.find(src.c_str()) != -1);
	}





// 	_xstring& operator=(const char* src)
// 	{
// 		m_buffer.assign(src);
// 		return *this;
// 	}

	void clear()
	{
		m_buffer.clear();
	}

	char* _Myptr()
	{
		return m_buffer._Myptr();
	}

	char operator[](int index)
	{
		return m_buffer[index];
	}

	_xstring& format(const char* fmt, ...)
	{
		string tmp;
		va_list va;

		va_list args;
		va_start(args, fmt);

		size_t sz = vsnprintf(NULL, 0, fmt, args);
		tmp._Grow(sz + 1);
		tmp.resize(sz);




		vsnprintf_s(tmp._Myptr(), tmp.capacity(),_TRUNCATE, fmt, args);

		va_end(args);

		m_buffer = tmp;


		return *this;
	}
};



class _wxstring
{
private:
	std::wstring m_buffer;
	typedef unsigned long size_t;
public:
	_wxstring() :m_buffer(){ ; }
	_wxstring(const wchar_t* src, size_t sz){ m_buffer.assign(src, sz); }
	_wxstring(const wchar_t* src){ m_buffer.assign(src); }
	_wxstring(const std::wstring& src) :m_buffer(src){ ; }


	operator std::wstring()
	{
		return m_buffer;
	}

	operator wchar_t*()
	{
		return m_buffer._Myptr();
	}

	wchar_t* _Myptr()
	{
		return m_buffer._Myptr();
	}



	wchar_t operator[](int index)
	{
		return m_buffer[index];
	}

	size_t size()
	{
		return m_buffer.size();
	}

	
	void resize(size_t sz)
	{
		 m_buffer.resize(sz);
	}


	const  wchar_t* c_str()
	{
		return m_buffer.c_str();
	}

	_wxstring& append(_wxstring& a)
	{
		m_buffer.append(a.c_str());
		return *this;
	}

	_wxstring& append(const wchar_t* src)
	{
		m_buffer.assign(src);
		return *this;
	}

	_wxstring& append(const wchar_t* src, size_t sz)
	{
		m_buffer.append(src, sz);
		return *this;
	}



	_wxstring& assign(_wxstring& a)
	{
		m_buffer.assign(a.c_str());
		return *this;
	}

	_wxstring& assign(std::wstring& a)
	{
		m_buffer.assign(a);
		return *this;
	}

	_wxstring& assign(const wchar_t* src)
	{
		m_buffer.assign(src);
		return *this;
	}

	_wxstring& assign(const wchar_t* src, size_t sz)
	{
		m_buffer.assign(src, sz);
		return *this;
	}

	_wxstring& operator=(_wxstring& a)
	{
		m_buffer.assign(a.c_str());
		return *this;
	}

	_wxstring& operator=(std::wstring& a)
	{
		m_buffer.assign(a.c_str());
		return *this;
	}

	_wxstring& operator=(const wchar_t*& src)
	{
		m_buffer.assign(src);
		return *this;
	}

	_wxstring substr(int iStart, size_t cnt = -1)
	{
		_wxstring tmp = m_buffer.substr(iStart, cnt);
		return tmp;
	}

	_wxstring left(size_t cnt)
	{
		_wxstring tmp = m_buffer.substr(0, cnt);
		return tmp;
	}

	_wxstring right(size_t cnt)
	{
		size_t sz = m_buffer.size();
		_wxstring tmp = m_buffer.substr(sz - cnt, cnt);
		return tmp;
	}

	bool Find(const wstring& src)
	{
		return (m_buffer.find(src.c_str()) != -1);
	}

	bool operator==(const wchar_t* src)
	{
		return m_buffer == src;
	}

	_xstring& operator=(int tmp)
	{
		format(L"%d", tmp);
	}
	_xstring& operator=(unsigned int tmp)
	{
		format(L"%u", tmp);
	}

	_xstring& operator=(const float& tmp)
	{
		format(L"%f", tmp);
	}

	_xstring& operator=(const double& tmp)
	{
		format(L"%f", tmp);
	}

	void clear()
	{
		m_buffer.clear();
	}

	
	vector<wstring>& split(const wstring& sep)
	{
		vector<wstring> result;
		int startpos = 0;
		int prevpos = 0;
		while (-1 != (startpos = m_buffer.find(sep, prevpos)))
		{
			
			

			prevpos = startpos + sep.length();
		}

		return result;
	}



	_wxstring& format(const wchar_t* fmt, ...)
	{


		va_list args;
		va_start(args, fmt);
	
		SetLastError(ERROR_SUCCESS);
		wchar_t szBuffer[1028];
		int nLength = _vscwprintf(fmt, args);
		assert(GetLastError() == ERROR_SUCCESS);

		m_buffer._Grow(nLength+1);
		m_buffer.resize(nLength);

		vswprintf_s(m_buffer._Myptr(), m_buffer.capacity(), fmt, args);

		va_end(args);


		return *this;
	}
};

typedef _xstring xstring;
typedef _wxstring wxstring;



