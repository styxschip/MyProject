// play_airplaneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "play_airplane.h"
#include "play_airplaneDlg.h"
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
// CPlay_airplaneDlg dialog

CPlay_airplaneDlg::CPlay_airplaneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlay_airplaneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlay_airplaneDlg)
	m_ProcessID = 0;
	m_BulletNum = 0;
	m_Mode = -1;
	m_Shift = 0;
	m_AI = FALSE;
	m_bAways = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlay_airplaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPlay_airplaneDlg)
	DDX_Text(pDX, IDC_EDIT1, m_ProcessID);
	DDX_Text(pDX, IDC_EDIT2, m_BulletNum);
	DDX_Radio(pDX, IDC_RADIO1, m_Mode);
	DDX_Text(pDX, IDC_EDIT3, m_Shift);
    DDX_Check(pDX, IDC_CHECKAI, m_AI);
    DDX_Control(pDX, IDC_SPIN1, m_Spin);
	DDX_Check(pDX, IDC_CHECK_LIVEAWAYS, m_bAways);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlay_airplaneDlg, CDialog)
	//{{AFX_MSG_MAP(CPlay_airplaneDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonFindProcess)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioMode)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioMode2)
	ON_BN_CLICKED(IDC_BUTTON_NUM, OnButtonSetNum)
	ON_BN_CLICKED(IDC_BUTTON_SHIFT, OnButtonShift)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpinPen)
	ON_BN_CLICKED(IDC_CHECKAI, OnCheckAI)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonSetInitZero)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_LIVEAWAYS, OnCheckLiveaways)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlay_airplaneDlg message handlers

BOOL CPlay_airplaneDlg::OnInitDialog()
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
    m_hProcess = NULL;
    m_airplaneX = 0;
    m_airplaneY = 0;
    m_IsFind = TRUE;

	m_Spin.SetBuddy(GetDlgItem(IDC_EDIT3));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlay_airplaneDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlay_airplaneDlg::OnPaint() 
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
HCURSOR CPlay_airplaneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CPlay_airplaneDlg::FindGameProc()
{
    HWND hWnd = ::FindWindow("wcTKKN", NULL);
    if (hWnd == NULL)
    {
        return FALSE;
    }
    GetWindowThreadProcessId(hWnd, &m_ProcessID);
    if (m_ProcessID == 0)
    {
        return FALSE;
    }
    
    m_hProcess = OpenProcess(
        PROCESS_ALL_ACCESS,
        FALSE,
        m_ProcessID);

    if (m_hProcess == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

void CPlay_airplaneDlg::OnButtonFindProcess() 
{
	// TODO: Add your control notification handler code here
    if (FindGameProc() == FALSE)
    {
        AfxMessageBox("没找到游戏进程!");
        return;
    }
        
    //获取当前为普通模式还是无敌模式
    char szBuf[MAXBYTE] = {0};
    SIZE_T nReadedBytes = 0;
    SIZE_T nWriteedBytes = 0;
    
    BOOL bRet = ReadProcessMemory(
                            m_hProcess,
                            (LPVOID)0x00403616,
                            szBuf,
                            1,
                            &nReadedBytes);
    if (!bRet)
    {
        AfxMessageBox("读取内存失败");
        m_Mode = -1;
    }
    else if (szBuf[0] == '\xEB') //获取普通还是无敌
    {
        m_Mode = 1;
    }
    else
    {
        m_Mode = 0;
    }

    //获取子弹数量0x00406DA8
    DWORD dwSum = 0;
    bRet = ReadProcessMemory(
                        m_hProcess,
                        (LPVOID)0x00406DA8,
                        &dwSum,
                        4,
                        &nReadedBytes);
    m_BulletNum = dwSum;

    //获得速度0x004020F5
    bRet = ReadProcessMemory(
                    m_hProcess,
                    (LPVOID)0x004020F5,
                    szBuf,
                    1,
                    &nReadedBytes);

    m_Shift = szBuf[0];

    //当前飞机位置
    //0x00406D6C   4   nplanX,
    //0x00406D70   4   nplanY,
    bRet = ReadProcessMemory(
                m_hProcess,
                (LPVOID)0x00406D6C,
                &m_airplaneX,
                4,
                &nReadedBytes);
    
    bRet = ReadProcessMemory(
                m_hProcess,
                (LPVOID)0x00406D70,
                &m_airplaneY,
                4,
                &nReadedBytes);

    UpdateData(FALSE); //设置数据
}

void CPlay_airplaneDlg::OnRadioMode() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); //获取数据

    char szBuf[MAXBYTE] = {0};
    SIZE_T nReadedBytes = 0;
    SIZE_T nWriteedBytes = 0;
    BOOL bRet = FALSE;
    //普通模式
    szBuf[0] = '\x74';
    bRet = WriteProcessMemory(
                        m_hProcess,
                        (LPVOID)0x00403616,
                        szBuf,
                        1,
                        &nWriteedBytes);
    if (!bRet)
    {
        AfxMessageBox("修改内存失败");
    }

    UpdateData(FALSE); //设置数据
}

