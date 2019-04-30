### 介绍

这是在2018年2-3月份学习windows核心编程等相关技术编写的游戏辅助。本仓库旨在介绍游戏辅助与计算机专业课程的种种联系，了解游戏辅助的工作流程以及开发方法。我当时在某个寒假的时候花了一个月学习这方面的技术，感觉很大程度的提升了我的windows编程，以及操作系统，计算机网络，汇编语言等多方面的水平（对后面的专业课打了不错的基础），所以打算将这个东西分享出来，只是作为学习（没有作为商业用途），里面的代码有很多中文函数和变量（业界流行用中文），方便阅读，现在也没有做这方面的研究了。

这方面涉及到的技术：

- windows API Hook ：主要是应用层API Hook，如hook send发包函数；如果是做游戏保护方面，需要懂驱动层Hook，例如SSDT，Shadow SSDT，IDT Hook等等。 这方面很重要，很多windows应用程序就是靠API Hook工作的，例如杀毒软件，病毒，以及游戏保护系统
- windows 模块注入 ：将你编译的dll模块注入到游戏应用程序中，这方面的技术很多，有修改PE文件的方法，远程线程注入（一般游戏在Hook CreateRemoteThread函数，所以注入失败），劫持注入，输入法注入等。比较容易使用的是劫持注入和输入法注入。
- 软件逆向 ： 使用olldgb等调试器调试游戏会得到汇编代码，一般需要动态调试游戏，在某些关键的语句中下断点，理清游戏的逻辑，这方面比较的困难（不过一般的计算机专业学生都有一定的汇编能力），可以说所以的功能都从这里来的。
- 熟悉 windows 编程中常用的api，以及懂得如何编译dll（对编译重定位链接这些流程要熟悉）。这方面可以看《windows核心编程》这本书，一开始感觉有点难以编写，后面就慢慢习惯了。。

游戏辅助的3种方法：
 - 指定地址内存值修改(例如 `*(int*)(0x1234567) = 12345` )
 - 指定地址函数call调用（汇编实现)
 - 发包收包(Hook send)
 
 仓库的代码结构
  - DNFHelper ：编译出dll的源码，是功能实现的核心
  - InjectDll ：将dll注入到exe进程的实现（使用输入法注入）,是一个简单的MFC桌面应用
  - HelperStart ：在无图形界面下将dll注入到exe进程里，并做一些简单的处理。

### 功能概况

当时实现了很多功能，留了几张截图

- 召唤人偶

  用人偶来帮你打伤害

  ![20180203210201](./pic/20180203210201.jpg)

  

- 面板（攻击力）提升

   例如修改自身的力量，把自身物攻提到21w，开启这个功能很容易造成游戏数据异常。。
   ![](./pic/20180207203719.jpg)


- 召唤高达

  这个有点像是个人偶，不过人物可以坐在里面操控，攻击力很高
  ![](./pic/20180203135449.jpg)

  

- 自动攻击
  这个是最常用的，让怪物附近自动生成技能然后攻击怪物，攻击的血量是可以指定的，如果血量足够高就是全屏秒杀了。

  ![F7DEE10C-83FE-406E-8954-58A366E8FF49-520-00000046E47E4ADE_tmp](./pic/F7DEE10C-83FE-406E-8954-58A366E8FF49-520-00000046E47E4ADE_tmp.jpg)

  

- 人物透明，3S评分

  ![IMG_0300](./pic/IMG_0300.JPG)

  

- 加buff
  可以给自己加大量的buff让自己变得更强

  ![IMG_0318](./pic/IMG_0318.jpg)



- 自动过图，自动做任务，自动捡物，聚怪，符咒等等

  可以做成一整套全自动的系统，自动升级，自动搬砖，没疲劳了自动换号等。这里录了一个1分钟的视频放在了根目录下（由于用手机录的视频有点大，压缩成一个mp4格式的，不到3MB）



### 相关知识解答

由于现在windows下的游戏绝大多数都是x86的，所以下面只讨论32位方面的问题，在64位下会不太一样。

#### 1. 什么是基址？

这个概念是最常被提起的，也是最重要的。所谓基址，就是程序中某些变量的地址，这些地址是恒定不变的（在游戏被更新之前）。为什么是基址不会改变呢？可以运行下面这个简单的实例，

```cpp
#include <stdio.h>

int a = 123;

int main() {
	printf("%X", &a);	
} 
```

多次运行之后你会发现，输出的值都是不变的，因为a的地址是不变的（即使将 `int a = 123` 写到函数体内，输出的值也是恒定的）。

