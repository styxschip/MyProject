// MyCFFDlg.h : header file
//

#if !defined(AFX_MYCFFDLG_H__D0B7EE0D_2363_4A78_94BA_19CE5D730369__INCLUDED_)
#define AFX_MYCFFDLG_H__D0B7EE0D_2363_4A78_94BA_19CE5D730369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyCFFDlg dialog

class CMyCFFDlg : public CDialog
{
// Construction
public:
	CMyCFFDlg(CWnd* pParent = NULL);	// standard constructor
    virtual void OnOK()
    {
        return;
    }

    void InitControls(char *szExe);
    BOOL IsPEFile(char *szPath);
    void ShowDosHeader(CListCtrl& listCtrl);
    void ShowNTHeader(CListCtrl& listCtrl);
    void ShowFileHeader(CListCtrl& listCtrl);
    void ShowOptionalHeader(CListCtrl& listCtrl);
    void ShowDataDir(CListCtrl& listCtrl);
    void ShowSectionHeader(CListCtrl& listCtrl);
    void ShowIAT(CListCtrl& listCtrl);
    void ShowImportDirectory(CListCtrl& listCtrl);
    void ShowExportDirectory(CListCtrl& listCtrl);
    void ShowRelocationDirectory(CListCtrl& listCtrl);
    void ShowResourceDirectory(CListCtrl& listCtrl);
    

    char* GetSize(DWORD dwSize);
    char* GetHexFmt(DWORD dwSize);

    //RVA 转 FA 函数内不用 不检查合法性 ，默认PE中的地址都正确
    BOOL RVA2FA(DWORD dwRVA, DWORD& dwFA);

    // PE的起始位置
    DWORD m_e_lfanew;

    //PE 文件大小
    DWORD m_dwFileSize;
    DWORD m_dwHighSize;

    //内存对齐和文件对齐
    DWORD m_SectionAlignment;
    DWORD m_FileAlignment;

    // 记录NumberOfSections的值
    DWORD m_dwNumberOfSections;

    // 记录NumberOfRvaAndSizes 数据目录大小
    DWORD m_NumberOfRvaAndSizes;

    // 头的总大小
    DWORD m_SizeOfHeaders;

    // 建议载入地址
    DWORD m_ImageBase;
    
    //被打开的exe文件的缓存
    char *m_pExeFileBuf;
    
    //被打开文件的句柄
    HANDLE m_hExeFile;

    //listCtr在窗口的坐标
    CRect m_rcListCtrl;

    //一个CtrlList不够用再来一个
    CListCtrl m_lstExtend;

// Dialog Data
	//{{AFX_DATA(CMyCFFDlg)
	enum { IDD = IDD_MYCFF_DIALOG };
	CListCtrl	m_LstMemberOfStruct;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCFFDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyCFFDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenexe();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusVa();
	afx_msg void OnSetfocusRva();
	afx_msg void OnSetfocusFileoffset();
	afx_msg void OnChangeVa();
	afx_msg void OnChangeRva();
	afx_msg void OnChangeFileoffset();
	afx_msg void OnClickStruct(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCFFDLG_H__D0B7EE0D_2363_4A78_94BA_19CE5D730369__INCLUDED_)
