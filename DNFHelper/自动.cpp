#include "stdafx.h"
#include "自动.h"
#include "召唤人偶怪物.h"
#include "全屏.h"
#include "判断.h"
#include "微调buff.h"

int 地图[100][100][4];
bool 地图初始化 = false;
int 走法[100][100] = { -1 };
extern int 技能频率;
extern int 技能伤害;
extern int 技能个数;
extern int 技能代码;
extern int 技能代码集合[100];
extern int 技能代码个数;

int 方向集合[16][4] =
{
	{ 0, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 1, 1, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 1, 0, 0 },
	{ 1, 0, 1, 0 },
	{ 1, 1, 1, 0 },
	{ 0, 0, 0, 1 },
	{ 0, 1, 0, 1 },
	{ 0, 0, 1, 1 },
	{ 0, 1, 1, 1 },
	{ 1, 0, 0, 1 },
	{ 1, 1, 0, 1 },
	{ 1, 0, 1, 1 },
	{ 1, 1, 1, 1 }
};

void 获取方向(int *p, int 通道)
{
	if (通道 < 0 || 通道>15)
	{
		p[0] = p[1] = p[2] = p[3] = 0;
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		p[i] = 方向集合[通道][i];
	}
}

void 获取地图()
{
	int 地图编号 = 解密(取偏移地址(房间编号, { 时间基址,A星偏移_0,0 }) + (BOSS房间X - 24)) + 1;
	int 地图宽度 = read(取偏移地址(房间编号, { 时间基址,A星偏移_0,A星偏移_1,0 }) + 地图编号 * 8 - 8);
	int 地图高度 = read(取偏移地址(房间编号, { 时间基址,A星偏移_0,A星偏移_1,0 }) + 地图编号 * 8 - 4);
	int 临时地址 = read(取偏移地址(房间编号, { 时间基址,A星偏移_0,A星偏移_2,0 }) + 地图编号 * 20 - 16);
	int 数量 = 地图宽度 * 地图高度;
	int 通道, x, y;
	memset(地图, 0, sizeof(地图));
	for (int i = 0; i < 数量; i++)
	{
		通道 = read(临时地址 + 4 * i);
		x = i % 地图宽度;
		y = i / 地图宽度;
		for (int j = 0; j < 4; j++)
		{
			地图[x][y][j] = 方向集合[通道][j];
		}
	}
}

int 计算方向(Point cur, Point next)
{
	int x = cur.x - next.x;
	int y = cur.y - next.y;
	if (x == 0)
	{
		if (y == 1)
			return 2; //上
		else if(y == -1)
			return 3; // 下
	}
	if (y == 0)
	{
		if (x == 1)
			return 0;//左
		else if (x == -1)
			return 1;//右
	}
	return -1;
}

int 获取走法()
{
	获取地图();
	int curx = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间X);
	int cury = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间Y);
	int bossx = 解密(read(read(read(房间编号) + 时间基址) + 204) + BOSS房间X);
	int bossy = 解密(read(read(read(房间编号) + 时间基址) + 204) + BOSS房间Y);
	int Vis[100][100];
	memset(Vis, 0, sizeof(Vis));
	memset(走法, -1, sizeof(走法));
	int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
	std::stack<Point> sta;
	sta.push(Point{ curx,cury });
	Vis[curx][cury] = 1;
	int step = 1;
	while (!sta.empty())
	{
		Point point = sta.top();
		int i;
		int x = point.x;
		int y = point.y;
		for (i = 0; i < 4; i++)
		{
			int nx = x + dir[i][0], ny = y + dir[i][1];
			if (nx >= 0 && ny >= 0 && 地图[x][y][i] == 1 && Vis[nx][ny] == 0)
			{
				Vis[nx][ny] = 1;
				sta.push(Point{ nx,ny });
				走法[x][y] = i;
				step++;
				if (nx == bossx && ny == bossy)
					return step;
				break;
			}
		}
		if (i == 4)
		{
			step--;
			sta.pop();
		}
	}
	return 0;
}

Point 计算下一个点坐标(int 方向)
{
	int X = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间X);
	int Y = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间Y);
	switch (方向)
	{
	case 2:
		Y++;
		break;
	case 3:
		Y--;
		break;
	case 0:
		X--;
		break;
	case 1:
		X++;
	}
	return Point(X, Y);
}