- `int a = 123` 写在main外部：a是存储在静态存储区的（.data段），在程序编译阶段已经就确定了存储的位置(即段内偏移量是不会变的，除非程序改动后重新编译)
- `int a = 123` 写在main里面，a的地址不变的原因：a存储在栈区，栈指针esp在编译阶段已经可以确定是如何移动的（esp初始位置也是确定的），因此a的地址不变（这里只考虑单线程程序，多线程由于有独立栈空间，输出的结果会有变化）

游戏里面的基址通常是一个指针，通过基址+偏移量的方法，定位出关键数据的位置，举个例子

```cpp
#include <stdio.h>

struct people {
	char* name;
	int age;
	int power; //攻击力	
    // ...
};
 
people *instance; // 人物指针

int main() {
	instance = (people*)malloc(sizeof(people));
	// ... 后面对任务进行初始化等 
} 
```

由于基址是不变的，所以instance这个变量的地址其实在编译阶段就已经确定了，假设我们通过某种方法获取到instance的地址0x12345678。我们可以通过 `*(int*)(0x12345678)` 获取到people实际的存储地址，如果我们想要修改人物的攻击力，可以通过`*(int*)(*(int*)(0x12345678) + 8) = 12345678` 这里8为人物攻击力的偏移量，得到该地址后取指针就可以得到人物攻击力的真实存储地址了。

游戏中的结构体一般都比较复杂，有多级偏移，一般就是 `[[[[基址]+偏移1]+偏移2]+偏移3] ... ` 这种模式([]为去变量的地址)

**如果想了解更多，可以去看虚拟地址，操作系统的分段和分页机制，加深对C语言的指针的理解**



#### 2. 什么是模块注入？

简单理解：就是在正常运行的程序中注入你编写的模块（dll后缀名），然后让正常运行的程序执行你模块中的代码。通常模块注入后会在模块入口中创建一个后台的工作线程，线程可以共享进程内的全局变量，而且分配和读写内存也不会受到种种限制。

模块注入成功是一切的基础。模块注入的方法很多，大家在网上可以寻找相关的资料学习。我在仓库中提供了我比较常用的输入法注入的源码，稳定性还可以。

远程线程注入：进程A对进程B创建一个线程（进程A拥有权限的情况下），然后在线程调用了LoadLibrary函数，使得模块被加载（这里分享一段远程线程注入的代码，核心函数RemoteInject）

