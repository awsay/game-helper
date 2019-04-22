#pragma once
#include "stdafx.h"
#include "CALL.h"
#include "内存读写.h"
#include "全屏.h"
#include "自动.h"
#include "组包.h"
#include "常用功能.h"
#include "召唤人偶怪物.h"
#include <stdio.h>
#include <string.h>

void 任务测试()
{
	int 首地址_ = read(read(任务基址) + 任务首地址);
	int 尾地址_ = read(read(任务基址) + 任务尾地址);
	int 任务数量 = (尾地址_ - 首地址_) / 4;
	for (int i = 0; i < 任务数量; i++)
	{
		int 任务地址 = read(首地址_ + i * 4);
		int 任务类型_ = read(任务地址 + 任务类型);
		if (任务类型_ == 0)
		{
			int 任务ID = read(任务地址);
			if (任务ID == 0) continue;
			完成call(任务ID);
			超级延时(300);
			公告(L"完成 任务ID为：%d",任务ID);
		}
	}
}

void 任务测试2()
{
	int 首地址_ = read(read(任务基址) + 0x8);
	int 尾地址_ = read(read(任务基址) + 0xc);
	int 任务数量 = (尾地址_ - 首地址_) / 4;
	for (int i = 0; i < 任务数量; i++)
	{
		int 任务地址 = read(首地址_ + i * 4);
		int 任务类型_ = read(任务地址 + 任务类型);
		int 任务ID = read(任务地址);
		完成call(任务ID);
		超级延时(300);
		提交call(任务ID);
		超级延时(300);
		公告(L"完成 任务ID为：%d", 任务ID);
	}
}

void 任务测试3()
{
	int 首地址_ = read(read(任务基址) + 任务首地址);
	int 尾地址_ = read(read(任务基址) + 任务尾地址);
	int 任务数量 = (尾地址_ - 首地址_) / 4;
	for (int i = 0; i < 任务数量; i++)
	{
		int 任务地址 = read(首地址_ + i * 4);
		int 任务类型_ = read(任务地址 + 任务类型);
		if (任务类型_ == 0)
		{
			int 任务ID = read(任务地址);
			提交call(任务ID);
		}
	}
}
FILE* fp;
char sz_add[30];
void 技能释放(int 代码)
{
	VMBEGIN
	int 一级偏移 = read(人物基址);
	int x = (int)read<float>(一级偏移 + X坐标);
	int y = (int)read<float>(一级偏移 + Y坐标);
	释放call(一级偏移, 代码, 1, x, y, 0);
	VMEND
}

DWORD 技能主循环(LPVOID)
{
	fopen_s(&fp, "C:\\code.txt", "a+");
	fseek(fp, 0, SEEK_END);
	for (int i = 24000; i <= 40000; i++)
	{
		if (是否在城镇() == true)
		{
			Sleep(5000);
			i--;
			continue;
		}
		公告(L"当前i 为 %d", i);
		技能释放(i);
	}
	return 0;
}

HANDLE aaa = NULL;
int 技能遍历开关()
{
	if (aaa == NULL)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)技能主循环, NULL, 0, NULL);
	else
	{
		TerminateThread(aaa, 0);
		aaa = NULL;
	}
	return 0;
}

void 类型遍历()
{
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	if (二级偏移 <= 0)
	{
		公告(L"二级偏移为%d", 二级偏移);
		return;
	}
	int 首地址_ = read(二级偏移 + 首地址);
	int 尾地址_ = read(二级偏移 + 尾地址);
	int 怪物数量 = (尾地址_ - 首地址_) / 4;
	公告(L"首地址为%d 尾地址为%d  怪物数量为%d", 首地址_, 尾地址_, 怪物数量);
	int x, y, z;
	for (int i = 1; i <= 怪物数量; i++)
	{
		int 地址 = read(首地址_ + 4 * i);
		int 类型 = read(地址 + 类型偏移);
		int 阵营 = read(地址 + 阵营偏移);
		int 血量 = read(地址 + 怪物血量偏移);
		x = (int)read<float>(地址 + X坐标);
		y = (int)read<float>(地址 + Y坐标);
		z = (int)read<float>(地址 + Z坐标);
		if ((类型 == 529 || 类型 == 545 || 类型 == 273))
			公告(L"地址：%d  类型：%d  阵营：%d  血量：%d  x：%d  y：%d  z：%d", 地址, 类型, 阵营, 血量, x, y, z);
	}
}
DWORD 人偶代码遍历(LPVOID)
{
	for (int i = 400; i <= 1000; i++)
	{
		if (是否在城镇() == true)
		{
			Sleep(5000);
			i--;
			continue;
		}
		公告(L"当前代码为 %d", i);
		人偶call(i);
		Sleep(1000);
	}
	return 0;
}

void 人偶测试()
{
	if (aaa == NULL)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)人偶代码遍历, NULL, 0, NULL);
	else
	{
		TerminateThread(aaa, 0);
		aaa = NULL;
	}
}

void test()
{
	int type = GetPrivateProfileInt(L"测试", L"type", 1, filepath);
	if (type == 1)
	{
		int code = GetPrivateProfileInt(L"全屏", L"技能代码", 1, filepath);
		公告(L"代码 ： %d ", code);
		处理事件();
		技能释放(code);
	}
	else if (type == 2)
	{
		任务测试();
	}
	else if (type == 3)
	{
		任务测试3();
	}
	else if (type == 4)
	{
		int 一级偏移 = read(人物基址);
		int 二级偏移 = read(一级偏移 + 地图偏移);
		if (二级偏移 == 0)
		{
			系统公告(L"二级偏移为0 ，无法遍历");
			return;
		}
		int 首地址_ = read(二级偏移 + 首地址);
		int 尾地址_ = read(二级偏移 + 尾地址);
		int 怪物数量 = (尾地址_ - 首地址_) / 4;
		int 技能数量_ = 0;

		int rx = (int)read<float>(一级偏移 + X坐标);
		int ry = (int)read<float>(一级偏移 + Y坐标);
		公告(L"人物 x：%d  y：%d 怪物数量：%d", rx, ry,怪物数量);
		for (int i = 0; i <= 怪物数量; i++)
		{
			int 地址 = read(首地址_ + 4 * i);
			int 类型 = read(地址 + 类型偏移);
			int 阵营 = read(地址 + 阵营偏移);
			int 血量 = read(地址 + 怪物血量偏移);
			int 代码 = read(地址 + 对象代码);
			int x, y;
			x = (int)read<float>(地址 + X坐标);
			y = (int)read<float>(地址 + Y坐标);
			公告(L"类型：%d  血量：%d  代码：%d  x:%d  y:%d ", 类型, 血量, 代码, x, y);
		}
	}
}