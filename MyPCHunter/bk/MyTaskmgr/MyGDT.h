#if !defined(AFX_MYGDT_H__28822B3E_B7FC_4B3E_9923_EA30712AEC4F__INCLUDED_)
#define AFX_MYGDT_H__28822B3E_B7FC_4B3E_9923_EA30712AEC4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyGDT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyGDT dialog

class CMyGDT : public CDialog
{
// Construction
public:
	CMyGDT(CWnd* pParent = NULL);   // standard constructor

    DWORD GetGDTFromSys();

// Dialog Data
	//{{AFX_DATA(CMyGDT)
	enum { IDD = DLG_GDT };
	CListCtrl	m_LstGDT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyGDT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyGDT)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGDT_H__28822B3E_B7FC_4B3E_9923_EA30712AEC4F__INCLUDED_)
