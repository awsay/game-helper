#include "stdafx.h"
#include "常用函数.h"

template<typename T>
int 取变量地址(T& 变量)
{
	return reinterpret_cast<int>(&变量);
}

template<typename T>
int 取变量地址(T*& 变量)
{
	return reinterpret_cast<int>(变量);
}

WCHAR* AsciiToUnicode(const char* sBuf)
{
	int sBufSize = strlen(sBuf);
	DWORD dBufSize = MultiByteToWideChar(CP_ACP, 0, sBuf, -1, NULL, 0);

	WCHAR* dBuf = new WCHAR[dBufSize+1];
	wmemset(dBuf, 0, dBufSize);

	MultiByteToWideChar(CP_ACP, 0, sBuf, sBufSize, dBuf, dBufSize);
	return dBuf;
}

char* UnicodeToAscii(const WCHAR* sBuf)
{
	DWORD dBufSize = WideCharToMultiByte(CP_OEMCP, 0, sBuf, -1, NULL, 0, NULL, FALSE);

	char *dBuf = new char[dBufSize];
	memset(dBuf, 0, dBufSize);

	int nRet = WideCharToMultiByte(CP_OEMCP, 0, sBuf, -1, dBuf, dBufSize, NULL, FALSE);
	return dBuf;
}

std::string* UnicodeToString(const WCHAR* sBuf)
{
	const char* tmp = UnicodeToAscii(sBuf);
	std::string* ans = new std::string(tmp);
	return ans;
}

char* StringToAscii(std::string& e)
{
	const char *p = e.c_str();
	char* a = new char[e.size()+1];
	memcpy(a, p, sizeof(p));
	delete p;
	return a;
}

WCHAR* StringToUnicode(std::string& e)
{
	const char *p = e.c_str();
	WCHAR* a = new WCHAR[e.size()+1];
	for (size_t i = 0; i < e.size(); i++)
		a[i] = p[i];
	a[e.size() + 1] = '\0';
	return a;
}

void format(WCHAR* p, const wchar_t * str, ...)
{
	va_list va;
	va_start(va, str);
	_vstprintf_s(p, 200, str, va);
	va_end(va);
}

	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

BOOL EnableDebugPrivilege(BOOL fEnable)
{
	HANDLE hToken;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		CloseHandle(hToken);
	}
	else
	{
		return 0;
	}
	return (GetLastError() == ERROR_SUCCESS);
}