void CPlay_airplaneDlg::OnRadioMode2() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); //获取数据

    char szBuf[MAXBYTE] = {0};
    SIZE_T nReadedBytes = 0;
    SIZE_T nWriteedBytes = 0;
    BOOL bRet = FALSE;

    szBuf[0] = '\xEB';

    bRet = WriteProcessMemory(
                        m_hProcess,
                        (LPVOID)0x00403616,
                        szBuf,
                        1,
                        &nWriteedBytes);
    if (!bRet)
    {
        AfxMessageBox("修改内存失败");
    }
    
    UpdateData(FALSE); //设置数据
}

void CPlay_airplaneDlg::OnButtonSetNum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); //获取数据

    //设置子弹数量0x00406DA8
    BOOL bRet = FALSE;
    
    DWORD dwWrtSize = -1;
    bRet = WriteProcessMemory(
                    m_hProcess,
                    (LPVOID)0x00406DA8,
                    &m_BulletNum,
                    4,
                    &dwWrtSize);
    if (!bRet || dwWrtSize < 4)
    {
        AfxMessageBox("修改内存失败");
    }
    
    UpdateData(FALSE); //设置数据
}

void CPlay_airplaneDlg::OnButtonShift() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); //获取数据
    
    //设置速度0x004020F5
    BOOL bRet = FALSE;    
    DWORD dwWrtSize = -1;
    bRet = WriteProcessMemory(
        m_hProcess,
        (LPVOID)0x004020F5,
        &m_Shift,
        1,
        &dwWrtSize);
    if (!bRet || dwWrtSize < 1)
    {
        AfxMessageBox("修改内存失败");
    }
    
    UpdateData(FALSE); //设置数据
	
}

void CPlay_airplaneDlg::OnDeltaposSpinPen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    UpdateData(TRUE);
    if(pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头
    {    
        //对字符做相关处理 , 例如将"b" 变为 "a"    
        if (m_Shift == 0)
        {
            //默认速度0不用处理;
        } 
        if (m_Shift >= 1)
        {
            m_Shift--;
        } 
        if (m_Shift > 100)
        {
            m_Shift = 100;
        }
    }
    else if(pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
    {
        //对字符做相关处理 , 例如将"a" 变为 "b"
        if (m_Shift < 100)
        {
            m_Shift++;
        }
        else
        {
            m_Shift = 100;
        }
    }
    UpdateData(FALSE);
	*pResult = 0;
}

void CPlay_airplaneDlg::OnCheckAI() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_AI == TRUE)
    {
        SetTimer(MYTIMER, 50, NULL);
    }
    else
    {
        //正常模式
        AIFun(FALSE);
        KillTimer(MYTIMER);
    }
    UpdateData(FALSE);    
}

BOOL CPlay_airplaneDlg::CheckInRgn(stBullet *pBuf, 
                                   int nCount, 
                                   DWORD airplaneX, 
                                   DWORD airplaneY,
                                   DWORD nRgn)
{
    
    BOOL bRet = FALSE;
    POINT bPoint = {0}; 

    //创建飞机矩形区域
    POINT ptAry[2] = {0};
    
    ptAry[0].x = airplaneX + nRgn;
    ptAry[0].y = airplaneY + nRgn;
    
    ptAry[1].x = airplaneX - nRgn;
    ptAry[1].y = airplaneY - nRgn;
    

    DWORD dwX = 0;
    DWORD dwY = 0;
    
    for (int i = 0; i < nCount; i++)
    {
        dwX = (pBuf[i].dwX) >> 6;
        dwX -= 4;

        dwY = (pBuf[i].dwY) >> 6;
        dwY -= 4;

        bPoint.x = dwX;   
        bPoint.y = dwY;
        
        //判断bPoint点是否在矩形区域内，这个方法效率低
        //if (rgn.PtInRegion(bPoint))
        if (bPoint.x >= ptAry[1].x && bPoint.x <= ptAry[0].x &&
            bPoint.y <= ptAry[0].y && bPoint.y >= ptAry[1].y)
        {
            return TRUE;
        }        
    }
    return FALSE;
}

