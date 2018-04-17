#if !defined(AFX_SHOWCLTSCREEN_H__7A25CEB3_0F51_4540_9628_F66D93B160A7__INCLUDED_)
#define AFX_SHOWCLTSCREEN_H__7A25CEB3_0F51_4540_9628_F66D93B160A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowCltScreen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowCltScreen dialog

class CShowCltScreen : public CDialog
{
// Construction
public:
	CShowCltScreen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShowCltScreen)
	enum { IDD = DLG_CLTSCREEN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowCltScreen)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowCltScreen)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWCLTSCREEN_H__7A25CEB3_0F51_4540_9628_F66D93B160A7__INCLUDED_)
