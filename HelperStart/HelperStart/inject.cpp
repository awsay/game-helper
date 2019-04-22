#include "stdafx.h"
#include "inject.h"
#include <iostream>

using namespace std;

MyClass::MyClass(void)
{
	HKL InputHandle = NULL;

	HKL oldInputHandle = NULL;
}

MyClass::~MyClass(void)
{
}

typedef int  (WINAPI  *g_IMEClearPubString)();

typedef int  (WINAPI  *g_IMESetPubString)(PWCHAR tmpStr,DWORD UnloadDLL,DWORD loadNextIme,DWORD DllData1,DWORD DllData2,DWORD DllData3);

g_IMEClearPubString IMEClearPubString = NULL;

g_IMESetPubString IMESetPubString = NULL;

HINSTANCE hModule = NULL;
bool MyClass::_FreeResFile(DWORD dwResName, LPCWSTR lpResType, LPCWSTR lpFilePathName)
{
	HMODULE hInstance = ::GetModuleHandle(NULL);//得到自身实例句柄          
	HRSRC hResID = ::FindResource(hInstance,MAKEINTRESOURCE(dwResName),lpResType);//查找资源      
	HGLOBAL hRes = ::LoadResource(hInstance,hResID);//加载资源       
	LPVOID pRes = ::LockResource(hRes);//锁定资源                   
	if (pRes == NULL)//锁定失败       
	{           return FALSE;       
	}      
	DWORD dwResSize = ::SizeofResource(hInstance,hResID);//得到待释放资源文件大小       
	HANDLE hResFile = CreateFile(lpFilePathName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);//创建文件                  
	if (INVALID_HANDLE_VALUE == hResFile)     
	{                 
		return FALSE;       
	}              
	DWORD dwWritten = 0;//写入文件的大小          
	WriteFile(hResFile,pRes,dwResSize,&dwWritten,NULL);//写入文件      
	CloseHandle(hResFile);//关闭文件句柄                   
	return (dwResSize == dwWritten);//若写入大小等于文件大小，返回成功，否则失败 
	//使用示例: FreeResFile(IDR_MYRES,"MYRES","D:\\1.exe");
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//模块功能：释放资源中某类型的文件  ReleaseRes("Test.exe",(WORD)IDR_EXE1,"EXE"))
//返 回 值：成功返回TRUE，否则返回FALSE 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//参数说明：参数名			|   输入/输出  |        参数说明
//          strFileName     |"Test.exe"
//          wResID          |(WORD)IDR_EXE1
//          strFileType     |"EXE"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BOOL MyClass:: _FreeResFile2(string strFileName,WORD wResID,string strFileType)    
{    
	// 资源大小    
	DWORD   dwWrite=0;  

	//定位我们的自定义资源，这里因为我们是从本模块定位资源，所以将句柄简单地置为NULL即可

	HRSRC hrscr =  FindResourceA(NULL, MAKEINTRESOURCEA(wResID),strFileType.c_str());  

	if (NULL == hrscr)
	{
		return FALSE;
	}

	//获取资源的大小

	DWORD dwSize = SizeofResource( NULL,hrscr); 

	//加载资源

	HGLOBAL hGlobal = LoadResource(NULL,hrscr);   

	if (NULL == hGlobal)
	{
		return FALSE;
	}

	//锁定资源

	LPVOID pBuffer = LockResource(hGlobal); 

	if (NULL == pBuffer)
	{
		return FALSE;
	}

	// 创建文件    
	HANDLE  hFile = CreateFileA(
		strFileName.c_str(), 
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		); 

	if ( hFile == INVALID_HANDLE_VALUE )    
	{    
		return FALSE;    
	}    

	// 写入文件    
	WriteFile(hFile,pBuffer,dwSize,&dwWrite,NULL);   

	CloseHandle( hFile ); 

	return TRUE;    
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：取运行目录 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void MyClass::GetCurDirectory(WCHAR* CurPath)
{
	GetModuleFileName (NULL,CurPath,MAX_PATH);

	for(int i=MAX_PATH-1;i>=0;i--)
	{
		if(*(CurPath+i)!='\\')
		{
			*(CurPath+i)=0;
		}
		else
		{
			break;
		}
	}
} 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：枚举输入法，返回句柄  InputName：输入法名称，FileName 输入法文件名
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
HKL MyClass::EnumIme(string InputName,string FileName)
{
	HKL *HKLList;

	HKL Imehandle = NULL;

	int StrCount=0,FileCount=0;

	HKLList = NULL;

	UINT uCount = GetKeyboardLayoutList(0, NULL);

	if (0 != uCount)
	{
		HKLList = new HKL[uCount];

		//获得与系统中输入点的当前集相对应的键盘布局句柄。该函数将句柄拷贝到指定的缓冲区中
		GetKeyboardLayoutList(uCount, HKLList);
	}
	else
	{
		int nErr = GetLastError();

	}

	char strLayoutText[256];

	char strFileText[256];

	string InputString;

	string InputFile;

	cout << uCount << endl;

	for (UINT i = 0; i < uCount; i++)
	{
		//取得输入法名
		StrCount = ImmGetDescriptionA(HKLList[i], strLayoutText, 256);

		InputString = strLayoutText;
		cout << InputString << endl;

		if (InputString == InputName)
		{
			cout << "Same" << endl;
			//得到该输入法的文件名称，如果名称相同，返回输入法句柄。
			FileCount = ImmGetIMEFileNameA(HKLList[i], strFileText, 256);

			InputFile = strFileText;

			if (InputFile == FileName)
			{
				Imehandle = HKLList[i];

				break;
			}
		}	
	}
	delete[]HKLList;

	return Imehandle;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：获取输入法句柄字符串
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void MyClass::GetImeHandleString()
{
	//获得指定线程的活动键盘布局
	HKL iHandle = GetKeyboardLayout(NULL);

	//激活新输入法键盘布局
	::ActivateKeyboardLayout(InputHandle,NULL);
	//获取输入法键盘布局
	::GetKeyboardLayoutName(ImeSymbol);
	//激活原来活动键盘布局
	::ActivateKeyboardLayout (iHandle,NULL);
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：输入法注入 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
bool MyClass::ImeInstall(LPCWSTR lpszdllName)
{

	//保存原始键盘布局

	::SystemParametersInfo(SPI_GETDEFAULTINPUTLANG,NULL,oldInputHandle,NULL);

	//复制文件到目录

	//不需要拷贝  资源自给 CopyFile(L"Freeime.dll",L"C:\\WINDOWS\\SYSTEM32\\Freeime.ime",FALSE);

	//CopyFile(L"Gamedll.dll",L"C:\\WINDOWS\\SYSTEM32\\Gamedll.dll",FALSE);

	//加载输入法IME文件,必须在前面加载，否则会造成DLL共享变量错误。

	hModule = LoadLibrary(L"C:\\Windows\\System32\\Freeime.ime"); 
	
	//加载输入法

	InputHandle = ImmInstallIME(L"C:\\WINDOWS\\SYSTEM32\\Freeime.ime",L"极品五笔.12");
	
	//获取输入法标识符

	GetImeHandleString();

	//判断输入法句柄是否有效

	if (!ImmIsIME(InputHandle))
	{
		//句柄不存在，枚举输入法查找句柄
		InputHandle = EnumIme("紫光华宇拼音输入法V6.7","C:\\WINDOWS\\SYSTEM32\\unispim6.ime");
	}
	if (hModule)
	{
		IMESetPubString = (g_IMESetPubString)GetProcAddress(hModule, "IMESetPubString");
		if (IMESetPubString)
		{
			/*WCHAR CurPath[MAX_PATH+1] = {NULL};

			wmemset(CurPath,0,MAX_PATH+1);

			GetCurDirectory(CurPath);

			wcscat(CurPath,lpszdllName);*/

			IMESetPubString((PWCHAR)lpszdllName, 0, 0, 0, 0, 0);
		}
		else
		{
			FreeLibrary(hModule);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：激活指定窗口输入法
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BOOL MyClass::IMEActiv(HWND hWnd)
{

	if (InputHandle != NULL)
	{
		//激活指定窗口输入法
		::PostMessage(hWnd,WM_INPUTLANGCHANGEREQUEST,0x1,(LPARAM)InputHandle);
	} 
	else
	{
		//句柄不存在，枚举输入法句柄，激活
		HKL imehandle = EnumIme("极品五笔7.12","C:\\WINDOWS\\SYSTEM32\\Freeime.ime");
		if (imehandle != NULL)
		{
			::PostMessage(hWnd,WM_INPUTLANGCHANGEREQUEST,0x1,(LPARAM)imehandle);
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：停止输入法
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BOOL MyClass:: ImeStop()
{
	if (hModule)
	{
		IMEClearPubString = (g_IMEClearPubString)GetProcAddress(hModule,"IMEClearPubString"); 

		if (IMEClearPubString)
		{
			IMEClearPubString();
			FreeLibrary(hModule);
		}
		else
		{
			FreeLibrary(hModule);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：还原系统默认输入法，用于程序退出时还原对默认键盘布局的改变.Imehandle：输入法句柄
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void MyClass:: QuitImeSystemDefault(HKL imehandle)
{
	HKL retV = NULL;

	::SystemParametersInfo(SPI_GETDEFAULTINPUTLANG,0,retV,0);

	if (retV != imehandle)
	{
		::SystemParametersInfo(SPI_SETDEFAULTINPUTLANG,0,imehandle,SPIF_SENDWININICHANGE);
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：在所有的顶级窗口中激活指定句柄的输入法
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void MyClass:: SetAllWindowActiv(HKL imehandle)
{
	HWND mhWnd = NULL;

	do 
	{
		mhWnd = ::FindWindowExA(NULL,mhWnd,NULL,NULL);

		if (mhWnd != NULL)
		{
			::PostMessage(mhWnd,WM_INPUTLANGCHANGEREQUEST,0x1,(LPARAM)imehandle);
		}
	} while (mhWnd == NULL);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：卸载输入法
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BOOL MyClass:: ImeUnInstall()
{
	HKL handle;

	//恢复键盘原始布局
	QuitImeSystemDefault(oldInputHandle);

	//在所有顶级窗口激活原始键盘布局
	SetAllWindowActiv(oldInputHandle);

	if (InputHandle != NULL)
	{
		//卸载输入法
		UnloadKeyboardLayout(InputHandle);
	}
	else
	{
		//句柄不存在枚举输入法查找句柄
		HKL imehandle = EnumIme("极品五笔7.12","C:\\WINDOWS\\SYSTEM32\\Freeime.ime");

		if (imehandle != NULL)
		{
			UnloadKeyboardLayout(imehandle);
		} 
		else
		{
			return FALSE;
		}
	} 

	//输入法标识符存在，删除输入法注册表
	if (ImeSymbol != L"")
	{
		ImedeleteReg();
	}	

	//卸载输入法模块
	FreeLibrary(hModule);

	return TRUE;
}
//@//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//功能：删除输入法注册表,完美。
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void MyClass::ImedeleteReg()
{
	HKEY phkResult;

	int i = NULL;

	TCHAR ValueName[255];  //存储得到的键值名

	TCHAR lpData[255];     //存储得到的键值数据

	DWORD lenValue = 255;  //存储键值的数据长度

	DWORD lenData = 255;   //存储键值的数据长度

	//打开注册表项目，获取句柄
	RegOpenKey(HKEY_CURRENT_USER,L"Keyboard Layout\\Preload",&phkResult);

	//枚举注册表项目
	while(RegEnumValue (phkResult,i,ValueName,&lenValue,NULL,NULL,(LPBYTE)lpData,&lenData) != ERROR_NO_MORE_ITEMS)
	{
		//Dbg_Print("1:%ws\n",ImeSymbol);//输入法标识符

		//Dbg_Print("2:%ws\n",ValueName);//注册表项目

		//Dbg_Print("3:%ws\n",lpData);   //注册表项目数值

		if (lenData != NULL)
		{
			if (_tcscmp(ImeSymbol,lpData)==NULL)
			{
				//AfxMessageBox(_T("删除注册表。。"));
				RegDeleteValue (phkResult,ValueName);//删除项目数值
				break;
			}
		}

		//必须重新设置变量长度与变量数据为空，否则会造成数据不全 如：E0200804 不全为：0000804
		memset(lpData, 0, sizeof(lpData));

		memset(ValueName, 0, sizeof(ValueName));

		lenValue = 255; 

		lenData = 255;

		i++;
	}
}

void MyClass:: Stop_AND_UnInstall()
{
	ImeStop();
	ImeUnInstall();
}