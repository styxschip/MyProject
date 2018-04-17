// MyCFFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyCFF.h"
#include "MyCFFDlg.h"
#include "DlgINT.h"

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
// CMyCFFDlg dialog

//数据大小字符串数组
char *g_szSizeAry[] = {
    "BYTE",
    "WORD",
    "DWORD",
};

//格式化16进制的字符串
char *g_szHexFmtAry[] = {
    "%02X",
    "%04X",
    "%08X",
};
     
//结构体成员太多所以写了个宏
// 宏 各个参数
//m_LstMemberOfStruct, szBuf, 1,  "MajorLinkerVersion", pOp_Header->MajorLinkerVersion, pOp_Header->Magic,  
//FULL_MEMBER(lst, buf, index, str, m, l)
//list控件, 字符串缓冲, 第几个成员, 成员名, 成员偏移, 上一个成员偏移
#define FULL_MEMBER(lst, buf, index, str, m, l)  lst.InsertItem(index, str, 0);\
                                                 dwMemOffset += sizeof(l);\
                                                 wsprintf(buf, GetHexFmt(sizeof(dwMemOffset)), dwMemOffset); \
                                                 lst.SetItemText(index, 1, buf);\
                                                 lst.SetItemText(index, 2, GetSize(sizeof(m)));\
                                                 wsprintf(szBuf, GetHexFmt(sizeof(m)), m); \
                                                 lst.SetItemText(index, 3, szBuf);

//FULL_MEMBER 处理第一个成员，偏移手动输入，但是sizeof没有得0的情况所以单独处理
#define FULL_FIRSTMEMBER(lst, buf, offset, index, str, m)  lst.InsertItem(index, str, 0);\
                                                 wsprintf(buf, "%08X", offset); \
                                                 lst.SetItemText(index, 1, buf);\
                                                 lst.SetItemText(index, 2, GetSize(sizeof(m)));\
                                                 wsprintf(szBuf, GetHexFmt(sizeof(m)), m); \
                                                 lst.SetItemText(index, 3, szBuf);


CMyCFFDlg::CMyCFFDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCFFDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyCFFDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pExeFileBuf = NULL;
    m_hExeFile = NULL;

    m_e_lfanew = 0;
    m_dwNumberOfSections = 0;
    m_SizeOfHeaders = 0;
    m_NumberOfRvaAndSizes = 0;
    m_ImageBase = 0;

    m_dwFileSize = 0;
    m_dwHighSize = 0;

    m_SectionAlignment = 0;
    m_FileAlignment = 0;
}

void CMyCFFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCFFDlg)
	DDX_Control(pDX, LST_STRUCT, m_LstMemberOfStruct);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyCFFDlg, CDialog)
	//{{AFX_MSG_MAP(CMyCFFDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPENEXE, OnOpenexe)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree)
	ON_EN_SETFOCUS(EDT_VA, OnSetfocusVa)
	ON_EN_SETFOCUS(EDT_RVA, OnSetfocusRva)
	ON_EN_SETFOCUS(EDT_FILEOFFSET, OnSetfocusFileoffset)
	ON_EN_CHANGE(EDT_VA, OnChangeVa)
	ON_EN_CHANGE(EDT_RVA, OnChangeRva)
	ON_EN_CHANGE(EDT_FILEOFFSET, OnChangeFileoffset)
	ON_NOTIFY(NM_CLICK, LST_STRUCT, OnClickStruct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCFFDlg message handlers

BOOL CMyCFFDlg::OnInitDialog()
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
    //SetDlgItemText(EDT_EXEPATH, "winmine.exe");
    SetDlgItemText(EDT_EXEPATH, "OllyICE.exe");
    
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyCFFDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyCFFDlg::OnPaint() 
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
HCURSOR CMyCFFDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyCFFDlg::OnOpenexe() 
{
	// TODO: Add your control notification handler code here
    char szPath[MAX_PATH] = {0};
    BOOL bRet = FALSE;
    bRet = GetDlgItemText(EDT_EXEPATH, szPath, MAX_PATH);
    if (bRet == FALSE)
    {
        AfxMessageBox("GetDlgItemText 失败");
        return;
    }
    
    if (IsPEFile(szPath) != TRUE)
    {
        AfxMessageBox("不是PE文件");
        return;
    }

    InitControls(szPath);

    //有些数据要立刻获取    
    PIMAGE_DOS_HEADER pDos_stub = NULL;
    pDos_stub = (PIMAGE_DOS_HEADER)m_pExeFileBuf;
    m_e_lfanew = pDos_stub->e_lfanew;

    PIMAGE_FILE_HEADER pFile_Header = NULL;
    pFile_Header = (PIMAGE_FILE_HEADER)(m_pExeFileBuf + m_e_lfanew + sizeof(DWORD));

    m_dwNumberOfSections = pFile_Header->NumberOfSections;

    PIMAGE_OPTIONAL_HEADER pOp_Header = NULL;
    
    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    pOp_Header = (PIMAGE_OPTIONAL_HEADER)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14);

    m_SizeOfHeaders = pOp_Header->SizeOfHeaders;
    m_NumberOfRvaAndSizes = pOp_Header->NumberOfRvaAndSizes;
    m_ImageBase = pOp_Header->ImageBase;
    char szText[MAXBYTE] = {0};
    wsprintf(szText, "%08X", m_ImageBase);
    SetDlgItemText(EDT_IMAGEBASE, szText);

    m_SectionAlignment = pOp_Header->SectionAlignment;
    m_FileAlignment = pOp_Header->FileAlignment;
}

BOOL CMyCFFDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pExeFileBuf != NULL)
    {
        delete[] m_pExeFileBuf;
        m_pExeFileBuf = NULL;
    }
    if(m_hExeFile != NULL)
    {
        CloseHandle(m_hExeFile);
    }
	return CDialog::DestroyWindow();
}

void CMyCFFDlg::InitControls(char *szExe)
{
    //设置整行选中 显示网格线
    m_LstMemberOfStruct.SetExtendedStyle(m_LstMemberOfStruct.GetExtendedStyle()|
                                         LVS_EX_GRIDLINES|
                                         LVS_EX_FULLROWSELECT);
    //控件上的数据都删掉
    m_LstMemberOfStruct.DeleteAllItems();

    //保存坐标
    m_LstMemberOfStruct.GetWindowRect(&m_rcListCtrl);

    m_Tree.DeleteAllItems();

    char drive[_MAX_DRIVE] = {0};
    char dir[_MAX_DIR] = {0};
    char fname[_MAX_FNAME] = {0};
    char ext[_MAX_EXT] = {0};
    
    _splitpath(szExe, drive, dir, fname, ext);

    char szBuf[MAX_PATH] = {"File: "};
    strcat(szBuf, fname);
    strcat(szBuf, ext);
    
    HTREEITEM hRoot = m_Tree.InsertItem(szBuf, 1, 1);
    m_Tree.InsertItem("IMAGE_DOS_HEADER", hRoot);

    HTREEITEM hNT_HEADERS = m_Tree.InsertItem("IMAGE_NT_HEADERS", hRoot);
    m_Tree.InsertItem("IMAGE_SECTION_HEADER[x]", hRoot);
    m_Tree.InsertItem("Import Directory", hRoot);
    m_Tree.InsertItem("Export Directory", hRoot);
    m_Tree.InsertItem("Relocation Directory", hRoot);
    m_Tree.InsertItem("Resource Directory", hRoot);

    m_Tree.InsertItem("IMAGE_FILE_HEADER", hNT_HEADERS);
    HTREEITEM hOp_Headers = m_Tree.InsertItem("IMAGE_OPTIONAL_HEADER", hNT_HEADERS);
    m_Tree.InsertItem("IMAGE_DATA_DIRECTORY[x]", hOp_Headers);

    m_Tree.Expand(hRoot, TVE_EXPAND);
    m_Tree.Expand(hNT_HEADERS, TVE_EXPAND);
    m_Tree.Expand(hOp_Headers, TVE_EXPAND);
    return;
}