void 坐标过图(int 方向)
{
	int 一级偏移 = read(人物基址);
	int 二级偏移 = read(一级偏移 + 地图偏移);
	int tmp = read(房间编号);
	tmp = read(tmp + 时间基址);
	tmp = read(tmp + 坐标结构偏移01);
	int 坐标结构 = tmp + (方向 + 方向 * 8) * 4 + 坐标结构偏移02;
	坐标结构 = 坐标结构 + 4 * 方向;
	int x = read(坐标结构);
	int y = read(坐标结构 + 4);
	int xf = read(坐标结构 + 8);
	int yf = read(坐标结构 + 12);
	int cx, cy;
	switch (方向)
	{
	case 0:
		cx = x + xf + 20;
		cy = y + yf / 2;
		break;
	case 1:
		cx = x - 20;
		cy = y + yf / 2;
		break;
	case 2:
		cx = x + xf / 2;
		cy = y + yf + 20;
		break;
	case 3:
		cx = x + xf / 2;
		cy = y - 20;
		break;
	default:
		break;
	}
	坐标call(一级偏移, cx, cy, 0);
	超级延时(200);
	坐标call(一级偏移, x + xf / 2, y, 0);
}

void 方向过图(int 方向)
{

	int X = 取偏移地址(商店基址 - 8, { 时间基址, A星偏移_3, 0 });
	int Y = 取偏移地址(商店基址 - 8, { 时间基址, A星偏移_4, 0 });
	公告(L"X：%d  Y:%d ", X, Y);
	Sleep(1000);
	switch(方向)
	{
	case 2:
		Y--;
		break;
	case 3:
		Y++;
		break;
	case 0:
		X--;
		break;
	case 1:
		X++;
		break;
	}
	组包过图(X, Y);
}

void 过图()
{
	// 0：左  1：右  2：上  3：下
	int curx = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间X);
	int cury = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间Y);
	if (走法[curx][cury] >= 0)
	{
		坐标过图(走法[curx][cury]);
	}
	else
		获取走法();
	if (是否开门() == true)
		获取走法();
}

DWORD 自动过图(LPVOID)
{
	VMBEGIN
	int 技能键[12] = { 'A','S','D','F','G','H','Q','W','E','R','T','Y' };
	int flag = 0;
	int cnt = 0;
	int time = 0;
	int 当前角色 = 0;
	while(true)
	{
		处理事件();
		if (read(人物基址) <= 0)
		{
			Sleep(5000);
			if (read(人物基址) > 0)
				continue;
			选角色(当前角色, 1);
			continue;
		}
		if (是否在城镇() == false)
		{
			flag = 999999;
			if (地图初始化 == false && 是否在BOSS房() == false)
			{
				地图初始化 = true;
				获取走法();
			}
			if (是否在BOSS房() == true)
			{
				怪物跟随();
				地图初始化 = false;
				拾取();
				for (int i = 0; i < 5; i++)
				{
					if (是否有物品() == true)
					{
						拾取();
						聚物();
						按键('X');
						Sleep(500);
					}
					else break;
				}
				int 键 = 技能键[cnt % 12];
				按键(键);
				按键(VK_ESCAPE);
				time = 0;
			}
			else if (是否开门() == true)
			{
				拾取();
				Sleep(300);
				拾取();
				过图();
			}
			else
			{
				怪物跟随();
				if (time > 35 && time % 5 == 0)
					过图();
				int 键 = 技能键[cnt% 12];
				按键(键);
			}
			按键(VK_SPACE, 666);
			cnt++;
			time++;
			if (cnt % 100 == 0)
			{
				cnt = cnt * cnt % 4567;
			}
		}
		else
		{
			time = 0;
			地图初始化 = false;
			if (剩余疲劳() == 0)
			{
				Sleep(1000);
				系统公告(L"当前角色疲劳已经用完");
				当前角色++;
				Sleep(4000);
				处理事件();
				选角色(0, 0);
				处理事件();
				Sleep(3000);
				选角色(当前角色, 1);
				flag = 0;
				continue;
			}
			if (flag == 999999 + 5)
			{
				装备卖出();
				系统公告(L"装备已经卖出");
				for (int i = 0; i < 5; i++)
				{
					MessageBeep(MB_ICONHAND);
					Sleep(500);
				}
				MessageBox(NULL, L"任务", L"任务", MB_OK);
			}
			flag++;
			Sleep(1000);
		}

	}
	return 0;
	VMEND
}

DWORD 自动挂机(LPVOID)
{
	int cnt = 0;
	while(true)
	{
		处理事件();
		if (read(人物基址) <= 0)
		{
			Sleep(1000);
			continue;
		}
		cnt++;
		if (是否在城镇() == false)
		{
			按键(VK_SPACE);
			按键(VK_SPACE);
			if (是否在BOSS房() == true)
			{
				按键(VK_ESCAPE);
				按键(VK_ESCAPE);
				按键(VK_SPACE);
				按键(VK_SPACE);
			}
			else
			{
				int curx = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间X);
				int cury = read(read(read(read(房间编号) + 时间基址) + 204) + 当前房间Y);
				int 方向 = -1;
				switch (走法[curx][cury])
				{
				case 0:方向 = VK_LEFT; break;
				case 1:方向 = VK_RIGHT; break;
				case 2:方向 = VK_UP; break;
				case 3:方向 = VK_DOWN; break;
				}
				按键(方向, 800);
			}
		}
		if (cnt % 100 == 0)
		{
			cnt = cnt * cnt * cnt % 1234;
		}
	}
	return 0;
}

