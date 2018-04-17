#if !defined(AFX_PROCESSMGRDLG_H__FFE5A6C6_DF02_4E9F_A2F4_42653F26933F__INCLUDED_)
#define AFX_PROCESSMGRDLG_H__FFE5A6C6_DF02_4E9F_A2F4_42653F26933F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessMgrDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessMgrDlg dialog

#include <Psapi.h>

#pragma comment (lib,"Psapi.lib")

//保存进程信息的结构体
typedef struct _tagProcessInfo
{
    UCHAR m_szBuf[MAXBYTE];
    UCHAR m_szFullPath[MAXBYTE];
    __int64 m_dwPID; //x64 是 QWORD
    __int64 qwEPROCESS;
}ProcessInfo, *PProcessInfo;

class CProcessMgrDlg : public CDialog
{
// Construction
public:
	CProcessMgrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProcessMgrDlg)
	enum { IDD = DLG_PROCESS };
	CListCtrl	m_LstProcess;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessMgrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessMgrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReadprocmem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSMGRDLG_H__FFE5A6C6_DF02_4E9F_A2F4_42653F26933F__INCLUDED_)
