/*
在应用层通过修改PEB结构中的模块链，来隐藏本进程中的某个模块
测试结果：对ProcessExplorer有效，对IS无效,使用PCHunter64查看模块，隐藏的模块会显示不同的颜色；
		  对PowerTools有效
*/

#include "stdafx.h"
#include "LDR断链 清除PE.h"

static BOOL RemoveLdr(HINSTANCE DllInstance)
{
	PLIST_ENTRY InLoadOrderModuleList = NULL;
	PLIST_ENTRY Current = NULL;
	PLDR_DATA_TABLE_ENTRY Entry = NULL;

	__try
	{
		__asm
		{
			mov eax, dword ptr fs : [0x30]
			mov eax, dword ptr[eax + 0xC]
			lea eax, dword ptr[eax + 0xC]
			mov InLoadOrderModuleList, eax
		}

		Current = InLoadOrderModuleList->Flink;
		do
		{
			Entry = (PLDR_DATA_TABLE_ENTRY)Current;
			if (Entry->DllBase == DllInstance)
			{
				break;
			}
			Current = Current->Flink;
		} while (Current != InLoadOrderModuleList);

		if (Current == InLoadOrderModuleList)
		{
			SetLastError(ERROR_MOD_NOT_FOUND);
			return FALSE;
		}

		Entry->InLoadOrderLinks.Blink->Flink = Entry->InLoadOrderLinks.Flink;
		Entry->InLoadOrderLinks.Flink->Blink = Entry->InLoadOrderLinks.Blink;
		Entry->InMemoryOrderLinks.Blink->Flink = Entry->InMemoryOrderLinks.Flink;
		Entry->InMemoryOrderLinks.Flink->Blink = Entry->InMemoryOrderLinks.Blink;
		Entry->InInitializationOrderLinks.Blink->Flink = Entry->InInitializationOrderLinks.Flink;
		Entry->InInitializationOrderLinks.Flink->Blink = Entry->InInitializationOrderLinks.Blink;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SetLastError(EXCEPTION_ACCESS_VIOLATION);
		return FALSE;
	}

	return TRUE;
}

static BOOL ClearPE(HINSTANCE DllInstance)
{
	PIMAGE_DOS_HEADER DosHeader = NULL;
	PIMAGE_NT_HEADERS NtHeader = NULL;
	DWORD OldProtect = 0;

	if (!VirtualProtect(DllInstance, 0x1000, PAGE_EXECUTE_READWRITE, &OldProtect))
	{
		return FALSE;
	}

	__try
	{
		DosHeader = (PIMAGE_DOS_HEADER)DllInstance;
		if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			SetLastError(ERROR_BAD_FORMAT);
			return FALSE;
		}

		NtHeader = (PIMAGE_NT_HEADERS)((PBYTE)DosHeader + DosHeader->e_lfanew);
		if (NtHeader->Signature != IMAGE_NT_SIGNATURE)
		{
			SetLastError(ERROR_BAD_FORMAT);
			return FALSE;
		}

		RtlZeroMemory(DosHeader, 0x1000);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		SetLastError(EXCEPTION_ACCESS_VIOLATION);
		return FALSE;
	}

	if (!VirtualProtect(DllInstance, 0x1000, OldProtect, &OldProtect))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL Hide(HMODULE DllInstance)
{
	return RemoveLdr(DllInstance) && ClearPE(DllInstance);
}
