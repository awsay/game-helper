#pragma once
#include "stdafx.h"
#include "内存读写.h"
#include "CALL.h"
#include "组包.h"
#include "公告.h"
#include "加密解密.h"
#include "判断.h"
#include "常用功能.h"
#include <stack>

struct Point
{
	int x, y;
	Point(){}
	Point(int x,int y):x(x),y(y){}
};

void 获取方向(int *p, int 通道);

void 获取地图();

int 计算方向(Point cur, Point next);

int 获取走法();

Point 计算下一个点坐标(int 方向);

void 坐标过图(int 方向);

void 方向过图(int 方向);

void 过图();

DWORD 自动过图(LPVOID);

DWORD 自动挂机(LPVOID);

void 自动();

VOID MAIN();