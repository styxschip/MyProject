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
		// NOTE: the ClassWizard will add DDX and DDV calls here
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
        return 0;
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
