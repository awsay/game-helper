#include "stdafx.h"
#include "CALL.h"
#include "公告.h"

void 缓冲call(int 包头) 
{
	VMBEGIN
	int 缓冲call地址 = 缓冲Call;
	__asm
	{
		push 包头
		mov ecx, 发包基址
		mov ecx, dword ptr ss : [ecx]
		call 缓冲call地址
	}
	VMEND
}

void 加密call(int 参数, int 长度)
{
	VMBEGIN
	int call地址 = 0;
	if (长度 == 1)call地址 = 加密包Call;
	if (长度 == 2)call地址 = 加密包Call + 48;
	if (长度 == 4)call地址 = 加密包Call + 96;
	if (长度 == 8)call地址 = 加密包Call + 144;
	__asm
	{
		push 参数
		mov ecx, 发包基址
		mov ecx, [ecx]
		mov eax, call地址
		call eax
	}
	VMEND
}


void 发包call()
{
	VMBEGIN
	int 发包Call地址 = 发包Call;
	_asm
	{
		mov ecx, 发包基址
		call 发包Call地址;
	}
	VMEND
}

void 过图call(int 方向)
{
	VMBEGIN
	__asm
	{
		mov ecx, 房间编号
		mov ecx, [ecx]
		mov ecx, [ecx + 0020A024h]
		mov ecx, [ecx + 0000008Ch]
		push 0xFF
		push 0xFF
		push 00h
		push 00h
		push 00h
		push 00h
		push 00h
		push [ebp + 08h]
		mov eax, 过图Call
		call eax
	}
	VMEND
}

void 坐标call(int 对象地址, int x, int y, int z)
{
	__asm
	{
		mov esi, [ebp + 08h]
		mov edi, [esi]
		push [ebp + 14h]
		push [ebp + 10h]
		push [ebp + 0Ch]
		mov eax, [edi + 000000A8h]
		mov ecx, esi
		mov eax, eax
		mov eax, eax
		mov eax, eax
		call eax
	}
}

void 接受call(int 任务ID)
{
	__asm
	{
		push [ebp + 08h]
		mov eax, 接受Call
		call eax
	}
}

void 提交call(int 任务ID)
{
	__asm
	{
		push 01h
		push 0xFF
		push[ebp + 08h]
		mov ecx, 任务基址
		mov eax, 提交Call
		mov eax, eax
		call eax
	}
}

void 释放call(int 对象基址, int 代码, int 伤害, int X, int Y, int Z)
{
	_asm
	{
		push Z
		push Y
		push X
		push 伤害
		push 代码
		push 对象基址
		mov edi, 释放Call
		mov eax, edi
		call eax
		add esp, 0x18
	}
}

void 完成call(int 任务ID)
{
	__asm
	{
		push 0xFF
		push 255
		push 任务ID
		mov eax,完成Call
		call eax
	}
}

void 爆物call(int 对象)
{
	__asm
	{
		push 对象
		mov ecx,对象
		mov esi,ecx
		mov eax,爆物Call
		call eax
		add esp,4
	}
}
