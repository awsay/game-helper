#pragma once

#include "stdafx.h"
#include <wchar.h>
#include <iostream>

bool 置可读写(HANDLE 进程句柄, int 内存地址, SIZE_T 内存长度);

void 字符传输(int 内存地址, int 数值地址, int 写入长度);

WCHAR* 读文本(int 内存地址, int 读取长度);

bool 写文本(int 内存地址, const WCHAR* 写入数据);

void 处理事件();

void 超级延时(int 延时间隔);

int 取偏移地址(int 基址, std::initializer_list<int> 偏移数组);

template<typename T=int>
T read(int address)
{
	if (address == 0x18181818)
		return 0;
	if (address <= 0x00400000)
		return 0;
	if (IsBadReadPtr((VOID*)address, sizeof(T)))
		return 0;
	HANDLE hProcess = GetCurrentProcess(); ;
	T result;
	int length = sizeof(T);
	DWORD oldProtect;
	VirtualProtectEx(hProcess, (LPVOID)address, length, PAGE_EXECUTE_READWRITE, &oldProtect);//提权
	ReadProcessMemory(hProcess, (LPCVOID)address, &result, length, FALSE);//内存读整数
	VirtualProtectEx(hProcess, (LPVOID)address, length, oldProtect, &oldProtect);//提权
	CloseHandle(hProcess);
	return (T)result;
}

template<typename T = int>
BOOL write(int address,T data)
{
	if (address == 0x18181818)
		return FALSE;
	if (address <= 0x00400000)
		return FALSE;
	if (IsBadWritePtr((VOID*)address, sizeof(T)))
		return FALSE;
	HANDLE hProcess = GetCurrentProcess();
	DWORD oldProtect;
	int length = sizeof(T);
	VirtualProtectEx(hProcess, (LPVOID)address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL a = WriteProcessMemory(hProcess, (LPVOID)address, (LPCVOID)&data, length, 0);
	VirtualProtectEx(hProcess, (LPVOID)address, length, oldProtect, &oldProtect);
	CloseHandle(hProcess);//关闭对象
	return a;
}
