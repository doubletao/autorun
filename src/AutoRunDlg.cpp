
// AutoRunDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoRun.h"
#include "AutoRunDlg.h"
#include "afxdialogex.h"
#include "GlobalFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CONTROL_BUTTON_ID = 1001;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CTooltipsDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CTooltipsDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CTooltipsDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CTooltipsDialog)
END_MESSAGE_MAP()


// CAutoRunDlg 对话框



CAutoRunDlg::CAutoRunDlg(CWnd* pParent /*=NULL*/)
	: CTooltipsDialog(IDD_AUTORUN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CTooltipsDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoRunDlg, CTooltipsDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_COMMAND_RANGE(CONTROL_BUTTON_ID, CONTROL_BUTTON_ID + 10, OnRangeButtonCmds)
END_MESSAGE_MAP()


// CAutoRunDlg 消息处理程序

BOOL CAutoRunDlg::OnInitDialog()
{
	CTooltipsDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitializeUI();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAutoRunDlg::LoadConfig()
{
	CString strConfigPath = CGlobalFunction::GetCurPath();
	strConfigPath.Append(_T("\\AutoRun\\"));
	CString strConfigFile = strConfigPath + _T("Config.ini");

	CString strTitle;
	GetPrivateProfileString(_T("config"), _T("window_title"), strTitle, strTitle.GetBuffer(MAX_PATH), MAX_PATH, strConfigFile);
	strTitle.ReleaseBuffer();
	SetWindowText(strTitle);

	CString strBkImg;
	GetPrivateProfileString(_T("config"), _T("background_img"), strBkImg, strBkImg.GetBuffer(MAX_PATH), MAX_PATH, strConfigFile);
	strBkImg.ReleaseBuffer();
	m_strBkImg = strConfigPath + strBkImg;

	m_nWndWidth = GetPrivateProfileInt(_T("config"), _T("window_width"), 600, strConfigFile);
	m_nWndHeight = GetPrivateProfileInt(_T("config"), _T("window_height"), 400, strConfigFile);

	int nBtnCount = 0;
	nBtnCount = GetPrivateProfileInt(_T("config"), _T("button_count"), 0, strConfigFile);
	CString strButton;
	for (size_t i = 0; i < nBtnCount; i++)
	{
		strButton.Format(_T("button%d"), i + 1);
		StuButtonInfo btnInfo;
		CString strImg;
		GetPrivateProfileString(_T("config"), strButton + _T("_img"), strImg, strImg.GetBuffer(MAX_PATH), MAX_PATH, strConfigFile);
		strImg.ReleaseBuffer();
		btnInfo.strImg = strConfigPath + strImg;
		GetPrivateProfileString(_T("config"), strButton + _T("_tips"), btnInfo.strTips, btnInfo.strTips.GetBuffer(MAX_PATH), MAX_PATH, strConfigFile);
		btnInfo.strTips.ReleaseBuffer();
		btnInfo.x = GetPrivateProfileInt(_T("config"), strButton + _T("_x"), 0, strConfigFile);
		btnInfo.y = GetPrivateProfileInt(_T("config"), strButton + _T("_y"), 0, strConfigFile);
		btnInfo.width = GetPrivateProfileInt(_T("config"), strButton + _T("_width"), 0, strConfigFile);
		btnInfo.height = GetPrivateProfileInt(_T("config"), strButton + _T("_height"), 0, strConfigFile);
		GetPrivateProfileString(_T("config"), strButton + _T("_command"), btnInfo.strCommand, btnInfo.strCommand.GetBuffer(MAX_PATH), MAX_PATH, strConfigFile);
		btnInfo.strCommand.ReleaseBuffer();
		m_vecButtonInfo.push_back(btnInfo);
	}
}

void CAutoRunDlg::InitializeUI()
{
	LoadConfig();
	SetBackgroundImg(m_strBkImg);
	MoveWindow(0, 0, m_nWndWidth,m_nWndHeight);
	CenterWindow();
	for (size_t i = 0; i < m_vecButtonInfo.size(); i++)
	{
		m_vecButtonInfo[i].pButton = new CImgButton;
		m_vecButtonInfo[i].pButton->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(0, 0, 10, 10), this, CONTROL_BUTTON_ID + i);
		m_vecButtonInfo[i].pButton->MoveWindow(m_vecButtonInfo[i].x, m_vecButtonInfo[i].y, m_vecButtonInfo[i].width, m_vecButtonInfo[i].height);
		m_vecButtonInfo[i].pButton->ShowWindow(SW_SHOW);
		m_vecButtonInfo[i].pButton->SetImage(m_vecButtonInfo[i].strImg);
		if (!m_vecButtonInfo[i].strTips.IsEmpty())
		{
			ModifyTooltips(CONTROL_BUTTON_ID + i, m_vecButtonInfo[i].strTips);
		}
	}
}

void CAutoRunDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CTooltipsDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoRunDlg::OnPaint()
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
		CTooltipsDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAutoRunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoRunDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecButtonInfo.size(); i++)
	{
		if (m_vecButtonInfo[i].pButton)
		{
			delete m_vecButtonInfo[i].pButton;
			m_vecButtonInfo[i].pButton = nullptr;
		}
	}
	m_vecButtonInfo.clear();
}

void CAutoRunDlg::OnRangeButtonCmds(UINT nID)
{
	int nBtnIndex = nID - CONTROL_BUTTON_ID;
	if (nBtnIndex > -1 && nBtnIndex < m_vecButtonInfo.size())
	{
		if (!m_vecButtonInfo[nBtnIndex].strCommand.IsEmpty())
		{
			if (m_vecButtonInfo[nBtnIndex].strCommand == _T("exit"))
			{
				OnCancel();
			}
			else
			{
				ShellExecute(NULL, _T("open"), m_vecButtonInfo[nBtnIndex].strCommand, NULL, NULL, SW_SHOWNORMAL);
			}
		}
	}
}
