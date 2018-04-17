#if !defined(AFX_KERNELMODULEDLG_H__1C6C381F_E598_4154_A17A_CCA76C7A6E28__INCLUDED_)
#define AFX_KERNELMODULEDLG_H__1C6C381F_E598_4154_A17A_CCA76C7A6E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KernelModuleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKernelModuleDlg dialog

typedef LONG NTSTATUS;

typedef NTSTATUS (*pfn_ZWQUERYSYSTEMINFORMATION)
(
	IN ULONG	SystemInformationClass,
    OUT PVOID	SystemInformation,
    IN ULONG	Length,
    OUT PULONG	ReturnLength
);


typedef unsigned long DWORD;

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY
{
    ULONG Unknow1;
    ULONG Unknow2;
    ULONG Unknow3;
    ULONG Unknow4;
    PVOID Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT NameLength;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    char ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
    ULONG Count;//内核中以加载的模块的个数
    SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

//枚举模块结构体
typedef struct _tagEnumModule
{
    ULONGLONG m_qwImageBase;
    ULONG m_ImageSize;
    CHAR szFullPathName[MAXBYTE];
}EnumModule, *PEnumModule;


class CKernelModuleDlg : public CDialog
{
// Construction
public:
	CKernelModuleDlg(CWnd* pParent = NULL);   // standard constructor

    DWORD GetKrnlModuleFromSys();

    BOOLEAN EnumKM();

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
