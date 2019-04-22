#pragma once

#include "stdafx.h"
#include "基址.h"
#include "常用功能.h"
#include "加密解密.h"
#include "公告.h"
#include "内存读写.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "CALL.h"


int 取OBJ文本路径(int ID);

int 取技能Atk目录(int Obj路径文本);

void 修改伤害(int 代码, int 伤害);

void 全屏遍历();

DWORD 技能循环(LPVOID lpParam);

void 自身全屏攻击();

void 全屏秒杀();
