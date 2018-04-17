// DialogReg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "DialogReg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogReg dialog


CDialogReg::CDialogReg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogReg)
	DDX_Control(pDX, IDC_TREE1, m_TreeNode);
	DDX_Control(pDX, IDC_LIST1, m_LstValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogReg, CDialog)
	//{{AFX_MSG_MAP(CDialogReg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogReg message handlers

BOOL CDialogReg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    }
	return CDialog::PreTranslateMessage(pMsg);
}

//根据列名、内容自动设置列宽
void CDialogReg::AutoSet(CListCtrl& ListCtl)
{    
    CHeaderCtrl* pCtlHeader = ListCtl.GetHeaderCtrl();
    int nColumnCount = pCtlHeader->GetItemCount();
    //根据列名、内容自动设置列宽    
    for(int i = 0; i < nColumnCount; i++)  
    {  
        ListCtl.SetColumnWidth(i, LVSCW_AUTOSIZE);  
        int nColumnWidth = ListCtl.GetColumnWidth(i); 
        ListCtl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  
        int nHeaderWidth = ListCtl.GetColumnWidth(i);  
        ListCtl.SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));  
    } 
}

LONG CDialogReg::GetSubKeyName(HKEY hKey, int nIndex, char *szKeyName, DWORD *pdwSize)
{
    LONG retCode = ERROR_SUCCESS;
    retCode = RegEnumKeyEx(
                            hKey, 
                            nIndex, 
                            szKeyName, 
                            pdwSize, 
                            NULL, 
                            NULL, 
                            NULL, 
                            NULL); 
    return retCode;
}

void CDialogReg::RegTreeInstNode(HKEY hKey, CTreeCtrl& TreeNode, HTREEITEM hNode)
{
    //遍历节点
    char szSubKey[MAXBYTE] = {0};
    int i = 0;
    LONG retCode = ERROR_SUCCESS;
    DWORD dwSize = MAXBYTE;
    for (i = 0; retCode == ERROR_SUCCESS; i++) 
    { 
        retCode = GetSubKeyName(hKey, i, szSubKey, &dwSize);
        if (retCode == ERROR_SUCCESS) 
        {
            TreeNode.InsertItem(szSubKey, hNode);
            dwSize = MAXBYTE;
        }
    } 
}


BOOL CDialogReg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//     static TVITEM tvi;     
//     tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
//     wsprintf(szKeyName, "%s", "计算机");
//     tvi.pszText = szKeyName;
//     tvi.cchTextMax = MAXBYTE;
//     m_TreeNode.SetItem(&tvi);

    //填充treectrl的根节点和五个节点
    HTREEITEM hRoot = m_TreeNode.InsertItem("计算机");
    HTREEITEM hHKCR = m_TreeNode.InsertItem("HKEY_CLASSES_ROOT", hRoot);
    //设一个数，方便在treectrl中处理hKey
    m_TreeNode.SetItemData(hHKCR, 1);

    HTREEITEM hHKCU = m_TreeNode.InsertItem("HKEY_CURRENT_USER", hRoot);
    m_TreeNode.SetItemData(hHKCU, 2);

    HTREEITEM hHKLM = m_TreeNode.InsertItem("HKEY_LOCAL_MACHINE", hRoot);
    m_TreeNode.SetItemData(hHKLM, 3);

    HTREEITEM hHKUR = m_TreeNode.InsertItem("HKEY_USERS", hRoot);
    m_TreeNode.SetItemData(hHKUR, 4);

    HTREEITEM hHKCC = m_TreeNode.InsertItem("HKEY_CURRENT_CONFIG", hRoot);
    m_TreeNode.SetItemData(hHKCC, 5);

    m_TreeNode.Expand(hRoot, TVE_TOGGLE/*TVE_EXPAND*/);

    //将五个根节点填充好
    RegTreeInstNode(HKEY_CLASSES_ROOT, m_TreeNode, hHKCR);
    RegTreeInstNode(HKEY_CURRENT_USER, m_TreeNode, hHKCU);
    RegTreeInstNode(HKEY_LOCAL_MACHINE, m_TreeNode, hHKLM);
    RegTreeInstNode(HKEY_USERS, m_TreeNode, hHKUR);
    RegTreeInstNode(HKEY_CURRENT_CONFIG, m_TreeNode, hHKCC);

    //设置ListCtrl风格
    m_LstValue.SetExtendedStyle(m_LstValue.GetExtendedStyle() |
                                LVS_EX_GRIDLINES |
                                LVS_EX_FULLROWSELECT);
    //设置ListCtrl列名
    m_LstValue.InsertColumn(1, "名称", LVCFMT_LEFT, 200);
    m_LstValue.InsertColumn(2, "类型", LVCFMT_LEFT, 100);
    m_LstValue.InsertColumn(3, "数据", LVCFMT_LEFT, 200);

    //AutoSet(m_LstValue);
    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
