// RemoteMgr_CltDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RemoteMgr_Clt.h"
#include "RemoteMgr_CltDlg.h"
#include "../common/OutputDbgMsg.h"
#include "../common/MyPacket.h"

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
// CRemoteMgr_CltDlg dialog

CRemoteMgr_CltDlg *CRemoteMgr_CltDlg::m_pCltDlg = NULL;

CRemoteMgr_CltDlg::CRemoteMgr_CltDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteMgr_CltDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteMgr_CltDlg)
	m_dwPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_sockClient = INVALID_SOCKET;
    m_hReadPipe = NULL;
    m_hWritePipe = NULL;
    m_hCmdReadPipe = NULL;
    m_hCmdWritePipe = NULL;
    m_hCmd = NULL;  
    m_pszBuf = NULL;
}

void CRemoteMgr_CltDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteMgr_CltDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_IP);
	DDX_Text(pDX, EDT_PORT, m_dwPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRemoteMgr_CltDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteMgr_CltDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_CONNECT, OnConnect)
	ON_BN_CLICKED(BTN_TEST, OnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteMgr_CltDlg message handlers

BOOL CRemoteMgr_CltDlg::OnInitDialog()
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
    m_pCltDlg = this;
    UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRemoteMgr_CltDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRemoteMgr_CltDlg::OnPaint() 
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
HCURSOR CRemoteMgr_CltDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRemoteMgr_CltDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here
    //创建套接字
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

    //先启动线程接收数据
    CWinThread *pCWinThread = AfxBeginThread(RecvThread, (LPVOID)this);

    //创建管道相关
    MakePipe();
    
    char szTitle[MAXBYTE] = {0};
    GetWindowText(szTitle, MAXBYTE);
    strcat(szTitle, " - 已连接");
    SetWindowText(szTitle);

    //连接成功后禁用连接按钮
    GetDlgItem(BTN_CONNECT)->EnableWindow(FALSE);
}

BOOL CRemoteMgr_CltDlg::InitSocekt()
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

BOOL CRemoteMgr_CltDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    TRACE("ssc CRemoteMgr_CltDlg::DestroyWindow() begin!");
    //向服务端发送下线通知
    CMyPacket pkt;
    CMySocket mySock(m_sockClient);

    BYTE btType = CLT_OFFLINE;

    pkt.Append((char*)&btType, sizeof(btType));
    mySock.SendPacket(pkt);
    MYTRACE("ssc send CLT_OFFLINE");
    
    DWORD dwWrtSize = 0;
    BOOL bRet = FALSE;
    bRet = WriteFile(m_hWritePipe,
                    "exit\r\n",
                    strlen("exit\r\n"),
                    &dwWrtSize,
                    NULL);
    if (bRet == FALSE)
    {
        MYTRACE("ssc WriteFile failed[%d]");
        TerminateProcess(m_hCmd, 0);
    }

    CloseHandle(m_hReadPipe);
    CloseHandle(m_hWritePipe);
    CloseHandle(m_hCmdReadPipe);
    CloseHandle(m_hCmdWritePipe);
    CloseHandle(m_hCmd);

    closesocket(m_sockClient);
    WSACleanup();
    TRACE("ssc CRemoteMgr_CltDlg::DestroyWindow() end!");
	return CDialog::DestroyWindow();
}

