#if !defined(AFX_SHADOWSSDTDLG_H__BB8F9AFF_C022_4A77_BF31_8E5995085A00__INCLUDED_)
#define AFX_SHADOWSSDTDLG_H__BB8F9AFF_C022_4A77_BF31_8E5995085A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShadowSSDTDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShadowSSDTDlg dialog

extern char *g_szShadowSSDTFuncName[];

class CShadowSSDTDlg : public CDialog
{
// Construction
public:
	CShadowSSDTDlg(CWnd* pParent = NULL);   // standard constructor

    DWORD GetShadowSSDTFromSys();

// Dialog Data
	//{{AFX_DATA(CShadowSSDTDlg)
	enum { IDD = DLG_SHADOWSSDT };
	CListCtrl	m_LstShadow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowSSDTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShadowSSDTDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADOWSSDTDLG_H__BB8F9AFF_C022_4A77_BF31_8E5995085A00__INCLUDED_)
