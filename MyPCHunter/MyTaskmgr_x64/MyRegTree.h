#if !defined(AFX_MYREGTREE_H__DBDFB608_55F9_43E9_8AEC_7F01BACF9E51__INCLUDED_)
#define AFX_MYREGTREE_H__DBDFB608_55F9_43E9_8AEC_7F01BACF9E51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyRegTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyRegTree window

class CMyRegTree : public CTreeCtrl
{
// Construction
public:
	CMyRegTree();

// Attributes
public:

// Operations
public:
    //获取子节点名
    LONG GetSubKeyName(HKEY hKey, int nIndex, char *szKeyName, DWORD *pdwSize);
    //填充树节点
    void RegTreeInstNode(HKEY hKey, CTreeCtrl& TreeNode, HTREEITEM hNode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRegTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyRegTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyRegTree)
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYREGTREE_H__DBDFB608_55F9_43E9_8AEC_7F01BACF9E51__INCLUDED_)
