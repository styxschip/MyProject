#if !defined(AFX_DLGINT_H__954EDB72_484F_4F8D_B96D_D61344A48817__INCLUDED_)
#define AFX_DLGINT_H__954EDB72_484F_4F8D_B96D_D61344A48817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgINT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgINT dialog

class DlgINT : public CDialog
{
// Construction
public:
	DlgINT(CWnd* pParent = NULL);   // standard constructor

    // 接收数据用
    void SetData(char *szModuleName, char *pDataBuf, int nCount);

    // dll名做窗口标题
    char *m_szModuleName;

    char *m_pDataBuf;
    int m_nCount;

// Dialog Data
	//{{AFX_DATA(DlgINT)
	enum { IDD = IDD_INT };
	CListCtrl	m_LstINT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgINT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgINT)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINT_H__954EDB72_484F_4F8D_B96D_D61344A48817__INCLUDED_)
