#include "stdafx.h"
#include "InjectDll.h"
#include "InjectDllDlg.h"
#include  "InjectClass.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

InjectClass myclass1;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CInjectDll2Dlg 对话框




CInjectDll2Dlg::CInjectDll2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectDll2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectDll2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, M_EDIT1);
}

BEGIN_MESSAGE_MAP(CInjectDll2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CInjectDll2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInjectDll2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInjectDll2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CInjectDll2Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CInjectDll2Dlg 消息处理程序

BOOL CInjectDll2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	edit2->SetWindowTextW(L"地下城与勇士");

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	myclass1._FreeResFile(IDR_MYIME1,L"myime",L"C:\\WINDOWS\\SYSTEM32\\Freeime.ime");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInjectDll2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInjectDll2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInjectDll2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CInjectDll2Dlg::OnBnClickedButton1()
{

	CFileDialog MYFILEDLG(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"li(*.dll)|*.dll|所有文件(*.*)|*.*|");
	int getresut=	MYFILEDLG.DoModal();
	if (getresut==IDOK)
	{
	/*
		CString CFileDialog::GetFileName( ) 得到完整的文件名，包括扩展名如：test.txt
		CString CFileDialog::GetExtName( ) 得到完整的文件扩展名，如：txt
		CString CFileDialog::GetFileTitle ( ) 得到完整的文件名，不包括目录名和扩展名如：test
		POSITION CFileDialog::GetStartPosition( ) 对于选择了多个文件的情况得到第一个文件位置。*/
		M_EDIT1.SetWindowText(MYFILEDLG.GetPathName());
	}
	
	
	
	
	// TODO: 在此添加控件通知处理程序代码
}
//
void CInjectDll2Dlg::OnBnClickedButton2()
{
	CString dllpath;
	M_EDIT1.GetWindowText(dllpath);
	if (!myclass1.ImeInstall(dllpath.GetBuffer()))
		MessageBox(L"安装失败");
	else
		MessageBox(L"安装成功");

}

void CInjectDll2Dlg::OnBnClickedButton3()
{
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	CString str;
	edit2->GetWindowTextW(str);
	CWnd* myhwn;
	myhwn = FindWindow(NULL,str.GetBuffer());
	if (!myhwn) {
		MessageBox(L"找不到窗口");
		return;
	}
	myclass1.IMEActiv(myhwn->m_hWnd);
	
	
}

void CInjectDll2Dlg::OnBnClickedButton4()
{
	if (myclass1.ImeStop())
		MessageBox(L"卸载成功");
	else
		MessageBox(L"卸载失败");


	// TODO: 在此添加控件通知处理程序代码
}
