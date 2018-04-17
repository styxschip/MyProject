// MyCmd_cltDlg.h : header file
//

#if !defined(AFX_MYCMD_CLTDLG_H__1AC61B33_54E6_4BBE_B5DE_1B58DDAD90BF__INCLUDED_)
#define AFX_MYCMD_CLTDLG_H__1AC61B33_54E6_4BBE_B5DE_1B58DDAD90BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyCmd_cltDlg dialog
#include <AFXTEMPL.H>

typedef struct tagTHREADARG
{
    SOCKET m_sockClient;
    HWND m_hWnd;
    HANDLE m_hReadPipe;
    HANDLE m_hWritePipe;
    char *m_pszBuf;
}THREADARG, *PTHREADARG;


class CMyCmd_cltDlg : public CDialog
{
// Construction
public:
	CMyCmd_cltDlg(CWnd* pParent = NULL);	// standard constructor
    BOOL InitSocekt();
    SOCKET m_sockClient;

    HANDLE m_hReadPipe;
    HANDLE m_hWritePipe;
    HANDLE m_hCmdReadPipe;
    HANDLE m_hCmdWritePipe;
    HANDLE m_hCmd;  
    static THREADARG m_ThreadArg;
    CList<HANDLE*, HANDLE*> m_lstHandle;
    static DWORD GetCMDOut(LPVOID lpParameter);
    static DWORD ThreadRecv(LPVOID lpParameter);
    //÷¥––cmd√¸¡Ó
    void ExecCommand();


// Dialog Data
	//{{AFX_DATA(CMyCmd_cltDlg)
	enum { IDD = IDD_MYCMD_CLT_DIALOG };
	CIPAddressCtrl	m_IP;
	DWORD	m_dwPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCmd_cltDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyCmd_cltDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnConnect();
	afx_msg void OnBtnconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMD_CLTDLG_H__1AC61B33_54E6_4BBE_B5DE_1B58DDAD90BF__INCLUDED_)
