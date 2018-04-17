// KernelModuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "KernelModuleDlg.h"
#include "MyDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKernelModuleDlg dialog


CKernelModuleDlg::CKernelModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKernelModuleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKernelModuleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKernelModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKernelModuleDlg)
	DDX_Control(pDX, LST_MODULE, m_LstModule);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKernelModuleDlg, CDialog)
	//{{AFX_MSG_MAP(CKernelModuleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKernelModuleDlg message handlers

BOOL CKernelModuleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //设置ListCtrl风格
    m_LstModule.SetExtendedStyle(m_LstModule.GetExtendedStyle() |
                                LVS_EX_GRIDLINES |
                                LVS_EX_FULLROWSELECT);
    //设置ListCtrl列名
    m_LstModule.InsertColumn(1, "驱动名", LVCFMT_LEFT, 200);
    m_LstModule.InsertColumn(2, "基地址", LVCFMT_LEFT, 200);
    m_LstModule.InsertColumn(3, "大小", LVCFMT_LEFT, 1000);
//     m_LstModule.InsertColumn(4, "驱动对象", LVCFMT_LEFT, 70);
//     m_LstModule.InsertColumn(5, "驱动路径", LVCFMT_LEFT, 200);
//     m_LstModule.InsertColumn(6, "服务名", LVCFMT_LEFT, 70);
//     m_LstModule.InsertColumn(7, "加载顺序", LVCFMT_LEFT, 60);
    //m_LstModule.InsertColumn(8, "文件厂商", LVCFMT_LEFT, 200);
    
    UpdateData(FALSE);

    GetKrnlModuleFromSys();

    //EnumKM();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CKernelModuleDlg::GetKrnlModuleFromSys()
{
    DWORD dwBytes = 0;
    char szBuf[MAXBYTE] = {0};

    PEnumModule pBuff = new EnumModule[500]; //500个够了吧。。。
    if (pBuff == NULL)
    {
        AfxMessageBox("new failed");
        return 1;
    }
    ZeroMemory(pBuff, sizeof(EnumModule) * 500);

    extern CMyTaskmgrApp theApp;
    HANDLE hDrvFile = theApp.m_hDrvFile;
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_ENUM_MODULE, 
                        (LPVOID)pBuff, 
                        500 * sizeof(EnumModule),
                        (LPVOID)pBuff,
                        500 * sizeof(EnumModule),
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_GDTLIMIT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        return 0;
    }

    for (int i = 0; i < 501; i++)
    {
        if (pBuff[i].m_ImageSize == 0)
        {
            break;
        }
        wsprintf(szBuf, "%s", pBuff[i].szFullPathName);
        m_LstModule.InsertItem(i, szBuf); //驱动名
        
        wsprintf(szBuf, "0xFFFFF8%010X", pBuff[i].m_qwImageBase);
        m_LstModule.SetItemText(i, 1, szBuf); //基地址
        
        wsprintf(szBuf, "0x%08X", pBuff[i].m_ImageSize);
        m_LstModule.SetItemText(i, 2, szBuf); //大小
    }

    if (pBuff != NULL)
    {
        delete pBuff;
        pBuff = NULL;
    }

    return TRUE;
}

typedef (*pfn_Wow64Disable)(PVOID* OldValue);
typedef (*pfn_Wow64Revert)(PVOID OldValue);


pfn_ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;

//r3下栈不平衡
BOOLEAN CKernelModuleDlg::EnumKM()
{
    PVOID OldValue = NULL;
    pfn_Wow64Disable funDis = (pfn_Wow64Disable)(GetProcAddress(LoadLibrary("Kernel32"), "Wow64DisableWow64FsRedirection"));
    funDis(&OldValue);

    ZwQuerySystemInformation = (pfn_ZWQUERYSYSTEMINFORMATION)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwQuerySystemInformation");

    char szBuf[MAXBYTE] = {0};
    ULONG NeedSize, i, ModuleCount, HLed=0, BufferSize = 0x5000;
    PVOID pBuffer = NULL;
    PCHAR pDrvName = NULL;
    NTSTATUS Result;
    PSYSTEM_MODULE_INFORMATION pSystemModuleInformation;
    do
    {
        //分配内存
        pBuffer = malloc( BufferSize );
        if( pBuffer == NULL )
            return 0;
        AfxMessageBox("aaaa");
        //查询模块信息
        Result = ZwQuerySystemInformation( 11, pBuffer, BufferSize, &NeedSize );
        AfxMessageBox("bbb");
        if( Result == 0xC0000004L )
        {
            free( pBuffer );
            BufferSize *= 2;
        }
        else if( Result<0 )
        {
            //查询失败则退出
            free( pBuffer );
            return 0;
        }
AfxMessageBox("11111111");
    }
    while( Result == 0xC0000004L );
    pSystemModuleInformation = (PSYSTEM_MODULE_INFORMATION)pBuffer;
    //获得模块的总数量
    ModuleCount = pSystemModuleInformation->Count;
    //遍历所有的模块
    for( i = 0; i < ModuleCount; i++ )
    {
        if((ULONG64)(pSystemModuleInformation->Module[i].Base) > (ULONG64)0x8000000000000000)
        {
            pDrvName = pSystemModuleInformation->Module[i].ImageName+pSystemModuleInformation->Module[i].ModuleNameOffset;
            wsprintf(szBuf, "0x%llx", (ULONG64)pSystemModuleInformation->Module[i].Base);
            m_LstModule.InsertItem(i, szBuf); 
            
            wsprintf(szBuf, "%s", pDrvName);
            m_LstModule.SetItemText(i, 3, szBuf); 
AfxMessageBox("22222222222");
        }
    }


    pfn_Wow64Revert funRevert = (pfn_Wow64Revert)(GetProcAddress(LoadLibrary("Kernel32"), "Wow64RevertWow64FsRedirection"));
    funRevert(OldValue);

    free(pBuffer);
    return 1;
}
