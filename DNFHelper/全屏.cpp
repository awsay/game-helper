#include "stdafx.h"
#include "全屏.h"

/*
11422 血爆
38002 血爆
54141 无形秒杀
25540 死灵人偶技能
39002 无形秒
*/
int 技能频率;
int 技能伤害;
int 技能个数;
int 技能代码;
int 技能代码集合[100];
int Obj路径[100], Atk[100] = { 0 };
int 技能代码个数;
int index = 0;

int 取OBJ文本路径(int ID)
{
	VMBEGIN
	int ebp_4 = 目录基址;
	int ebp_8 = 路径Call;
	_asm
	{
		push ID
		mov ecx, ebp_4
		call ebp_8
		mov ebp_4, eax
	}
	VMEND
	return ebp_4;
}

int 取技能Atk目录(int Obj路径文本)
{
	VMBEGIN
	int 副本信息_ = 副本信息;
	int 加载_Call = 加载Call;
	int 分割_Call = 分割Call;
	int Atk路径_基址 = 路径基址;
	int Atk路径_Call = 加载Call;
	int Atk目录 = 0;
	_asm
	{
		mov ecx, 副本信息_
		mov ecx, [ecx]
		push 0x01
		push Obj路径文本
		call 加载_Call
		test eax, eax
		je Label1
		mov edi, eax
		lea esi, [edi + 0x000000B4]
		lea eax, [edi + 0x000000F4]
		push esi
		push eax
		lea edx, 副本信息_
		push edx
		mov[edx], 0x00000000
		call 分割_Call
		add esp, 0x0C
		mov eax, [eax]
		mov ecx, Atk路径_基址
		mov ecx, [ecx]
		push 0x01
		push eax
		call Atk路径_Call
		Label1 :
		mov Atk目录, eax
	}
	return Atk目录;
	VMEND

}


void 修改伤害(int 代码, int 伤害)
{
	VMBEGIN
	int Obj路径 = 取OBJ文本路径(代码);
	if (Obj路径 > 0)
	{
		int Atk = 取技能Atk目录(Obj路径);
		if (Atk > 0)
		{
			超级加密(Atk + 32, 伤害);
		}
	}
	VMEND
}

void 爆物()
{
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	if (二级偏移 == 0)
		return;
	int 首地址_ = read(二级偏移 + 首地址);
	int 尾地址_ = read(二级偏移 + 尾地址);
	int 怪物数量 = (尾地址_ - 首地址_) / 4;
	for (int i = 0; i <= 怪物数量; i++)
	{
		int 地址 = read(首地址_ + 4 * i);
		int 类型 = read(地址 + 类型偏移);
		int 代码 = read(地址 + 对象代码);
		int x, y;
		x = (int)read<float>(地址 + X坐标);
		y = (int)read<float>(地址 + Y坐标);
		if (代码 == 10660)
		{
			处理事件();
			释放call(一级偏移, 11435, 0, x, y, 0);
			释放call(一级偏移, 11435, 0, x, y, 0);
			continue;
		}
		else if (类型 == 1057)
		{
			处理事件();
			释放call(一级偏移, 11435, 0, x, y, 0);
		}
	}
}

void 全屏遍历()
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
	int x, y, z;
	if (技能伤害 == 0)
	{
		int sum = 0, cnt = 0;
		for (int i = 1; i <= 怪物数量; i++)
		{
			int 地址 = read(首地址_ + 4 * i);
			if (地址 <= 0)
				continue;
			int 类型 = read(地址 + 类型偏移);
			int 阵营 = read(地址 + 阵营偏移);
			int 血量 = read(地址 + 怪物血量偏移);
			if ((类型 == 529 || 类型 == 545 || 类型 == 273) && 阵营 > 0 && 血量 > 0)
			{
				cnt++;
				sum += 血量;
			}
		}
		if (cnt == 0) return;
		for (int i = 0; i < 技能代码个数; i++)
		{
			超级加密(Atk[i] + 32, sum / cnt );
		}
	}
	for (int i = 0; i <= 怪物数量; i++)
	{
		int 地址 = read(首地址_ + 4 * i);
		if (地址 <= 0)
			continue;
		int 类型 = read(地址 + 类型偏移);
		int 阵营 = read(地址 + 阵营偏移);
		int 血量 = read(地址 + 怪物血量偏移);
		int 代码 = read(地址 + 对象代码);
		x = (int)read<float>(地址 + X坐标);
		y = (int)read<float>(地址 + Y坐标);
		z = (int)read<float>(地址 + Z坐标);
		if ((类型 == 529 || 类型 == 545 || 类型 == 273) &&  阵营 > 0 && 血量 > 0 && 地址 != 一级偏移)
		{
			技能代码 = 技能代码集合[index % 技能代码个数];
			释放call(一级偏移, 技能代码, 0, x, y, z);
			技能数量_ += 1;
			index++;
			if (技能数量_ == 技能个数)
				break;
		}
		
	}
	if (index > 100)
		index = 0;
	
	VMEND
}