BOOL CMyCFFDlg::IsPEFile(char *szPath)
{   
    BOOL bRet = FALSE;
    if (m_hExeFile != NULL)
    {
        CloseHandle(m_hExeFile);
    }
    m_hExeFile = CreateFile(szPath,
                      GENERIC_READ,
                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
    if (m_hExeFile == NULL)
    {
        AfxMessageBox("打开文件失败");
        return FALSE;
    }
    
    m_dwFileSize = GetFileSize(m_hExeFile, &m_dwHighSize);
    if (m_dwFileSize == INVALID_FILE_SIZE && m_dwHighSize == 0)
    {
        AfxMessageBox("GetFileSize ret 0");
        return FALSE;
    }
    
    if (m_pExeFileBuf != NULL)
    {
        delete[] m_pExeFileBuf;
        m_pExeFileBuf = NULL;
    }
    
    m_pExeFileBuf = new char[m_dwFileSize];
    
    DWORD dwReadBytes = 0;
    bRet = ReadFile(m_hExeFile, m_pExeFileBuf, m_dwFileSize, &dwReadBytes, NULL);
    if (bRet == FALSE || dwReadBytes <= 0)
    {
        AfxMessageBox("ReadFile read 0");
        return FALSE;
    }
    
    WORD wMZ = 0x5a4d;
    DWORD dwPE = 0x4550;
    if (*(WORD *)m_pExeFileBuf != wMZ)
    {
        return FALSE;
    }
    
    DWORD dwPESigAddr = (DWORD)m_pExeFileBuf;
    dwPESigAddr += offsetof(IMAGE_DOS_HEADER, e_lfanew);

    DWORD dwFlagAddr = *(DWORD *)dwPESigAddr;
    dwFlagAddr += (DWORD)m_pExeFileBuf;

    if (*(DWORD *)dwFlagAddr != dwPE)
    {
        return FALSE;
    }

    return TRUE;
}
char* CMyCFFDlg::GetSize(DWORD dwSize)
{
    switch (dwSize)
    {
    case 1:
	    return g_szSizeAry[0]; // BYTE
    case 2: 
        return g_szSizeAry[1]; // WORD
    case 4:
        return g_szSizeAry[2]; // DWORD
    }
    return NULL;
}

char* CMyCFFDlg::GetHexFmt(DWORD dwSize)
{
    switch (dwSize)
    {
    case 1:
        return g_szHexFmtAry[0]; // BYTE %02X
    case 2: 
        return g_szHexFmtAry[1]; // WORD %04X
    case 4:
        return g_szHexFmtAry[2]; // DWORD %08X
    }
    return NULL;
}

void CMyCFFDlg::ShowDosHeader(CListCtrl& listCtrl)
{
    char *szMemberAry[] = {
        "e_magic",
        "e_cblp",
        "e_cp",
        "e_crlc",
        "e_cparhdr",
        "e_minalloc",
        "e_maxalloc",
        "e_ss",
        "e_sp",
        "e_csum",
        "e_ip",
        "e_cs",
        "e_lfarlc",
        "e_ovno",
        "e_res", //ary 4
        "e_oemid",
        "e_oeminfo",
        "e_res2", // ary 10
        "e_lfanew"
    };

    PIMAGE_DOS_HEADER pDos_stub = NULL;
    pDos_stub = (PIMAGE_DOS_HEADER)m_pExeFileBuf;

    DWORD dwMemOffset = 0; 
    char szBuf[MAXBYTE] = {0};
    //显示 dos stub
    m_LstMemberOfStruct.InsertColumn(1, "Member", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(2, "Offset", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(3, "Size", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(4, "Value", LVCFMT_LEFT, 110);

    // 0 e_magic
    FULL_FIRSTMEMBER(m_LstMemberOfStruct, szBuf, 0, 0,
        szMemberAry[0],
        pDos_stub->e_magic);

//     listCtrl.InsertItem(0, "e_magic", 0);
//     wsprintf(szBuf, "%08X", 0);
//     listCtrl.SetItemText(0, 1, szBuf);
//     listCtrl.SetItemText(0, 2, GetSize(sizeof(pDos_stub->e_magic)));
//     wsprintf(szBuf, "%04X", pDos_stub->e_magic);
//     listCtrl.SetItemText(0, 3, szBuf);

    // 1 e_cblp
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 1,
        szMemberAry[1],
        pDos_stub->e_cblp, pDos_stub->e_magic);
    
    // 2 e_cp
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 2,
        szMemberAry[2],
        pDos_stub->e_cp, pDos_stub->e_cblp);

    // 3 e_crlc
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 3,
        szMemberAry[3],
        pDos_stub->e_crlc, pDos_stub->e_cp);

    // 4 e_cparhdr
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 4,
        szMemberAry[4],
        pDos_stub->e_cparhdr, pDos_stub->e_crlc);

    // 5 e_minalloc
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 5,
        szMemberAry[5],
        pDos_stub->e_minalloc, pDos_stub->e_cparhdr);

    // 6 e_maxalloc
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 6,
        szMemberAry[6],
        pDos_stub->e_maxalloc, pDos_stub->e_minalloc);

    // 7 e_ss
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 7,
        szMemberAry[7],
        pDos_stub->e_ss, pDos_stub->e_maxalloc);

    // 8 e_sp
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 8,
        szMemberAry[8],
        pDos_stub->e_sp, pDos_stub->e_ss);

    // 9 e_csum
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 9,
        szMemberAry[9],
        pDos_stub->e_csum, pDos_stub->e_sp);

    // 10 e_ip
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 10,
        szMemberAry[10],
        pDos_stub->e_ip, pDos_stub->e_csum);

    // 11 e_cs
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 11,
        szMemberAry[11],
        pDos_stub->e_cs, pDos_stub->e_ip);

    // 12 e_lfarlc
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 12,
        szMemberAry[12],
        pDos_stub->e_lfarlc, pDos_stub->e_cs);

    // 13 e_ovno
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 13,
        szMemberAry[13],
        pDos_stub->e_ovno, pDos_stub->e_lfarlc);

    // 14 e_res[0]
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 14,
        szMemberAry[14],
        pDos_stub->e_res[0], pDos_stub->e_ovno);

    // 14 e_res[1]
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 15,
        NULL,
        pDos_stub->e_res[1], pDos_stub->e_res[0]);
    
    // 14 e_res[2]
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 16,
        NULL,
        pDos_stub->e_res[2], pDos_stub->e_res[1]);

    // 14 e_res[3]
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 17,
        NULL,
        pDos_stub->e_res[3], pDos_stub->e_res[2]);

    // 15 e_oemid
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 18,
        szMemberAry[15],
        pDos_stub->e_oemid, pDos_stub->e_res[3]);

    // 16 e_oeminfo
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 19,
        szMemberAry[16],
        pDos_stub->e_oeminfo, pDos_stub->e_oemid);

    // 17 e_res[0]
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 20,
        szMemberAry[17],
        pDos_stub->e_res[0], pDos_stub->e_oeminfo);

    for (int i = 1; i < 10; i++)
    {
        // 17 e_res[1] ~ e_res[9]
        FULL_MEMBER(m_LstMemberOfStruct, szBuf, 20 + i,
            NULL,
            pDos_stub->e_res[i], pDos_stub->e_res[i - 1]);
    }

    // 18 e_lfanew
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 30,
        szMemberAry[18],
        pDos_stub->e_lfanew, pDos_stub->e_res[9]);
}

