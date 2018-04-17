// MyGDT.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "MyGDT.h"
#include "Function.h"
#include "MyDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyGDT dialog


CMyGDT::CMyGDT(CWnd* pParent /*=NULL*/)
	: CDialog(CMyGDT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyGDT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyGDT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyGDT)
	DDX_Control(pDX, LST_GDT, m_LstGDT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyGDT, CDialog)
	//{{AFX_MSG_MAP(CMyGDT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGDT message handlers

BOOL CMyGDT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //设置ListCtrl风格
    m_LstGDT.SetExtendedStyle(m_LstGDT.GetExtendedStyle() |
                              LVS_EX_GRIDLINES |
                              LVS_EX_FULLROWSELECT);
    //设置ListCtrl列名
    m_LstGDT.InsertColumn(1, "CPU序号", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(2, "段选择子", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(3, "基址", LVCFMT_LEFT, 200);
    m_LstGDT.InsertColumn(4, "界限", LVCFMT_LEFT, 200);
    m_LstGDT.InsertColumn(5, "段粒度", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(6, "段特权级", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(7, "类型", LVCFMT_LEFT, 100);
    
    UpdateData(FALSE);

    GetGDTFromSys();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CMyGDT::GetGDTFromSys()
{
    char szBuf[MAXBYTE] = {0};

    //获得CPU核心数
    DWORD dwNumberOfProcessors = CFunction::GetNumberOfProcessors();

    DWORD dwBytes = 0;
    //从驱动获取GDT长度 除以8得到GDT项数

    DWORD dwMap = 0; //GDT表项数
    DWORD dwGDTL = 0; //GDT表长度
    
    extern CMyTaskmgrApp theApp;
    HANDLE hDrvFile = theApp.m_hDrvFile;
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_GDTLIMIT, 
                        (LPVOID)&dwGDTL, 
                        sizeof(DWORD),
                        (LPVOID)&dwGDTL,
                        sizeof(DWORD),
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_GDTLIMIT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        return 0;
    }

    dwGDTL = dwGDTL + 1; //从0开始计数所以加1
    dwMap = dwGDTL / 8;

    //GDT描述符表最多有 核心数 x dwLimit 字节
    //传入传出都用pParams这块内存
    char *pParams = new char[dwNumberOfProcessors * dwGDTL];
    if (pParams == NULL)
    {
        wsprintf(szBuf, "new CONTORL_PARAMS faild!");
        AfxMessageBox(szBuf);
        return FALSE;
    }
    ZeroMemory(pParams, dwNumberOfProcessors * dwGDTL);
    //设置处理器数量
    ((PCONTORL_PARAMS)pParams)->CPUNumber = dwNumberOfProcessors;
    
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_GDT, 
                        pParams, 
                        sizeof(CONTORL_PARAMS),
                        pParams, 
                        dwNumberOfProcessors * dwGDTL, //这里的长度给错会在DispatchDeviceControl--IoCompleteRequest(pIrp, IO_NO_INCREMENT)时蓝屏
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_GDT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);

        if (pParams != NULL)
        {
            delete pParams;
            pParams = NULL;
        }
        return 0;
    }

    char szTmpBuf[MAXBYTE] = {0};
    int j = 0;
    int k = 0;
    WORD wAttrib = 0;
    DWORD dwLimite = 0;
    DWORD dwBase = 0;
    DWORD dwP = 0;
    UCHAR chType = 0;
    //解析后填充到ListCtrl
    for (DWORD i = 0; i < dwNumberOfProcessors * dwMap; i++)
    {
        //判断P位是否有效 有效才显示
        CFunction::GetAttribFormGDT((PGDT)(pParams + j * 8), &wAttrib);
        dwP = wAttrib & 0x80; //1000 0000B == 0x80 P为位1
        chType = CFunction::GetTypeValue(wAttrib);
        chType = chType & 1;
        if (dwP != 0x80 /*|| chType == 0*/) 
        {
            j++;
            continue;
        }
        else
        {
            CFunction::GetBaseFormGDT((PGDT)(pParams + j * 8), &dwBase);
            
            wsprintf(szTmpBuf, "%d", i / dwMap);
            m_LstGDT.InsertItem(k, szTmpBuf); //CPU序号
            
            wsprintf(szTmpBuf, "0x%04X", i);
            m_LstGDT.SetItemText(k, 1, szTmpBuf); //段选择子
            
            wsprintf(szTmpBuf, "0x%08X", dwBase);
            m_LstGDT.SetItemText(k, 2, szTmpBuf); //基址
            
            CFunction::GetLimiteFormGDT((PGDT)(pParams + j * 8), &dwLimite);
            wsprintf(szTmpBuf, "0x%08X", dwLimite);
            m_LstGDT.SetItemText(k, 3, szTmpBuf); //界限
            
            wsprintf(szTmpBuf, "%s", CFunction::GetG(wAttrib));
            m_LstGDT.SetItemText(k, 4, szTmpBuf); //段粒度

            wsprintf(szTmpBuf, "%d", CFunction::GetDLP(wAttrib));
            m_LstGDT.SetItemText(k, 5, szTmpBuf); //段特权级

            wsprintf(szTmpBuf, "%s", CFunction::GetTypeString(wAttrib));
            m_LstGDT.SetItemText(k, 6, szTmpBuf); //类型;

            j++;
            k++;
        }
        

    }

    
    if (pParams != NULL)
    {
        delete pParams;
        pParams = NULL;
    }
	
    return TRUE;
}
