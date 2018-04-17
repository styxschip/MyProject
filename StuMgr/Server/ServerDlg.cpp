// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#include "..\common\MyPacket.h"
#include "..\common\ThreadPool.h"
#include "..\common\ICommand.h"
#include "AcceptThread.h"

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
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_dwDBPort = 0;
	m_csUsrName = _T("sa");
	m_csPasWord = _T("sa");
	m_csDBName = _T("StuMgrDB");
	m_dwSrvPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_csConnstr = "";
    m_nMidCount = 0;
    m_sockListen = INVALID_SOCKET;
}

void CServerDlg::OnOK()
{
    return;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_LIST, m_LstMid);
	DDX_Control(pDX, IDC_IPADDRESS, m_IP);
	DDX_Text(pDX, EDT_DBPORT, m_dwDBPort);
	DDX_Text(pDX, EDT_USER, m_csUsrName);
	DDX_Text(pDX, EDT_PASWORD, m_csPasWord);
	DDX_Text(pDX, EDT_DBNAME, m_csDBName);
	DDX_Text(pDX, EDT_SRVPORT, m_dwSrvPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_CONNECT, OnConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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
    m_IP.SetAddress(htonl(inet_addr("192.168.4.70")));
    m_dwDBPort = 1433;
    m_dwSrvPort = 10086;
    UpdateData(FALSE);
    InitListCtrl();

    BOOL bRet = m_ThreadPool.Create(5);
    if (!bRet)
    {
        TRACE("ssc Srv pool.Create(5) failed[]");
    }
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint() 
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CServerDlg::InitListCtrl()
{
    m_LstMid.SetExtendedStyle(m_LstMid.GetExtendedStyle()|
                                LVS_EX_GRIDLINES|
                                LVS_EX_FULLROWSELECT);
    
    m_LstMid.InsertColumn(0, "IP", LVCFMT_LEFT, 160);
    m_LstMid.InsertColumn(1, "端口", LVCFMT_LEFT, 160);
    m_LstMid.InsertColumn(2, "SOCKET", LVCFMT_LEFT, 80);
    return TRUE;
}

void CServerDlg::OnConnect() 
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
        TRACE("ssc Srv socket() failed[%d]", WSAGetLastError());
        return;
    }
    //2 bind 绑定IP 和 端口
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    
    DWORD dwIP = 0;
    addr.sin_addr.S_un.S_addr = htonl(dwIP);
    
    addr.sin_port = htons((USHORT)m_dwSrvPort); //转成网络字节序
    bRet = bind(m_sockListen, (sockaddr*)&addr, sizeof(sockaddr));
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc Srv bind() failed[%d]", WSAGetLastError());
        return;
    }
    //3 监听 listen
    bRet = listen(m_sockListen, SOMAXCONN); //SOMAXCONN表示系统设置一个最大合理值
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc Srv listen() failed[%d]", WSAGetLastError());
        return;
    }
    
    CAcceptThread *pAcceptThread = new CAcceptThread((LPVOID)this);
    if (pAcceptThread == NULL)
    {
        TRACE("ssc Srv pAcceptThread == NULL");
        return;
    }
    
    m_ThreadPool.Handle(pAcceptThread);    
    
    GetDlgItem(IDC_CONSTATE)->SetWindowText("已连接");
    GetDlgItem(BTN_CONNECT)->EnableWindow(FALSE);


    //连接数据库
    CString csIP;
    m_IP.GetWindowText(csIP);

    m_csConnstr.Format("Provider=SQLOLEDB.1;Password=sa;Persist Security Info=True;User ID=sa;Initial Catalog=%s;Data Source=%s", m_csDBName, csIP);
    
    try
    {
        //1 创建一个 Connection 对象
        HRESULT hr = m_conn.CreateInstance("adodb.Connection");
        if (FAILED(hr))
        {
            TRACE("ssc srv conn err");
        }
        
        //2 打开数据源，建立同数据源的连接
        _bstr_t bstrConn = _bstr_t((LPCTSTR)m_csConnstr);
        hr = m_conn->Open(bstrConn, "", "", adConnectUnspecified);
        if (FAILED(hr))
        {
            TRACE("ssc srv Open err");
        }
    }
    catch (_com_error e)
    {
        TRACE("ssc svr %s", e.Description());
    }
}

BOOL CServerDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    TRACE("ssc Srv CServerDlg::DestroyWindow() begin!");

    //终止数据库连接
    HRESULT hr  = m_conn->Close();
    if (FAILED(hr))
    {
        TRACE("ssc Close err");
    }

    closesocket(m_sockListen);
    m_sockListen = INVALID_SOCKET;
    
    m_ThreadPool.Destory();
    
    TRACE("ssc Srv CServerDlg::DestroyWindow() end!");
	return CDialog::DestroyWindow();
}

