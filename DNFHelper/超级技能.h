#pragma once
#include "stdafx.h"
#include "加密解密.h"
#include "公告.h"

#define 遍历开始 0x3A78
#define 遍历结束 0x463C

struct 技能 
{
	WCHAR* 技能名称;
	int 技能等级;
};

void 技能遍历();