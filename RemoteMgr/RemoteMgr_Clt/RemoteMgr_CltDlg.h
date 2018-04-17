// RemoteMgr_CltDlg.h : header file
//

#if !defined(AFX_REMOTEMGR_CLTDLG_H__2906C5E0_81A4_48A7_9C71_767547947227__INCLUDED_)
#define AFX_REMOTEMGR_CLTDLG_H__2906C5E0_81A4_48A7_9C71_767547947227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
/////////////////////////////////////////////////////////////////////////////
// CRemoteMgr_CltDlg dialog

class CRemoteMgr_CltDlg : public CDialog
{
// Construction
public:
	CRemoteMgr_CltDlg(CWnd* pParent = NULL);	// standard constructor
    BOOL InitSocekt();
    void OnScreenReply();
    static UINT RecvThread(LPVOID lParam);

    //接收服务端发来的cmd
    void OnRecvCmd(SOCKET s, char* pszBuf, int nBufLength);

    //接收服务端发来的双击数据
    void OnRecvDoubleClick(SOCKET s, char* pszBuf, int nBufLength);

    //接收服务端发来的鼠标按下 抬起 移动
    void OnRecvLBtnDown(SOCKET s, char* pszBuf, int nBufLength);
    void OnRecvLBtnUp(SOCKET s, char* pszBuf, int nBufLength);
    void OnRecvMouseMove(SOCKET s, char* pszBuf, int nBufLength);

    //管道相关
    void MakePipe();

    static CRemoteMgr_CltDlg *m_pCltDlg;
    SOCKET m_sockClient;

    HANDLE m_hReadPipe;
    HANDLE m_hWritePipe;
    HANDLE m_hCmdReadPipe;
    HANDLE m_hCmdWritePipe;
    HANDLE m_hCmd;  
    char *m_pszBuf;
    //Handle加到链表中会不会方便管理？
    //CList<HANDLE*, HANDLE*> m_lstHandle;

    //处理管道输出相关的线程
    static DWORD GetCMDOut(LPVOID lpParameter);

    //执行cmd命令
    void ExecCommand();

// Dialog Data
	//{{AFX_DATA(CRemoteMgr_CltDlg)
	enum { IDD = IDD_REMOTEMGR_CLT_DIALOG };
	CIPAddressCtrl	m_IP;
	DWORD	m_dwPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteMgr_CltDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRemoteMgr_CltDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEMGR_CLTDLG_H__2906C5E0_81A4_48A7_9C71_767547947227__INCLUDED_)