void CMyCFFDlg::ShowNTHeader(CListCtrl& listCtrl)
{
    char szBuf[MAXBYTE] = {0};
    //先获取 IMAGE_NT_HEADERS 的起始地址
    PIMAGE_DOS_HEADER pDos_stub = NULL;
    pDos_stub = (PIMAGE_DOS_HEADER)m_pExeFileBuf;
    
    //显示 IMAGE_NT_HEADERS
    PIMAGE_NT_HEADERS pNt_Head = NULL;
    pNt_Head = (PIMAGE_NT_HEADERS)(m_pExeFileBuf + m_e_lfanew);
    
    m_LstMemberOfStruct.InsertColumn(1, "Member", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(2, "Offset", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(3, "Size", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(4, "Value", LVCFMT_LEFT, 110);

    m_LstMemberOfStruct.InsertItem(0, "Signature", 0);
    wsprintf(szBuf, "%08X", m_e_lfanew); 
    m_LstMemberOfStruct.SetItemText(0, 1, szBuf);
    
    listCtrl.SetItemText(0, 2, GetSize(sizeof(pNt_Head->Signature)));

    wsprintf(szBuf, "%08X", pNt_Head->Signature);
    m_LstMemberOfStruct.SetItemText(0, 3, szBuf);
}

void CMyCFFDlg::ShowFileHeader(CListCtrl& listCtrl)
{
    char szBuf[MAXBYTE] = {0};
    DWORD dwMemOffset = 0;

    PIMAGE_DOS_HEADER pDos_stub = NULL;
    pDos_stub = (PIMAGE_DOS_HEADER)m_pExeFileBuf;    

    PIMAGE_FILE_HEADER pFile_Header = NULL;
    pFile_Header = (PIMAGE_FILE_HEADER)(m_pExeFileBuf + m_e_lfanew + sizeof(DWORD));

    m_LstMemberOfStruct.InsertColumn(1, "Member", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(2, "Offset", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(3, "Size", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(4, "Value", LVCFMT_LEFT, 110);

    //0 Machine
    m_LstMemberOfStruct.InsertItem(0, "Machine", 0);

    dwMemOffset = (DWORD)pFile_Header - (DWORD)m_pExeFileBuf;
    wsprintf(szBuf, "%08X", dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(0, 1, szBuf);

    listCtrl.SetItemText(0, 2, GetSize(sizeof(pFile_Header->Machine)));

    wsprintf(szBuf, "%04X", pFile_Header->Machine); 
    m_LstMemberOfStruct.SetItemText(0, 3, szBuf);

    //1 NumberOfSections
    m_LstMemberOfStruct.InsertItem(1, "NumberOfSections", 0);

    dwMemOffset += sizeof(pFile_Header->Machine);        
    wsprintf(szBuf, "%08X", dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(1, 1, szBuf);

    listCtrl.SetItemText(1, 2, GetSize(sizeof(pFile_Header->NumberOfSections)));
    
    wsprintf(szBuf, "%04X", pFile_Header->NumberOfSections); 
    m_LstMemberOfStruct.SetItemText(1, 3, szBuf);

    //2 TimeDateStamp
    m_LstMemberOfStruct.InsertItem(2, "TimeDateStamp", 0);
    
    dwMemOffset += sizeof(pFile_Header->NumberOfSections);        
    wsprintf(szBuf, "%08X", dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(2, 1, szBuf);

    listCtrl.SetItemText(2, 2, GetSize(sizeof(pFile_Header->TimeDateStamp)));
    
    wsprintf(szBuf, "%08X", pFile_Header->TimeDateStamp); 
    m_LstMemberOfStruct.SetItemText(2, 3, szBuf);

    //3 PointerToSymbolTable
    m_LstMemberOfStruct.InsertItem(3, "PointerToSymbolTable", 0);
    
    dwMemOffset += sizeof(pFile_Header->TimeDateStamp);        
    wsprintf(szBuf, "%08X", dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(3, 1, szBuf);

    listCtrl.SetItemText(3, 2, GetSize(sizeof(pFile_Header->PointerToSymbolTable)));
    
    wsprintf(szBuf, "%08X", pFile_Header->PointerToSymbolTable); 
    m_LstMemberOfStruct.SetItemText(3, 3, szBuf);

    //4 NumberOfSymbols
    m_LstMemberOfStruct.InsertItem(4, "NumberOfSymbols", 0);

    dwMemOffset += sizeof(pFile_Header->PointerToSymbolTable);        
    wsprintf(szBuf, "%08X", dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(4, 1, szBuf);

    listCtrl.SetItemText(4, 2, GetSize(sizeof(pFile_Header->NumberOfSymbols)));
    
    wsprintf(szBuf, "%08X", pFile_Header->NumberOfSymbols); 
    m_LstMemberOfStruct.SetItemText(4, 3, szBuf);

    // 5 SizeOfOptionalHeader
    m_LstMemberOfStruct.InsertItem(5, "SizeOfOptionalHeader", 0);

    dwMemOffset += sizeof(pFile_Header-> PointerToSymbolTable);        
    wsprintf(szBuf, "%08X", dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(5, 1, szBuf);

    listCtrl.SetItemText(5, 2, GetSize(sizeof(pFile_Header->SizeOfOptionalHeader)));
    
    wsprintf(szBuf, "%04X", pFile_Header->SizeOfOptionalHeader); 
    m_LstMemberOfStruct.SetItemText(5, 3, szBuf);

    // 6 Characteristics
    m_LstMemberOfStruct.InsertItem(6, "Characteristics", 0);
    
    dwMemOffset += sizeof(pFile_Header->SizeOfOptionalHeader);        
    wsprintf(szBuf, "%08X", dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(6, 1, szBuf);

    listCtrl.SetItemText(6, 2, GetSize(sizeof(pFile_Header->Characteristics)));
    
    wsprintf(szBuf, "%04X", pFile_Header->Characteristics); 
    m_LstMemberOfStruct.SetItemText(6, 3, szBuf);
}

void CMyCFFDlg::ShowOptionalHeader(CListCtrl& listCtrl)
{    
    char szBuf[MAXBYTE] = {0};
    DWORD dwMemOffset = 0;

    PIMAGE_OPTIONAL_HEADER pOp_Header = NULL;
    
    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    //               m_e_lfanew + 0x4 + 0x14
    pOp_Header = (PIMAGE_OPTIONAL_HEADER)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14);

    m_LstMemberOfStruct.InsertColumn(1, "Member", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(2, "Offset", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(3, "Size", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(4, "Value", LVCFMT_LEFT, 110);
    
    //0 Magic
    m_LstMemberOfStruct.InsertItem(0, "Magic", 0);
    
    dwMemOffset = (DWORD)pOp_Header - (DWORD)m_pExeFileBuf;
    wsprintf(szBuf, GetHexFmt(sizeof(dwMemOffset)), dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(0, 1, szBuf);

    m_LstMemberOfStruct.SetItemText(0, 2, GetSize(sizeof(pOp_Header->Magic)));
    
    wsprintf(szBuf, GetHexFmt(sizeof(pOp_Header->Magic)), pOp_Header->Magic); 
    m_LstMemberOfStruct.SetItemText(0, 3, szBuf);
    
    //1 MajorLinkerVersion
    //m_LstMemberOfStruct, szBuf, 1,  "MajorLinkerVersion", pOp_Header->MajorLinkerVersion, pOp_Header->Magic,  
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 1,
        "MajorLinkerVersion",
        pOp_Header->MajorLinkerVersion, pOp_Header->Magic);
    
    //2 MinorLinkerVersion
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 2,
        "MinorLinkerVersion",
        pOp_Header->MinorLinkerVersion, pOp_Header->MajorLinkerVersion);
    
    // 3 SizeOfCode
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 3,
        "SizeOfCode",
        pOp_Header->SizeOfCode, pOp_Header->MinorLinkerVersion);
    
    // 4 SizeOfInitializedData
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 4,
        "SizeOfInitializedData",
        pOp_Header->SizeOfInitializedData, pOp_Header->SizeOfCode);
    
    // 5 SizeOfUninitializedData
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 5,
        "SizeOfUninitializedData",
        pOp_Header->SizeOfUninitializedData, pOp_Header->SizeOfInitializedData);
    
    // 6 AddressOfEntryPoint
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 6,
        "AddressOfEntryPoint",
        pOp_Header->AddressOfEntryPoint, pOp_Header->SizeOfUninitializedData);
    
    // 7 BaseOfCode
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 7,
        "BaseOfCode",
        pOp_Header->BaseOfCode, pOp_Header->AddressOfEntryPoint);
    
    // 8 BaseOfData
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 8,
        "BaseOfData",
        pOp_Header->BaseOfData, pOp_Header->BaseOfCode);
    
    // 9 ImageBase
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 9,
        "ImageBase",
        pOp_Header->ImageBase, pOp_Header->BaseOfData);

    // 10  SectionAlignment
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 10,
        "SectionAlignment",
        pOp_Header->SectionAlignment, pOp_Header->ImageBase);

    // 11 FileAlignment
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 11,
        "FileAlignment",
        pOp_Header->FileAlignment, pOp_Header->SectionAlignment);

    // 12 MajorOperatingSystemVersion
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 12,
        "MajorOperatingSystemVersion",
        pOp_Header->MajorOperatingSystemVersion, pOp_Header->FileAlignment);

    // 13 MinorOperatingSystemVersion
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 13,
        "MinorOperatingSystemVersion",
        pOp_Header->MinorOperatingSystemVersion, pOp_Header->MajorOperatingSystemVersion);
    
    // 14 MajorImageVersion
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 14,
        "MajorImageVersion",
        pOp_Header->MajorImageVersion, pOp_Header->MinorOperatingSystemVersion);

    // 15 MinorImageVersion
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 15,
        "MinorImageVersion",
        pOp_Header->MinorImageVersion, pOp_Header->MajorImageVersion);

    // 16 MajorSubsystemVersion
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 16,
        "MajorSubsystemVersion",
        pOp_Header->MajorSubsystemVersion, pOp_Header->MinorImageVersion);

    // 17 MinorSubsystemVersion
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 17,
        "MinorSubsystemVersion",
        pOp_Header->MinorSubsystemVersion, pOp_Header->MajorSubsystemVersion);

    // 18 Win32VersionValue
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 18,
        "Win32VersionValue",
        pOp_Header->Win32VersionValue, pOp_Header->MajorSubsystemVersion);

    // 19 SizeOfImage
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 19,
        "SizeOfImage",
        pOp_Header->SizeOfImage, pOp_Header->Win32VersionValue);

    // 20 SizeOfHeaders
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 20,
        "SizeOfHeaders",
        pOp_Header->SizeOfHeaders, pOp_Header->SizeOfImage);

    // 21 CheckSum
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 21,
        "CheckSum",
        pOp_Header->CheckSum, pOp_Header->SizeOfHeaders);

    // 22 Subsystem
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 22,
        "Subsystem",
        pOp_Header->Subsystem, pOp_Header->CheckSum);

    // 23 DllCharacteristics
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 23,
        "DllCharacteristics",
        pOp_Header->DllCharacteristics, pOp_Header->Subsystem);

    // 24 SizeOfStackReserve
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 24,
        "SizeOfStackReserve",
        pOp_Header->SizeOfStackReserve, pOp_Header->DllCharacteristics);

    // 25 SizeOfStackCommit
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 25,
        "SizeOfStackCommit",
        pOp_Header->SizeOfStackCommit, pOp_Header->SizeOfStackReserve);

    // 26 SizeOfHeapReserve
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 26,
        "SizeOfHeapReserve",
        pOp_Header->SizeOfHeapReserve, pOp_Header->SizeOfStackCommit);

    // 27 SizeOfHeapCommit
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 27,
        "SizeOfHeapCommit",
        pOp_Header->SizeOfHeapCommit, pOp_Header->SizeOfHeapReserve);

    // 28 LoaderFlags
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 28,
        "LoaderFlags",
        pOp_Header->LoaderFlags, pOp_Header->SizeOfHeapCommit);

    // 29 NumberOfRvaAndSizes
    FULL_MEMBER(m_LstMemberOfStruct, szBuf, 29,
        "NumberOfRvaAndSizes",
        pOp_Header->NumberOfRvaAndSizes, pOp_Header->LoaderFlags);

}