UINT CRemoteMgr_CltDlg::RecvThread(LPVOID lParam)
{
    CRemoteMgr_CltDlg *pCltDlg = (CRemoteMgr_CltDlg*)lParam;
    SOCKET sClient = pCltDlg->m_sockClient;
    int  nBufSize = 0;
    int  nTotalSize = 0;
    int  nCurrentSize = 0;
    int  nRecvedSize = 0;
    
    while(TRUE)
    {
        CMySocket skt(sClient);
        CMyPacket recvPkt;
        int nRet = skt.RecvPacket(recvPkt);
        //表示接收数据出错
        if (nRet == -1)
        {
            //错误处理
            break;
        }
        
        //到此处说明已经成功的接收到了一个完整的数据包,可以开始处理数据包
        char* pszBuf = recvPkt.GetBuf();
        int   nBufSize = recvPkt.GetLength();
       
        //到此处说明已经成功的接收到了一个完整的数据包,可以开始处理数据包
        switch(pszBuf[0])
        {
        case SVR_STARTUP_CMD:
            {
                //收到服务端启动CMD窗口的命令，才能开始发送cmd结果
                AfxBeginThread((AFX_THREADPROC)GetCMDOut, lParam);
            }
            break;
        case SVR_SCREEN:
            {
                m_pCltDlg->OnScreenReply();
            }
            break;  
        case SVR_CMD:
            {
                m_pCltDlg->OnRecvCmd(sClient, pszBuf + 1, nBufSize - 1);
            }
            break;  
        case SVR_LBTNDOWN:
            {
                m_pCltDlg->OnRecvLBtnDown(sClient, pszBuf + 1, nBufSize - 1);
            }
            break;
        case SVR_LBTNUP:
            {
                m_pCltDlg->OnRecvLBtnUp(sClient, pszBuf + 1, nBufSize - 1);
            }
            break;
        case SVR_MOUSEMOVE:
            {
                //m_pCltDlg->OnRecvMouseMove(sClient, pszBuf + 1, nBufSize - 1);
            }
            break;
        case SVR_DOUBLE_CLICK:
            {
                m_pCltDlg->OnRecvDoubleClick(sClient, pszBuf + 1, nBufSize - 1);
            }
            break;
        default:
            break;
        }        
    }  
    
    return TRUE;    
}  

void CRemoteMgr_CltDlg::OnRecvCmd(SOCKET s, char* pszBuf, int nBufLength)
{
    //处理svr发来的cmd

    //向管道中写数据
    BOOL bRet = FALSE;
    DWORD dwRealWrtSize = 0;
    bRet = WriteFile(m_hWritePipe, 
                    pszBuf,
                    nBufLength,
                    &dwRealWrtSize,
                    NULL);
    if (bRet == 0 || dwRealWrtSize != (DWORD)nBufLength)
    {
        TRACE("ssc WriteFile failed! bRet[%d] nBufLength[%d] dwRealWrtSize[%d]",
              bRet, nBufLength, dwRealWrtSize);
        MYTRACE("ssc WriteFile failed!");
        return;
    }
}

void CRemoteMgr_CltDlg::OnRecvDoubleClick(SOCKET s, char* pszBuf, int nBufLength)
{
    //模拟鼠标双击
    CPoint point;
    memcpy(&point, pszBuf, nBufLength);

    ::SetCursorPos(point.x, point.y);
    ::mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
    ::mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
}

void CRemoteMgr_CltDlg::OnRecvLBtnDown(SOCKET s, char* pszBuf, int nBufLength)
{
    CPoint point;
    memcpy(&point, pszBuf, nBufLength);
    
    ::SetCursorPos(point.x, point.y);
    ::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
}
void CRemoteMgr_CltDlg::OnRecvLBtnUp(SOCKET s, char* pszBuf, int nBufLength)
{
    CPoint point;
    memcpy(&point, pszBuf, nBufLength);
    
    ::SetCursorPos(point.x, point.y);
    ::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
}
void CRemoteMgr_CltDlg::OnRecvMouseMove(SOCKET s, char* pszBuf, int nBufLength)
{
    CPoint point;
    memcpy(&point, pszBuf, nBufLength);
    
    ::SetCursorPos(point.x, point.y);
    ::mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0 );
}

