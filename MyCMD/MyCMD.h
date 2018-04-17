// MyCMD.h : main header file for the MYCMD application
//

#if !defined(AFX_MYCMD_H__70BE5C07_7E39_41DA_B465_7EAEC338FB9B__INCLUDED_)
#define AFX_MYCMD_H__70BE5C07_7E39_41DA_B465_7EAEC338FB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyCMDApp:
// See MyCMD.cpp for the implementation of this class
//

class CMyCMDApp : public CWinApp
{
public:
	CMyCMDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCMDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyCMDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMD_H__70BE5C07_7E39_41DA_B465_7EAEC338FB9B__INCLUDED_)
