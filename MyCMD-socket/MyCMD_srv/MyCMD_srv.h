// MyCMD_srv.h : main header file for the MYCMD_SRV application
//

#if !defined(AFX_MYCMD_SRV_H__85E15ED8_ED65_415E_8CA5_AA95BD64AF79__INCLUDED_)
#define AFX_MYCMD_SRV_H__85E15ED8_ED65_415E_8CA5_AA95BD64AF79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyCMD_srvApp:
// See MyCMD_srv.cpp for the implementation of this class
//

class CMyCMD_srvApp : public CWinApp
{
public:
	CMyCMD_srvApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCMD_srvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyCMD_srvApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMD_SRV_H__85E15ED8_ED65_415E_8CA5_AA95BD64AF79__INCLUDED_)
