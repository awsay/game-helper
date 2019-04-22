#pragma once
#include "stdafx.h"
#include "基址.h"

void 缓冲call(int 包头);

void 加密call(int 参数, int 长度);

void 发包call();

void 过图call(int 方向);

void 坐标call(int 对象地址, int x, int y, int z);

void 接受call(int 任务ID);

void 提交call(int 任务ID);

void 释放call(int 对象基址, int 代码, int 伤害, int X, int Y, int Z);

void 完成call(int 任务ID);

void 爆物call(int 对象);