bool 自动开关 = false;
HANDLE 自动线程 = NULL;
HANDLE 挂机线程 = NULL;
void 自动()
{
	VMBEGIN
	if (自动开关 == false)
	{
		自动开关 = true;
		地图初始化 = false;
		系统公告(L"自动刷图开启");
		自动线程 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)自动过图, NULL, 0, NULL);
		挂机线程 = CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)自动挂机, NULL, 0, NULL);
		SetThreadPriority(自动线程, THREAD_PRIORITY_IDLE);
		SetThreadPriority(挂机线程, THREAD_PRIORITY_IDLE);
	}
	else
	{
		自动开关 = false;
		系统公告(L"自动刷图关闭");
		TerminateThread(自动线程,0);
		TerminateThread(挂机线程,0);
		memset(走法, -1, sizeof(走法));
	}
	VMEND
}


DWORD AllThread(LPVOID)
{
	int 技能键[12] = { 'A','S','D','F','G','H','Q','W','E','R','T','Y' };
	int flag = 0;
	int cnt = 0;
	bool 特效 = false;
	bool 透明评分 = false;
	bool 人偶 = false;
	int 键;
	int time = 0;
	while (true)
	{
		if (read(人物基址) <= 0)
		{
			Sleep(1000);
			特效 = false;
			continue;
		}
		if (是否在城镇() == false)
		{
			Sleep(8000);
			break;
		}
		else if(特效 == false)
		{
			特效 = true;
			特效Buff();
		}
		Sleep(500);
	}
	while(true)
	{
		if (read(人物基址) <= 0)
		{
			Sleep(1000);
			特效 = false;
			continue;
		}
		if (是否在城镇() == false)
		{
			flag = 999999;
			time++;
			if (time == 30 && 人偶 == false)
			{
				if (是否在BOSS房() == true)
					continue;
				人偶 = true;
				人偶call();
				Sleep(1000);
			}
			if (time > 30 && time % 5 == 0)
				过图();
			if (地图初始化 == false)
			{
				地图初始化 = true;
				透明评分 = false;
				获取走法();
				Sleep(1000);
			}
			if (是否在BOSS房() == true)
			{
				地图初始化 = false;
				time = 0;
				人偶 = false;
				透明评分 = false;
				拾取();
				聚物();
				按键(VK_ESCAPE);
			}
			else if (是否开门() == true)
			{
				if (透明评分 == false)
				{
					if (是否在BOSS房() == false)
					{
						透明();
						透明评分 = true;
						Sleep(500);
						评分();
						Sleep(500);
					}
				}
				拾取();
				聚物();
				拾取();
				Sleep(300);
				拾取();
				过图();
				Sleep(1000);
			}
			拾取();
			拾取();
			键 = 技能键[(cnt*cnt) % 12];
			按键(键);
			按键(VK_SPACE);
			按键(VK_SPACE);
			if (cnt % 100 == 0)
			{
				cnt = cnt * cnt % 4567;
			}

		}
		else if (特效 == false)
		{
			特效 = true;
			特效Buff();
		}
		else
		{
			地图初始化 = false;
			透明评分 = false;
			time = 0;
			人偶 = false;
			if (cnt % 100 == 0)
			{
				cnt = cnt * cnt * cnt % 1234;
			}
			if (flag == 999999 + 1)
			{
				for (int i = 0; i < 5; i++)
				{
					MessageBeep(MB_ICONHAND);
					Sleep(500);
				}
				MessageBox(NULL, L"任务", L"任务", MB_OK);
			}
			flag++;
			Sleep(3000);
		}
		cnt++;
	}
	return 0;
}

HANDLE 主线程 = NULL;
BOOL 主线程开关 = FALSE;
VOID MAIN()
{
	if (主线程开关 == FALSE)
	{
		主线程开关 = TRUE;
		系统公告(L"自动开启");
		自身全屏攻击();
		主线程 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AllThread, NULL, 0, NULL);
		SetThreadPriority(主线程, THREAD_PRIORITY_IDLE);
		//挂机线程 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)自动挂机, NULL, 0, NULL);
		//SetThreadPriority(挂机线程, THREAD_PRIORITY_IDLE);
	}
	else
	{
		系统公告(L"关闭自动");
		主线程开关 = TRUE;
		TerminateThread(主线程, 0);
		TerminateThread(挂机线程, 0);
	}
}