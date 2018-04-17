// play_airplaneDlg.h : header file
//

#if !defined(AFX_PLAY_AIRPLANEDLG_H__837F7307_EC09_41CB_A588_ACA57B5394CF__INCLUDED_)
#define AFX_PLAY_AIRPLANEDLG_H__837F7307_EC09_41CB_A588_ACA57B5394CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////////
// CPlay_airplaneDlg dialog

//子弹坐标结构体
#pragma pack(push)
#pragma pack(1)
struct stBullet
{
    DWORD dwX;
    DWORD dwY;
    char chAryNoUse[7];
};
#pragma pack(pop)

class CPlay_airplaneDlg : public CDialog
{
// Construction
public:
	CPlay_airplaneDlg(CWnd* pParent = NULL);	// standard constructor
    void AIFun(BOOL bIsAuto);

    //nRgn是飞机范围的值，作为参数方便调整
    BOOL CheckInRgn(stBullet *pBuf, 
                    int nCount, 
                    DWORD airplaneX, 
                    DWORD airplaneY,
                    DWORD nRgn);

    BOOL FindGameProc();
    HANDLE m_hProcess;
    DWORD m_airplaneX;
    DWORD m_airplaneY;
    CList<stBullet *, stBullet *> m_LstRelease;
    //是否打中
    BOOL m_IsFind; 

// Dialog Data
	//{{AFX_DATA(CPlay_airplaneDlg)
	enum { IDD = IDD_PLAY_AIRPLANE_DIALOG };
	DWORD	m_ProcessID;
	DWORD	m_BulletNum;
	int		m_Mode;
	BYTE	m_Shift;
	BOOL	m_AI;
    CSpinButtonCtrl	m_Spin;
	BOOL	m_bAways;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlay_airplaneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPlay_airplaneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonFindProcess();
	afx_msg void OnRadioMode();
	afx_msg void OnRadioMode2();
	afx_msg void OnButtonSetNum();
	afx_msg void OnButtonShift();
	afx_msg void OnDeltaposSpinPen(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAI();
	afx_msg void OnButtonSetInitZero();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckLiveaways();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAY_AIRPLANEDLG_H__837F7307_EC09_41CB_A588_ACA57B5394CF__INCLUDED_)
