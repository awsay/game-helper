#pragma once
#include <Windows.h>
#include <imm.h>
#include <string>
#pragma comment(lib,"IMM32.lib")

using namespace std;

class MyClass
{
public:
	HKL InputHandle;

	HKL oldInputHandle;

	TCHAR ImeSymbol[256];

	MyClass(void);
	~MyClass(void);
	bool _FreeResFile(DWORD dwResName, LPCWSTR lpResType, LPCWSTR lpFilePathName);
	BOOL _FreeResFile2(string strFileName, WORD wResID, string strFileType);
	bool ImeInstall(LPCWSTR );
	//参数：需要输入的DLL全路径
	void	GetCurDirectory(WCHAR* CurPath);//取当前运行目录
	HKL EnumIme(string InputName, string FileName);//枚举输入法 返回输入法句柄
	void GetImeHandleString();//功能：获取输入法句柄字符串
	BOOL IMEActiv(HWND hWnd);//激活指定窗口的输入法
	BOOL ImeStop();//停止输入法
	BOOL ImeUnInstall();//卸载输入法
	void QuitImeSystemDefault(HKL imehandle);///功能：还原系统默认输入法，用于程序退出时还原对默认键盘布局的改变.Imehandle：输入法句柄
	void SetAllWindowActiv(HKL imehandle);//功能：在所有的顶级窗口中激活指定句柄的输入法
	void ImedeleteReg();//删除输入法在注册表的痕迹
	void Stop_AND_UnInstall();

};
