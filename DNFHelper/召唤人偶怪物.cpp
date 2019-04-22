#include "stdafx.h"
#include "召唤人偶怪物.h"

int 返回人偶地址()
{
	VMBEGIN
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	int 首地址_ = read(二级偏移 + 首地址);
	int 尾地址_ = read(二级偏移 + 尾地址);
	int 人物地址 = read(人物基址);
	int 怪物数量 = (尾地址_ - 首地址_) / 4;
	for (int i = 1; i <= 怪物数量; i++)
	{
		int 怪物地址 = read(首地址_ + 4 * i);
		int 怪物类型 = read(怪物地址 + 类型偏移);
		int 怪物阵营 = read(怪物地址 + 阵营偏移);
		if (怪物类型 == 273 && 怪物阵营 == 0 && 怪物地址 != 人物地址)
		{
			return 首地址_ + 4 * i;
		}
	}
	return 0;
	VMEND
}

void 人偶call(int 代码)
{
	VMBEGIN
	if (代码 == 0)
		代码 = GetPrivateProfileInt(L"人偶怪物", L"人偶代码", 0, filepath);
	int 参数 = read(召唤参数);
	int 指针 = read(人物基址);
	int* 结构 = new int[5];
	结构[0] = 代码;
	结构[1] = -1;
	结构[2] = -1;
	结构[3] = 12345688; // 生存时间
	结构[4] = 1; // 生效
	int 召唤人偶Call_ = 召唤人偶Call;
	_asm
	{
		push 结构
		push 指针
		mov ecx, 参数
		call 召唤人偶Call_
	}
	VMEND
}


void 怪物call()
{
	VMBEGIN
	int 代码 = GetPrivateProfileInt(L"人偶怪物", L"怪物代码", 0, filepath);
	int 参数 = read(召唤参数);
	int 指针 = read(人物基址);
	int* 结构 = new int[5];
	结构[0] = 代码;
	结构[1] = -1;
	结构[2] = -1;
	结构[3] = 12345688; // 生存时间
	结构[4] = 1; // 生效
	int 召唤怪物Call_ = 召唤怪物Call;
	_asm
	{
		push 结构
		push 指针
		mov ecx, 参数
		call 召唤怪物Call_
	}
	系统公告(L"已经召唤怪物");
	VMEND
}