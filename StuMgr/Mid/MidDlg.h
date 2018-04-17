// MidDlg.h : header file
//

#if !defined(AFX_MIDDLG_H__518A84D1_BE1E_4231_8981_F88B2EC9B255__INCLUDED_)
#define AFX_MIDDLG_H__518A84D1_BE1E_4231_8981_F88B2EC9B255__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\ThreadPool.h"

/////////////////////////////////////////////////////////////////////////////
// CMidDlg dialog

class CMidDlg : public CDialog
{
// Construction
public:
	CMidDlg(CWnd* pParent = NULL);	// standard constructor
    virtual void OnOK();

    //初始化客户端列表
    BOOL InitListCtrl();

    //在线客户端数量
    int m_nCltCount;

    //客户端列表清理下线的客户端
    BOOL RemoveOfflineClt(SOCKET sClient);

    //处理客户端的查询语句
    BOOL RecvCltSQL(SOCKET sClient, char* pszBuf, int nBufLength, int nSQLType);

    //处理服务端的返回的结果
    BOOL GetSrvSelectSQL(SOCKET sClient, char* pszBuf, int nBufLength);

    //监听SOCKET
    SOCKET m_sockListen;

    //中转连接服务端的SOCKET
    SOCKET m_sockMid;

    //线程池
    CThreadPool m_ThreadPool;

    //将服务器返回的查询结果保存为缓存
    void SaveCache(char *pFimeName, char* pszBuf, int nBufSize);

// Dialog Data
	//{{AFX_DATA(CMidDlg)
	enum { IDD = IDD_MID_DIALOG };
	CListCtrl	m_LstClt;
	CIPAddressCtrl	m_IP;
	DWORD	m_dwMidPort;
	DWORD	m_dwSrvPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMidDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDDLG_H__518A84D1_BE1E_4231_8981_F88B2EC9B255__INCLUDED_)
