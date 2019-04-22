#include "stdafx.h"
#include "激活.h"
#include "测试.h"

void 消息循环()
{
	VMBEGIN
	MSG msg = { 0 };
	RegisterHotKey(NULL, 1, 0, VK_TAB);
	RegisterHotKey(NULL, 1, 0, VK_HOME);
	RegisterHotKey(NULL, 1, 0, VK_F1);
	RegisterHotKey(NULL, 1, 0, VK_F2);
	RegisterHotKey(NULL, 1, 0, VK_F3);
	RegisterHotKey(NULL, 1, 0, VK_F4);
	RegisterHotKey(NULL, 1, 0, VK_F5);
	RegisterHotKey(NULL, 1, 0, VK_F6);
	RegisterHotKey(NULL, 1, 0, VK_F7);
	RegisterHotKey(NULL, 1, 0, VK_F10);
	RegisterHotKey(NULL, 1, 0, VK_F11);
	
	RegisterHotKey(NULL, 1, 0, VK_END);
	RegisterHotKey(NULL, 1, MOD_ALT, VK_UP);
	RegisterHotKey(NULL, 1, MOD_ALT, VK_DOWN);
	RegisterHotKey(NULL, 1, MOD_ALT, VK_LEFT);
	RegisterHotKey(NULL, 1, MOD_ALT, VK_RIGHT);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_HOTKEY == msg.message)
		{
			switch (msg.wParam)
			{
			case 1:
				switch (LOWORD(msg.lParam))
				{
				case 0:
					switch (HIWORD(msg.lParam))
					{
					case VK_HOME:
						test();
						/*系统公告(L"F1 全屏攻击");
						系统公告(L"F2 自动透明开关");
						系统公告(L"F3 召唤怪物");
						系统公告(L"F4 召唤人偶");
						系统公告(L"F5 技能无CD");
						系统公告(L"F6 微调");
						系统公告(L"F11 特效Buff");
						系统公告(L"F12 全屏秒杀");
						系统公告(L"Tab 吸怪聚物");*/
						break;
					case VK_TAB:
						吸怪聚物();
						break;
					case VK_F1:
						自身全屏攻击();
						break;
					case VK_F2:
						自动透明();
						break;
					case VK_F3:
						怪物call();
						break;
					case VK_F4:
						人偶call();
						break;
					case VK_F5:
						无CD();
						break;
					case VK_F6:
						微调();
						break;
					case VK_F7:
						自动();
						break;
					case VK_F11:
						特效Buff();
						break;
					case VK_F10:
						全屏秒杀();
						break;
					case VK_END:
						break;
					}
					break;
				case MOD_ALT:
					switch (HIWORD(msg.lParam))
					{
					case VK_UP:
						坐标过图(2);
						break;
					case VK_DOWN:
						坐标过图(3);
						break;
					case VK_LEFT:
						坐标过图(0);
						break;
					case VK_RIGHT:
						坐标过图(1);
						break;
					}
					break;
				}
				break;
			}
		}
		Sleep(1000);
	}
	VMEND
}



void 激活()
{
	VMBEGIN
	初始化配置();
	系统公告(L"载入成功！");
	自动();
	//自动透明();
	自身全屏攻击();
	//检测();
	if (FindWindow(NULL, L"地下城与勇士") == NULL)
		return;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)消息循环, NULL, 0, NULL);
	SetThreadPriority(hThread, THREAD_PRIORITY_IDLE);
	//MAIN();
	VMEND
}