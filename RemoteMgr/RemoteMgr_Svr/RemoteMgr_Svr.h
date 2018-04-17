// RemoteMgr_Svr.h : main header file for the REMOTEMGR_SVR application
//

#if !defined(AFX_REMOTEMGR_SVR_H__A30BE687_E307_45D1_A722_7CF372CD02DE__INCLUDED_)
#define AFX_REMOTEMGR_SVR_H__A30BE687_E307_45D1_A722_7CF372CD02DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRemoteMgr_SvrApp:
// See RemoteMgr_Svr.cpp for the implementation of this class
//

#define WM_CLRSUBDLG WM_USER + 1

class CRemoteMgr_SvrApp : public CWinApp
{
public:
	CRemoteMgr_SvrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteMgr_SvrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRemoteMgr_SvrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEMGR_SVR_H__A30BE687_E307_45D1_A722_7CF372CD02DE__INCLUDED_)
