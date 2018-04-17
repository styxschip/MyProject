// MyTaskmgrDlg.h : header file
//


#if !defined(AFX_MYTASKMGRDLG_H__984B7CFE_940D_491B_B0F0_0CF48CF9929E__INCLUDED_)
#define AFX_MYTASKMGRDLG_H__984B7CFE_940D_491B_B0F0_0CF48CF9929E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogReg.h"
#include "MyCMDDlg.h"
#include "FileMgr.h"
#include "ProcessMgrDlg.h"
#include "MyGDT.h"
#include "MyIDT.h"
#include "MySSDTDlg.h"
#include "ShadowSSDTDlg.h"
#include "KernelModuleDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTaskmgrDlg dialog

typedef struct _tagTABInfo
{
    CWnd *m_pDlg; //tab窗口指针
    char *m_pDlgName;
}TABINFO, *PTABINFO;

class CMyTaskmgrDlg : public CDialog
{
// Construction
public:
	CMyTaskmgrDlg(CWnd* pParent = NULL);	// standard constructor

    //g_tabInfoAry
    PTABINFO m_ptabInfoAry;
    
    //记录tab页的数量
    int m_dwTabNum;

    CDialogReg m_DlgReg;
    CFileMgr m_DlgFile;
    CMyCMDDlg m_DlgCmd;
    CProcessMgrDlg m_DlgProcMgr;
    CMyGDT m_DlgGDT;
    CMyIDT m_DlgIDT;
    CMySSDTDlg m_DlgSSDT;
    CShadowSSDTDlg m_DlgShadow;
    CKernelModuleDlg m_DlgKrnlModule;
// Dialog Data
	//{{AFX_DATA(CMyTaskmgrDlg)
	enum { IDD = IDD_MYTASKMGR_DIALOG };
	CTabCtrl	m_tabCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTaskmgrDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyTaskmgrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTASKMGRDLG_H__984B7CFE_940D_491B_B0F0_0CF48CF9929E__INCLUDED_)
