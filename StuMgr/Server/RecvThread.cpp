// RecvThread.cpp: implementation of the CRecvThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "RecvThread.h"
#include "ServerDlg.h"
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
    m_sockMid = socket;
}

CRecvThread::~CRecvThread()
{
    m_lParam = NULL;
    m_sockMid = NULL;
}

BOOL CRecvThread::Exec()
{
    TRACE("ssc Server CRecvThread::Exec() begin!");
    
    CServerDlg *pServerDlg = (CServerDlg *)m_lParam;
    int nDataSize = 0;
    char *pBuf = NULL;
    int nRet = 0;
    
    while (TRUE)
    {
        if (pServerDlg->m_sockListen == INVALID_SOCKET)
        {
            TRACE("ssc Server pServerDlg->m_sockListen is error!!");
            return FALSE;
        }

        if (m_sockMid == INVALID_SOCKET)
        {
            TRACE("ssc Server m_sockMid is error!!");
            return FALSE;
        }
        CMyPacket recvPkt;
        CMySocket sock(m_sockMid);
        
        nRet = sock.RecvPacket(recvPkt);
        if (nRet == -1)
        {
            TRACE("ssc Server sock.RecvPacket nRet == -1!");
            break;
        }
        
        pBuf = recvPkt.GetBuf();
        nDataSize = recvPkt.GetLength();
        TRACE("ssc Server pBuf[0] is %d, nDataSize is %d", pBuf[0], nDataSize);
        switch (pBuf[0])
        {
        case CLT_ONLINE:
            {                
                // 已经在CAcceptThread中处理上线中转
                break;
            }
        case MID_OFFLINE:
            {
                pServerDlg->RemoveOfflineMid(m_sockMid);
                m_sockMid = INVALID_SOCKET;
                break;
            }
        case SQL_SELECT:
            {
                TRACE("ssc Srv pBuf[0] is SQL_SELECT"); 
                pServerDlg->RecvMidSQL(m_sockMid, pBuf + 1, nDataSize - 1, SQL_SELECT);         
                break;
            }
        case SQL_DEL:
            {
                TRACE("ssc Srv pBuf[0] is SQL_DEL"); 
                pServerDlg->RecvMidSQL(m_sockMid, pBuf + 1, nDataSize - 1, SQL_DEL);         
                break;
            }
        case SQL_ADD:
            {
                TRACE("ssc Srv pBuf[0] is SQL_ADD"); 
                pServerDlg->RecvMidSQL(m_sockMid, pBuf + 1, nDataSize - 1, SQL_ADD);         
                break;
            }
        case SQL_UPDATE:
            {
                TRACE("ssc Srv pBuf[0] is SQL_UPDATE"); 
                pServerDlg->RecvMidSQL(m_sockMid, pBuf + 1, nDataSize - 1, SQL_UPDATE);         
                break;
            }
        default:
            break;
        }
    }
    
    return 0;
}
