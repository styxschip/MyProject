// MyCMD_srvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RemoteMgr_Svr.h"
#include "MyCMD_srvDlg.h"
#include "../common/MyPacket.h"
#include "../common/OutputDbgMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCMD_srvDlg dialog


CMyCMD_srvDlg::CMyCMD_srvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCMD_srvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyCMD_srvDlg)
	m_csCmd = _T("");
	m_csShow = _T("");
	//}}AFX_DATA_INIT
    //m_sockListen = INVALID_SOCKET;
}


void CMyCMD_srvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCMD_srvDlg)
	DDX_Text(pDX, EDT_CMD, m_csCmd);
	DDX_Text(pDX, EDT_SHOW, m_csShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyCMD_srvDlg, CDialog)
	//{{AFX_MSG_MAP(CMyCMD_srvDlg)
	ON_BN_CLICKED(BTN_RUN, OnRun)
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCMD_srvDlg message handlers


//重写CDialog的虚函数OnOK()实现输入回车执行cmd命令
void CMyCMD_srvDlg::OnOK()
{
    ::SendMessage(GetDlgItem(BTN_RUN)->GetSafeHwnd(), BM_CLICK, NULL, NULL);
    return;
}

BOOL CMyCMD_srvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
       
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyCMD_srvDlg::OnRun() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    //向父窗口发送EDT_CMD中的数据
    COPYDATASTRUCT cds;
    char szBuf[MAXBYTE] = {0};
    m_csCmd += "\r\n";
    strcpy(szBuf, m_csCmd);
    cds.dwData = CLT_CMD;       //标识码
    cds.cbData = m_csCmd.GetLength(); //缓冲区大小,不包含'\0'
    cds.lpData = szBuf;   //缓冲区指针
    HWND curHwnd = GetSafeHwnd();
    HWND hParentWnd = ::GetParent(curHwnd);
    
    ::SendMessage(hParentWnd,
                    WM_COPYDATA,
                    (WPARAM)curHwnd,
                    (LPARAM)&cds);
}

BOOL CMyCMD_srvDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (pCopyDataStruct->dwData == CLT_CMD)
    {
        OutputDebugMsg(__LINE__, __FILE__, "ssc (char*)pCopyDataStruct->lpData is %s", (char*)pCopyDataStruct->lpData);
        TRACE("ssc CMyCMD_srvDlg OnCopyData begin!");
        m_csShow += (char*)pCopyDataStruct->lpData;
        OutputDebugMsg(__LINE__, __FILE__, "ssc m_csShow is %s", m_csShow);
        UpdateData(FALSE);
        HWND hEdit = GetDlgItem(EDT_SHOW)->GetSafeHwnd();
        //输出窗口光标定位到最后一行
        ::SendMessage(hEdit, WM_VSCROLL, SB_BOTTOM, 0);
    }
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CMyCMD_srvDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

    HWND hParent = ::GetParent(GetSafeHwnd());
    ::SendMessage(hParent, WM_CLRSUBDLG, (WPARAM)GetSafeHwnd(), NULL);
}
