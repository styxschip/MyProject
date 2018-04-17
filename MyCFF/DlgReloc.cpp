// DlgReloc.cpp : implementation file
//

#include "stdafx.h"
#include "MyCFF.h"
#include "DlgReloc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReloc dialog


CDlgReloc::CDlgReloc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReloc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReloc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgReloc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReloc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReloc, CDialog)
	//{{AFX_MSG_MAP(CDlgReloc)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReloc message handlers