void CMyCFFDlg::ShowDataDir(CListCtrl& listCtrl)
{    
    char szBuf[MAXBYTE] = {0};
    DWORD dwMemOffset = 0;

//     PIMAGE_OPTIONAL_HEADER pOp_Header = NULL;
//     
//     // DOS_Header + Signature + File_HEADERS 共 0xF0 字节
//     pOp_Header = (PIMAGE_OPTIONAL_HEADER)(m_pExeFileBuf + 0xF0);
// 
//     //计算 IMAGE_DATA_DIRECTORY 数组个数
//     int nCount = pOp_Header->NumberOfRvaAndSizes;
// 
//     //wsprintf(szBuf, "%d", nCount);
//     //nCount = strtol(szBuf, NULL, 16);

    PIMAGE_DATA_DIRECTORY pDat_Dir = NULL;
   
    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    // 所以 IMAGE_DATA_DIRECTORY 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) + offset m_NumberOfRvaAndSizes
    // //                                           m_e_lfanew + 0x4 + 0x14 + 0x60
    pDat_Dir = (PIMAGE_DATA_DIRECTORY)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60);
    dwMemOffset = m_e_lfanew + 0x4 + 0x14 + 0x60;
    
// typedef struct _IMAGE_DATA_DIRECTORY {
//     DWORD   VirtualAddress;
//     DWORD   Size;
// } IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

    char *szMemberAry[] = {
        "EXPORT RVA",
        "EXPORT Size",
        "IMPORT RVA",
        "IMPORT Size",
        "RESOURCE RVA",
        "RESOURCE Size",
        "EXCEPTION RVA",
        "EXCEPTION Size",
        "SECURITY RVA",
        "SECURITY Size",
        "BASERELOC RVA",
        "BASERELOC Size",
        "DEBUG RVA",
        "DEBUG Size",
        "ARCHITECTURE RVA",
        "ARCHITECTURE Size",
        "GLOBALPTR RVA",
        "GLOBALPTR Size",
        "TLS RVA",
        "TLS Size",
        "LOAD_CONFIG RVA",
        "LOAD_CONFIG Size",
        "BOUND_IMPORT RVA",
        "BOUND_IMPORT Size",
        "IAT RVA",
        "IAT Size",
        "DELAY_IMPORT RVA",
        "DELAY_IMPORT Size",
        "COM_DESCRIPTOR RVA",
        "COM_DESCRIPTOR Size",
        "UnKnow RVA",
        "UnKnow Size",
    };


    m_LstMemberOfStruct.InsertColumn(1, "Member", LVCFMT_LEFT, 150);
    m_LstMemberOfStruct.InsertColumn(2, "Offset", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(3, "Size", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(4, "Value", LVCFMT_LEFT, 110);

    // 0  EXPORT RVA
    m_LstMemberOfStruct.InsertItem(0, szMemberAry[0], 0);
    wsprintf(szBuf, GetHexFmt(sizeof(dwMemOffset)), dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(0, 1, szBuf);
    
    m_LstMemberOfStruct.SetItemText(0, 2, GetSize(sizeof(pDat_Dir[0].VirtualAddress)));
    
    wsprintf(szBuf, GetHexFmt(sizeof(pDat_Dir[0].VirtualAddress)), pDat_Dir[0].VirtualAddress); 
    m_LstMemberOfStruct.SetItemText(0, 3, szBuf);

    // 1  EXPORT Size
    m_LstMemberOfStruct.InsertItem(1, szMemberAry[1], 0);
    dwMemOffset += sizeof(pDat_Dir[0].VirtualAddress);
    wsprintf(szBuf, GetHexFmt(sizeof(dwMemOffset)), dwMemOffset); 
    m_LstMemberOfStruct.SetItemText(1, 1, szBuf);
    
    m_LstMemberOfStruct.SetItemText(1, 2, GetSize(sizeof(pDat_Dir[0].Size)));
    
    wsprintf(szBuf, GetHexFmt(sizeof(pDat_Dir[0].Size)), pDat_Dir[0].Size); 
    m_LstMemberOfStruct.SetItemText(1, 3, szBuf);
    
    for (DWORD i = 1; i < m_NumberOfRvaAndSizes; i++)
    {
        if (i < 16)
        {
            FULL_MEMBER(m_LstMemberOfStruct, szBuf, i * 2,
                szMemberAry[i*2],
                pDat_Dir[i].VirtualAddress, sizeof(DWORD));
            
            FULL_MEMBER(m_LstMemberOfStruct, szBuf, i * 2 + 1,
                szMemberAry[i*2+1],
                pDat_Dir[i].Size, sizeof(DWORD));
        }
        else
        {
            FULL_MEMBER(m_LstMemberOfStruct, szBuf, i * 2,
                szMemberAry[30],
                pDat_Dir[i].VirtualAddress, sizeof(DWORD));
            
            FULL_MEMBER(m_LstMemberOfStruct, szBuf, i * 2 + 1,
                szMemberAry[31],
                pDat_Dir[i].Size, sizeof(DWORD));
        }
    }
}

void CMyCFFDlg::ShowSectionHeader(CListCtrl& listCtrl)
{
//     typedef struct _IMAGE_SECTION_HEADER {
//         BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
//         union {
//             DWORD   PhysicalAddress;
//             DWORD   VirtualSize;
//         } Misc;
//         DWORD   VirtualAddress;
//         DWORD   SizeOfRawData;
//         DWORD   PointerToRawData;
//         DWORD   PointerToRelocations;
//         DWORD   PointerToLinenumbers;
//         WORD    NumberOfRelocations;
//         WORD    NumberOfLinenumbers;
//         DWORD   Characteristics;
//     } IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

    char *szMemberNameAry[] = {
            "Name",
            "VirtualSize",
            "VirtualAddress",
            "SizeOfRawData",
            "PointerToRawData",
            "PointerToRelocations",
            "PointerToLinenumbers",
            "NumberOfRelocations",
            "NumberOfLinenumbers",
            "Characteristics"
    };

    char szBuf[MAXBYTE] = {0};
    DWORD dwMemOffset = 0;
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    // IMAGE_SECTION_HEADER 的偏移和 NumberOfRvaAndSizes 有关
    // 截至到NumberOfRvaAndSizes 大小为 offset = m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) + offset NumberOfRvaAndSizes
    //                                           m_e_lfanew + 0x4 + 0x14 + 0x60
    // 所以 IMAGE_SECTION_HEADER 的偏移是 offset + NumberOfRvaAndSizes * _IMAGE_DATA_DIRECTORY
    // = offset + NumberOfRvaAndSizes * (sizeof(DWORD)*2)

    pSectionHeader = (PIMAGE_SECTION_HEADER)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60 +
                                             m_NumberOfRvaAndSizes * 
                                             (sizeof(DWORD) * 2) );

    DWORD i = 0;
    for (i = 0; i < 10; i++)
    {
        m_LstMemberOfStruct.InsertColumn(i+1, 
                                         szMemberNameAry[i],
                                         LVCFMT_LEFT,
                                         80);
    }

    for (i = 0; i < (int)m_dwNumberOfSections; i++)
    {
        memset(szBuf, 0, sizeof(szBuf));
        memcpy(szBuf, pSectionHeader->Name, sizeof(pSectionHeader->Name));
        m_LstMemberOfStruct.InsertItem(i, szBuf, 0);

        wsprintf(szBuf, "%08X", pSectionHeader->Misc.VirtualSize);
        m_LstMemberOfStruct.SetItemText(i, 1, szBuf);

        wsprintf(szBuf, "%08X", pSectionHeader->VirtualAddress);
        m_LstMemberOfStruct.SetItemText(i, 2, szBuf);

        wsprintf(szBuf, "%08X", pSectionHeader->SizeOfRawData);
        m_LstMemberOfStruct.SetItemText(i, 3, szBuf);

        wsprintf(szBuf, "%08X", pSectionHeader->PointerToRawData);
        m_LstMemberOfStruct.SetItemText(i, 4, szBuf);

        wsprintf(szBuf, "%08X", pSectionHeader->PointerToRelocations);
        m_LstMemberOfStruct.SetItemText(i, 5, szBuf);

        wsprintf(szBuf, "%08X", pSectionHeader->PointerToLinenumbers);
        m_LstMemberOfStruct.SetItemText(i, 6, szBuf);

        wsprintf(szBuf, "%04X", pSectionHeader->NumberOfRelocations);
        m_LstMemberOfStruct.SetItemText(i, 7, szBuf);

        wsprintf(szBuf, "%04X", pSectionHeader->NumberOfLinenumbers);
        m_LstMemberOfStruct.SetItemText(i, 8, szBuf);

        wsprintf(szBuf, "%08X", pSectionHeader->Characteristics);
        m_LstMemberOfStruct.SetItemText(i, 9, szBuf);

        char *pTmp = (char *)pSectionHeader;
        pTmp += 0x28;
        pSectionHeader = (PIMAGE_SECTION_HEADER)pTmp;
    }
}

void CMyCFFDlg::ShowIAT(CListCtrl& listCtrl)
{
    char szBuf[MAXBYTE] = {0};
    
    listCtrl.InsertColumn(1, "OriginalFirstThunk", LVCFMT_LEFT, 150);
    listCtrl.InsertColumn(2, "TimeDateStamp", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(3, "ForwarderChain", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(4, "Name", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(5, "FirstThunk", LVCFMT_LEFT, 110);
    
    
    PIMAGE_DATA_DIRECTORY pDat_Dir = NULL;
    
    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    // 所以 IMAGE_DATA_DIRECTORY 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) + offset m_NumberOfRvaAndSizes
    // //                                           m_e_lfanew + 0x4 + 0x14 + 0x60
    pDat_Dir = (PIMAGE_DATA_DIRECTORY)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60);
    
    DWORD dwAddr = (DWORD)pDat_Dir[12].VirtualAddress;
    DWORD dwFA = 0;
    if (RVA2FA(dwAddr, dwFA) == FALSE)
    {
        return;
    }
    PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(m_pExeFileBuf + dwFA);
    
    int i = 0;
    while (TRUE)
    {        
        if (pImport[i].Name == 0)
        {
            break;
        }
        
        DWORD dwRVA = pImport[i].OriginalFirstThunk;
        RVA2FA(dwRVA, dwFA);
        wsprintf(szBuf, "%08X", dwFA);
        listCtrl.InsertItem(i, szBuf, 0);
        
        dwRVA = pImport[i].TimeDateStamp;
        RVA2FA(dwRVA, dwFA);
        wsprintf(szBuf, "%08X", dwFA);
        listCtrl.SetItemText(i, 1, szBuf);
        
        dwRVA = pImport[i].ForwarderChain;
        RVA2FA(dwRVA, dwFA);
        wsprintf(szBuf, "%08X", dwFA);
        listCtrl.SetItemText(i, 2, szBuf);
        
        dwRVA = pImport[i].Name;
        RVA2FA(dwRVA, dwFA);
        wsprintf(szBuf, "%s", m_pExeFileBuf + dwFA);
        listCtrl.SetItemText(i, 3, szBuf);
        
        dwRVA = pImport[i].FirstThunk;
        RVA2FA(dwRVA, dwFA);
        wsprintf(szBuf, "%08X", dwFA);
        listCtrl.SetItemText(i, 4, szBuf);
        
        i++;
    }
}

void CMyCFFDlg::ShowImportDirectory(CListCtrl& listCtrl)
{
    char szBuf[MAXBYTE] = {0};

    listCtrl.InsertColumn(1, "Module Name", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(2, "OriginalFirstThunk(FA)", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(3, "TimeDateStamp", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(4, "ForwarderChain", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(5, "Name", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(6, "FirstThunk", LVCFMT_LEFT, 110);


    PIMAGE_DATA_DIRECTORY pDat_Dir = NULL;
    
    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    // 所以 IMAGE_DATA_DIRECTORY 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) + offset m_NumberOfRvaAndSizes
    // //                                           m_e_lfanew + 0x4 + 0x14 + 0x60
    pDat_Dir = (PIMAGE_DATA_DIRECTORY)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60);

    IMAGE_IMPORT_DESCRIPTOR stEnd = {0};
    
    DWORD dwAddr = (DWORD)pDat_Dir[1].VirtualAddress;
    DWORD dwFA = 0;
    if (RVA2FA(dwAddr, dwFA) == FALSE)
    {
        return;
    }
    PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(m_pExeFileBuf + dwFA);
    
    int i = 0;
    while (TRUE)
    {        
        DWORD dwRVA = pImport[i].FirstThunk;
        RVA2FA(dwRVA, dwFA);
        if (*(DWORD *)(m_pExeFileBuf + dwFA) == 0)
        {
            i++;
            continue;
        }
        if (memcmp(&pImport[i], &stEnd, sizeof(stEnd)) == 0)
        {
            break;
        }

        dwRVA = pImport[i].Name;
        RVA2FA(dwRVA, dwFA);
        wsprintf(szBuf, "%s", m_pExeFileBuf + dwFA);
        listCtrl.InsertItem(i, szBuf, 0);
        
        dwRVA = pImport[i].OriginalFirstThunk;
        RVA2FA(dwRVA, dwFA);
        wsprintf(szBuf, "%08X", dwFA);
        listCtrl.SetItemText(i, 1, szBuf);

        wsprintf(szBuf, "%08X", pImport[i].TimeDateStamp);
        listCtrl.SetItemText(i, 2, szBuf);

        wsprintf(szBuf, "%08X", pImport[i].ForwarderChain);
        listCtrl.SetItemText(i, 3, szBuf);

        RVA2FA(dwRVA, dwFA);wsprintf(szBuf, "%08X", pImport[i].Name);
        listCtrl.SetItemText(i, 4, szBuf);

        wsprintf(szBuf, "%08X", pImport[i].FirstThunk); 
        listCtrl.SetItemText(i, 5, szBuf);
        
        i++;
    }

}

void CMyCFFDlg::ShowExportDirectory(CListCtrl& listCtrl)
{   
// typedef struct _IMAGE_EXPORT_DIRECTORY {
//     DWORD   Characteristics;
//     DWORD   TimeDateStamp;
//     WORD    MajorVersion;
//     WORD    MinorVersion;
//     DWORD   Name;
//     DWORD   Base;
//     DWORD   NumberOfFunctions;
//     DWORD   NumberOfNames;
//     DWORD   AddressOfFunctions;     // RVA from base of image
//     DWORD   AddressOfNames;         // RVA from base of image
//     DWORD   AddressOfNameOrdinals;  // RVA from base of image
// } IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

    char *szMemberAry[] = {
        "Characteristics",
        "TimeDateStamp",
        "MajorVersion",
        "MinorVersion",
        "Name",
        "Base",
        "NumberOfFunctions",
        "NumberOfNames",
        "AddressOfFunctions",   
        "AddressOfNames",       
        "AddressOfNameOrdinals"
    };

    char szBuf[MAXBYTE] = {0};
    DWORD dwMemOffset = 0;

    listCtrl.InsertColumn(1, "Member", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(2, "Offset", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(3, "Size", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(4, "Value", LVCFMT_LEFT, 110);

    PIMAGE_DATA_DIRECTORY pDat_Dir = NULL;
    
    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    // 所以 IMAGE_DATA_DIRECTORY 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) + offset m_NumberOfRvaAndSizes
    // //                                           m_e_lfanew + 0x4 + 0x14 + 0x60
    pDat_Dir = (PIMAGE_DATA_DIRECTORY)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60);

    //通过 pDat_Dir 计算 IMAGE_EXPORT_DIRECTORY
    DWORD dwExportAddr = (DWORD)(pDat_Dir[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    DWORD dwExportFA = 0;
    RVA2FA(dwExportAddr, dwExportFA);
    dwMemOffset = dwExportFA;

    PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(dwExportFA + (DWORD)m_pExeFileBuf);

    FULL_FIRSTMEMBER(listCtrl, szBuf, dwExportFA, 0,
                     szMemberAry[0],
                     pExport->Characteristics);

    FULL_MEMBER(listCtrl, szBuf, 1,
                szMemberAry[1],
                pExport->TimeDateStamp, pExport->Characteristics);

    FULL_MEMBER(listCtrl, szBuf, 2,
                szMemberAry[2],
                pExport->MajorVersion, pExport->TimeDateStamp);

    FULL_MEMBER(listCtrl, szBuf, 3,
                szMemberAry[3],
                pExport->MinorVersion, pExport->MajorVersion);

    FULL_MEMBER(listCtrl, szBuf, 4,
                szMemberAry[4],
                pExport->Name, pExport->MinorVersion);

    FULL_MEMBER(listCtrl, szBuf, 5,
                szMemberAry[5],
                pExport->Base, pExport->Name);

    FULL_MEMBER(listCtrl, szBuf, 6,
                szMemberAry[6],
                pExport->NumberOfFunctions, pExport->Base);

    FULL_MEMBER(listCtrl, szBuf, 7,
        szMemberAry[7],
                pExport->NumberOfNames, pExport->NumberOfFunctions);

    FULL_MEMBER(listCtrl, szBuf, 8,
        szMemberAry[8],
                pExport->AddressOfFunctions, pExport->NumberOfNames);

    FULL_MEMBER(listCtrl, szBuf, 9,
                szMemberAry[9],
                pExport->AddressOfNames, pExport->AddressOfFunctions);

    FULL_MEMBER(listCtrl, szBuf, 10,
                szMemberAry[10],
                pExport->AddressOfNameOrdinals, pExport->AddressOfNames);


    CRect rcNew = m_rcListCtrl;
    rcNew.bottom = rcNew.bottom / 4 * 3;
    ScreenToClient(&rcNew);
    m_LstMemberOfStruct.MoveWindow(&rcNew, TRUE);

    //扩展的CtrlList
//     RECT rcExtend = m_rcListCtrl;
//     rcExtend.top = rcExtend.top / 2;

    CRect rect = m_rcListCtrl;    
    GetDlgItem(LST_STRUCT)->GetWindowRect(&rect);//获取控件的屏幕坐标
    rect.top += 200;
    rect.bottom += 150;
    ScreenToClient(&rect);//转换为对话框上的客户坐标

    m_lstExtend.Create(WS_BORDER |   
                       WS_VISIBLE |
                       LVS_REPORT |
                       LVS_EX_GRIDLINES |
                       LVS_EX_FULLROWSELECT |
                       LVS_SINGLESEL,
                       rect,
                       AfxGetMainWnd(),
                       5000);
    m_lstExtend.SetExtendedStyle(m_LstMemberOfStruct.GetExtendedStyle());

    m_lstExtend.InsertColumn(1, "Ordlinal", LVCFMT_LEFT, 110);
    m_lstExtend.InsertColumn(2, "Hint", LVCFMT_LEFT, 110);
    m_lstExtend.InsertColumn(3, "Name", LVCFMT_LEFT, 110);
    m_lstExtend.InsertColumn(4, "Function RVA(Entry Point)", LVCFMT_LEFT, 170);

    DWORD dwAddressOfFunctions = pExport->AddressOfFunctions;
    DWORD dwTmp = dwAddressOfFunctions;
    RVA2FA(dwTmp, dwAddressOfFunctions);
    dwAddressOfFunctions += (DWORD)m_pExeFileBuf;

    DWORD dwAddrNameOrdinals = pExport->AddressOfNameOrdinals;
    dwTmp = dwAddrNameOrdinals;
    RVA2FA(dwTmp, dwAddrNameOrdinals);
    dwAddrNameOrdinals += (DWORD)m_pExeFileBuf;

    DWORD dwAddrNames = pExport->AddressOfNames;
    dwTmp = dwAddrNames;
    RVA2FA(dwTmp, dwAddrNames);
    dwAddrNames += (DWORD)m_pExeFileBuf;

    DWORD dwNumberOfFunctions = pExport->NumberOfFunctions;
    for (DWORD i = 0; i < dwNumberOfFunctions; i++)
    {
        DWORD dwOrdlinal = pExport->Base + i;

        for (DWORD k = 0; k < pExport->NumberOfNames; k++)
        {
            if (dwOrdlinal - pExport->Base == ((PWORD)dwAddrNameOrdinals)[k])
            {
                wsprintf(szBuf, "%08X", dwOrdlinal);
                m_lstExtend.InsertItem(0, szBuf, 0);
                
                wsprintf(szBuf, "%08X", k);
                m_lstExtend.SetItemText(0, 1, szBuf);

                dwTmp = ((PDWORD)dwAddrNames)[k];
                DWORD dwFA = 0;
                RVA2FA(dwTmp, dwFA);
                dwFA += (DWORD)m_pExeFileBuf;
                m_lstExtend.SetItemText(0, 2, (PCHAR)dwFA);

                dwTmp = ((PDWORD)dwAddressOfFunctions)[k];
                //RVA2FA(dwTmp, dwFA);
                //dwFA += (DWORD)m_pExeFileBuf;
                wsprintf(szBuf, "%08X", dwTmp);
                m_lstExtend.SetItemText(0, 3, szBuf);
            }
            else
            {
                //还有 Bug;
            }
        }
    }
}
void CMyCFFDlg::ShowRelocationDirectory(CListCtrl& listCtrl)
{
    char szBuf[MAXBYTE] = {0};
    DWORD dwMemOffset = 0;

    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    // 所以 IMAGE_DATA_DIRECTORY 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) + offset m_NumberOfRvaAndSizes
    // //                                           m_e_lfanew + 0x4 + 0x14 + 0x60
    PIMAGE_DATA_DIRECTORY pDat_Dir = NULL;
    pDat_Dir = (PIMAGE_DATA_DIRECTORY)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60);
    
    //通过 pDat_Dir 计算 IMAGE_BASE_RELOCATION
    
    DWORD dwRelocationAddr = (DWORD)(pDat_Dir[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
    DWORD dwCountSize = (DWORD)(pDat_Dir[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);

    DWORD dwRelocationFA = 0;
    RVA2FA(dwRelocationAddr, dwRelocationFA);

    PIMAGE_BASE_RELOCATION pRelocation = (PIMAGE_BASE_RELOCATION)(dwRelocationFA + (DWORD)m_pExeFileBuf);
    
    listCtrl.InsertColumn(1, "RVA", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(2, "Size Of Block", LVCFMT_LEFT, 110);
    listCtrl.InsertColumn(3, "Items", LVCFMT_LEFT, 110);

    PIMAGE_BASE_RELOCATION pNext = pRelocation;
    DWORD dwCurSize = 0;
    
    int i = 0;
    while (dwCountSize - dwCurSize)
    {
        wsprintf(szBuf, "%08X",  pNext->VirtualAddress);
        listCtrl.InsertItem(i, szBuf, 0);

        wsprintf(szBuf, "%08X", pNext->SizeOfBlock);
        listCtrl.SetItemText(i, 1, szBuf);
        dwCurSize += pNext->SizeOfBlock;

        int nItemCount = 0;
        PWORD pItemAry = (PWORD)((DWORD)pNext + 8);
        for (DWORD j = 0; j < (pNext->SizeOfBlock - 8) / 2; j++)
        {
            WORD wItem = pItemAry[j];
            if (wItem >> 12  == 3) //有效的项
            {                
                nItemCount++;
            }
        }
        
        wsprintf(szBuf, "%d", nItemCount);
        listCtrl.SetItemText(i, 2, szBuf);
        pNext = (PIMAGE_BASE_RELOCATION)((DWORD)pNext + (DWORD)(pNext->SizeOfBlock));
    }

}

void CMyCFFDlg::ShowResourceDirectory(CListCtrl& listCtrl)
{
    char szBuf[MAXBYTE] = {0};
    DWORD dwMemOffset = 0;
    
    // pOp_Header 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) 的位置
    // 所以 IMAGE_DATA_DIRECTORY 在 m_e_lfanew + sizeof(dword) + sizeof(File_HEADERS) + offset m_NumberOfRvaAndSizes
    // //                                           m_e_lfanew + 0x4 + 0x14 + 0x60
    PIMAGE_DATA_DIRECTORY pDat_Dir = NULL;
    pDat_Dir = (PIMAGE_DATA_DIRECTORY)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60);
    
    //通过 pDat_Dir 计算 IMAGE_DIRECTORY_ENTRY_RESOURCE
    
    DWORD dwResAddr = (DWORD)(pDat_Dir[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
    DWORD dwCountSize = (DWORD)(pDat_Dir[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);
    
    DWORD dwResFA = 0;
    RVA2FA(dwResAddr, dwResFA);

    dwMemOffset += dwResFA;
    
    PIMAGE_RESOURCE_DIRECTORY pRes = (PIMAGE_RESOURCE_DIRECTORY)(dwResFA + (DWORD)m_pExeFileBuf);

    m_LstMemberOfStruct.InsertColumn(1, "Member", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(2, "Offset", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(3, "Size", LVCFMT_LEFT, 110);
    m_LstMemberOfStruct.InsertColumn(4, "Value", LVCFMT_LEFT, 110);

//     typedef struct _IMAGE_RESOURCE_DIRECTORY { 
//         DWORD Characteristics;
//             DWORD TimeDateStamp; 
//             WORD MajorVersion;
//             WORD MinorVersion;
//             WORD NumberOfNamedEntries;
//             WORD NumberOfIdEntries;
//             // IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[]; NumberOfNamedEntries + NumberOfIdEntries = ???? 
// } IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;
    char *szMemberAry[] = {
                            "Characteristics", 
                            "TimeDateStamp", 
                            "MajorVersion",
                            "MinorVersion",
                            "NumberOfNamedEntries",
                            "NumberOfIdEntries"
    };
    FULL_FIRSTMEMBER(listCtrl, szBuf, dwResFA, 0,
                    szMemberAry[0],
                    pRes->Characteristics);
    
    FULL_MEMBER(listCtrl, szBuf, 1,
                    szMemberAry[1],
                    pRes->TimeDateStamp, pRes->Characteristics);
    
    FULL_MEMBER(listCtrl, szBuf, 2,
                    szMemberAry[2],
                    pRes->MajorVersion, pRes->TimeDateStamp);
    
    FULL_MEMBER(listCtrl, szBuf, 3,
                    szMemberAry[3],
                    pRes->MinorVersion, pRes->MajorVersion);

    FULL_MEMBER(listCtrl, szBuf, 4,
                    szMemberAry[4],
                    pRes->NumberOfNamedEntries, pRes->MinorVersion);

    FULL_MEMBER(listCtrl, szBuf, 5,
                    szMemberAry[5],
                    pRes->NumberOfIdEntries, pRes->NumberOfNamedEntries);

}



void CMyCFFDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
    CString str = m_Tree.GetItemText(pNMTreeView->itemNew.hItem);
    
    //m_LstMemberOfStruct位置恢复一下
    CRect rcOriginal = m_rcListCtrl;
    ScreenToClient(&rcOriginal);
    m_LstMemberOfStruct.MoveWindow(&rcOriginal, TRUE);

    m_lstExtend.DestroyWindow();

    //m_LstMemberOfStruct清空一下
    m_LstMemberOfStruct.DeleteAllItems();
    //清空所有列
    while (m_LstMemberOfStruct.DeleteColumn(0));

    DWORD dwMemOffset = 0; 

    if (str == "IMAGE_DOS_HEADER")
    {
        ShowDosHeader(m_LstMemberOfStruct);
    }
    else if (str == "IMAGE_NT_HEADERS")
    {
        ShowNTHeader(m_LstMemberOfStruct);
    }
    else if (str == "IMAGE_FILE_HEADER")
    {
        ShowFileHeader(m_LstMemberOfStruct);
    }
    else if (str == "IMAGE_OPTIONAL_HEADER")
    {
        ShowOptionalHeader(m_LstMemberOfStruct);
    }
    else if (str == "IMAGE_DATA_DIRECTORY[x]")
    {
        ShowDataDir(m_LstMemberOfStruct);
    }
    else if (str == "IMAGE_SECTION_HEADER[x]")
    {
        ShowSectionHeader(m_LstMemberOfStruct);
    }
    else if (str == "Import Directory")
    {
        ShowImportDirectory(m_LstMemberOfStruct);
    }
    else if (str == "Export Directory")
    {
        ShowExportDirectory(m_LstMemberOfStruct);
    }
    else if (str == "Relocation Directory")
    {
        ShowRelocationDirectory(m_LstMemberOfStruct);
    }
    else if (str == "Resource Directory")
    {
        ShowResourceDirectory(m_LstMemberOfStruct);
    }

	*pResult = 0;
}

void CMyCFFDlg::OnSetfocusVa() 
{
	// TODO: Add your control notification handler code here
    CEdit *pVA = (CEdit *)GetDlgItem(EDT_VA);
    pVA->SetReadOnly(FALSE);

    CEdit *pRVA = (CEdit *)GetDlgItem(EDT_RVA);
    pRVA->SetReadOnly(TRUE);

    CEdit *pFileOffset = (CEdit *)GetDlgItem(EDT_FILEOFFSET);
    pFileOffset->SetReadOnly(TRUE);
}

void CMyCFFDlg::OnSetfocusRva() 
{
	// TODO: Add your control notification handler code here
    CEdit *pVA = (CEdit *)GetDlgItem(EDT_VA);
    pVA->SetReadOnly(TRUE);
    
    CEdit *pRVA = (CEdit *)GetDlgItem(EDT_RVA);
    pRVA->SetReadOnly(FALSE);
    
    CEdit *pFileOffset = (CEdit *)GetDlgItem(EDT_FILEOFFSET);
    pFileOffset->SetReadOnly(TRUE);
	
}

void CMyCFFDlg::OnSetfocusFileoffset() 
{
	// TODO: Add your control notification handler code here
    CEdit *pVA = (CEdit *)GetDlgItem(EDT_VA);
    pVA->SetReadOnly(TRUE);
    
    CEdit *pRVA = (CEdit *)GetDlgItem(EDT_RVA);
    pRVA->SetReadOnly(TRUE);
    
    CEdit *pFileOffset = (CEdit *)GetDlgItem(EDT_FILEOFFSET);
    pFileOffset->SetReadOnly(FALSE);
}

void CMyCFFDlg::OnChangeVa() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    

    //输入 VA 的值，说明要计算File Offset
    //RVA + ImageBase(hInstance) = VA
    
    if (m_pExeFileBuf == NULL)
    {
        return;
    }

    //光标不在时不处理
    CEdit *pVA = (CEdit *)GetDlgItem(EDT_VA);
    if (AfxGetMainWnd()->GetFocus() != pVA)
    {
        return;
    }
    
    char szVA[MAXBYTE] = {0};
    GetDlgItemText(EDT_VA, szVA, MAXBYTE);

    DWORD dwVA = strtoul(szVA, NULL, 16);
    if (dwVA >= 0x7FFEFFFF) // VA 地址不合法
    {
        return;
    }
    else if (dwVA <= 0x10000) //小于64k的地址
    {
        return;
    }

    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    pSectionHeader = (PIMAGE_SECTION_HEADER)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60 +
                                             m_NumberOfRvaAndSizes * 
                                             (sizeof(DWORD) * 2) );

    DWORD dwTmpFA = pSectionHeader[m_dwNumberOfSections - 1].PointerToRawData;
    DWORD dwTmpVA = pSectionHeader[m_dwNumberOfSections - 1].VirtualAddress;

    if (dwVA > m_dwFileSize - dwTmpFA + dwTmpVA + m_ImageBase)
    {
        //VA 不在文件范围内就不用处理
        return;
    }

    char szImageBase[MAXBYTE] = {0};
    GetDlgItemText(EDT_IMAGEBASE, szImageBase, MAXBYTE);

    DWORD dwImageBase = strtoul(szImageBase, NULL, 16);

    //RVA得出负数时直接返回
    DWORD dwRVA = dwVA - dwImageBase;
    if (dwRVA >= 0x7FFFFFFF)
    {
        SetDlgItemText(EDT_RVA, "00000000");
        return;
    }
    char szRVA[MAXBYTE] = {0};
    wsprintf(szRVA, "%08X", dwVA - dwImageBase);
    SetDlgItemText(EDT_RVA, szRVA);
    
    char szFA[MAXBYTE] = {0};
    if (dwVA - dwImageBase <= m_SizeOfHeaders)
    {
        //在头部范围内 RVA 和 FA 一样
        wsprintf(szFA, "%08X", dwVA - dwImageBase);
        SetDlgItemText(EDT_FILEOFFSET, szFA);
    }
}

void CMyCFFDlg::OnChangeRva() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

    if (m_pExeFileBuf == NULL)
    {
        return;
    }

    //光标不在时不处理
    CEdit *pRVA = (CEdit *)GetDlgItem(EDT_RVA);
    if (AfxGetMainWnd()->GetFocus() != pRVA)
    {
        return;
    }

    
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    pSectionHeader = (PIMAGE_SECTION_HEADER)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60 +
                                             m_NumberOfRvaAndSizes * 
                                             (sizeof(DWORD) * 2) );

    char szVA[MAXBYTE] = {0};
    char szFA[MAXBYTE] = {0};
    
    char szRVA[MAXBYTE] = {0};
    GetDlgItemText(EDT_RVA, szRVA, MAXBYTE);

    DWORD dwRVA = strtoul(szRVA, NULL, 16);

    //在头部范围内直接转为文件偏移
    if (dwRVA <= m_SizeOfHeaders)
    {
        wsprintf(szFA, "%08X", dwRVA);
        SetDlgItemText(EDT_FILEOFFSET, szFA);
    }
    else
    {
        //在SECTION_DATA中要转换一下
        DWORD i = 0;
    for (i = 0; i < m_dwNumberOfSections - 1; i++)
    {
        DWORD dwLow = pSectionHeader[i].VirtualAddress + m_ImageBase;
        DWORD dwHigh = pSectionHeader[i + 1].VirtualAddress + m_ImageBase;
        if (dwLow <= dwRVA + m_ImageBase && dwRVA + m_ImageBase < dwHigh)
        {
            break;
        }
    }
    // 此时 RVA 命中第 i 节
    // 判断 FA 是否在文件中
    DWORD dwOffset = dwRVA - pSectionHeader[i].VirtualAddress;
    DWORD dwTmpFA = pSectionHeader[i].PointerToRawData;
    DWORD dwFALen = pSectionHeader[i].SizeOfRawData;
    if (dwTmpFA <= dwTmpFA + dwOffset && dwOffset < dwFALen && dwOffset + dwTmpFA <= m_dwFileSize)
    {
            //FA 存在
            wsprintf(szFA, "%08X", dwOffset + dwTmpFA);
        }
        else
        {
            wsprintf(szFA, "%08X", 0);
        }
        
        SetDlgItemText(EDT_FILEOFFSET, szFA);
    }

    wsprintf(szVA, "%08X", dwRVA + m_ImageBase);
    SetDlgItemText(EDT_VA, szVA);
}


void CMyCFFDlg::OnChangeFileoffset() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    if (m_pExeFileBuf == NULL)
    {
        return;
    }
    
    //光标不在时不处理
    CEdit *pFA = (CEdit *)GetDlgItem(EDT_FILEOFFSET);
    if (AfxGetMainWnd()->GetFocus() != pFA)
    {
        return;
    }
    
    
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    pSectionHeader = (PIMAGE_SECTION_HEADER)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60 +
                                             m_NumberOfRvaAndSizes * 
                                             (sizeof(DWORD) * 2) );
    
    char szVA[MAXBYTE] = {0};
    char szFA[MAXBYTE] = {0};
    char szRVA[MAXBYTE] = {0};
    DWORD dwRVA = 0;
    GetDlgItemText(EDT_FILEOFFSET, szFA, MAXBYTE);
    DWORD dwFA = strtoul(szFA, NULL, 16);

    if (dwFA > m_dwFileSize)
    {
        return; //大于文件大小直接返回
    }

    //在头部范围内直接转为RVA
    if (dwFA <= m_SizeOfHeaders)
    {
        dwRVA = dwFA;
        wsprintf(szRVA, "%08X", dwRVA);
    }
    else
    {
        //判断在哪个节
        DWORD i = 0;
        for (i = 0; i < m_dwNumberOfSections; i++)
        {
            if (i + 1 <= m_dwNumberOfSections)
            {
                DWORD dwLow = pSectionHeader[i].PointerToRawData;
                DWORD dwHigh = dwLow + pSectionHeader[i].SizeOfRawData;
                if (dwLow <= dwFA && dwFA < dwHigh)
                {
                    break;
                }
            }
            else
            {
                i = m_dwNumberOfSections - 1;
                break;
            }
        }

         DWORD dwOffset = dwFA - pSectionHeader[i].PointerToRawData;
         dwRVA = dwOffset + pSectionHeader[i].VirtualAddress;

         wsprintf(szRVA, "%08X", dwRVA);
    }
    
    SetDlgItemText(EDT_RVA, szRVA);
    wsprintf(szVA, "%08X", dwRVA + m_ImageBase);
    SetDlgItemText(EDT_VA, szVA);
}

BOOL CMyCFFDlg::RVA2FA(DWORD dwRVA, DWORD& dwFA)
{
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    pSectionHeader = (PIMAGE_SECTION_HEADER)(m_pExeFileBuf + m_e_lfanew + 0x4 + 0x14 + 0x60 +
                                             m_NumberOfRvaAndSizes * 
                                             (sizeof(DWORD) * 2) );

    DWORD i = 0;
    for (i = 0; i < m_dwNumberOfSections - 1; i++)
    {
        DWORD dwLow = pSectionHeader[i].VirtualAddress + m_ImageBase;
        DWORD dwHigh = pSectionHeader[i + 1].VirtualAddress + m_ImageBase;
        if (dwLow <= dwRVA + m_ImageBase && dwRVA + m_ImageBase < dwHigh)
        {
            break;
        }
    }

    // 此时 RVA 命中第 i 节
    // 判断 FA 是否在文件中
    DWORD dwOffset = dwRVA - pSectionHeader[i].VirtualAddress;
    DWORD dwTmpFA = pSectionHeader[i].PointerToRawData;
    DWORD dwFALen = pSectionHeader[i].SizeOfRawData;
    if (dwTmpFA <= dwTmpFA + dwOffset && dwOffset < dwFALen && dwOffset + dwTmpFA <= m_dwFileSize)
    {
        //FA 存在
        dwFA = dwOffset + dwTmpFA;
        return TRUE;
    }
    else
    {
        dwFA = 0;
        return FALSE;
    }
}

void CMyCFFDlg::OnClickStruct(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	HTREEITEM hItem = m_Tree.GetSelectedItem();
    CString csText = m_Tree.GetItemText(hItem);
    if (csText == "Import Directory")
    {
        POSITION Pos = m_LstMemberOfStruct.GetFirstSelectedItemPosition();
        int nSelect = -1;
        
        while (Pos)
        {
            nSelect = m_LstMemberOfStruct.GetNextSelectedItem(Pos);    
        }
        
        if (Pos == 0 && nSelect == -1)
        {
            return;
        }
        
        char szModule[MAXBYTE] = {0};
        char szBuf[MAXBYTE] = {0};
        int nCount = 0;
        
        //获得dll名
        m_LstMemberOfStruct.GetItemText(nSelect, 0, szModule, MAXBYTE);
        
        //获得 IAT 第 5 列
        m_LstMemberOfStruct.GetItemText(nSelect, 5, szBuf, MAXBYTE);
        DWORD dwIATRva = strtoul(szBuf, NULL, 16);
        DWORD dwFA = 0;
        RVA2FA(dwIATRva, dwFA);
        
        DWORD *pdwPtr = (DWORD *)(m_pExeFileBuf + dwFA);
        int i = 0;
        while (TRUE)
        {
            if (*(pdwPtr + i) != 0)
            {
                i++;
            }
            else
            {
                break;
            }
        }
        
        nCount = i;
        DWORD dwLen = sizeof(DWORD) + sizeof(WORD) + MAXBYTE;
        
        char *pData = new char[nCount * dwLen];
        if (pData == NULL)
        {
            return;
        }
        ZeroMemory(pData, nCount * dwLen);
        
        i = 0;
        while (TRUE)
        {
            if (*(pdwPtr + i) != 0)
            {
                memcpy((pData + i * dwLen), (pdwPtr + i), sizeof(DWORD));
            }
            else
            {
                break;
            }
            i++;
        }        
        
        // 获得 OFT ,已经转为FA
        m_LstMemberOfStruct.GetItemText(nSelect, 1, szBuf, MAXBYTE);
        DWORD dwINTFa = strtoul(szBuf, NULL, 16);
        DWORD dwHintNameAddr = (DWORD)(m_pExeFileBuf + dwINTFa);
        
        i = 0;
        for (; i < nCount; i++)
        {        
            DWORD dwTmp = *(DWORD *)(dwHintNameAddr + i * sizeof(DWORD));
            DWORD dwTmpFa = 0;
            RVA2FA(dwTmp, dwTmpFa);
            dwTmpFa += (DWORD)m_pExeFileBuf;
            char *pAddr = (char *)dwTmpFa;
            memcpy((pData + i * dwLen) + sizeof(DWORD), pAddr, sizeof(WORD));
            strcpy((pData + i * dwLen) + sizeof(DWORD) + sizeof(WORD), pAddr + sizeof(WORD));
        }        
        
        DlgINT DlgInt;
        DlgInt.SetData(szModule, pData, i);
        int nResponse = DlgInt.DoModal();        
        
        if (pData != NULL)
        {
            delete[] pData;
            pData = NULL;
        }        
    }
    else if (csText == "Relocation Directory")
    {
        //点击重定位表
        POSITION Pos = m_LstMemberOfStruct.GetFirstSelectedItemPosition();
        int nSelect = -1;
        
        while (Pos)
        {
            nSelect = m_LstMemberOfStruct.GetNextSelectedItem(Pos);    
        }
        
        if (Pos == 0 && nSelect == -1)
        {
            return;
        }
        
        char szModule[MAXBYTE] = {0};
        char szBuf[MAXBYTE] = {0};
        int nCount = 0;
        
        //获得选中的第一列的数据


    }
    else
    {
        *pResult = 0;
        return;
    }

	*pResult = 0;
}
