#include "stdafx.h"
#include "组包.h"
#include "公告.h"

void 组包翻牌(int x,int y)
{
	VMBEGIN
	缓冲call(69);
	发包call();
	缓冲call(70);
	发包call();
	缓冲call(71);
	加密call(x, 1);
	加密call(y, 1);
	发包call();
	VMEND
}


void 组包拾取(int 物品地址)
{
	VMBEGIN
	缓冲call(43);
	加密call(物品地址, 4);
	加密call(0, 1);
	加密call(1, 1);
	for (int i = 0; i < 7; i++)
		加密call(0, 2);
	发包call();
	VMEND
}


void 组包分解(int 装备位置)
{
	VMBEGIN
	缓冲call(26);
	加密call(装备位置, 2);
	加密call(0, 1);
	加密call(65535, 2);
	加密call(319, 4);
	发包call();
	VMEND
}

void 选角色(int 角色位置, int 类型)
{
	VMBEGIN
	if (类型 == 0)
	{
		缓冲call(7);
		发包call();
	}
	else
	{
		缓冲call(4);
		加密call(角色位置, 4);
		发包call();
	}
	VMEND
}

void 组包秒杀(int 地址)
{
	缓冲call(39);
	加密call(地址, 4);
	加密call(0, 2);
	for (int i = 0; i < 4; i++)
		加密call(0, 4);
	加密call(0, 1);
	加密call(0, 2);
	加密call(0, 2);
	for (int i = 0; i < 4; i++)
		加密call(0, 1);
	加密call(0, 2);
	加密call(0, 1);
	加密call(0, 4);
	加密call(0, 8);
	for (int i = 0; i < 7; i++)
		加密call(0, 2);
	加密call(0, 1);
	加密call(0, 1);
	发包call();
}

void 组包过图(int X, int Y)
{
	VMBEGIN
	缓冲call(45);
	加密call(X, 1);
	加密call(Y, 1);
	for (int i = 0; i < 2; i++)
		加密call(0, 4);
	for (int i = 0; i < 9; i++)
		加密call(0, 2);
	for (int i = 0; i < 8; i++)
		加密call(0, 4);
	for (int i = 0; i < 7; i++)
		加密call(0, 2);
	加密call(0, 4);
	for (int i = 0; i < 3; i++)
		加密call(0, 2);
	加密call(0, 8);
	加密call(0, 4);
	加密call(0, 2);
	发包call();
	VMEND
}

void 组包移动(int 区域, int 位置, int X, int Y)
{
	VMBEGIN
	缓冲call(36);
	加密call(区域, 1);
	加密call(位置, 1);
	加密call(X, 2);
	加密call(Y, 2);
	加密call(5, 1);
	加密call(9, 2);
	加密call(2, 2);
	加密call(0, 4);
	加密call(0, 1);
	发包call();
	VMEND
}

void 组包进图(int 地图信息, int 地图难度, int 地图深渊, int 地图练习)
{
	VMBEGIN
	缓冲call(16);
	加密call(地图信息, 4);
	加密call(地图难度, 1);
	加密call(0, 2);
	加密call(地图深渊, 1);
	加密call(地图练习, 1);
	加密call(65535, 2);
	加密call(0, 4);
	加密call(0, 4);
	加密call(0, 1);
	发包call();
	VMEND
}

void 组包卖物(int 位置)
{
	VMBEGIN
	缓冲call(22);
	加密call(0, 1);
	加密call(位置, 2);
	加密call(1, 2);
	加密call(0, 4);
	加密call(位置 + 1, 4);
	加密call(0, 4);
	发包call();
	VMEND
}

void 组包剧情(int 地图信息, int 地图难度, int 任务ID)
{
	VMBEGIN
	缓冲call(16);
	加密call(地图信息, 4);
	加密call(地图难度, 1);
	加密call(0, 2);
	加密call(0, 1);
	加密call(0, 1);
	加密call(0, 4);
	加密call(任务ID, 4);
	加密call(0, 1);
	加密call(0, 1);
	发包call();
	VMEND
}
