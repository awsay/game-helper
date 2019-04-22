#include "stdafx.h"
#include "常用功能.h"
#include "微调buff.h"

void 评分()
{
	if (是否在城镇() == false) {
		int 评分地址 = read(评分基址);
		超级加密(评分地址 + 272, 6542475);
		Sleep(300);
	}

}

void 透明call(int 人物)
{
	VMBEGIN
	int call地址 = 无敌Call;
	_asm
	{
		mov ecx, 人物
		mov esi, ecx
		push 0xFF
		push 0x01
		push 0x01
		push 0x01
		mov edx, call地址
		call edx
	}
	VMEND
}

void 透明()
{
	if (是否在城镇() == false)
	{
		透明call(read(人物基址));
	}
}

BOOL 自动透明开关 = FALSE;
HANDLE 透明线程 = NULL;
BOOL 人物透明 = FALSE;
DWORD 透明循环()
{
	int 特效 = 0;
	while(true)
	{
		if (read(人物基址) <= 0)
		{
			特效 = 0;
			Sleep(1000);
			continue;
		}
		if (是否在城镇())
		{
			人物透明 = FALSE;
			if (特效 == 0)
			{
				特效 = 1;
				特效Buff();
			}
		}
		else if (是否在BOSS房())
		{
			人物透明 = FALSE;
		}
		else if (人物透明 == FALSE)
		{
			人物透明 = TRUE;
			透明();
			Sleep(500);
			评分();
			Sleep(500);
		}
		Sleep(4000);
	}
	return 0;
}

void 自动透明()
{
	if (自动透明开关 == TRUE)
	{
		系统公告(L"关闭自动透明");
		自动透明开关 = FALSE;
		SuspendThread(透明线程);
		SetThreadPriority(透明线程, THREAD_PRIORITY_IDLE);
	}
	else
	{
		自动透明开关 = TRUE;
		系统公告(L"开启自动透明");
		if (透明线程 == NULL)
			透明线程 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)透明循环, NULL, 0, NULL);
		else
			ResumeThread(透明线程);
	}
}


bool 无CD开关 = false;
const int 技能无CD = 0x0225CA86;
void 无CD()
{
	VMBEGIN
	DWORD oldprotect;
	VirtualProtect((LPVOID)技能无CD, 8, PAGE_EXECUTE_READWRITE, &oldprotect);
	if (无CD开关 == FALSE)
	{
		write<BYTE>(技能无CD, 0xeb);
		无CD开关 = TRUE;
		系统公告(L"技能无cd开启");
	}
	else
	{
		write<BYTE>(技能无CD, 0x75);
		无CD开关 = FALSE;
		系统公告(L"技能无cd关闭");
	}
	VirtualProtect((LPVOID)技能无CD, 8, oldprotect, &oldprotect);
	VMEND
}


void 设置坐标(int 对象地址, int x, int y, int z)
{
	write(read(对象地址 + 172) + 16, x);
	write(read(对象地址 + 172) + 20, y);
}

void 拾取()
{
	VMBEGIN
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	int 首地址_ = read(二级偏移 + 首地址);
	int 尾地址_ = read(二级偏移 + 尾地址);
	int 物品数量 = (尾地址_ - 首地址_) / 4;
	for (int i = 1; i <= 物品数量; i++)
	{
		int 地址 = read(首地址_ + 4 * i);
		int 类型 = read(地址 + 类型偏移);
		if (类型 == 289 && 地址 != 一级偏移)
		{
			组包拾取(解密(地址 + 160));
			Sleep(100);
		}
	}
	VMEND
}


int 取出地址(int 地址)
{
	int edi = read(地址 + 160);
	int ecx = read(read(解密基址) + (edi >> 16) * 4 + 36);
	ecx = read(ecx + (edi & 65535) * 4 + 8468);
	ecx = ecx & 65535;
	ecx = ecx + (ecx << 16);
	return ecx ^ (read(地址 + 160) + 8);
}



void 吸怪聚物()
{
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	int 首地址_ = read(二级偏移 + 首地址);
	int 尾地址_ = read(二级偏移 + 尾地址);
	int 人物地址 = 一级偏移;
	int 怪物数量 = (尾地址_ - 首地址_) / 4;
	float x = read<float>(人物地址 + X坐标);
	float y = read<float>(人物地址 + Y坐标);
	for (int i = 1; i <= 怪物数量; i++)
	{
		int 地址 = read(首地址_ + 4 * i);
		int 类型 = read(地址 + 类型偏移);
		if ((类型 == 529 || 类型 == 545 || 类型 == 273 || 类型 == 289) &&地址 != 一级偏移)
		{
			write(read(地址 + 172) + 16, x);
			write(read(地址 + 172) + 20, y);
		}
	}

}

void 聚物()
{
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
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
			write(read(地址 + 172) + 16, x);
			write(read(地址 + 172) + 20, y);
		}
	}
}

void 初始化配置()
{
	char *filepathA = UnicodeToAscii(filepath);
	if (_access(filepathA, 0) == -1)
	{
		WritePrivateProfileString(L"全屏", L"技能代码", L"25540",filepath);
		WritePrivateProfileString(L"全屏", L"秒杀代码", L"39002", filepath);
		WritePrivateProfileString(L"全屏", L"技能伤害", L"123456", filepath);
		WritePrivateProfileString(L"全屏", L"技能个数", L"4", filepath);
		WritePrivateProfileString(L"全屏", L"技能频率", L"1000", filepath);
		WritePrivateProfileString(L"人偶怪物", L"人偶代码", L"416", filepath);
		WritePrivateProfileString(L"人偶怪物", L"怪物代码", L"69200", filepath);
		WritePrivateProfileString(L"微调", L"特效代码", L"165|1109|1108|168|176|175|121", filepath);
	}
	delete filepathA;
}

