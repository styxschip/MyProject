// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include <AFXTEMPL.H>
#include "Client.h"
#include "ClientDlg.h"
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
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
	m_dwPort = 0;
	m_dwClassID = 0;
	m_csClassName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::OnOK()
{
    return;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, LST_TBLCALSS, m_LstTBLClass);
	DDX_Control(pDX, IDC_IPADDRESS, m_IP);
	DDX_Text(pDX, EDT_PORT, m_dwPort);
	DDX_Text(pDX, EDT_CLASSID, m_dwClassID);
	DDX_Text(pDX, EDT_CLASSNAME, m_csClassName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_CONNECT, OnConnect)
	ON_BN_CLICKED(BTN_SELECTCLASS, OnSelectclass)
	ON_BN_CLICKED(IDC_CLASSDEL, OnClassdel)
	ON_BN_CLICKED(BTN_CLASSADD, OnClassadd)
	ON_NOTIFY(LVN_ITEMCHANGED, LST_TBLCALSS, OnItemchangedTblcalss)
	ON_BN_CLICKED(BTN_CLASSUPDATE, OnClassupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
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
    m_IP.SetAddress(htonl(inet_addr("127.0.0.1")));
    m_dwPort = 27149;
    UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint() 
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CClientDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//向中转发送下线通知
    CMyPacket pkt;
    CMySocket mySock(m_sockClient);
    
    BYTE btType = CLT_OFFLINE;
    
    pkt.Append((char*)&btType, sizeof(btType));
    mySock.SendPacket(pkt);
    TRACE("ssc clt send CLT_OFFLINE");

    closesocket(m_sockClient);
    m_sockClient = INVALID_SOCKET;

	return CDialog::DestroyWindow();
}

void CClientDlg::OnConnect() 
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
        TRACE("ssc clt socket() failed[%d]", WSAGetLastError());
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
        TRACE("ssc clt connect() failed[%d]", WSAGetLastError());
        return;
    }
    
    //先启动线程接收数据
    AfxBeginThread(RecvThread, (LPVOID)this);
    
    //连接状态设置为已连接
    GetDlgItem(IDC_CONSTATE)->SetWindowText("已连接");

    
    //连接成功后禁用连接按钮
    GetDlgItem(BTN_CONNECT)->EnableWindow(FALSE);	

    //客户端发送上线通知
    CMyPacket pkt;
    CMySocket mySock(m_sockClient);
    
    BYTE btType = CLT_ONLINE;
    
    pkt.Append((char*)&btType, sizeof(btType));
    mySock.SendPacket(pkt);
}

UINT CClientDlg::RecvThread(LPVOID lParam)
{
    CClientDlg *pCltDlg = (CClientDlg*)lParam;
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
        case SQL_RESULT:  //处理中转返回的结果
            {
                pCltDlg->GetMidRet(pszBuf + 1, nBufSize - 1);
                break;
            }
        case SQL_SUCCESS:
            {
                TRACE("ssc clt SQL_SUCCESS");
                pCltDlg->OnSelectclass();
                break;
            }
        default:
            break;
        }        
    }  
    
    return TRUE;    
}

/*
void CClientDlg::GetMidRet(char* pszBuf, int nBufSize)
{
    HANDLE hFile = CreateFile("text.dat",           // open MYFILE.TXT 
        GENERIC_ALL,              // open for reading 
        FILE_SHARE_WRITE,           // share for reading 
        NULL,                      // no security 
        CREATE_ALWAYS,             // existing file only 
        FILE_ATTRIBUTE_NORMAL,     // normal file 
        NULL);  

    DWORD dwNumberOfBytesWritten = 0;
    WriteFile(
        hFile,
        (LPCVOID)pszBuf,
        (DWORD)nBufSize, 
        &dwNumberOfBytesWritten,
        NULL);

    CloseHandle(hFile);
}
*/

