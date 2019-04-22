#pragma once

/*
在应用层通过修改PEB结构中的模块链，来隐藏本进程中的某个模块
测试结果：对ProcessExplorer有效，对IS无效,使用PCHunter64查看模块，隐藏的模块会显示不同的颜色；
		  对PowerTools有效
*/

#include "stdafx.h"
#include <Subauth.h>
#include <stdio.h>

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	LPVOID DllBase;
	LPVOID EntryPoint;
	SIZE_T SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

static BOOL RemoveLdr(HINSTANCE DllInstance);

static BOOL ClearPE(HINSTANCE DllInstance);

BOOL Hide(HMODULE DllInstance);