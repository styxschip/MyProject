// DlgINT.cpp : implementation file
//

#include "stdafx.h"
#include "MyCFF.h"
#include "DlgINT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgINT dialog

DlgINT::DlgINT(CWnd* pParent /*=NULL*/)
	: CDialog(DlgINT::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgINT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_szModuleName = NULL;
    m_pDataBuf = NULL;
    m_nCount = 0;
}


void DlgINT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgINT)
	DDX_Control(pDX, LST_IMPORTTABLE, m_LstINT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgINT, CDialog)
	//{{AFX_MSG_MAP(DlgINT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgINT message handlers

BOOL DlgINT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    //初始化List控件
    ;
    //设置整行选中 显示网格线
    m_LstINT.SetExtendedStyle(m_LstINT.GetExtendedStyle()|
                              LVS_EX_GRIDLINES|
                              LVS_EX_FULLROWSELECT);
    //控件上的数据都删掉
    m_LstINT.DeleteAllItems();

    //初始化列
    m_LstINT.InsertColumn(1, "IAT", LVCFMT_LEFT, 110);
    m_LstINT.InsertColumn(2, "Hint", LVCFMT_LEFT, 110);
    m_LstINT.InsertColumn(3, "Name(Function Name)", LVCFMT_LEFT, 150);

    //填充数据
    
    char szFunName[MAXBYTE] = {0};
    DWORD dwIAT = 0;
    WORD  wHint = 0;

    DWORD dwLen = sizeof(DWORD) + sizeof(WORD) + MAXBYTE;
    
    char szBuf[MAXBYTE] = {0};
    
    for (int i = 0; i < m_nCount; i++)
    {
        memcpy(&dwIAT, m_pDataBuf + i * dwLen, sizeof(dwIAT));
        memcpy(&wHint, m_pDataBuf + i * dwLen + sizeof(dwIAT), sizeof(wHint));
        memcpy(szFunName, m_pDataBuf + i * dwLen + sizeof(dwIAT) + sizeof(wHint), MAXBYTE);
        
        wsprintf(szBuf, "%08X", dwIAT);
        m_LstINT.InsertItem(i, szBuf, 0);
        
        wsprintf(szBuf, "%04X", wHint);
        m_LstINT.SetItemText(i, 1, szBuf);
        
        m_LstINT.SetItemText(i, 2, szFunName);
    }
	SetWindowText(m_szModuleName);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgINT::SetData(char *szModuleName, char *pDataBuf, int nCount)
{
    m_szModuleName = szModuleName;
    m_pDataBuf = pDataBuf;
    m_nCount = nCount;
}

