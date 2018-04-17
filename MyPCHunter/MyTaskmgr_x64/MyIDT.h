#if !defined(AFX_MYIDT_H__7846D074_96F6_4F5A_94A2_E58DB6E40270__INCLUDED_)
#define AFX_MYIDT_H__7846D074_96F6_4F5A_94A2_E58DB6E40270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyIDT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyIDT dialog

extern char *g_szIDTFuncName[];

class CMyIDT : public CDialog
{
// Construction
public:
	CMyIDT(CWnd* pParent = NULL);   // standard constructor

    DWORD GetIDTFromSys();

// Dialog Data
	//{{AFX_DATA(CMyIDT)
	enum { IDD = DLG_IDT };
	CListCtrl	m_LstIDT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyIDT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyIDT)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYIDT_H__7846D074_96F6_4F5A_94A2_E58DB6E40270__INCLUDED_)
