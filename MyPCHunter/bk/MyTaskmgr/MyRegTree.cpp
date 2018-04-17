// MyRegTree.cpp : implementation file
//

#include "stdafx.h"
#include <AFXTEMPL.H>
#include "MyTaskmgr.h"
#include "MyRegTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRegTree

CMyRegTree::CMyRegTree()
{
}

CMyRegTree::~CMyRegTree()
{
}


BEGIN_MESSAGE_MAP(CMyRegTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyRegTree)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRegTree message handlers

void CMyRegTree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

    CString csStr;
    CList<CString, CString&> lstStr;
    DWORD dwIndex = -1;
    DWORD dwTmp = -1;
    HTREEITEM hItem = pNMTreeView->itemNew.hItem;
    if (this->GetParentItem(hItem) != NULL)
    {
        if (pNMTreeView->action == TVE_EXPAND)   //节点展开 +变-
        {            
            HTREEITEM hCurItem = hItem;
            CString csTmp;
            // = this->GetItemText(hCurItem);
            while (csTmp != "计算机" && csTmp.IsEmpty() == FALSE)
            {             
                csStr.Insert(0, csTmp);
                hCurItem = this->GetParentItem(hCurItem);
                //获取节点索引
                DWORD dwTmp = this->GetItemData(hCurItem);
                if (dwTmp > 0)
                {
                    dwIndex = dwTmp;
                }
                else
                {
                    //不是五个根节点时再获取节点字符串
                    csStr.Insert(0, "\\");  
                    csTmp = this->GetItemText(hCurItem);

                    char szSubKey[MAXBYTE] = {0};
                    int i = 0;
                    LONG retCode = ERROR_SUCCESS;
                    DWORD dwSize = MAXBYTE;
                    for (i = 0; retCode == ERROR_SUCCESS; i++) 
                    { 
                        //retCode = GetSubKeyName(hKey, i, szSubKey, &dwSize);//??
                        //if (retCode == ERROR_SUCCESS) 
                        //{
                            //lstStr.AddTail(szSubKey);
                            //dwSize = MAXBYTE;
                        //}
                     } 
                } //end if (dwTmp > 0)
            }            
        } //end while (csTmp != "计算机" && csTmp.IsEmpty() == FALSE)
    }
    csStr.TrimLeft('\\');
    //AfxMessageBox(csStr);

    HKEY hKeyAry[] = {
        HKEY_CLASSES_ROOT,
        HKEY_CURRENT_USER,
        HKEY_LOCAL_MACHINE,
        HKEY_USERS,
        HKEY_CURRENT_CONFIG
    };

    HKEY hKey = NULL;
    LONG lRet = RegOpenKeyEx(hKeyAry[dwIndex - 1], csStr, 0, KEY_READ, &hKey);
    if (lRet == ERROR_SUCCESS)
    {
        RegTreeInstNode(hKey, *this, pNMTreeView->itemNew.hItem);
    }
    
	*pResult = 0;
}

LONG CMyRegTree::GetSubKeyName(HKEY hKey, int nIndex, char *szKeyName, DWORD *pdwSize)
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

void CMyRegTree::RegTreeInstNode(HKEY hKey, CTreeCtrl& TreeNode, HTREEITEM hNode)
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

