// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__9D418DA3_BA7F_4106_B309_CECE670EAF44__INCLUDED_)
#define AFX_SERVERDLG_H__9D418DA3_BA7F_4106_B309_CECE670EAF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\ThreadPool.h"

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor
    virtual void OnOK();
    BOOL InitListCtrl();

    //在线中转数量
    int m_nMidCount;
    
    //中转列表清理下线的中转
    BOOL RemoveOfflineMid(SOCKET sMid);
    
    //处理中转的查询语句
    BOOL RecvMidSQL(SOCKET sMid, char* pszBuf, int nBufLength, int nSQLType);
    
    //监听SOCKET
    SOCKET m_sockListen;
    
    //线程池
    CThreadPool m_ThreadPool;

    //连接字符串
    CString m_csConnstr;

    //操作数据库相关
    _ConnectionPtr m_conn;
    _RecordsetPtr  m_rs;

    //执行 SQL 语句
    _RecordsetPtr CServerDlg::ExecuteSQLstr(_ConnectionPtr conn, char *pszSql);

    //获得结果
    CString CServerDlg::GetRecord(const _RecordsetPtr& rs);

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CListCtrl	m_LstMid;
	CIPAddressCtrl	m_IP;
	DWORD	m_dwDBPort;
	CString	m_csUsrName;
	CString	m_csPasWord;
	CString	m_csDBName;
	DWORD	m_dwSrvPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__9D418DA3_BA7F_4106_B309_CECE670EAF44__INCLUDED_)
