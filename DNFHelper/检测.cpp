#include "stdafx.h"
#include "检测.h"
#include "内存读写.h"

BOOL EnablePriv()
{
	//提升进程权限的方法
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tkp;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);//修改进程权限
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);//通知系统修改进程权限
	}
	return((GetLastError() == ERROR_SUCCESS));
}

DWORD GetProcessIDByName(const WCHAR* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) 
	{
		if (lstrcmp(pe.szExeFile, pName) == 0) 
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}


DWORD 杀进程(LPVOID)
{
	VMBEGIN
	HANDLE hProcess;
	//while (true)
	//{
	//HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TPHelper.exe"));
	//TerminateProcess(hProcess, 0);
	//Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TPSvc.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TP3Helper.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"CrossProxy.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TPWeb.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"GameLoader.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TenioDL.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TenSafe_1.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TenSafe.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"TXPlatform.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"qbclient.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"tgp_gamead.exe"));
	TerminateProcess(hProcess, 0);
	Sleep(1000);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIDByName(L"Ten.exe"));
	TerminateProcess(hProcess, 0);
	return 0;
	VMEND
}

using namespace std;

map<ULONG, string> ModuleAddress;
map<string, vector<ULONG> >ModuleThread;

void MapModuleAddress(const WCHAR* ModuleName, ULONG address)
{
	string* name = UnicodeToString(ModuleName);
	transform(name->begin(), name->end(), name->begin(), (int(*)(int))tolower);
	ModuleAddress[address] = *name;
}

void MapModuleThread(ULONG tid, ULONG address)
{
	map<ULONG, string>::iterator it = ModuleAddress.begin(), it2;
	for (; it != ModuleAddress.end(); it++)
	{
		it++;
		it2 = it;
		it--;
		if (it2 == ModuleAddress.end() || (it->first <= address && address <= it2->first))
		{
			ModuleThread[it->second].push_back(tid);
			return;
		}
	}
}

void EnumModule(DWORD pid)
{
	MODULEENTRY32 Mod32;

	Mod32.dwSize = sizeof(Mod32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

	if (hSnap == INVALID_HANDLE_VALUE)
	{
		系统公告(L"无法打开该程序模块！\n");
		CloseHandle(hSnap);
		return;
	}

	BOOL bModule = Module32First(hSnap, &Mod32);
	while (bModule)
	{
		if (Mod32.th32ProcessID == pid)
		{
			MapModuleAddress(Mod32.szModule, (ULONG)Mod32.modBaseAddr);
		}
		bModule = Module32Next(hSnap, &Mod32);
	}

	CloseHandle(hSnap);
}


DWORD WINAPI GetThreadStartAddress(DWORD tid)
{
	HANDLE hThread = OpenThread(PROCESS_ALL_ACCESS, FALSE, tid);
	NTSTATUS ntStatus;
	HANDLE hDupHandle;
	DWORD dwStartAddress;

	NTQUERYINFORMATIONTHREAD NtQueryInformationThread = (NTQUERYINFORMATIONTHREAD)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationThread");
	if (NtQueryInformationThread == NULL)
		return 0;

	HANDLE hCurrentProcess = GetCurrentProcess();
	if (!DuplicateHandle(hCurrentProcess, hThread, hCurrentProcess, &hDupHandle, THREAD_QUERY_INFORMATION, FALSE, 0))
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return 0;
	}
	ntStatus = NtQueryInformationThread(hDupHandle, ThreadQuerySetWin32StartAddress, &dwStartAddress, sizeof(DWORD), NULL);
	CloseHandle(hDupHandle);

	if (ntStatus != STATUS_SUCCESS)
		return 0;

	return dwStartAddress;
}

BOOL EnumThread(DWORD dwOwnerPID)
{
	HANDLE        hThreadSnap = NULL;
	BOOL          bRet = FALSE;
	THREADENTRY32 te32 = { 0 };


	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwOwnerPID);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return (FALSE);

	te32.dwSize = sizeof(THREADENTRY32);

	if (Thread32First(hThreadSnap, &te32))
	{
		do
		{
			if (te32.th32OwnerProcessID == dwOwnerPID)
			{
				ULONG address = GetThreadStartAddress(te32.th32ThreadID);
				MapModuleThread(te32.th32ThreadID, address);
			}
		} while (Thread32Next(hThreadSnap, &te32));
		bRet = TRUE;
	}
	else
		bRet = FALSE;         
	CloseHandle(hThreadSnap);

	return (bRet);
}

void CloseModuleThread(string Module)
{
	transform(Module.begin(), Module.end(), Module.begin(), (int(*)(int))tolower);
	HANDLE hThread;
	for (size_t i = 0; i < ModuleThread[Module].size(); i++)
	{
		hThread = OpenThread(THREAD_TERMINATE, FALSE, ModuleThread[Module][i]);
		//SuspendThread(hThread);
		//SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST);
		TerminateThread(hThread, 0);
	}
}


DWORD 结束线程(LPVOID)
{
	DWORD pid;
	pid = GetCurrentProcessId();
	while (true)
	{
		处理事件();
		ModuleThread.clear();
		ModuleAddress.clear();
		EnumModule(pid);
		EnumThread(pid);
		//CloseModuleThread("tcj.dll");
		CloseModuleThread("TerSafe.dll");
		CloseModuleThread("TenCaptcha.dll");
		CloseModuleThread("TCJHelper.dll");
		CloseModuleThread("TenSLX.dll");
		CloseModuleThread("TenRPCS.dll");
		CloseModuleThread("SecureCheck.dll");
		CloseModuleThread("GameRpcs.dll");
		CloseModuleThread("GameDataPlatformServer.dll");
		//CloseModuleThread("BugTrack.dll");
		//CloseModuleThread("SSOPlatform.dll");
		//CloseModuleThread("CrossShell.dll"); 
		//CloseModuleThread("policyprobe.dll");
		//CloseModuleThread("CRYPT32.dll");
		Sleep(1000*30);
	}
}
HANDLE 结束线程_ = NULL;

void 检测()
{
	VMBEGIN
	EnablePriv();
	//杀进程(NULL);
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)杀进程线程, NULL, 0, NULL);
	if(结束线程_ == NULL)
		结束线程_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)结束线程, NULL, 0, NULL);
    VMEND
}