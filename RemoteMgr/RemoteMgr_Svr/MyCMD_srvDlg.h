#if !defined(AFX_MYCMD_SRVDLG_H__4B9A71DC_A1C2_425E_BDB4_BEDDB2834237__INCLUDED_)
#define AFX_MYCMD_SRVDLG_H__4B9A71DC_A1C2_425E_BDB4_BEDDB2834237__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCMD_srvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCMD_srvDlg dialog
typedef struct _tagCLIENTINFO
{
    char m_szIP[MAXBYTE];
    char m_szMac[MAXBYTE];
}CLIENTINFO, *PCLIENTINFO;

class CMyCMD_srvDlg : public CDialog
{
// Construction
public:
	CMyCMD_srvDlg(CWnd* pParent = NULL);   // standard constructor

    //重写CDialog的虚函数OnOK()实现输入回车执行cmd命令
    virtual void OnOK();
    //SOCKET m_sockListen;

// Dialog Data
	//{{AFX_DATA(CMyCMD_srvDlg)
	enum { IDD = IDD_MYCMD_SRV_DIALOG };
	CString	m_csCmd;
	CString	m_csShow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCMD_srvDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyCMD_srvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRun();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMD_SRVDLG_H__4B9A71DC_A1C2_425E_BDB4_BEDDB2834237__INCLUDED_)
