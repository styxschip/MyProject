// MyCmd_clt.h : main header file for the MYCMD_CLT application
//

#if !defined(AFX_MYCMD_CLT_H__5557BC9D_632D_4820_BB68_03EF6100F3BC__INCLUDED_)
#define AFX_MYCMD_CLT_H__5557BC9D_632D_4820_BB68_03EF6100F3BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyCmd_cltApp:
// See MyCmd_clt.cpp for the implementation of this class
//

class CMyCmd_cltApp : public CWinApp
{
public:
	CMyCmd_cltApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCmd_cltApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyCmd_cltApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMD_CLT_H__5557BC9D_632D_4820_BB68_03EF6100F3BC__INCLUDED_)
