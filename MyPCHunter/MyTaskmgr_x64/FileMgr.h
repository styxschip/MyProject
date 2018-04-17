#if !defined(AFX_FILEMGR_H__8507846F_2AD0_431F_96AC_C1F47B855075__INCLUDED_)
#define AFX_FILEMGR_H__8507846F_2AD0_431F_96AC_C1F47B855075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileMgr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileMgr dialog

class CFileMgr : public CDialog
{
// Construction
public:
	CFileMgr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileMgr)
	enum { IDD = DLG_FILEMGR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileMgr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileMgr)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMGR_H__8507846F_2AD0_431F_96AC_C1F47B855075__INCLUDED_)
