// MyCMDDlg.h : header file
//

#if !defined(AFX_MYCMDDLG_H__28350852_7700_410C_9951_9E859AC2AA78__INCLUDED_)
#define AFX_MYCMDDLG_H__28350852_7700_410C_9951_9E859AC2AA78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

#define MYMSG WM_USER + 1
/////////////////////////////////////////////////////////////////////////////
// CMyCMDDlg dialog

typedef struct tagTHREADARG
{
    HWND m_hWnd;
    HANDLE m_hReadPipe;
    char *m_pszBuf;
}THREADARG, *PTHREADARG;

class CMyCMDDlg : public CDialog
{
// Construction
public:
	CMyCMDDlg(CWnd* pParent = NULL);	// standard constructor
    HANDLE m_hReadPipe;
    HANDLE m_hWritePipe;
    HANDLE m_hCmdReadPipe;
    HANDLE m_hCmdWritePipe;
    HANDLE m_hCmd;
    HANDLE m_hThread;    
    static THREADARG m_ThreadArg;
    CList<HANDLE*, HANDLE*> m_lstHandle;

    static DWORD WINAPI GetCMDOut(LPVOID lpParameter);
    //÷¥––cmd√¸¡Ó
    void ExecCommand();
// Dialog Data
	//{{AFX_DATA(CMyCMDDlg)
	enum { IDD = IDD_MYCMD_DIALOG };
	CString	m_csEnter;
	CString	m_csShow;
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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnRun();
    afx_msg void OnMyMsg();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMDDLG_H__28350852_7700_410C_9951_9E859AC2AA78__INCLUDED_)