void CPlay_airplaneDlg::AIFun(BOOL bIsAuto)
{  
    if (bIsAuto == FALSE)
    {
        return;
    }
    BOOL bRet = FALSE;
    
    int nCount = -1;
    DWORD dwWrtSize = -1;
    DWORD nReadedBytes = -1;
    
    while (TRUE)
    {
        if (FindGameProc() == FALSE)
        {
            return;
        }
        //读取子弹个数0x00406DA8
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406DA8,
            &nCount,
            4,
            &dwWrtSize);
        
        //子弹坐标
        //[DWORD]x [DWORD]y [char[7]]未知数据
        stBullet *pBuf = new stBullet[nCount];
        ASSERT(pBuf);
        
        //在Ontimer里释放pBuf
        if (pBuf != NULL)
        {
            m_LstRelease.AddTail(pBuf);
        }
        //不用释放
        //memset(pBuf, 0, nCount * sizeof(stBullet));
        
        //读取子弹数组首地址0x00406E10
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406E10,
            pBuf,
            nCount * sizeof(stBullet),
            &dwWrtSize);
        
        //设置飞机坐标
        //0x00406D6C   4   nplanX,
        //0x00406D70   4   nplanY,     
        //         
        // 左上角 x == 0， y == e0
        // 右上角 x == 130， y == e0
        // 左下角 x == 0， y == 0
        // 右下角 x == 130  y == 0
        //
        //飞机和子弹的原点在游戏区左下角
        //
        //当前飞机位置
        //0x00406D6C   4   nplanX,
        //0x00406D70   4   nplanY,
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D6C,
            &m_airplaneX,
            4,
            &nReadedBytes);
        
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D70,
            &m_airplaneY,
            4,
            &nReadedBytes);
        
        DWORD airplaneX = m_airplaneX;
        DWORD airplaneY = m_airplaneY;
        m_IsFind = CheckInRgn(pBuf, nCount, airplaneX, airplaneY, 15);

        while (m_IsFind == TRUE)
        { 
            //飞机即将被打中，需要换坐标
            airplaneX += 5;
            if (airplaneX > 0x130)
            {
                airplaneY += 5;
                airplaneX = 10;
            }
            if (airplaneY > 0xDE)
            {
                airplaneY = 10;
            }        
            
            m_IsFind = CheckInRgn(pBuf, nCount, airplaneX, airplaneY, 15);
        } // while (m_IsFind == TRUE) 
        
        //说明遍历了所有子弹坐标都没打中飞机，设置飞机坐标
        m_airplaneX = airplaneX;
        m_airplaneY = airplaneY;
        
        //设置飞机坐标
        //0x00406D6C   4   nplanX,
        //0x00406D70   4   nplanY,
        bRet = WriteProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D6C,
            &m_airplaneX,
            4,
            &dwWrtSize);
        
        bRet = WriteProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D70,
            &m_airplaneY,
            4,
            &dwWrtSize);
        return;
    } //end while (TRUE)    
    return;
}

void CPlay_airplaneDlg::OnButtonSetInitZero() 
{
	// TODO: Add your control notification handler code here
	//0x0040469E   4    初始子弹个数
    DWORD dwWrtSize = -1;
    DWORD dwNum = 0;
    BOOL bRet = WriteProcessMemory(
                            m_hProcess,
                            (LPVOID)0x0040469E,
                            &dwNum,
                            4,
                            &dwWrtSize);

}

void CPlay_airplaneDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	KillTimer(MYTIMER);
	// TODO: Add your message handler code here
}

void CPlay_airplaneDlg::OnOK() 
{
	// TODO: Add extra validation here
    KillTimer(MYTIMER);
	CDialog::OnOK();
}

void CPlay_airplaneDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	KillTimer(MYTIMER);
	CDialog::OnCancel();
}

void CPlay_airplaneDlg::OnTimer(UINT nIDEvent) 
{
    if (FindGameProc() == FALSE)
    {
        return;
    }
    AIFun(TRUE);
    POSITION pos = m_LstRelease.GetHeadPosition();
    while (pos)
    {
        stBullet *pTmp = m_LstRelease.GetNext(pos);
        if (pTmp != NULL)
        {
            delete[] pTmp;
            pTmp = NULL;
        }
    }
    m_LstRelease.RemoveAll();
   
}

void CPlay_airplaneDlg::OnCheckLiveaways() 
{
	// TODO: Add your control notification handler code here
    if (FindGameProc() == FALSE)
    {
        return;
    }

    UpdateData(TRUE);
	m_bAways =  TRUE;
    UpdateData(FALSE);
    KillTimer(MYTIMER);

    while (m_bAways)
    {
        AIFun(TRUE);
        POSITION pos = m_LstRelease.GetHeadPosition();
        while (pos)
        {
            stBullet *pTmp = m_LstRelease.GetNext(pos);
            if (pTmp != NULL)
            {
                delete[] pTmp;
                pTmp = NULL;
            }
        }
        m_LstRelease.RemoveAll();
    }   
}
