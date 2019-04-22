#pragma once
#include "基址.h"
#include "内存读写.h"
#include "CALL.h"

void 组包翻牌(int x, int y);

void 组包拾取(int 物品地址);

void 组包分解(int 装备位置);

void 选角色(int 角色位置, int 类型);

void 组包秒杀(int 地址);

void 组包过图(int X, int Y);

void 组包移动(int 区域, int 位置, int X, int Y);

void 组包进图(int 地图信息, int 地图难度, int 地图深渊 = 0, int 地图练习 = 0);

void 组包卖物(int 位置);

void 组包剧情(int 地图信息, int 地图难度, int 任务ID);