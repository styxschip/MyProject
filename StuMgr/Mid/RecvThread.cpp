// RecvThread.cpp: implementation of the CRecvThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mid.h"
#include "RecvThread.h"
#include "MidDlg.h"
#include "../common/MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecvThread::CRecvThread(LPVOID lParam, SOCKET socket)
{
    m_lParam = lParam;
    m_sockClient = socket;
}

CRecvThread::~CRecvThread()
{
    m_lParam = NULL;
    m_sockClient = NULL;
}

BOOL CRecvThread::Exec()
{
    TRACE("ssc Mid CRecvThread::Exec() begin!");
    
    CMidDlg *pMidDlg = (CMidDlg *)m_lParam;
    int nDataSize = 0;
    char *pBuf = NULL;
    int nRet = 0;
    
    while (TRUE)
    {
        if (pMidDlg->m_sockListen == INVALID_SOCKET)
        {
            TRACE("ssc pMidDlg->m_sockListen is error!!");
            return FALSE;
        }

        if (m_sockClient == INVALID_SOCKET)
        {
            TRACE("ssc m_sockClient is error!!");
            return FALSE;
        }
        CMyPacket recvPkt;
        CMySocket sock(m_sockClient);
        
        nRet = sock.RecvPacket(recvPkt);
        if (nRet == -1)
        {
            TRACE("ssc sock.RecvPacket nRet == -1!");
            break;
        }
        
        pBuf = recvPkt.GetBuf();
        nDataSize = recvPkt.GetLength();
        TRACE("ssc pBuf[0] is %d, nDataSize is %d", pBuf[0], nDataSize);
        switch (pBuf[0])
        {
        case CLT_ONLINE:
            {                
                // 已经在CAcceptThread中处理上线客户端
                break;
            }
        case CLT_OFFLINE:
            {
                pMidDlg->RemoveOfflineClt(m_sockClient);
                m_sockClient = INVALID_SOCKET;
                break;
            }
        case SQL_SELECT:
            {
                //从客户端接收到的SQL语句转发到服务端
                TRACE("ssc pBuf[0] is SQL_SELECT"); 
                pMidDlg->RecvCltSQL(m_sockClient, pBuf + 1, nDataSize - 1, SQL_SELECT);         
                break;
            }
        case SQL_DEL:
            {
                TRACE("ssc pBuf[0] is SQL_DEL"); 
                pMidDlg->RecvCltSQL(m_sockClient, pBuf + 1, nDataSize - 1, SQL_DEL) ;
                break;
            }
        case SQL_ADD:
            {
                TRACE("ssc pBuf[0] is SQL_ADD"); 
                pMidDlg->RecvCltSQL(m_sockClient, pBuf + 1, nDataSize - 1, SQL_ADD) ;
                break;
            }
        case SQL_UPDATE:
            {
                TRACE("ssc pBuf[0] is SQL_UPDATE"); 
                pMidDlg->RecvCltSQL(m_sockClient, pBuf + 1, nDataSize - 1, SQL_UPDATE) ;
                break;
            }
        default:
            break;
        }
    }
    
    return TRUE;
}
