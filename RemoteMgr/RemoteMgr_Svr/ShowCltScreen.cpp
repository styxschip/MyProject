// ShowCltScreen.cpp : implementation file
//

#include "stdafx.h"
#include "RemoteMgr_Svr.h"
#include "ShowCltScreen.h"
#include "RemoteMgr_SvrDlg.h"
#include "../common/MyPacket.h"
#include "../common/OutputDbgMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowCltScreen dialog


CShowCltScreen::CShowCltScreen(CWnd* pParent /*=NULL*/)
	: CDialog(CShowCltScreen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowCltScreen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShowCltScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowCltScreen)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowCltScreen, CDialog)
	//{{AFX_MSG_MAP(CShowCltScreen)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowCltScreen message handlers

void CShowCltScreen::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
    
    //给主窗口发消息，通知该窗口已经关闭
    HWND hParent = ::GetParent(GetSafeHwnd());
    ::SendMessage(hParent, WM_CLRSUBDLG, (WPARAM)GetSafeHwnd(), NULL);	
}

void CShowCltScreen::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    SOCKET sClient = CRemoteMgr_SvrDlg::m_SelSocket;

    BYTE btType = SVR_LBTNDOWN;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(sClient);
    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        return;
    }

    //附加point数据
    if (pkt.Append((char*)&point, sizeof(CPoint)) == NULL)
    {
        return;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        return;
    }
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CShowCltScreen::OnLButtonUp(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    SOCKET sClient = CRemoteMgr_SvrDlg::m_SelSocket;
    
    BYTE btType = SVR_LBTNUP;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(sClient);
    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        return;
    }
    
    if (pkt.Append((char*)&point, sizeof(CPoint)) == NULL)
    {
        return;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        return;
    }
    CDialog::OnLButtonUp(nFlags, point);
}

void CShowCltScreen::OnMouseMove(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    SOCKET sClient = CRemoteMgr_SvrDlg::m_SelSocket;
    
    BYTE btType = SVR_MOUSEMOVE;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(sClient);
    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        return;
    }
    
    if (pkt.Append((char*)&point, sizeof(CPoint)) == NULL)
    {
        return;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        return;
    }

    CDialog::OnMouseMove(nFlags, point);
}


void CShowCltScreen::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    SOCKET sClient = CRemoteMgr_SvrDlg::m_SelSocket;
    
    BYTE btType = SVR_DOUBLE_CLICK;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    
    CMyPacket pkt;
    CMySocket sendSkt(sClient);
    if (pkt.Append((char*)&btType, 1) == NULL)
    {
        return;
    }

    if (pkt.Append((char*)&point, sizeof(CPoint)) == NULL)
    {
        return;
    }
    
    nRet = sendSkt.SendPacket(pkt);
    if (nRet == -1)
    {
        return;
    }

	CDialog::OnLButtonDblClk(nFlags, point);
}


