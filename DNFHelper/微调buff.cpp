#include "stdafx.h"
#include "微调buff.h"

int address = 4198144;
void Buff(int Buff类型)
{
	__asm
	{
		pushad
		push address
		push Buff类型
		mov eax, BUFF参数1
		call eax
		mov ecx, eax
		mov ebx, BUFF参数2
		call ebx
		mov esi, eax
		push Buff类型
		mov ecx, BUFF参数1
		call ecx
		mov ecx, eax
		mov edx, BUFF参数3
		call edx
		mov esi, BUFF参数1
		call esi
		popad
		ret
	}
}

bool 微调开关 = false;
void 微调()
{
	VMBEGIN
		if (微调开关 == false)
		{
			系统公告(L"微调开启");
			微调开关 = true;
			超级加密(read(read(人物基址) + 称号偏移) + 力量偏移, 50000);
		}
		else
		{
			系统公告(L"微调关闭");
			微调开关 = false;
			超级加密(read(read(人物基址) + 称号偏移) + 力量偏移, 0);
		}
	VMEND
}

void 特效call(int buff)
{
	VMBEGIN
	int 特效基地址 = read(特效基址);
	write(0x400600, read(特效基地址 + 特效偏移));
	write(0x400604, 特效基地址);
	int 特效缓冲Call_ = 特效缓冲Call;
	int 特效释放Call_ = 特效释放Call;
	int 特效Call_ = 特效Call;
	int 特效Call1_ = 特效Call1;
	int 特效Call2_ = 特效Call2;
	__asm
	{
		mov ecx, 00400600h
		call 特效缓冲Call_
		push 01h
		mov ecx, 00400604h
		call 特效释放Call_
		push 02h
		mov ecx, 00400604h
		call 特效释放Call_
		push 00400604h
		push buff
		call 特效Call_
		mov ecx, eax
		call 特效Call1_
		mov esi, eax
		push esi
		call 特效Call_
		mov ecx, eax
		call 特效Call2_
		call 特效Call_
	}
	VMEND
}

void 特效Buff()
{
	if (read(人物基址) <= 0)
		return;
	系统公告(L"开启特效Buff");
	WCHAR 特效[200];
	GetPrivateProfileString(L"微调", L"特效代码", L"", 特效, sizeof(特效), filepath);
	std::string* str = UnicodeToString(特效);
	std::vector<std::string> result = split(*str, '|');
	for (size_t i = 0; i < result.size(); i++)
	{
		int 特效代码 = atoi(result[i].c_str());
		特效call(特效代码);
	}
}

