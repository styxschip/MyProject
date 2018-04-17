// RemoteMgr_SvrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RemoteMgr_Svr.h"
#include "RemoteMgr_SvrDlg.h"
#include "../common/MyPacket.h"
#include "../common/OutputDbgMsg.h"
#include "ShowCltScreen.h"

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
// CRemoteMgr_SvrDlg dialog

CRemoteMgr_SvrDlg *CRemoteMgr_SvrDlg::m_pMainDlg = NULL;


CRemoteMgr_SvrDlg::CRemoteMgr_SvrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteMgr_SvrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteMgr_SvrDlg)
	m_dwPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_sockListen = NULL;
    
    m_nCltCount = 0;
}

SOCKET CRemoteMgr_SvrDlg::m_SelSocket = SOCKET_ERROR;

void CRemoteMgr_SvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteMgr_SvrDlg)
	DDX_Control(pDX, IDC_LISTCLT, m_CltLstCtrl);
	DDX_Control(pDX, IDC_IPADDRESS, m_IP);
	DDX_Text(pDX, EDT_PORT, m_dwPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRemoteMgr_SvrDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteMgr_SvrDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_START, OnStart)
	ON_BN_CLICKED(BTN_GETSCREEN, OnGetscreen)
	ON_BN_CLICKED(BTN_CMD, OnCmd)
	ON_WM_COPYDATA()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCLT, OnItemchangedListclt)
    ON_MESSAGE(WM_CLRSUBDLG, OnCleanSubDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteMgr_SvrDlg message handlers

BOOL CRemoteMgr_SvrDlg::OnInitDialog()
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
    
    InitListCtrl();

    InitSocekt();
    UpdateData(FALSE);


	m_pMainDlg = this;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRemoteMgr_SvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRemoteMgr_SvrDlg::OnPaint() 
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
HCURSOR CRemoteMgr_SvrDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CRemoteMgr_SvrDlg::InitSocekt()
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

BOOL CRemoteMgr_SvrDlg::InitListCtrl()
{
    m_CltLstCtrl.SetExtendedStyle(m_CltLstCtrl.GetExtendedStyle()|
                                    LVS_EX_GRIDLINES|
                                    LVS_EX_FULLROWSELECT);

    m_CltLstCtrl.InsertColumn(0, "IP", LVCFMT_LEFT, 160);
    m_CltLstCtrl.InsertColumn(1, "端口", LVCFMT_LEFT, 160/*80*/);
    m_CltLstCtrl.InsertColumn(2, "SOCKET", LVCFMT_LEFT, 80);
    //m_CltLstCtrl.InsertColumn(1, "MAC", LVCFMT_LEFT, 160);
    return TRUE;
}

BOOL CRemoteMgr_SvrDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    TRACE("ssc CRemoteMgr_SvrDlg::DestroyWindow() begin!");
    if (m_mapsCltWithScrnDlg.IsEmpty() == FALSE)
    {
        POSITION pos = m_mapsCltWithScrnDlg.GetStartPosition();
        while (pos)
        {
            CShowCltScreen *pTmpDlg = NULL;
            SOCKET sTmp = SOCKET_ERROR;
            m_mapsCltWithScrnDlg.GetNextAssoc(pos, sTmp, pTmpDlg);
            if (pTmpDlg != NULL)
            {
                delete pTmpDlg;
                pTmpDlg = NULL;
            }
            if (sTmp != SOCKET_ERROR)
            {
                closesocket(sTmp);
                sTmp = SOCKET_ERROR;
            }
            
        }
        m_mapsCltWithScrnDlg.RemoveAll();
    }

    if (m_mapsCltWithCmdDlg.IsEmpty() == FALSE)
    {
        POSITION pos = m_mapsCltWithCmdDlg.GetStartPosition();
        while (pos)
        {
            CMyCMD_srvDlg *pTmpDlg = NULL;
            SOCKET sTmp = SOCKET_ERROR;
            m_mapsCltWithCmdDlg.GetNextAssoc(pos, sTmp, pTmpDlg);
            if (pTmpDlg != NULL)
            {
                delete pTmpDlg;
                pTmpDlg = NULL;
            }
            if (sTmp != SOCKET_ERROR)
            {
                closesocket(sTmp);
                sTmp = SOCKET_ERROR;
            }            
        }        
        m_mapsCltWithCmdDlg.RemoveAll();
    }

    if (m_mapHwndWithsClt.IsEmpty() == FALSE)
    {
        m_mapHwndWithsClt.RemoveAll();
    }
    
	closesocket(m_sockListen);

//     if (m_LstCltSock.IsEmpty() == FALSE)
//     {
//         POSITION pos = m_LstCltSock.GetHeadPosition();
//         while (pos)
//         {
//             SOCKET sTmp = m_LstCltSock.GetNext(pos);
//             closesocket(sTmp);
//         }
//         m_LstCltSock.RemoveAll();
//     }

    WSACleanup();
    TRACE("ssc CRemoteMgr_SvrDlg::DestroyWindow() end!");
	return CDialog::DestroyWindow();
}
extern CRemoteMgr_SvrApp theApp;

