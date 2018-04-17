// Mid.h : main header file for the MID application
//

#if !defined(AFX_MID_H__99812FB7_2DF0_4FE7_9BB2_F5D48995B7D2__INCLUDED_)
#define AFX_MID_H__99812FB7_2DF0_4FE7_9BB2_F5D48995B7D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMidApp:
// See Mid.cpp for the implementation of this class
//

class CMidApp : public CWinApp
{
public:
	CMidApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMidApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MID_H__99812FB7_2DF0_4FE7_9BB2_F5D48995B7D2__INCLUDED_)
