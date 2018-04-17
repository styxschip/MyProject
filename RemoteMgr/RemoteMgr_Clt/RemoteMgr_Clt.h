// RemoteMgr_Clt.h : main header file for the REMOTEMGR_CLT application
//

#if !defined(AFX_REMOTEMGR_CLT_H__9BCD784F_4804_4763_A34A_189F7DD17662__INCLUDED_)
#define AFX_REMOTEMGR_CLT_H__9BCD784F_4804_4763_A34A_189F7DD17662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRemoteMgr_CltApp:
// See RemoteMgr_Clt.cpp for the implementation of this class
//

class CRemoteMgr_CltApp : public CWinApp
{
public:
	CRemoteMgr_CltApp();

    SOCKET m_theAppSock;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteMgr_CltApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRemoteMgr_CltApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEMGR_CLT_H__9BCD784F_4804_4763_A34A_189F7DD17662__INCLUDED_)
