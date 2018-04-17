#if !defined(AFX_MYCMDDLG_H__C5F41E88_FE4A_46AC_A70B_784C910FE8C1__INCLUDED_)
#define AFX_MYCMDDLG_H__C5F41E88_FE4A_46AC_A70B_784C910FE8C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyCMDDlg dialog

typedef struct tagTHREADPARAM
{
    CString *m_pcsStr;
    HANDLE m_hPipe;
}THREADPARAM, *PTHREADPARAM;

class CMyCMDDlg : public CDialog
{
// Construction
public:
	CMyCMDDlg(CWnd* pParent = NULL);	// standard constructor

public:
    HANDLE hCmdReadPipe;
    HANDLE hCmdWritePipe;
    HANDLE hMyReadPipe;
    HANDLE hMyWritePipe;

    PROCESS_INFORMATION m_pi;
// Dialog Data
	//{{AFX_DATA(CMyCMDDlg)
	enum { IDD = DLG_MYCMD };
	CString	m_csEnter;
	CString	m_csShowCmd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCMDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyCMDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMDDLG_H__C5F41E88_FE4A_46AC_A70B_784C910FE8C1__INCLUDED_)
