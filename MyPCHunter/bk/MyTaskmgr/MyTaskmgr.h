// MyTaskmgr.h : main header file for the MYTASKMGR application
//

#if !defined(AFX_MYTASKMGR_H__443897C1_9839_4B92_87AF_4640288F80C3__INCLUDED_)
#define AFX_MYTASKMGR_H__443897C1_9839_4B92_87AF_4640288F80C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include <Winsvc.h>

/////////////////////////////////////////////////////////////////////////////
// CMyTaskmgrApp:
// See MyTaskmgr.cpp for the implementation of this class
//

class CMyTaskmgrApp : public CWinApp
{
public:
	CMyTaskmgrApp();

    SC_HANDLE m_schSCManager;
    SC_HANDLE m_schService;

    //关联驱动的对象
    HANDLE m_hDrvFile; 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTaskmgrApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL InitApplication();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyTaskmgrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTASKMGR_H__443897C1_9839_4B92_87AF_4640288F80C3__INCLUDED_)