void CRemoteMgr_SvrDlg::OnStart() 
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
    
    AfxBeginThread((AFX_THREADPROC)AcceptThread, (LPVOID)this);

    char szTitle[MAXBYTE] = {0};
    GetWindowText(szTitle, MAXBYTE);
    strcat(szTitle, " - 服务已启动");
    SetWindowText(szTitle);
}


BOOL CRemoteMgr_SvrDlg::OnGetscreen() 
{
	// TODO: Add your control notification handler code here
    if (m_sockListen == NULL || m_sockListen == INVALID_SOCKET)
    {
        TRACE("ssc m_sockListen == %p ", m_sockListen);
        return FALSE;
    }
    if (m_SelSocket == INVALID_SOCKET || m_SelSocket == NULL)
    {
        AfxMessageBox("请选择一个客户端!");
        return FALSE;
    }
    //获取客户在线列表选中的项
    SOCKET sClient = NULL;  
    sClient = m_SelSocket;
    
    CShowCltScreen *pdlgCltSc  = new CShowCltScreen;
    if (pdlgCltSc == NULL)
    {
        MYTRACE("ssc new CShowCltScreen failed!!!");
        return FALSE;
    }

    pdlgCltSc->Create(DLG_CLTSCREEN, this);
    pdlgCltSc->ShowWindow(SW_SHOW);
    m_mapHwndWithsClt.SetAt(pdlgCltSc->GetSafeHwnd(), sClient);
    m_mapsCltWithScrnDlg.SetAt(sClient, pdlgCltSc);
    
    //向发送截屏命令
    BYTE btType = SVR_SCREEN;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(sClient);
    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        return FALSE;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CRemoteMgr_SvrDlg::OnCmd() 
{
	// TODO: Add your control notification handler code here
    if (m_sockListen == NULL || m_sockListen == INVALID_SOCKET)
    {
        TRACE("ssc m_sockListen == %p ", m_sockListen);
        return FALSE;
    }
    
    if (m_SelSocket == INVALID_SOCKET || m_SelSocket == NULL)
    {
        AfxMessageBox("请选择一个客户端!");
        return FALSE;
    }

    //获取客户在线列表选中的项
    SOCKET sClient = m_SelSocket; 
    
    //创建窗口
    CMyCMD_srvDlg *pdlgCltCmd = new CMyCMD_srvDlg;
    //关闭窗口时，子窗口通知父窗口delete窗口指针并释放CMap
    if (pdlgCltCmd == NULL)
    {
        MYTRACE("ssc CMyCMD_srvDlg new failed!");
        return FALSE;
    }
    pdlgCltCmd->Create(IDD_MYCMD_SRV_DIALOG, this);
    pdlgCltCmd->ShowWindow(SW_SHOW);
    m_mapHwndWithsClt.SetAt(pdlgCltCmd->GetSafeHwnd(), sClient);
    m_mapsCltWithCmdDlg.SetAt(sClient, pdlgCltCmd);


    //得到客户端的socket后，发送SVR_STARTUP_CMD;
    BYTE btType = SVR_STARTUP_CMD;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(sClient);
    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        MYTRACE("ssc pkt.Append failed");
        return FALSE;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        MYTRACE("ssc sendSkt.SendPacket(pkt) failed");
        return FALSE;
    }

    
    return TRUE;
}

