#include "stdafx.h"
#include "公告.h"


void 系统公告(LPCWSTR 内容)
{
	VMBEGIN
	int 公告类型 = 37;
	int 公告颜色 = 黄色; //黄色
	int 喇叭公告_ = 喇叭公告;

	_asm
	{
		mov ecx, 商店基址
		mov ecx, [ecx]
		mov ecx, [ecx + 0x50]
		push 00h
		push 00h
		push 00h
		push 00h
		push 公告类型
		push 公告颜色
		push 内容
		call 喇叭公告_
	}
	VMEND
}

void 公告(const wchar_t * str, ...)
{
	WCHAR* p = new WCHAR[200];
	va_list va;
	va_start(va, str);
	_vstprintf_s(p, 200, str, va);
	va_end(va);
	系统公告(p);
	delete p;
}