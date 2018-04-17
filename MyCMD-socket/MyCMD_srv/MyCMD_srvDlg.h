// MyCMD_srvDlg.h : header file
//

#if !defined(AFX_MYCMD_SRVDLG_H__057D8374_B959_4833_9A08_2B4C777C2FCC__INCLUDED_)
#define AFX_MYCMD_SRVDLG_H__057D8374_B959_4833_9A08_2B4C777C2FCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyCMD_srvDlg dialog

typedef struct _tagTHREADARGS
{
    SOCKET m_Msg;
    SOCKET m_Listen;
    char *m_pszBuf;
}THREADARGS, *PTHREADARGS;

class CMyCMD_srvDlg : public CDialog
{
// Construction
public:
	CMyCMD_srvDlg(CWnd* pParent = NULL);	// standard constructor

    static BOOL GetMsg(LPVOID lpThreadParameter);
    BOOL InitSocekt();

    SOCKET m_sockListen;
    SOCKET m_sockMsg;
    static THREADARGS m_ThreadArg;
// Dialog Data
	//{{AFX_DATA(CMyCMD_srvDlg)
	enum { IDD = IDD_MYCMD_SRV_DIALOG };
	CIPAddressCtrl	m_IP;
	CString	m_csShow;
	CString	m_csCmd;
	DWORD	m_dwPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCMD_srvDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyCMD_srvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRun();
    afx_msg void OnMyMsg();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMD_SRVDLG_H__057D8374_B959_4833_9A08_2B4C777C2FCC__INCLUDED_)