UINT CRemoteMgr_SvrDlg::AcceptThread(LPVOID lParam)
{   
    CRemoteMgr_SvrDlg *pMainDlg = (CRemoteMgr_SvrDlg *)lParam;
    int nLength = sizeof(sockaddr);
    sockaddr_in addr;
    SOCKET sListen = pMainDlg->m_sockListen;
    while (TRUE)
    {
        SOCKET sClient = accept(sListen, (sockaddr*)&addr, &nLength);    
        if (sClient != INVALID_SOCKET )
        {
            TRACE("ssc sClient is %p", sClient);

            pMainDlg->m_CltLstCtrl.InsertItem(pMainDlg->m_nCltCount, inet_ntoa(addr.sin_addr));
            char szTmpBuf[MAXBYTE] = {0};
            wsprintf(szTmpBuf, "%d", ntohs(addr.sin_port));
            pMainDlg->m_CltLstCtrl.SetItemText(pMainDlg->m_nCltCount, 1, szTmpBuf);

            wsprintf(szTmpBuf, "%08X", sClient);
            pMainDlg->m_CltLstCtrl.SetItemText(pMainDlg->m_nCltCount, 2, szTmpBuf);
            //在线客户端计数加1,客户端关闭时记得减掉
            pMainDlg->m_nCltCount++;
            
            AfxBeginThread((AFX_THREADPROC)RecvThread, (LPVOID)sClient);
        }
        else
        {
            MYTRACE("ssc accept failed!");
            return -1;
        }
    }
    
    return 0;
}

UINT CRemoteMgr_SvrDlg::RecvThread(LPVOID lParam)
{
    TRACE("ssc CRemoteMgr_SvrDlg::RecvThread begin! lParam = %p", lParam);
    SOCKET sClient = (SOCKET)lParam;
    int nDataSize = 0;
    char *pBuf = NULL;
    int nRet = 0;

    while (TRUE)
    {
        if (sClient == SOCKET_ERROR || sClient == INVALID_SOCKET)
        {
            MYTRACE("ssc sClient is error!!");
            return FALSE;
        }
        CMyPacket recvPkt;
        CMySocket mysock(sClient);

        nRet = mysock.RecvPacket(recvPkt);
        if (nRet == -1)
        {
            MYTRACE("ssc mysock.RecvPacket nRet == -1!");
            break;
        }

        pBuf = recvPkt.GetBuf();
        nDataSize = recvPkt.GetLength();
        TRACE("sssc pBuf[0] is %d, nDataSize is %d", pBuf[0], nDataSize);
        switch (pBuf[0])
        {
        case CLT_SCREEN:
            {
                TRACE("pBuf[0] is CLT_SCREEN"); 
                m_pMainDlg->GetClientScreen(sClient, pBuf + 1, nDataSize - 1);
            }            
        	break;
        case CLT_CMD:
            {
                TRACE("pBuf[0] is CLT_CMD"); 
                m_pMainDlg->GetClientCmd(sClient, pBuf + 1, nDataSize - 1);
            }
            break;
        case CLT_OFFLINE:
            {
                //客户端下线消息,服务端删除客户端在线列表
                TRACE("pBuf[0] is CLT_OFFLINE"); 
                m_pMainDlg->RemoveOfflineClt(sClient);
            }
        default:
            break;
        }
    }

    return 0;
}

BOOL CRemoteMgr_SvrDlg::GetClientScreen(SOCKET sClient, char* pszBuf, int nBufLength)
{
    //显示发送过来的数据

    //开始获取当前屏幕的数据
    CShowCltScreen *pScrnDlg = NULL;
    m_mapsCltWithScrnDlg.Lookup(sClient, pScrnDlg);

    CDC *pDlgDC = pScrnDlg->GetDC();
    
    CDC memDC;
    memDC.CreateCompatibleDC(pDlgDC);
    
    //获取桌面的宽度和高度
    int nWidth = *(DWORD*)pszBuf;
    int nHeigth = *(DWORD*)(pszBuf+4);
    int nCount = 4 * nWidth * nHeigth; 
    
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDlgDC, nWidth, nHeigth);
    
    bitmap.SetBitmapBits(nCount, pszBuf+8);
    
    memDC.SelectObject(bitmap);
    
    int bRet = pDlgDC->BitBlt(0, 0, nWidth, nHeigth, &memDC, 0, 0, SRCCOPY);
    
    
    //请求下一帧
    BYTE btType = SVR_SCREEN;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(sClient);
    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        return FALSE;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        return FALSE;
    }
    
    return TRUE;
}

