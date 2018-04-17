// play_airplane.h : main header file for the PLAY_AIRPLANE application
//

#if !defined(AFX_PLAY_AIRPLANE_H__9FA5B1EA_5E12_4C54_B522_CF6CEFFEC272__INCLUDED_)
#define AFX_PLAY_AIRPLANE_H__9FA5B1EA_5E12_4C54_B522_CF6CEFFEC272__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlay_airplaneApp:
// See play_airplane.cpp for the implementation of this class
//

class CPlay_airplaneApp : public CWinApp
{
public:
	CPlay_airplaneApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlay_airplaneApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPlay_airplaneApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAY_AIRPLANE_H__9FA5B1EA_5E12_4C54_B522_CF6CEFFEC272__INCLUDED_)