void CClientDlg::GetMidRet(char* pszBuf, int nBufSize)
{
    TRACE("ssc clt CClientDlg::GetMidRe(%p, %d) begin!", pszBuf, nBufSize);
    if (pszBuf == NULL)
    {
        return;
    }
    if (nBufSize <= 0)
    {
        return;
    }
    
    int i = 0;

    //清空旧数据
    m_LstTBLClass.DeleteAllItems();
    
    int nColCount = m_LstTBLClass.GetHeaderCtrl()->GetItemCount();
    
    // Delete all of the columns.
    for (i = 0; i < nColCount; i++)
    {
        m_LstTBLClass.DeleteColumn(0);
    }

    //初始化ListCtrl
    m_LstTBLClass.SetExtendedStyle(m_LstTBLClass.GetExtendedStyle()|
                                   LVS_EX_GRIDLINES|
                                   LVS_EX_FULLROWSELECT);


    //解析中转返回的结果
    CString csColNum;    
    AfxExtractSubString(csColNum, pszBuf, 0, '\n');

    int nTableCol = 0;
    nTableCol = strtoul(csColNum.GetBuffer(0), NULL, 10);
    TRACE("ssc clt table Column num is %d", nTableCol);

    pszBuf += csColNum.GetLength() + 1; //+1表示略过数字后面的'\n'    
    CString csRet = pszBuf;
    
    //取出列名和内容,aryStr[0]是列名,其余是内容
    CArray<CString, CString> aryStr;
    aryStr.Add("");

    int nRow = 0;
    while (TRUE)
    {
        AfxExtractSubString(aryStr[nRow], pszBuf, nRow, '\n');
        if (aryStr[nRow].IsEmpty() != TRUE)
        {
            aryStr.Add(aryStr[nRow]);
        }
        else
        {
            break;
        }
        nRow++;
    }

    for (i = 0; i < nTableCol; i++)
    {
        CString csTmp;
        AfxExtractSubString(csTmp, aryStr[0], i, '\t');
        TRACE("ssc clt table Column[%d] name is %s", i, csTmp);
        m_LstTBLClass.InsertColumn(i, csTmp, LVCFMT_LEFT, 100);
    } 

    for (i = 1; i < nRow; i++)
    {
        for (int j = 0; j < nTableCol; j++)
        {
            CString csValue;
            AfxExtractSubString(csValue, aryStr[i], j, '\t');
            if (j == 0)
            {
                m_LstTBLClass.InsertItem(i - 1, csValue);
                TRACE("ssc clt table value is %s", csValue);
            }
            else
            {
                m_LstTBLClass.SetItemText(i - 1, j, csValue);
                TRACE("ssc clt table value is %s", csValue);
            }            
        }
    }

    // 根据内容，自动调整CListCtrl的列宽 
    int nColumnCount = m_LstTBLClass.GetItemCount();  
    
    for(i = 0; i < nColumnCount; i++)  
    {  
        m_LstTBLClass.SetColumnWidth(i, LVSCW_AUTOSIZE);  
        int nColumnWidth = m_LstTBLClass.GetColumnWidth(i); 
        
        m_LstTBLClass.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  
        int nHeaderWidth = m_LstTBLClass.GetColumnWidth(i);  
        
        m_LstTBLClass.SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));  
    } 
}

void CClientDlg::SQLRun(char* pszSQL, int nSQLType) 
{
	// TODO: Add your control notification handler code here
    CMySocket skt(m_sockClient);
    CMyPacket sendPkt;
    
    BYTE btType = nSQLType;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    if (sendPkt.Append((char*)&btType, sizeof(btType)) == NULL )
    {
        TRACE("ssc clt sendPkt.Append failed!");
        return;
    }

    CString csSql = pszSQL;

    char *pszBuf = csSql.GetBuffer(0);
    
    if (sendPkt.Append(pszBuf, strlen(pszBuf) + 1) == NULL )
    {
        TRACE("ssc clt sendPkt.Append failed!");
        return;
    }
    
    skt.SendPacket(sendPkt);

// 如果pszBuf需要new时再处理
//     if (pszBuf != NULL)
//     {
//         delete[] pszBuf;
//         pszBuf = NULL;
//     }
}

void CClientDlg::OnSelectclass() 
{
	// TODO: Add your control notification handler code here
	SQLRun("SELECT * FROM tbl_class", SQL_SELECT);
}

void CClientDlg::OnClassdel() 
{
	// TODO: Add your control notification handler code here
    int nItem = m_LstTBLClass.GetNextItem(-1, LVNI_SELECTED);
    if (nItem == -1)
    {
        TRACE("ssc clt del m_LstTBLClass ret -1");
        return;
    }    
    
    //获取指定行指定列的值
    CString csSel = m_LstTBLClass.GetItemText(nItem, 0);

    char szBuf[MAXBYTE] = {0};
    wsprintf(szBuf, 
             "DELETE FROM tbl_class WHERE cid = %d", 
             strtoul(csSel.GetBuffer(0), NULL, 10));

	SQLRun(szBuf, SQL_DEL);

    //Sleep(1000); //这里有 bug, Mid Recv 的时候数据包 size 值异常大导致 new 失败
    //OnSelectclass(); //已经改为服务端返回成功消息再通过mid通知客户端更新
}

void CClientDlg::OnClassadd() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    char szBuf[MAXBYTE] = {0};
    wsprintf(szBuf, 
            "INSERT INTO tbl_class (cid, cname) VALUES (%d, '%s');", 
            m_dwClassID,
            m_csClassName.GetBuffer(0));

    SQLRun(szBuf, SQL_ADD);
}

void CClientDlg::OnItemchangedTblcalss(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
    if (pNMListView->uOldState == 0)
    {
        CString csID = m_LstTBLClass.GetItemText(pNMListView->iItem, 0);
        m_dwClassID = strtoul(csID.GetBuffer(0), NULL, 10);

        m_csClassName = m_LstTBLClass.GetItemText(pNMListView->iItem, 1);
    }
    UpdateData(FALSE);
	*pResult = 0;
}

void CClientDlg::OnClassupdate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

    char szBuf[MAXBYTE] = {0};
    wsprintf(szBuf, 
        "UPDATE tbl_class SET cname = '%s'  WHERE cid = '%d';", 
        m_csClassName.GetBuffer(0),
        m_dwClassID);
    
    SQLRun(szBuf, SQL_ADD);
}
