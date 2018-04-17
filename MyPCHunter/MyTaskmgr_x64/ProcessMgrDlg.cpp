// ProcessMgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "ProcessMgrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessMgrDlg dialog


CProcessMgrDlg::CProcessMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessMgrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessMgrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProcessMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessMgrDlg)
	DDX_Control(pDX, LST_PROCESS, m_LstProcess);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessMgrDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessMgrDlg)
	ON_BN_CLICKED(BTN_READPROCMEM, OnReadprocmem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessMgrDlg message handlers

BOOL CProcessMgrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //设置ListCtrl风格
    m_LstProcess.SetExtendedStyle(m_LstProcess.GetExtendedStyle() |
                                    LVS_EX_GRIDLINES |
                                    LVS_EX_FULLROWSELECT);
    //设置ListCtrl列名
    m_LstProcess.InsertColumn(1, "映象名称", LVCFMT_LEFT, 100);
    m_LstProcess.InsertColumn(2, "进程ID", LVCFMT_LEFT, 100);
    m_LstProcess.InsertColumn(3, "映象路径", LVCFMT_LEFT, 300);
    m_LstProcess.InsertColumn(4, "EPROCESS", LVCFMT_LEFT, 200);

    
    UpdateData(FALSE);


    //从驱动中拿进程信息
    char szTmpBuf[MAXBYTE] = {0};
    DWORD dwBytes = 0;

    extern CMyTaskmgrApp theApp;
    HANDLE hDrvFile = theApp.m_hDrvFile;

    //存放进程信息的堆空间
    //new 1000个进程 实际应该没有这么多
    
    PProcessInfo pBuff = new ProcessInfo[1000];
    if (pBuff == NULL)
    {
        AfxMessageBox("new failed!");
        return FALSE;
    }
    ZeroMemory(pBuff, 1000 * sizeof(ProcessInfo));

    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_PROC_INFO, 
                        pBuff, 
                        1000 * sizeof(ProcessInfo),
                        pBuff,
                        1000 * sizeof(ProcessInfo),
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szTmpBuf, "DeviceIoControl IOCTRL_GET_PROC_INFO ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szTmpBuf);
        return 0;
    }

    int i = 0;
    while(pBuff[i].qwEPROCESS != NULL)
    {      
        if (pBuff[i].m_szBuf != NULL)
        {
            wsprintf(szTmpBuf, "%s", pBuff[i].m_szBuf);
        }
        else
        {
            wsprintf(szTmpBuf, "%s", "");
        }
        m_LstProcess.InsertItem(i, szTmpBuf); //映象名称


        if(pBuff[i].m_szFullPath != NULL)
        {   
            wsprintf(szTmpBuf, "%s", pBuff[i].m_szFullPath);
        }
        else
        {
            wsprintf(szTmpBuf, "%s", "");
        }
            
        m_LstProcess.SetItemText(i, 2, szTmpBuf); //映象路径;

        wsprintf(szTmpBuf, "%ld", pBuff[i].m_dwPID);
        m_LstProcess.SetItemText(i, 1, szTmpBuf); //进程ID
        
        
        wsprintf(szTmpBuf, "0xFFFFFF80%08x", pBuff[i].qwEPROCESS);
        m_LstProcess.SetItemText(i, 3, szTmpBuf); //EPROCESS

        i++;
    }

    if (pBuff != NULL)
    {
        delete pBuff;
        pBuff = NULL;
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProcessMgrDlg::OnReadprocmem() 
{
	// TODO: Add your control notification handler code here
    char szBuf[MAXBYTE] = {0};
    DWORD dwBytes = 0;

    extern CMyTaskmgrApp theApp;
    HANDLE hDrvFile = theApp.m_hDrvFile;
    
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_READ_PROC_MEM, 
                        (LPVOID)&dwBytes, 
                        sizeof(DWORD),
                        (LPVOID)&dwBytes,
                        sizeof(DWORD),
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_READ_PROC_MEM ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        return;
    }
}
