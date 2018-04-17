// MyCMD_srvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyCMD_srv.h"
#include "MyCMD_srvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MYMSG WM_USER + 1

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
// CMyCMD_srvDlg dialog


THREADARGS CMyCMD_srvDlg::m_ThreadArg = {NULL};

CMyCMD_srvDlg::CMyCMD_srvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCMD_srvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyCMD_srvDlg)
	m_csShow = _T("");
	m_csCmd = _T("");
	m_dwPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_sockListen = NULL;
    m_sockMsg = NULL;
}

void CMyCMD_srvDlg::OnMyMsg()
{    
    m_csShow += m_ThreadArg.m_pszBuf;
    HWND hEdit = GetDlgItem(EDT_SHOW)->GetSafeHwnd();
    
    UpdateData(FALSE);
    //输出窗口光标定位到最后一行
    ::SendMessage(hEdit, WM_VSCROLL, SB_BOTTOM, 0);
}

BOOL CMyCMD_srvDlg::GetMsg(LPVOID lpThreadParameter)
{
    PTHREADARGS pargs = (PTHREADARGS)lpThreadParameter;

    BOOL bRet = FALSE;
   
    char szBuf[MAXWORD] = {0};
    while(TRUE)
    {
        if (pargs->m_Msg == NULL)
        {            
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.S_un.S_addr = 0;//0表示接受任意IP地址的连接
            addr.sin_port = htons(1234); //转成网络字节序
            
            //4 接收连接
            int nLength = sizeof(sockaddr);
            
            pargs->m_Msg = accept(pargs->m_Listen, (sockaddr*)&addr, &nLength);
            if (pargs->m_Msg == INVALID_SOCKET)
            {
                TRACE("ssc accept() failed[%d]", WSAGetLastError());
                return FALSE;
            }
        }

        int nRecvedBytes = recv(pargs->m_Msg,
            szBuf,
            MAXWORD,
            0);
        if (nRecvedBytes != SOCKET_ERROR && nRecvedBytes > 0 )
        {
            memset(((PTHREADARGS)lpThreadParameter)->m_pszBuf, 0, MAXWORD);
            strcpy(((PTHREADARGS)lpThreadParameter)->m_pszBuf, szBuf);
            //TRACE("ssc srv recv [%s]", ((PTHREADARGS)lpThreadParameter)->m_pszBuf);
            //TRACE中字符串长度不能超过512
            ::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), MYMSG, NULL, NULL);
        }
        else
        {
            ;
        }
    }
    return TRUE;
}

void CMyCMD_srvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCMD_srvDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_IP);
	DDX_Text(pDX, EDT_SHOW, m_csShow);
	DDX_Text(pDX, EDT_CMD, m_csCmd);
	DDX_Text(pDX, EDT_PORT, m_dwPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyCMD_srvDlg, CDialog)
	//{{AFX_MSG_MAP(CMyCMD_srvDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_RUN, OnRun)
    ON_MESSAGE(MYMSG, OnMyMsg)
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCMD_srvDlg message handlers

BOOL CMyCMD_srvDlg::OnInitDialog()
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
    m_dwPort = 27149;
    m_IP.SetAddress(htonl(inet_addr("0.0.0.0")));
    InitSocekt();
    UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control    
}

void CMyCMD_srvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyCMD_srvDlg::OnPaint() 
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
HCURSOR CMyCMD_srvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CMyCMD_srvDlg::InitSocekt()
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

void CMyCMD_srvDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    UpdateData(FALSE);
    BOOL bRet = FALSE;
    m_sockListen = socket(AF_INET, 
        SOCK_STREAM,//流式,TCP
        0);
    if (m_sockListen == INVALID_SOCKET)
    {
        TRACE("ssc socket() failed[%d]", WSAGetLastError());
        return;
    }
    //2 bind 绑定IP 和 端口
    sockaddr_in addr;
    addr.sin_family = AF_INET;

    DWORD dwIP = 0;
    m_IP.GetAddress(dwIP);
    addr.sin_addr.S_un.S_addr = htonl(dwIP);

    addr.sin_port = htons((USHORT)m_dwPort); //转成网络字节序
    bRet = bind(m_sockListen, (sockaddr*)&addr, sizeof(sockaddr));
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc bind() failed[%d]", WSAGetLastError());
        return;
    }
    //3 监听 listen
    bRet = listen(m_sockListen, SOMAXCONN); //SOMAXCONN表示系统设置一个最大合理值
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc listen() failed[%d]", WSAGetLastError());
        return;
    }
    
    m_ThreadArg.m_Listen = m_sockListen;
    m_ThreadArg.m_Msg = m_sockMsg;
    m_ThreadArg.m_pszBuf = new char[MAXWORD];
    if (m_ThreadArg.m_pszBuf == NULL)
    {
        TRACE("ssc new failed");
        return;
    }
    
    AfxBeginThread((AFX_THREADPROC)GetMsg, (LPVOID)&m_ThreadArg);
}

BOOL CMyCMD_srvDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    if (m_ThreadArg.m_pszBuf != NULL)
    {
        delete[] m_ThreadArg.m_pszBuf;
        m_ThreadArg.m_pszBuf = NULL;
    }
    closesocket(m_sockListen);
    closesocket(m_sockMsg);
	WSACleanup( );

	return CDialog::DestroyWindow();
}

void CMyCMD_srvDlg::OnRun() 
{
	// TODO: Add your control notification handler code here
    m_sockMsg = m_ThreadArg.m_Msg;
    if (m_sockMsg == NULL)
    {
        TRACE("ssc OnRun() m_sockMsg == NULL");
        return;
    }
    UpdateData(TRUE);
    m_csCmd += "\r\n";
    int nSendedBytes = send(m_sockMsg,
                            m_csCmd.GetBuffer(0),
                            m_csCmd.GetLength(),
                            0);

    if (nSendedBytes != m_csCmd.GetLength())
    {
        TRACE("ssc send() failed[%d]", WSAGetLastError());
        return;
    }
    else
    {
        TRACE("ssc send() [%s]", m_csCmd.GetBuffer(0));
    }

}