BOOL CServerDlg::RemoveOfflineMid(SOCKET sMid)
{
    TRACE("ssc Srv CServerDlg::RemoveOfflineMid(%p) begin!", sMid);
    if (sMid == INVALID_SOCKET)
    {
        TRACE("ssc Server sMid is error");
        return FALSE;
    }
    TRACE("ssc Server sMid is %p", sMid);
    
    //清除在线列表中的客户端
    char szSocket[MAXBYTE] = {0};
    wsprintf(szSocket, "%08X", sMid);
    
    CString csFind = "";
    
    int nCount = m_LstMid.GetItemCount();
    
    for (int i = 0; i < nCount; i++)
    {
        csFind = m_LstMid.GetItemText(i, 2);
        if (csFind == szSocket)
        {
            m_LstMid.DeleteItem(i);
            m_nMidCount--;
            break;
        }
    }
    
    TRACE("ssc Server m_nMidCount is %d", m_nMidCount);
    
    //此时需要关闭客户端socket
    closesocket(sMid);
    sMid = INVALID_SOCKET;
    
    return TRUE;
}

BOOL CServerDlg::RecvMidSQL(SOCKET sMid, char* pszBuf, int nBufLength, int nSQLType)
{
    TRACE("ssc Srv RecvMidSQL(%p, %p, %d)", sMid, pszBuf, nBufLength);
    if (sMid == INVALID_SOCKET)
    {
        TRACE("ssc Srv sMid is INVALID_SOCKET!");
        return FALSE;
    }
    
    if (pszBuf == NULL)
    {
        TRACE("ssc Srv pszBuf is NULL!");
        return FALSE;
    }
    
    if (nBufLength <= 0)
    {
        TRACE("ssc Srv nBufLength <= 0!");
        return FALSE;
    }
    //提取处数据包中的客户度套接字，得到结果后再发给中转
    SOCKET sClient = INVALID_SOCKET;
    memcpy(&sClient, pszBuf, sizeof(sClient));
    pszBuf += sizeof(sClient);
    nBufLength -= sizeof(sClient);
    TRACE("ssc Srv SQL ret to client is %p", sClient);
    TRACE("ssc Srv Recv From Mid Sql is %s", pszBuf);

    //执行SQL语句
    m_rs = ExecuteSQLstr(m_conn, pszBuf);

    //对于增删改不需要返回结果
    if (nSQLType == SQL_ADD || 
        nSQLType == SQL_DEL ||
        nSQLType == SQL_UPDATE)
    {
        TRACE("ssc Srv No need SQL Ret RS, ret SQL_SUCCESS only");

        CMySocket skt(sMid);
        CMyPacket sendPkt;

        BYTE btType = SQL_SUCCESS;
        int nBufSize = sizeof(BYTE);
        int nRet = 0;
        if (sendPkt.Append((char*)&btType, sizeof(btType)) == NULL )
        {
            TRACE("ssc Srv sendPkt.Append failed!");
            return FALSE;
        }

        if (sendPkt.Append((char*)&sClient, sizeof(sClient)) == NULL )
        {
            TRACE("ssc Srv sendPkt.Append failed!");
            return FALSE;
        }

        skt.SendPacket(sendPkt);

        return TRUE;
    }

    //将结果返回给中转
    CString csRet = GetRecord(m_rs);
    CMySocket skt(sMid);
    CMyPacket sendPkt;
    
    BYTE btType = SQL_RESULT;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    if (sendPkt.Append((char*)&btType, sizeof(btType)) == NULL )
    {
        TRACE("ssc Srv sendPkt.Append failed!");
        return FALSE;
    }

    if (sendPkt.Append((char*)&sClient, sizeof(sClient)) == NULL )
    {
        TRACE("ssc Srv sendPkt.Append failed!");
        return FALSE;
    }
    
    if (sendPkt.Append(csRet.GetBuffer(0), csRet.GetLength() + 1) == NULL )
    {
        TRACE("ssc Srv sendPkt.Append failed!");
        return FALSE;
    }
    
    skt.SendPacket(sendPkt);

    return TRUE;
}

_RecordsetPtr CServerDlg::ExecuteSQLstr(_ConnectionPtr conn, char *pszSql)
{
    TRACE("ssc Srv CServerDlg::ExecuteSQLstr()");
    //执行一个 SQL 命令
    variant_t t;
    return conn->Execute((_bstr_t)pszSql, &t, adCmdText);
}

CString CServerDlg::GetRecord(const _RecordsetPtr& rs)
{
    TRACE("ssc Srv GetRecord()");
    CString csRet = "";
    //显示记录
    //获取记录集中的列数
    int nCount = rs->Fields->GetCount();
    csRet.Format("%d", nCount);
    TRACE("ssc Srv rs->Fields->GetCount() is %d", nCount);
    csRet += "\n";
    
    //遍历列名
    for (int i = 0; i < nCount; i++)
    {
        CString csTmp = (char*)rs->Fields->Item[(long)i]->Name;
        TRACE("ssc Srv GetRecord ret colname %s", csTmp.GetBuffer(0)) ;
        csRet += csTmp;
        csRet += "\t";
    }
    csRet += "\n";
    
    //遍历记录
    while (!rs->myEOF)
    {
        for (int i = 0; i < nCount; i++)
        {
            CString csTmp = (char*)(_bstr_t)(rs->Fields->Item[(long)i]->Value);
            TRACE("ssc Srv GetRecord ret value %s", csTmp.GetBuffer(0));
            csRet += csTmp;
            csRet += "\t";
        }
        csRet += "\r\n";
        rs->MoveNext();
    }

    return csRet;
}