BOOL CRemoteMgr_SvrDlg::GetClientCmd(SOCKET sClient, char* pszBuf, int nBufLength)
{
    //获得客户端cmd返回的数据数据并发送给MyCMD窗口
    LRESULT lRet = 0;
    COPYDATASTRUCT cds;
    char *pBuf = new char[nBufLength];
    if (pBuf == NULL)
    {
        MYTRACE("ssc new failed!");
        return FALSE;
    }
    memset(pBuf, 0, nBufLength);
    memcpy(pBuf, pszBuf, nBufLength);

    cds.dwData = CLT_CMD;       //标识码
    cds.cbData = nBufLength; //缓冲区大小
    cds.lpData = pBuf;   //缓冲区指针
    HWND curHwnd = GetSafeHwnd();
    //通过socket找窗口
    HWND hSubWnd = NULL;
    CMyCMD_srvDlg *pCmdDlg = NULL;    
    m_mapsCltWithCmdDlg.Lookup(sClient, pCmdDlg);
    hSubWnd = pCmdDlg->GetSafeHwnd();

    TRACE("ssc GetClientCmd now sendmessage data to hSubWnd");
    TRACE("ssc hSubWnd is %p, curHwnd is %p", hSubWnd, curHwnd);
    OutputDebugMsg(__LINE__, __FILE__, "ssc pBuf is %s", pBuf);
    lRet = ::SendMessage(hSubWnd,
                          WM_COPYDATA,
                          (WPARAM)curHwnd,
                          (LPARAM)&cds);
    TRACE("SendMessage ret %d", lRet);

    if (pBuf != NULL)
    {
        delete[] pBuf;
        pBuf = NULL;
    }
    return TRUE;
}

BOOL CRemoteMgr_SvrDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
    if (pCopyDataStruct->dwData == CLT_CMD)
    {
        //向客户端发送指令
        BYTE btType = SVR_CMD;
        int nBufSize = sizeof(BYTE);
        int nRet = 0;

        SOCKET sClient = NULL;
        m_mapHwndWithsClt.Lookup(pWnd->GetSafeHwnd(), sClient);

        CMyPacket pkt;
        CMySocket sendSkt(sClient);
        if (pkt.Append((char*)&btType, 1) == NULL)
        {
            return FALSE;
        }

        if (pkt.Append((char*)pCopyDataStruct->lpData, pCopyDataStruct->cbData) == NULL)
        {
            return FALSE;
        }
        
        nRet = sendSkt.SendPacket(pkt);
        if (nRet == -1)
        {
            return FALSE;
        }        
    }
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

/*
#include <windows.h>   
#include <stdio.h>   
#include <winsock.h>   
#include "iphlpapi.h"   

#pragma comment ( lib, "ws2_32.lib" )   
#pragma comment ( lib, "Iphlpapi.lib" )   

void main( int argc, char ** argv )   
{   
    int numberOfHost = 1;   
    struct hostent *remoteHostent;   
    
    //处理命令行参数   
    if ( argc == 3 )   
        numberOfHost = atoi( argv[2] );   
    if ( ( argc >3 ) || ( argc < 2 ) )   
    {   
        printf( "RmtHost v0.2 - Get remote HostName /MacAddress\n" );   
        printf( "by ShotgunLabs ( Shotgun@xici.net )\n\n" );   
        printf( "Usage　:\n\tRmtHost.exe [RemoteIP] \n\n" );   
        printf( "Example:\n\tRmtHost.exe 192.168.0.3\n" );   
        printf( "\tRmtHost.exe 192.168.0.3 255\n\n" );   
        exit( 0 );   
    }   
    
    
    //初始化SOCKET   
    WSADATA wsaData;   
    int iRet = WSAStartup(MAKEWORD(2,1), &wsaData);   
    if ( iRet != 0 )   
    {   
        printf( "WSAStartup Error:%d\n", GetLastError() );   
        exit( 0 );   
    }   
    int nRemoteAddr = inet_addr( argv[1] );   
    remoteHostent= (struct hostent*)malloc( sizeof(struct hostent ));   
    struct in_addr sa;   
    for ( int i = 0; i < numberOfHost; i ++ )   
    {   
        //获取远程机器名   
        sa.s_addr = nRemoteAddr;   
        printf( "\nIpAddress : %s\n", inet_ntoa( sa ) );   
        remoteHostent = gethostbyaddr( (char*)&nRemoteAddr,4, AF_INET );   
        if ( remoteHostent )   
            printf( "HostName　: %s\n",remoteHostent->h_name );   
        else   
            printf( "gethostbyaddr Error:%d\n",GetLastError() );   
        //发送ARP查询包获得远程MAC地址   
        
        unsigned char macAddress[6];   
        ULONG macAddLen = 6;   
        iRet = SendARP(nRemoteAddr, (unsigned long)NULL,(PULONG)&macAddress, &macAddLen);   
        if ( iRet == NO_ERROR )   
        {   
            printf( "MacAddress: " );   
            for( int i =0; i<6; i++ )   
            {   
                printf( "%.2x", macAddress[i] );   
                if ( i<5 ) printf( "-" );   
            }   
            printf( "\n" );   
        }   
        else   
        {
            printf( "SendARP Error:%d\n", GetLastError());   
        }
        nRemoteAddr = htonl( ntohl( nRemoteAddr ) + 1 );   
    }   
}   
*/

