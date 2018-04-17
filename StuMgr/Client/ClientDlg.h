// ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__CED3232C_5C7A_47D8_9D30_D0E1EDD16797__INCLUDED_)
#define AFX_CLIENTDLG_H__CED3232C_5C7A_47D8_9D30_D0E1EDD16797__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

    //客户端接收数据的线程
    static UINT RecvThread(LPVOID lpParameter);

    //客户端socket
    SOCKET m_sockClient;

    //屏蔽回车
    virtual void OnOK();

    //处理中转返回的结果
    void GetMidRet(char* pszBuf, int nBufSize);

    //执行SQL语句函数
    void SQLRun(char* pszSQL, int nSQLType);

// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CListCtrl	m_LstTBLClass;
	CIPAddressCtrl	m_IP;
	DWORD	m_dwPort;
	DWORD	m_dwClassID;
	CString	m_csClassName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnSelectclass();
	afx_msg void OnClassdel();
	afx_msg void OnClassadd();
	afx_msg void OnItemchangedTblcalss(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClassupdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__CED3232C_5C7A_47D8_9D30_D0E1EDD16797__INCLUDED_)
