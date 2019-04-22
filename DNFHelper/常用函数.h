#pragma once
#include "stdafx.h"
#include <tchar.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename T>
int 取变量地址(T& 变量);

template<typename T>
int 取变量地址<T*>(T& 变量);

WCHAR* AsciiToUnicode(const char* sBuf);

char* UnicodeToAscii(const WCHAR* sBuf);

std::string* UnicodeToString(const WCHAR* sBuf);

char* StringToAscii(std::string& e);

WCHAR* StringToUnicode(std::string& e);

void format(WCHAR* p, const wchar_t * str, ...);

template<typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

BOOL EnableDebugPrivilege(BOOL fEnable);