void CRemoteMgr_SvrDlg::OnItemchangedListclt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
    if (pNMListView->uOldState == 0)
    {
        CString csPort = m_CltLstCtrl.GetItemText(pNMListView->iItem, 2);
        m_SelSocket = strtoul(csPort.GetBuffer(0), NULL, 16);
    }
	*pResult = 0;
}

//WM_CLRSUBDLG
void CRemoteMgr_SvrDlg::OnCleanSubDlg(WPARAM wParam, LPARAM lParam)
{
    if (wParam == NULL)
    {
        MYTRACE("ssc OnCleanSubDlg wParam is NULL!!!");
        return;
    }
    BOOL bRet = FALSE;
    HWND hWnd = (HWND)wParam;
    SOCKET sClient = NULL;
    CShowCltScreen *pScrnDlg = NULL;
    CMyCMD_srvDlg *pCmdDlg = NULL;
    
    bRet = m_mapHwndWithsClt.Lookup(hWnd, sClient);
    if (bRet == FALSE)
    {
        TRACE("ssc m_mapHwndWithsClt lookup hWnd[%p] is FALSE", hWnd);
        return;
    }

    if (m_mapsCltWithScrnDlg.IsEmpty() != TRUE)
    {
        bRet = m_mapsCltWithScrnDlg.Lookup(sClient, pScrnDlg);
        if (bRet == TRUE)
        {
            m_mapsCltWithScrnDlg.RemoveKey(sClient);
            
            if (pScrnDlg != NULL)
            {
                delete pScrnDlg;
                pScrnDlg = NULL;
            }  
            //客户端socket不能关闭,因为可能关掉窗口后
            //还要再打开所以还需要socket
            //closesocket(sClient);

            return;
        }
    } // end if (m_mapsCltWithScrnDlg.IsEmpty() != TRUE)

    if (m_mapsCltWithCmdDlg.IsEmpty() != TRUE)
    {
        bRet = m_mapsCltWithCmdDlg.Lookup(sClient, pCmdDlg);
        if (bRet == TRUE)
        { 
            m_mapsCltWithCmdDlg.RemoveKey(sClient);
            
            if (pCmdDlg != NULL)
            {
                delete pCmdDlg;
                pCmdDlg = NULL;
            } 
            //客户端socket不能关闭,因为可能关掉窗口后
            //还要再打开所以还需要socket
            //closesocket(sClient);
            return;  
        }
    }
    m_mapHwndWithsClt.RemoveKey(hWnd);
}

BOOL CRemoteMgr_SvrDlg::RemoveOfflineClt(SOCKET sClient)
{
    if (sClient == SOCKET_ERROR || sClient == INVALID_SOCKET)
    {
        TRACE("ssc sClient is error");
        return FALSE;
    }
    TRACE("ssc sClient is %p", sClient);

    //清除在线列表中的客户端
    char szSocket[MAXBYTE] = {0};
    wsprintf(szSocket, "%08X", sClient);
    
    CString csFind = "";
    
    int nCount = m_CltLstCtrl.GetItemCount();
    
    for (int i = 0; i < nCount; i++)
    {
        csFind = m_CltLstCtrl.GetItemText(i, 2);
        if (csFind == szSocket)
        {
            m_CltLstCtrl.DeleteItem(i);
            m_nCltCount--;
            break;
        }
    }

    //清理窗体
//     CMyCMD_srvDlg *pCmdDlg = NULL;
//     CShowCltScreen *pScrnDlg = NULL;
// 
//     if (m_mapsCltWithCmdDlg.IsEmpty() == FALSE)
//     {
//         m_mapsCltWithCmdDlg.Lookup(sClient, pCmdDlg);
//     }
//     
//     if (m_mapsCltWithScrnDlg.IsEmpty() == FALSE)
//     {
//         m_mapsCltWithScrnDlg.Lookup(sClient, pScrnDlg);
//     }
// 
//     if (m_mapHwndWithsClt.IsEmpty() == FALSE)
//     {
//         if (pCmdDlg != NULL)
//         {
//             OnCleanSubDlg((WPARAM)(pCmdDlg->GetSafeHwnd()), NULL);            
//         }
//         if (pScrnDlg != NULL)
//         {
//             OnCleanSubDlg((WPARAM)(pScrnDlg->GetSafeHwnd()), NULL);
//         }
//     }
    
    //此时需要关闭客户端socket
    closesocket(sClient);
    
    return TRUE;
}