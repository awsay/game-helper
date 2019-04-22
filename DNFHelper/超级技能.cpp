#include "stdafx.h"
#include "超级技能.h"

技能 所有技能[200];
int 技能数 = 0;
void 技能遍历()
{
	
	int 开始 = 遍历开始, 结束 = 遍历结束;
	for (int i = 开始; i <= 结束; i+=4)
	{
		int 等级 = 解密(read(read(人物基址) + i) + 技能等级偏移);
		if (等级 <= 0) continue;
		WCHAR* 名称 = 读文本(read(read(read(人物基址)+i)+技能名称偏移), 100);
		所有技能[技能数].技能等级 = 等级;	
		所有技能[技能数++].技能名称 = 名称;
		系统公告(名称);
	}
}