// FileMgr.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "FileMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileMgr dialog


CFileMgr::CFileMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CFileMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileMgr, CDialog)
	//{{AFX_MSG_MAP(CFileMgr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileMgr message handlers
