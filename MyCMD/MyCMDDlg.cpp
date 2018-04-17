// MyCMDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyCMD.h"
#include "MyCMDDlg.h"

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
// CMyCMDDlg dialog
THREADARG CMyCMDDlg::m_ThreadArg = {NULL, NULL, NULL};

CMyCMDDlg::CMyCMDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCMDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyCMDDlg)
	m_csEnter = _T("");
	m_csShow = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_hReadPipe = NULL;
    m_hWritePipe = NULL;
    m_hCmdReadPipe = NULL;
    m_hCmdWritePipe = NULL;
    m_hThread = NULL;
    m_hCmd = NULL;

    m_lstHandle.AddTail(&m_hReadPipe);
    m_lstHandle.AddTail(&m_hWritePipe);
    m_lstHandle.AddTail(&m_hCmdReadPipe);
    m_lstHandle.AddTail(&m_hCmdWritePipe);
    m_lstHandle.AddTail(&m_hThread);
    m_lstHandle.AddTail(&m_hCmd);
}

void CMyCMDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCMDDlg)
	DDX_Text(pDX, EDT_ENTER, m_csEnter);
	DDX_Text(pDX, EDT_SHOW, m_csShow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyCMDDlg, CDialog)
	//{{AFX_MSG_MAP(CMyCMDDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(BTN_RUN, OnRun)
    ON_MESSAGE(MYMSG, OnMyMsg)
	ON_BN_CLICKED(BTN_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCMDDlg message handlers

void CMyCMDDlg::OnMyMsg()
{
    m_csShow += m_ThreadArg.m_pszBuf;
    HWND hEdit = GetDlgItem(EDT_SHOW)->GetSafeHwnd();

    UpdateData(FALSE);
    if (m_ThreadArg.m_pszBuf != NULL)
    {
        delete m_ThreadArg.m_pszBuf;
        m_ThreadArg.m_pszBuf = NULL;
    }
    //输出窗口光标定位到最后一行
    ::SendMessage(hEdit, WM_VSCROLL, SB_BOTTOM, 0);
}


DWORD WINAPI CMyCMDDlg::GetCMDOut(LPVOID lpParameter)
{
    BOOL bRet = FALSE;
    
    while(TRUE)
    {
        HANDLE hReadPipe = ((PTHREADARG)lpParameter)->m_hReadPipe;
        DWORD dwBufSize = 0;

        bRet = PeekNamedPipe(hReadPipe, NULL, 0, NULL, &dwBufSize, NULL);
        if (bRet == FALSE)
        {
            return FALSE; // when close hReadPipe, this thread can be exit
            TRACE("ssc PeekNamedPipe failed[%d]", GetLastError());
        }
        if (dwBufSize == 0)
        {
            continue;
        }
        
        char *pszBuf = new char[dwBufSize + 1];
        memset(pszBuf, 0, dwBufSize + 1);

        if (pszBuf == NULL)
        {
            TRACE("ssc new char failed");
            ExitProcess(0);
        }

        DWORD dwRealSize = 0;
        bRet = ReadFile(hReadPipe, pszBuf, dwBufSize, &dwRealSize, NULL);
        if (bRet == FALSE)
        {
            TRACE("ssc ReadFile failed[%d]", GetLastError());
            continue;
        }

        if (dwRealSize < dwBufSize)
        {
            TRACE("ssc ReadFile error dwRealSize is [%d], dwBufSize is [%d]", 
                  dwRealSize, dwBufSize);

            continue;
        }
        
        ((PTHREADARG)lpParameter)->m_pszBuf = pszBuf;

        //sendmessage to EDT_SHOW, update show text
        HWND hWnd = ((PTHREADARG)lpParameter)->m_hWnd;
        bRet = ::SendMessage(hWnd, MYMSG, NULL, NULL);
        if (bRet == FALSE)
        {
            TRACE("SendMessage failed[%d]", GetLastError());
        }
    }
    return TRUE;
}

BOOL CMyCMDDlg::OnInitDialog()
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
	BOOL bRet = FALSE;
    SECURITY_ATTRIBUTES sa;
    memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));
    sa.bInheritHandle = TRUE;

    //m_hReadPipe -- m_hCmdWritePipe
    //m_hWritePipe -- m_hCmdReadPipe
    bRet = CreatePipe(&m_hReadPipe, &m_hCmdWritePipe, &sa, 0);
    if (bRet == FALSE)
    {
        TRACE("ssc CreatePipe(m_hReadPipe, m_hCmdWritePipe)failed[%d]", GetLastError());
        return FALSE;
    }

    bRet = CreatePipe(&m_hCmdReadPipe, &m_hWritePipe, &sa, 0);
    if (bRet == FALSE)
    {
        TRACE("ssc CreatePipe(&m_hCmdReadPipe, &m_hWritePipe)failed[%d]", GetLastError());
        return FALSE;
    }
    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(PROCESS_INFORMATION));

    STARTUPINFO si;
    memset(&si, 0, sizeof(STARTUPINFO));
    si.dwFlags = STARTF_USESTDHANDLES; 
    si.hStdInput = m_hCmdReadPipe;
    si.hStdOutput = m_hCmdWritePipe;
    si.hStdError = m_hCmdWritePipe;

    char szCmdPath[MAXBYTE] = {0};
    UINT nSize = GetSystemDirectory(szCmdPath, MAXBYTE);
    if (nSize == 0)
    {
        TRACE("ssc GetSystemDirectory failed[%d]", GetLastError());
        return FALSE;
    }
    strcat(szCmdPath, "\\cmd.exe");
    bRet = CreateProcess(NULL, 
                         szCmdPath,
                         &sa,
                         NULL,
                         TRUE,
                         CREATE_NO_WINDOW,
                         NULL,
                         NULL,
                         &si,
                         &pi);
    if (bRet == FALSE)
    {
        TRACE("ssc CreateProcess failed[%d]", GetLastError());
        return FALSE;
    }

    m_hCmd = pi.hProcess;
   
    m_ThreadArg.m_hWnd = GetSafeHwnd();
    m_ThreadArg.m_hReadPipe = m_hReadPipe;

    m_hThread = CreateThread(&sa, 0, GetCMDOut, (LPVOID)&m_ThreadArg, 0, NULL);
    if (m_hThread == FALSE)
    {
        TRACE("ssc CreateThread failed[%d]", GetLastError());
        return FALSE;
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyCMDDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyCMDDlg::OnPaint() 
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
HCURSOR CMyCMDDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyCMDDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    DWORD dwWrtSize = 0;
    BOOL bRet = FALSE;
	bRet = WriteFile(m_hWritePipe,
                    "exit\r\n",
                    strlen("exit\r\n"),
                    &dwWrtSize,
                    NULL);
    if (bRet == FALSE)
    {
        TRACE("ssc WriteFile failed[%d]", GetLastError());
        TerminateProcess(m_hCmd, 0);
    }

    if (m_lstHandle.IsEmpty() == TRUE)
    {
        return;
    }

    POSITION pos = m_lstHandle.GetHeadPosition();
    while (pos)
    {
        HANDLE *phTmp = m_lstHandle.GetNext(pos);
        CloseHandle(*phTmp);
    }
    m_lstHandle.RemoveAll();
}

void CMyCMDDlg::ExecCommand()
{
    DWORD dwRealWrtSize = 0;
    m_csEnter += "\r\n";
    BOOL bRet = FALSE;
    bRet = WriteFile(m_hWritePipe, 
                     m_csEnter.GetBuffer(0),
                     m_csEnter.GetLength(),
                     &dwRealWrtSize,
                     NULL);
    if (bRet == FALSE)
    {
        TRACE("ssc OnRun() WriteFile failed[%d]", GetLastError());
        return;
    }
    if ((DWORD)m_csEnter.GetLength() != dwRealWrtSize)
    {
        TRACE("ssc OnRun() WriteFile error, bufsize is [%d], dwRealWrtSize is [%d]", 
                m_csEnter.GetLength(),
                dwRealWrtSize);
        return;
    }
}

void CMyCMDDlg::OnRun() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    ExecCommand();
    UpdateData(FALSE);
}

void CMyCMDDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
    CString csSave = m_csEnter;
	m_csEnter = '\x03'; //Ctrl+C的asii码，用来停止当前cmd命令，例如ping xxx -t
    ExecCommand();
    m_csEnter = csSave;
    UpdateData(FALSE);
}
