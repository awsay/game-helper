#pragma once
#include <imm.h>
#pragma comment(lib,"IMM32.lib")
#pragma warning( disable: 4005 )
#pragma warning( disable: 4102 )
#pragma warning( disable: 4101 )
#pragma warning( disable: 4305 )
#pragma warning( disable: 4312 )
#pragma warning( disable: 4311 )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4267 )
#pragma warning( disable: 4996 )
#pragma warning( disable: 4533 )
#pragma warning( disable: 4700 )

class InjectClass
{
public:
	HKL InputHandle ;

	HKL oldInputHandle ;
	
	TCHAR ImeSymbol[255];

	InjectClass(void);
	
	~InjectClass(void);
	bool _FreeResFile(DWORD dwResName, LPCWSTR lpResType, LPCWSTR lpFilePathName);

	BOOL _FreeResFile2(CString strFileName,WORD wResID,CString strFileType);    

	bool ImeInstall(LPCWSTR /*lpszdllName[]*/);

	//参数：需要输入的DLL全路径
	void	GetCurDirectory(WCHAR* CurPath);//取当前运行目录

	HKL EnumIme(CString InputName,CString FileName);//枚举输入法 返回输入法句柄

	void GetImeHandleString();//功能：获取输入法句柄字符串

	BOOL IMEActiv(HWND hWnd);//激活指定窗口的输入法

	BOOL ImeStop();//停止输入法

	BOOL ImeUnInstall();//卸载输入法

	void QuitImeSystemDefault(HKL imehandle);///功能：还原系统默认输入法，用于程序退出时还原对默认键盘布局的改变.Imehandle：输入法句柄
	
	void SetAllWindowActiv(HKL imehandle);//功能：在所有的顶级窗口中激活指定句柄的输入法
	
	void ImedeleteReg();//删除输入法在注册表的痕迹
	
	void Stop_AND_UnInstall();
	
	void  Test(CString MSG);

};