void CRemoteMgr_CltDlg::OnScreenReply() 
{
    // TODO: Add your control notification handler code here
    BOOL bRet = FALSE;
    //开始获取当前屏幕的数据
    CDC memDC;
    CDC* pDesktopDC = GetDesktopWindow()->GetDC();
    memDC.CreateCompatibleDC(pDesktopDC);
    
    //获取桌面的宽度和高度
    int nWidth = GetSystemMetrics(SM_CXSCREEN);
    int nHeigth = GetSystemMetrics(SM_CYSCREEN);
    
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDesktopDC, nWidth, nHeigth);
    
    memDC.SelectObject(bitmap);
    
    bRet = memDC.BitBlt(0, 0, nWidth, nHeigth, pDesktopDC, 0, 0, SRCCOPY);
    
    int nCount = nWidth * nHeigth * sizeof(DWORD); 
    
    char* pszBuf = new char[nCount];
    if (pszBuf == NULL)
    {
        MYTRACE("ssc new failed!");
        return;
    }
    
    //获取位图中的数据
    bitmap.GetBitmapBits(nCount, pszBuf);
    
    BYTE btType = CLT_SCREEN;
    int nBufSize = sizeof(BYTE) + nCount;
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(m_pCltDlg->m_sockClient);

    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        MYTRACE("ssc pkt.Append failed!");
        return ;
    }
    
    if (pkt.Append((char*)&nWidth, sizeof(DWORD)) == NULL)
    {
        MYTRACE("ssc pkt.Append failed!");
        return ;
    }

    if (pkt.Append((char*)&nHeigth, sizeof(DWORD)) == NULL)
    {
        MYTRACE("ssc pkt.Append failed!");
        return ;
    }
    
    if (pkt.Append((char*)pszBuf, nCount) == NULL)
    {
        MYTRACE("ssc pkt.Append failed!");
        return ;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        MYTRACE("ssc sendSkt.SendPacket failed!");
        return ;
    }
    if (pszBuf != NULL)
    {
        delete[] pszBuf;
        pszBuf = NULL;
    }

    Sleep(500); //防止客户端CPU占用率太高
}

void CRemoteMgr_CltDlg::MakePipe()
{
    BOOL bRet = FALSE;
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
        MYTRACE("ssc GetSystemDirectory failed");
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
}

DWORD CRemoteMgr_CltDlg::GetCMDOut(LPVOID lpParameter)
{
    BOOL bRet = FALSE;
    DWORD dwBufSize = 0;
    HANDLE hReadPipe = ((CRemoteMgr_CltDlg*)lpParameter)->m_hReadPipe;        
    SOCKET sClient = ((CRemoteMgr_CltDlg*)lpParameter)->m_sockClient;
    
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
            //管道没有数据时，暂停0.5s防止CPU满载
            Sleep(500);
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
            MYTRACE("ssc ReadFile failed");
            continue;
        }
        
        if (dwRealSize < dwBufSize)
        {
            TRACE("ssc ReadFile error dwRealSize is [%d], dwBufSize is [%d]", 
                dwRealSize, dwBufSize);
            
            continue;
        }
        
        //cmd输出管道中有数据时则发送给服务端
        CMyPacket sendPkt;
        CMySocket mySock(sClient);

        BYTE btType = CLT_CMD;
        int nBufSize = sizeof(BYTE);
        int nRet = 0;
        if (sendPkt.Append((char*)&btType, sizeof(btType)) == NULL )
        {
            MYTRACE("ssc sendPkt.Append failed!");
            return FALSE;
        }

        if (sendPkt.Append(pszBuf, strlen(pszBuf) + 1) == NULL )
        {
            MYTRACE("ssc sendPkt.Append failed!");
            return FALSE;
        }

        mySock.SendPacket(sendPkt);

        if (pszBuf != NULL)
        {
            delete[] pszBuf;
            pszBuf = NULL;
        }
    }
    return TRUE;
}

void CRemoteMgr_CltDlg::OnTest() 
{
	// TODO: Add your control notification handler code here
    //mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 46, 42, 0, 0 );
    //mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 46, 42, 0, 0 );
}
