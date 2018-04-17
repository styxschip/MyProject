#if !defined(AFX_KERNELMODULEDLG_H__1C6C381F_E598_4154_A17A_CCA76C7A6E28__INCLUDED_)
#define AFX_KERNELMODULEDLG_H__1C6C381F_E598_4154_A17A_CCA76C7A6E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KernelModuleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKernelModuleDlg dialog

class CKernelModuleDlg : public CDialog
{
// Construction
public:
	CKernelModuleDlg(CWnd* pParent = NULL);   // standard constructor

    DWORD GetKrnlModuleFromSys();

// Dialog Data
	//{{AFX_DATA(CKernelModuleDlg)
	enum { IDD = DLG_KRNLMODULE };
	CListCtrl	m_LstModule;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKernelModuleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKernelModuleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KERNELMODULEDLG_H__1C6C381F_E598_4154_A17A_CCA76C7A6E28__INCLUDED_)
