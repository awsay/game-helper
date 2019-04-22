#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <vector>
#include  <io.h>
#include "基址.h"
#include "加密解密.h"
#include "公告.h"
#include "组包.h"
#include "常用函数.h"
#include "判断.h"

void 评分();

void 透明call(int 人物);

void 透明();

DWORD 透明循环();

void 自动透明();

void 无CD();

void 设置坐标(int 对象地址, int x, int y, int z);

void 拾取();

int 取出地址(int 地址);

void 吸怪聚物();

void 聚物();

void 初始化配置();

int 键映射(int 键);

void 按下(int 键);

void 抬起(int 键);

void 按键(int 键, int 时长 = 0);

void 装备卖出();

void 怪物跟随();