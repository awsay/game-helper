#pragma once

#include "stdafx.h"
#include "常用函数.h"
#include "公告.h"
#include <TlHelp32.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

BOOL EnablePriv();

DWORD GetProcessIDByName(const WCHAR* pName);

DWORD 杀进程(LPVOID);

void MapModuleAddress(const WCHAR* ModuleName, ULONG address);

void MapModuleThread(ULONG tid, ULONG address);

void EnumModule(DWORD pid);

typedef LONG    NTSTATUS;
typedef NTSTATUS(WINAPI *NTQUERYINFORMATIONTHREAD)(HANDLE, LONG, PVOID, ULONG, PULONG);
#define ThreadQuerySetWin32StartAddress 9
#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)  

DWORD WINAPI GetThreadStartAddress(DWORD tid);

BOOL EnumThread(DWORD dwOwnerPID);

void CloseModuleThread(std::string Module);

DWORD 结束线程(LPVOID);

void 检测();