DWORD 技能循环(LPVOID lpParam)
{
	VMBEGIN
	系统公告(L"全屏开启");
	int cnt = 0;
	memset(Atk, 0, sizeof(Atk));
	while (true)
	{
		if (read(人物基址) <= 0)
		{
			Sleep(1000);
			continue;
		}
		if (是否在城镇() == false)
		{
			Sleep(8000);
			break;
		}
		Sleep(500);
	}
	for (int i = 0; i < 技能代码个数; i++)
	{
		Atk[i] = 取技能Atk目录(Obj路径[i]);
	}
	if (技能伤害 > 1)
	{
		for (int i = 0; i < 技能代码个数; i++)
		{
			超级加密(Atk[i] + 32, 技能伤害);
		}
	}
	修改伤害(11435, 123456);
	while (true)
	{
		if (read(人物基址) <= 0)
		{
			Sleep(1000);
			continue;
		}	
#ifdef 测试
		技能代码 = GetPrivateProfileInt(L"全屏", L"技能代码", 0, filepath);
		技能伤害 = GetPrivateProfileInt(L"全屏", L"技能伤害", 0, filepath);
		技能个数 = GetPrivateProfileInt(L"全屏", L"技能个数", 0, filepath);
		技能频率 = GetPrivateProfileInt(L"全屏", L"技能频率", 0, filepath);
#endif
		if (cnt % 200 == 0 && 技能伤害 == 1)
		{
			int x = 当前等级();
			double 伤害 = 0.000443*pow(x, 5) - 0.04617*pow(x, 4) + 1.751*pow(x, 3) - 15.49*pow(x, 2) + 113.4*x + 192.4;
			for (int i = 0; i < 技能代码个数; i++)
			{
				超级加密(Atk[i] + 32, (int)伤害);
			}
		}
		爆物();
		处理事件();
		全屏遍历();
		Sleep(技能频率);
		cnt++;
	}
	VMEND
	return 0;
}

bool 自身全屏开启 = false;
HANDLE 技能线程2 = NULL;
void 自身全屏攻击()
{
	VMBEGIN
	if (自身全屏开启 == false)
	{
		系统公告(L"自身全屏开启");
		自身全屏开启 = true;
		WCHAR 代码[200];
		GetPrivateProfileString(L"全屏", L"技能代码", L"25540|24012", 代码, sizeof(代码), filepath);
		std::string* str = UnicodeToString(代码);
		std::vector<std::string> result = split(*str, '|');
		技能代码个数 = result.size();
		for (size_t i = 0; i < result.size(); i++)
		{
			技能代码集合[i] = atoi(result[i].c_str());
			Obj路径[i] = 取OBJ文本路径(技能代码集合[i]);
		}
		技能伤害 = GetPrivateProfileInt(L"全屏", L"技能伤害", 0, filepath);
		技能个数 = GetPrivateProfileInt(L"全屏", L"技能个数", 0, filepath);
		技能频率 = GetPrivateProfileInt(L"全屏", L"技能频率", 0, filepath);
		WCHAR tmp[200];
		format(tmp, L"技能代码:%s 技能伤害:%d 技能频率:%d", 代码, 技能伤害, 技能频率);
		系统公告(tmp);
		技能线程2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)技能循环, NULL, 0, NULL);
		SetThreadPriority(技能线程2, THREAD_PRIORITY_IDLE);
	}
	else
	{
		自身全屏开启 = false;
		TerminateThread(技能线程2, 0);
		系统公告(L"自身全关闭");
	}
	VMEND
}


bool 全屏秒杀开关 = false;
HANDLE 技能线程3 = NULL;
void 全屏秒杀()
{
	VMBEGIN
		/*if (全屏秒杀开关 == false)
		{
			系统公告(L"全屏秒杀开启");
			技能代码 = GetPrivateProfileInt(L"全屏", L"秒杀代码", 0, filepath);
			全屏秒杀开关 = true;
			技能线程3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)技能循环, NULL, 0, NULL);

		}
		else
		{
			全屏秒杀开关 = false;
			TerminateThread(技能线程3, 0);
			系统公告(L"全屏秒杀关闭");
		}*/
	VMEND

}