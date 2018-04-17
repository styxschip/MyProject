#if !defined(AFX_MYSSDTDLG_H__54FD8C2D_6F7A_4F42_92F0_75964705C310__INCLUDED_)
#define AFX_MYSSDTDLG_H__54FD8C2D_6F7A_4F42_92F0_75964705C310__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySSDTDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySSDTDlg dialog

extern char *g_szSSDTFuncName[];

class CMySSDTDlg : public CDialog
{
// Construction
public:
	CMySSDTDlg(CWnd* pParent = NULL);   // standard constructor

    DWORD GetSSDTFromSys();

// Dialog Data
	//{{AFX_DATA(CMySSDTDlg)
	enum { IDD = DLG_SSDT };
	CListCtrl	m_LstSSDT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySSDTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMySSDTDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSSDTDLG_H__54FD8C2D_6F7A_4F42_92F0_75964705C310__INCLUDED_)
