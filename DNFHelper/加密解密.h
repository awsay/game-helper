#pragma once

#include "基址.h"
#include "内存读写.h"

void 超级加密(int 地址, int 数值, int 类型 = 0);

//int 超级解密(int 地址)
//{
//	int EAX, ecx8, esi, EDX;
//	EAX = read(地址);
//	ecx8 = read(地址 + 4);
//	esi = read(解密基址);
//	EDX = EAX >> 16;
//	EDX = read((EDX << 2) + esi + 36);
//	EAX = EAX & 65535;
//	EAX = read((EAX << 2) + EDX + 8468);
//	__asm
//	{
//		mov eax, EAX
//		movzx edx, eax
//		mov eax, esi
//		mov esi, edx
//		shl esi, 10h
//		or esi, edx
//		mov edx, EDX
//		xor esi, edx
//		xchg eax, esi
//		leave
//		retn 0x0004
//	}
//	return 0;
//}

int 解密(int 地址);
