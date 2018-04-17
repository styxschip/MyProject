#if !defined(AFX_DLGRELOC_H__1D8CAA83_0EF7_43E0_9063_168BF0E5449A__INCLUDED_)
#define AFX_DLGRELOC_H__1D8CAA83_0EF7_43E0_9063_168BF0E5449A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReloc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReloc dialog

class CDlgReloc : public CDialog
{
// Construction
public:
	CDlgReloc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgReloc)
	enum { IDD = DLG_RELOC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReloc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgReloc)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRELOC_H__1D8CAA83_0EF7_43E0_9063_168BF0E5449A__INCLUDED_)