```cpp
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <winsock.h>
#include <string.h>
#include <io.h>
#pragma comment(lib, "ws2_32.lib")

//将单字节char*转化为宽字节wchar_t*  
wchar_t* AnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}

//将宽字节wchar_t*转化为单字节char*  
inline char* UnicodeToAnsi(const wchar_t* szStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	return pResult;
}


BOOL Is64BitOS()
{
	typedef VOID(WINAPI *LPFN_GetNativeSystemInfo)(__out LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandleW(L"kernel32"), "GetNativeSystemInfo");
	if (fnGetNativeSystemInfo)
	{
		SYSTEM_INFO stInfo = { 0 };
		fnGetNativeSystemInfo(&stInfo);
		if (stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64
			|| stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Is64BitPorcess(HANDLE hProcess)
{
	if (!Is64BitOS())
		return FALSE;
	else
	{
		if (hProcess)
		{
			typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
			LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleW(L"kernel32"), "IsWow64Process");
			if (NULL != fnIsWow64Process)
			{
				BOOL bIsWow64 = FALSE;
				fnIsWow64Process(hProcess, &bIsWow64);
				if (bIsWow64)
					return FALSE;
				else
					return TRUE;
			}
		}
	}
	return FALSE;
}

void EnableDebugPriv()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

	CloseHandle(hToken);
}

VOID EnumProcess()
{
	EnableDebugPriv();
	DWORD pid = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);
	printf("            进程名称   进程ID    父进程ID  32/64位\n");
	if (Process32First(snapshot, &process))
	{
		do
		{
			printf("%20.20ls    %5d       %5d  ", process.szExeFile, process.th32ProcessID, process.th32ParentProcessID);
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process.th32ProcessID);
			if (Is64BitPorcess(hProcess))
				printf("   64位\n");
			else
				printf("   32位\n");
			CloseHandle(hProcess);
		} while (Process32Next(snapshot, &process));
	}
	CloseHandle(snapshot);
}

HANDLE GetProcessByName(LPCSTR name)
{
	EnableDebugPriv();
	DWORD pid = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);
	if (Process32First(snapshot, &process))
	{
		do
		{
			if (stricmp(UnicodeToAnsi(process.szExeFile), name) == 0)
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}
	CloseHandle(snapshot);

	if (pid != 0)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		return hProcess;
	}
	return NULL;

}

HANDLE GetProcessByWindowName(LPWSTR WindowsName)
{
	HANDLE hProcess = NULL;
	DWORD pid, ERRO;
	HWND dnf = ::FindWindow(NULL, WindowsName);
	GetWindowThreadProcessId(dnf, &pid);
	EnableDebugPriv();
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL)
	{
		printf("找不到进程!\n");
		return 0;
	}
	return hProcess;
}

BOOL RemoteInject(HANDLE hProcess, LPCSTR szLibPath)
{
	HANDLE hThread;
	HMODULE modHandle = GetModuleHandle(_T("Kernel32"));
	DWORD dwsize = (strlen(szLibPath) + 1)*sizeof(CHAR);
	LPVOID pLibRemote = VirtualAllocEx(hProcess, NULL, dwsize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProcess, pLibRemote, (LPVOID)szLibPath, dwsize, NULL);
	if (GetLastError()) return FALSE;
	LPTHREAD_START_ROUTINE func = (LPTHREAD_START_ROUTINE)GetProcAddress(modHandle, "LoadLibraryA");
	DWORD tid;
	hThread = CreateRemoteThread(hProcess, NULL, 0,
		func,
		pLibRemote,
		0,
		&tid);
	if (GetLastError()) return FALSE;
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, pLibRemote, dwsize, MEM_DECOMMIT);
	return TRUE;
}

void usage()
{
	printf("功能1：枚举系统中的进程\n");
	printf("inject -e\n\n");
	printf("功能2：向指定进程注入代码\n");
	printf("inject {exe} {dll}\n");
	printf("inject -pid {pid} {dll}\n");
	printf("例如: inject.exe chrome.exe mydll.dll\n");
	printf("注意exe版本和dll版本以及注入的程序的版本要一致\n");
	exit(0);
}

int main(int argc, char* argv[])
{
	CHAR szLibPath[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, szLibPath);
	CHAR szDllName[MAX_PATH] = { 0 };
	HANDLE hProcess = NULL;
	if (argc == 2)
	{
		if (strcmp(argv[1], "-e"))
			usage;
		EnumProcess();
		return 0;
	}
	if (argc == 4)
	{
		if (strcmp(argv[1], "-pid"))
			usage();
		else
		{
			DWORD pid = atoi(argv[2]);
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		}
		strcpy(szDllName, argv[3]);
	}
	if (argc == 3)
	{
		strcpy(szDllName, argv[2]);
		hProcess = GetProcessByName(argv[1]);
	}
	if (argc != 2 && argc != 4 && argc != 3) 
		usage();
	if (hProcess == NULL)
	{
		printf("找不到进程!\n");
		return 0;
	}
	BOOL Is64_1 = Is64BitPorcess(hProcess);
	BOOL Is64_2 = Is64BitPorcess(GetCurrentProcess());
	if (Is64_1 != Is64_2)
	{
		if (Is64_1)
			printf("请运行64位版本\n");
		else
			printf("请运行32位版本\n");
		return 0;
	}
	strcpy(szLibPath + strlen(szLibPath), "\\");
	strcpy(szLibPath + strlen(szLibPath), szDllName);
	if (_access(szLibPath, 0))
	{
		printf("%s : dll不存在\n", szLibPath);
		return 0;
	}
	if (!RemoteInject(hProcess, szLibPath))
		printf("注入失败\n");
	else
		printf("注入成功\n");
	return 0;
}

```



#### 3. 什么是API Hook?

简单理解：假设我现在Hook了A函数，当其他线程调用了A函数，但实际上确是执行了B函数，而调用者却完全不知情。

例如

```cpp
int A() {
    // 这一个是正常的函数，一般是游戏开发者编写的
	return 1;
}

int B() {
    // 这个是恶意的函数（一般是你模块注入成功后你编写的函数）
	// 通常是先保存A的地址，然后
    return 0;
}

int main() {
    // Hook(A, B); // 在其他线程中，A函数被Hook为B函数  
    int res = A(); // 这里调用者不知情，实际上调用的是B函数
}
```

分享Hook MessageBox弹窗的例子。

