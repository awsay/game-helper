#include "stdafx.h"
#include "内存读写.h"

bool 置可读写(HANDLE 进程句柄, int 内存地址, SIZE_T 内存长度)
{
	typedef BOOL(NTAPI *LPZwProtectVirtualMemory)(HANDLE ProcessHandle,
		PVOID *     BaseAddress,
		SIZE_T *     NumberOfBytesToProtect,
		ULONG     NewAccessProtection,
		PULONG     OldAccessProtection
		);
	LPZwProtectVirtualMemory ZwProtectVirtualMemory = (LPZwProtectVirtualMemory)GetProcAddress(LoadLibrary(L"ntdll.dll"), "ZwProtectVirtualMemory");
	return ZwProtectVirtualMemory(进程句柄, (LPVOID*)内存地址, &内存长度, 64, NULL);
}



void 字符传输(int 内存地址, int 数值地址, int 写入长度)
{
	/*
	当为read，地址1为数据地址，地址2为内存地址
	当为write，地址1为内存地址，地址2为数据地址
	*/
	_asm
	{
		push esi
		push edi
		pushfd
		cld
		mov ecx, [ebp + 10h]
		mov esi, [ebp + 0Ch]
		mov edi, [ebp + 08h]
		rep movsb
		popfd
		pop edi
		pop esi
	}
}


WCHAR* 读文本(int 内存地址, int 读取长度)
{
	WCHAR* tmp = new WCHAR[200]; // tmp为数据的地址，&tmp为这个指针的地址
	wmemset(tmp, 0, sizeof(tmp));
	if (IsBadReadPtr((VOID*)内存地址, 读取长度))
		return NULL;
	字符传输(reinterpret_cast<int>(tmp), 内存地址, 读取长度);
	return tmp;
}

bool 写文本(int 内存地址, const WCHAR* 写入数据)
{
	DWORD len = (lstrlen(写入数据) + 1) * sizeof(WCHAR); // 多一个\0
	if (IsBadWritePtr((VOID*)内存地址, len))
		return false;
	字符传输(内存地址, reinterpret_cast<int>(写入数据), len);
	return true;
}


void 处理事件()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
}

void 超级延时(int 延时间隔)
{
	LARGE_INTEGER int64;
	int64.QuadPart = -10 * 延时间隔 * 1000;
	HANDLE hTimer = CreateWaitableTimerA(0, false, 0);
	SetWaitableTimer(hTimer, &int64, 0, 0, 0, false);
	while(MsgWaitForMultipleObjects(1, &hTimer, false, -1, 255) != 0)
	{
		处理事件();
	}
	CloseHandle(hTimer);
}

int 取偏移地址(int 基址,std::initializer_list<int> 偏移数组)
{
	int 地址 = 基址;
	for (auto p = 偏移数组.begin(); p != 偏移数组.end(); p++)
	{
		地址 = read(地址);
		地址 = 地址 + (*p);
	}
	return 地址;
}