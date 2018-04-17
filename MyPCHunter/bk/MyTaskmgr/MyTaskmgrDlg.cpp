// MyTaskmgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "MyTaskmgrDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTaskmgrDlg dialog

CMyTaskmgrDlg::CMyTaskmgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyTaskmgrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyTaskmgrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyTaskmgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyTaskmgrDlg)
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyTaskmgrDlg, CDialog)
	//{{AFX_MSG_MAP(CMyTaskmgrDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTaskmgrDlg message handlers

BOOL CMyTaskmgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here


    m_DlgGDT.Create(DLG_GDT, FromHandle(m_tabCtrl.GetSafeHwnd()));
    m_DlgIDT.Create(DLG_IDT, FromHandle(m_tabCtrl.GetSafeHwnd()));
    m_DlgSSDT.Create(DLG_SSDT, FromHandle(m_tabCtrl.GetSafeHwnd()));
    m_DlgShadow.Create(DLG_SHADOWSSDT, FromHandle(m_tabCtrl.GetSafeHwnd()));
    m_DlgKrnlModule.Create(DLG_KRNLMODULE, FromHandle(m_tabCtrl.GetSafeHwnd()));
    m_DlgProcMgr.Create(DLG_PROCESS, FromHandle(m_tabCtrl.GetSafeHwnd()));

    m_DlgFile.Create(DLG_FILEMGR, FromHandle(m_tabCtrl.GetSafeHwnd()));
    m_DlgReg.Create(DLG_REGEDIT, FromHandle(m_tabCtrl.GetSafeHwnd()));
    m_DlgCmd.Create(DLG_MYCMD, FromHandle(m_tabCtrl.GetSafeHwnd()));

    //把各个tab的窗口存到数组方便使用
    //添加tab时记得在这个数组中加一下
    static TABINFO g_tabInfoAry[] = {
        {&m_DlgGDT, "GDT"},
        {&m_DlgIDT, "IDT"},
        {&m_DlgSSDT, "SSDT"},
        {&m_DlgShadow, "ShadowSSDT"},
        {&m_DlgKrnlModule, "驱动模块"},
        {&m_DlgProcMgr, "进程管理"},
        //{&m_DlgFile, "文件管理"},
        //{&m_DlgReg, "注册表"},
        //{&m_DlgCmd, "MyCMD"},
    };

    m_dwTabNum = sizeof(g_tabInfoAry) / (4 + 4);
    m_ptabInfoAry = g_tabInfoAry;
    
    
    //     m_tabCtrl.InsertItem(0, "文件管理");
    //     m_tabCtrl.InsertItem(1, "注册表");
    //     m_tabCtrl.InsertItem(3, "MyCMD");
    //     m_tabCtrl.InsertItem(0, "GDT");
    //     m_tabCtrl.InsertItem(1, "进程管理");
    //以上改用循环
    int i = 0;
    for (i = 0; i < m_dwTabNum; i++)
    {
        m_tabCtrl.InsertItem(i, g_tabInfoAry[i].m_pDlgName);
    }

    RECT rc;
    m_tabCtrl.GetClientRect(&rc);
    rc.top += 20;

    //dlg.MoveWindow(&rc)
    //dlg.ShowWindow(FALSE)
    //用循环实现
    //0下标ShowWindow(TRUE)
    for (i = 0; i < m_dwTabNum; i++)
    {
        g_tabInfoAry[i].m_pDlg->MoveWindow(&rc);
        g_tabInfoAry[i].m_pDlg->ShowWindow(FALSE); 
    }
    g_tabInfoAry[0].m_pDlg->ShowWindow(TRUE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyTaskmgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyTaskmgrDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyTaskmgrDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CMyTaskmgrDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    }
	return CDialog::PreTranslateMessage(pMsg);
}

void CMyTaskmgrDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    int nCurSel = m_tabCtrl.GetCurSel();

    for (int i = 0; i < m_dwTabNum; i++)
    {
        if (i != nCurSel)
        {
            //其它的ShowWindow(FALSE)
            (m_ptabInfoAry[i].m_pDlg)->ShowWindow(FALSE);
        }
    }
    
    //选中的tab页ShowWindow(TRUE)
    (m_ptabInfoAry[nCurSel].m_pDlg)->ShowWindow(TRUE);

    *pResult = 0;
}
