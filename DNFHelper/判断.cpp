#include "stdafx.h"
#include "判断.h"

bool 是否在城镇()
{
	int tmp = read(read(人物基址) + 地图偏移);
	return tmp == 0;
}

bool 是否有物品()
{
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	if (二级偏移 <= 0)
		return false;
	int 首地址_ = read(二级偏移 + 首地址);
	int 尾地址_ = read(二级偏移 + 尾地址);
	int 人物地址 = 一级偏移;
	int 数量 = (尾地址_ - 首地址_) / 4;
	float x = read<float>(人物地址 + X坐标);
	float y = read<float>(人物地址 + Y坐标);
	for (int i = 1; i <= 数量; i++)
	{
		int 地址 = read(首地址_ + 4 * i);
		int 类型 = read(地址 + 类型偏移);
		if (类型 == 289 && 地址 != 一级偏移)
		{
			return true;
		}
	}
	return false;
}

bool 是否开门()
{
	if (解密(read(read(人物基址) + 地图偏移) + 280) == 0)
		return true;
	return false;
}


int 已用疲劳值()
{
	return 解密(已用疲劳);
}

int 剩余疲劳()
{
	return 解密(最大疲劳) - 解密(已用疲劳);
}

bool 是否制裁()
{
	return read(制裁基址) != 0;
}

bool 是否在BOSS房()
{
	int bossx = 解密(read(read(read(房间编号) + 时间基址) + 204) + BOSS房间X);
	int bossy = 解密(read(read(read(房间编号) + 时间基址) + 204) + BOSS房间Y);
	int curx = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间X);
	int cury = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间Y);
	return curx == bossx && cury == bossy;
}

bool 是否进入角色()
{
	if (read(人物基址) > 0)
		return true;
	return false;
}

int 当前等级()
{
	return read(角色等级);
}
