// AcceptThread.cpp: implementation of the CAcceptThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mid.h"
#include "AcceptThread.h"
#include "MidDlg.h"
#include "RecvThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcceptThread::CAcceptThread(LPVOID lParam)
{
    m_lParam = lParam;
}

CAcceptThread::~CAcceptThread()
{
    m_lParam = NULL;
}

BOOL CAcceptThread::Exec()
{
    TRACE("ssc CAcceptThread::Exec() begin!");
    CMidDlg *pMidDlg = (CMidDlg *)m_lParam;
    if (pMidDlg->m_sockListen == INVALID_SOCKET)
    {
        return FALSE;
    }
    if (m_lParam == NULL)
    {
        return FALSE;
    }

    int nLength = sizeof(sockaddr);
    sockaddr_in addr;

    while (TRUE)
    {        
        if (pMidDlg->m_sockListen == INVALID_SOCKET)
        {
            return FALSE;
        }

        SOCKET sClient = accept(pMidDlg->m_sockListen, (sockaddr*)&addr, &nLength);    
        if (sClient != INVALID_SOCKET)
        {
            CRecvThread *pRecvThread = new CRecvThread(m_lParam, sClient);
            if (pRecvThread == NULL)
            {
                TRACE("ssc Mid CRecvThread new failed!");
                return FALSE;
            }
            
            pMidDlg->m_ThreadPool.Handle(pRecvThread); 

            //更新客户端在线列表
            pMidDlg->m_LstClt.InsertItem(pMidDlg->m_nCltCount, inet_ntoa(addr.sin_addr));
            char szTmpBuf[MAXBYTE] = {0};
            wsprintf(szTmpBuf, "%d", ntohs(addr.sin_port));
            pMidDlg->m_LstClt.SetItemText(pMidDlg->m_nCltCount, 1, szTmpBuf);
            
            wsprintf(szTmpBuf, "%08X", sClient);
            pMidDlg->m_LstClt.SetItemText(pMidDlg->m_nCltCount, 2, szTmpBuf);
            //在线客户端计数加1,客户端关闭时记得减掉
            pMidDlg->m_nCltCount++;

        }
        else
        {
            TRACE("ssc accept failed!");
            return FALSE;
        }
    }
    
    return TRUE;
}
