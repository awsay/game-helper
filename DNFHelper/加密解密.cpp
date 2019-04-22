#include "stdafx.h"
#include "加密解密.h"

void 超级加密(int 地址, int 数值, int 类型)
{
	int 加密ID, 偏移参数, 偏移地址, data, 地址尾;
	short ax, si;
	加密ID = read(地址);
	偏移参数 = read(read(解密基址) + (加密ID >> 16) * 4 + 36);
	偏移地址 = 偏移参数 + (加密ID & 65535) * 4 + 8468;
	偏移参数 = read(偏移地址);
	data = 偏移参数 & 65535;
	data = data + (data << 16);
	ax = (short)(偏移参数 & 65535);
	si = 0;
	if (类型 == 0)
	{
		地址尾 = 地址 & 15;
		switch (地址尾)
		{
		case 0:
			si = (short)(数值 >> 16);
			si = (short)(si - ax);
			si = (short)(si + 数值);
			break;
		case 4:
			si = (short)((数值 & 65535) - (数值 >> 16));;
			break;
		case 8:
			si = (short)(数值 >> 16);
			si = (short)(si * 数值);
			break;
		case 12:
			si = (short)(数值 >> 16);
			si = (short)(si + 数值);
			si = (short)(si + ax);
			break;
		default:
			return;
		}
	}
	else if (类型 == 1)
		si = (数值 & 255);
	else if (类型 == 2)
		si = 数值;
	else if (类型 == 3)
	{
		si = (数值 >> 16);
		si += 数值;
	}
	else if (类型 == 4)
	{
		si = (数值 >> 16);
		si = si + (数值 & 65535);
	}
	else
		return;
	ax = (short)(si ^ ax);
	data = data ^ 数值;
	write(地址 + (类型 != 4 ? 4 : 8),data);
	write<short>(偏移地址 + 2 , ax);
}

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

int 解密(int 地址)
{
	int eax = read(地址);
	int esi = read(解密基址);
	int edx = eax >> 16;
	edx = read(esi + edx * 4 + 36);
	eax = eax & 65535;
	eax = read(edx + eax * 4 + 8468);
	edx = eax & 65535;
	esi = edx << 16;
	esi = esi | edx;
	esi = esi ^ (read(地址 + 4));
	return esi;
}