```cpp
#include <windows.h>

//定义函数原型: MessageBoxA
typedef int WINAPI OLD_MessageBoxA( __in_opt HWND hWnd, __in_opt LPCSTR lpText, __in_opt LPCSTR lpCaption, __in UINT uType);

DWORD addr1; // 保存源码MeesageBoxA的地址

__declspec(naked)   int __stdcall My_MessageBoxA( __in_opt HWND hWnd, __in_opt LPCSTR lpText, __in_opt LPCSTR lpCaption, __in UINT uType)
{
	__asm
	{
			mov edi, edi
			push ebp
			mov ebp ,esp 
	}	

	lpText="已被HOOK";  // 修改数据
	__asm
	{
		mov eax,addr1
		add eax,5 // 前5字节是我们的长jump指令
		jmp eax   // 跳到MessageBox真正的代码区域
	}			
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD jump_code=0;
	byte  mye9 = 0xe9;
	HMODULE DLLHANDLE= LoadLibraryA("USER32.DLL");
	addr1= (DWORD)(GetProcAddress(DLLHANDLE,"MessageBoxA"));

	//改变内存读写属性 
	VirtualProtect((LPVOID)addr1,10,PAGE_EXECUTE_READWRITE,NULL);
	 	 
	//修改第一字节
	if (WriteProcessMemory(GetCurrentProcess(),(LPVOID)addr1,&mye9,1,NULL)==0)
	{
		printf("写入HOOK数据失败");
	}

	 //修改后4字节 :跳转码  JMP 1234
	 //跳转码=将要跳到的函数-原来的函数地址-5
	 //我们的跳转码=My_MessageBoxA-addr1-5
	 jump_code=(DWORD)&My_MessageBoxA-addr1-5;
	 if (WriteProcessMemory(GetCurrentProcess(),(DWORD*)(addr1+1),&jump_code,4,NULL)==0)
	 {
		 printf("写入HOOK数据失败");
	 }

	::MessageBoxA(0,"HOOK1","提示",0);
	::MessageBoxA(0,"HOOK2","提示",0);
	::MessageBoxA(0,"HOOK3","提示",0);

	 return 0;
}
```

运行之后调用MessageBoxA，窗口中的内容都是"已被Hook"。

这种Hook方法称为Inline Hook，因为在windows api中，如果函数被声明为__stdcall的话，汇编后的前5个字节是这样的

```asm
mov edi, edi  // 2字节
push ebp   // 1字节
mov ebp, esp // 2字节
```

这5个字节刚好可以组成一条长jump指令，只要修改函数代码段即可。



#### 4. 参数入栈方式 

在windows下参数一般都是从右往左入栈的，下面一个简单的程序

```cpp
int fun(int a, float b, int c) {
    return 1;
}

int main() {
    ...
    func(1, 2, 3);
}
```

汇编之后，fun函数通常是这样的。

```cpp
push        ebp  
mov         ebp,esp  // 保存esp寄存器到ebp中

sub         esp,0C0h  // 开辟栈空间

push        ebx  
push        esi  
push        edi  // 保存寄存器

lea         edi,[ebp-0C0h]  
mov         ecx,30h  
mov         eax,0CCCCCCCCh  
rep stos    dword ptr es:[edi]  // 将栈空间初始化

mov         eax,1  // 将返回值设置1，eax为返回值寄存器

pop         edi  
pop         esi  
pop         ebx  
mov         esp,ebp  // 复原esp寄存器
pop         ebp   // 复原ebp寄存器
ret // 回到调用者的位置，相当于 add esp, 4; jmp [esp-4];  
```

调用者方汇编代码通常是这样的

```cpp
push 3
push 2
push 1
call func  // 相当于 push pc; jmp esp
add esp, 0xCh // 因为push了3次，esp降低了12字节,所以需要add回来
```

**想了解更多，可以搜索程序运行时的栈空间布局，例如func函数的第一个变量地址是ebp-8，第二个变量是ebp-12，返回值存储在eax寄存器中**



#### 5. 如何实现花里胡哨的特效（例如怪物自动扣血）？

几乎所有的辅助功能，都是调用游戏开发者写的某个函数的，比如怪物受到技能攻击，我们首先需要通过汇编找到攻击的call地址，然后在我们的dll模块中调用这个call从而实现效果，一般使用汇编来实现。

（这里源码中基本都是中文，vs支持中文）

基本流程：通过遍历取得所有怪物的坐标地址和指针，然后用技能call对怪物进行攻击，使得怪物掉血。

```cpp
void 释放call(int 对象基址, int 代码, int 伤害, int X, int Y, int Z)
{
	_asm
	{
		push Z
		push Y
		push X
		push 伤害
		push 代码
		push 对象基址
		mov eax, 释放Call
		call eax
		add esp, 0x18
	}
}

void 全屏遍历()
{
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
}
```



#### 6. 发包相关

一般都是下send断点，然后通过调用堆栈定位关键函数，这方面需要对网络编程和调试水平非常的熟悉，发包可以实现很多功能，甚至是脱机刷图。。。



































