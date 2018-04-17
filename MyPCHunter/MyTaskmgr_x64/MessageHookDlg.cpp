// MessageHookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "MessageHookDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageHookDlg dialog


extern CMyTaskmgrApp theApp;
HANDLE hMyDrv = theApp.m_hDrvFile;

CMessageHookDlg::CMessageHookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageHookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageHookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMessageHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageHookDlg)
	DDX_Control(pDX, LST_MSGHOOK, m_LstMsgHook);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageHookDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageHookDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageHookDlg message handlers

BOOL CMessageHookDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_LstMsgHook.SetExtendedStyle(m_LstMsgHook.GetExtendedStyle() |
                                    LVS_EX_GRIDLINES |
                                    LVS_EX_FULLROWSELECT);
    //设置ListCtrl列名
    m_LstMsgHook.InsertColumn(1, "句柄", LVCFMT_LEFT, 100);
    m_LstMsgHook.InsertColumn(2, "标志", LVCFMT_LEFT, 100);
    m_LstMsgHook.InsertColumn(3, "类型", LVCFMT_LEFT, 100);
    m_LstMsgHook.InsertColumn(4, "函数", LVCFMT_LEFT, 100);
    m_LstMsgHook.InsertColumn(5, "eThread", LVCFMT_LEFT, 100);
    m_LstMsgHook.InsertColumn(6, "进程名", LVCFMT_LEFT, 100);

    
    UpdateData(FALSE);
    
    EnumMsgHook();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMessageHookDlg::RKM(UINT64 Address, PVOID Buffer, SIZE_T Length)
{
	IoControl(hMyDrv ,CTL_CODE_GEN(0x809), &Address, 8, NULL, 0);		//address
	IoControl(hMyDrv ,CTL_CODE_GEN(0x80A), &Length, 8, NULL, 0);		//length
	IoControl(hMyDrv ,CTL_CODE_GEN(0x804), NULL, 0, Buffer, Length);	//get buffer
}

void CMessageHookDlg::WKM(UINT64 Address, PVOID Buffer, SIZE_T Length)
{
	IoControl(hMyDrv ,CTL_CODE_GEN(0x809), &Address, 8, NULL, 0);		//address
	IoControl(hMyDrv ,CTL_CODE_GEN(0x80A), &Length, 8, NULL, 0);		//length
	IoControl(hMyDrv ,CTL_CODE_GEN(0x805), Buffer, Length, NULL, 0);	//set buffer
}

UINT64 CMessageHookDlg::GetQWORD(UINT64 address)
{
	UINT64 y=0;
	RKM(address,&y,8);
	return y;
}

UINT32 CMessageHookDlg::GetDWORD(UINT64 address)
{
	UINT32 y=0;
	RKM(address,&y,4);
	return y;
}

PUCHAR CMessageHookDlg::GetPNbyET(UINT64 ethread)
{
	PUCHAR y = (PUCHAR)malloc(16);
	IoControl(hMyDrv ,CTL_CODE_GEN(0x7FF), &ethread, 8, y, 16);
	return y;
}


char *CMessageHookDlg::GetHookType(int Id)
{
	char *string;
	string=(char*)malloc(32);
	switch(Id)
	{
		case -1:
		{
			strcpy(string,"WH_MSGFILTER");
			break;
		}
		case 0:
		{
			strcpy(string,"WH_JOURNALRECORD");
			break;
		}
		case 1:
		{
			strcpy(string,"WH_JOURNALPLAYBACK");
			break;
		}
		case 2:
		{
			strcpy(string,"WH_KEYBOARD");
			break;
		}
		case 3:
		{
			strcpy(string,"WH_GETMESSAGE");
			break;
		}
		case 4:
		{
			strcpy(string,"WH_CALLWNDPROC");
			break;
		}
		case 5:
		{
			strcpy(string,"WH_CBT");
			break;
		}
		case 6:
		{
			strcpy(string,"WH_SYSMSGFILTER");
			break;
		}
		case 7:
		{
			strcpy(string,"WH_MOUSE");
			break;
		}
		case 8:
		{
			strcpy(string,"WH_HARDWARE");
			break;
		}
		case 9:
		{
			strcpy(string,"WH_DEBUG");
			break;
		}
		case 10:
		{
			strcpy(string,"WH_SHELL");
			break;
		}
		case 11:
		{
			strcpy(string,"WH_FOREGROUNDIDLE");
			break;
		}
		case 12:
		{
			strcpy(string,"WH_CALLWNDPROCRET");
			break;
		}
		case 13:
		{
			strcpy(string,"WH_KEYBOARD_LL");
			break;
		}
		case 14:
		{
			strcpy(string,"WH_MOUSE_LL");
			break;
		}
		default:
		{
			strcpy(string,"????");
			break;
		}
	}
	return string;
}

char *CMessageHookDlg::GetHookFlagString(int Flag)
{
	char *string;
	string=(char*)malloc(8);
	if(Flag==1 || Flag==3)
		strcpy(string,"Global");
	else
		strcpy(string,"Local");
	return string;
}

void CMessageHookDlg::EnumMsgHook()
{
    char szBuf[MAXBYTE] = {0};
	int i=0;
	UINT64 pgSharedInfo = (UINT64)GetProcAddress(GetModuleHandleA("user32.dll"), "gSharedInfo");
	UINT64 phe = GetQWORD(pgSharedInfo+8);	//+0x008 aheList          : Ptr64 _HANDLEENTRY
	UINT64 count = GetQWORD(GetQWORD(pgSharedInfo)+8);
	HANDLEENTRY heStruct={0};
	HOOK_INFO Hook={0};
	for(i=0;i<count;i++)
	{
		memcpy(&heStruct,(PVOID)(phe + i*sizeof(HANDLEENTRY)),sizeof(HANDLEENTRY));
		if(heStruct.bType==5)
		{
			RKM(heStruct.phead,&Hook,sizeof(HOOK_INFO));
			//printf("hHandle:     0x%llx\n",Hook.hHandle);
            wsprintf(szBuf, "0x%llx", Hook.hHandle);
            AfxMessageBox(szBuf);
            m_LstMsgHook.InsertItem(i, szBuf);

			//printf("iHookFlags:  %s\n",GetHookFlagString(Hook.iHookFlags));
            wsprintf(szBuf, "%s", GetHookFlagString(Hook.iHookFlags));
            m_LstMsgHook.SetItemText(i, 1, szBuf); 

			//printf("iHookType:   %s\n",GetHookType(Hook.iHookType));
            wsprintf(szBuf, "%s", GetHookType(Hook.iHookType));
            m_LstMsgHook.SetItemText(i, 2, szBuf); 


			//printf("OffPfn:      0x%llx\n",Hook.OffPfn);
            wsprintf(szBuf, "0x%llx", Hook.OffPfn);
            m_LstMsgHook.SetItemText(i, 3, szBuf); 

			//printf("ETHREAD:     0x%llx\n",GetQWORD((UINT64)(Hook.Win32Thread)));
            wsprintf(szBuf, "0x%llx", GetQWORD((UINT64)(Hook.Win32Thread)));
            m_LstMsgHook.SetItemText(i, 4, szBuf); 

			//printf("ProcessName: %s\n\n",GetPNbyET(GetQWORD((UINT64)(Hook.Win32Thread))));
            wsprintf(szBuf, "%s", GetPNbyET(GetQWORD((UINT64)(Hook.Win32Thread))));
            m_LstMsgHook.SetItemText(i, 4, szBuf); 
		}
        else
        {
            AfxMessageBox("heStruct.bType!=5");
        }
	}
}