int 键映射(int 键)
{
	if (键 == VK_F1)
		键 = 328;
	else if (键 == VK_F2)
		键 = 329;
	else if (键 == VK_F3)
		键 = 330;
	else if (键 == VK_F4)
		键 = 331;
	else if (键 == VK_F5)
		键 = 332;
	else if (键 == VK_F6)
		键 = 333;
	else if (键 == VK_F7)
		键 = 334;
	else if (键 == VK_F8)
		键 = 335;
	else if (键 == VK_F9)
		键 = 336;
	else if (键 == VK_F10)
		键 = 337;
	else if (键 == VK_F11)
		键 = 338;
	else if (键 == VK_F12)
		键 = 339;
	else if (键 == '1')
		键 = 271;
	else if (键 == '2')
		键 = 272;
	else if (键 == '3')
		键 = 273;
	else if (键 == '4')
		键 = 274;
	else if (键 == '5')
		键 = 275;
	else if (键 == '6')
		键 = 276;
	else if (键 == '7')
		键 = 277;
	else if (键 == '8')
		键 = 278;
	else if (键 == '9')
		键 = 279;
	else if (键 == '0')
		键 = 280;
	else if (键 == 'Q')
		键 = 285;
	else if (键 == 'W')
		键 = 286;
	else if (键 == 'E')
		键 = 287;
	else if (键 == 'R')
		键 = 288;
	else if (键 == 'T')
		键 = 289;
	else if (键 == 'Y')
		键 = 290;
	else if (键 == 'U')
		键 = 291;
	else if (键 == 'I')
		键 = 292;
	else if (键 == 'O')
		键 = 293;
	else if (键 == 'P')
		键 = 294;
	else if (键 == 'A')
		键 = 299;
	else if (键 == 'S')
		键 = 300;
	else if (键 == 'D')
		键 = 301;
	else if (键 == 'F')
		键 = 302;
	else if (键 == 'G')
		键 = 303;
	else if (键 == 'H')
		键 = 304;
	else if (键 == 'J')
		键 = 305;
	else if (键 == 'K')
		键 = 306;
	else if (键 == 'L')
		键 = 307;
	else if (键 == 'Z')
		键 = 313;
	else if (键 == 'X')
		键 = 314;
	else if (键 == 'C')
		键 = 315;
	else if (键 == 'V')
		键 = 316;
	else if (键 == 'B')
		键 = 317;
	else if (键 == 'N')
		键 = 318;
	else if (键 == 'M')
		键 = 319;
	else if (键 == VK_UP)
		键 = 469;
	else if (键 == VK_DOWN)
		键 = 477;
	else if (键 == VK_LEFT)
		键 = 472;
	else if (键 == VK_RIGHT)
		键 = 474;
	else if (键 == VK_SPACE)
		键 = 326;
	else if (键 == MOD_ALT)
		键 = 325;
	else if (键 == VK_ESCAPE)
		键 = 270;
	return 键;
}

void 按下(int 键)
{
	write<BYTE>(read(按键基址) + 键映射(键), 0x01);
}

void 抬起(int 键)
{
	write<BYTE>(read(按键基址) + 键映射(键), 0x00);
}

void 按键(int 键, int 时长)
{
	if (时长 == 0)
		时长 = 333;
	键 = 键映射(键);
	write<BYTE>(read(按键基址) + 键, 0x01);
	Sleep(时长);
	write<BYTE>(read(按键基址) + 键, 0x00);
}

void 装备卖出()
{
	int 背包指针 = read(背包基址);
	int 首地址_ = read(背包指针 + 背包偏移);
	int 装备栏 = 首地址_ + 36;
	for (int i = 0; i < 56; i++)
	{
		int 装备地址 = read(装备栏 + 4 * i);
		if (装备地址 != 0)
		{
			int 装备属性 = read(装备地址 + 装备品级);
			if (装备属性 == 0 || 装备属性 == 1 || 装备属性 == 2)
			{

				组包卖物(i + 8);
				处理事件();
				Sleep(100);
			}
		}
	}
}

void 怪物跟随()
{
	VMBEGIN
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	if (二级偏移 == 0)
		return;
	int 首地址_ = read(二级偏移 + 首地址);
	int 尾地址_ = read(二级偏移 + 尾地址);
	int 怪物数量 = (尾地址_ - 首地址_) / 4;
	int 技能数量_ = 0;
	int x, y;
	for (int i = 1; i <= 怪物数量; i++)
	{
		int 地址 = read(首地址_ + 4 * i);
		if (地址 <= 0)
			continue;
		int 类型 = read(地址 + 类型偏移);
		int 阵营 = read(地址 + 阵营偏移);
		int 血量 = read(地址 + 怪物血量偏移);
		int 代码 = read(地址 + 对象代码);
		if ((类型 == 529 || 类型 == 545 || 类型 == 273) && 阵营 > 0 && 血量 > 0 && 地址 != 一级偏移)
		{
			x = (int)read<float>(地址 + X坐标);
			y = (int)read<float>(地址 + Y坐标);
			坐标call(一级偏移, x, y, 0);
			break;
		}

	}
	VMEND
}
