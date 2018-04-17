// GetSrvRet.cpp: implementation of the CGetSrvRet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mid.h"
#include "GetSrvRet.h"
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

CGetSrvRet::CGetSrvRet(LPVOID lParam)
{
    m_lParam = lParam;
}

CGetSrvRet::~CGetSrvRet()
{

}

BOOL CGetSrvRet::Exec()
{
    TRACE("ssc Mid CGetSrvRet::Exec() begin!");

    CMidDlg *pMidDlg = (CMidDlg *)m_lParam;
    int nDataSize = 0;
    char *pBuf = NULL;
    int nRet = 0;

    while (TRUE)
    {
        if (pMidDlg->m_sockMid == INVALID_SOCKET)
        {
            TRACE("ssc pMidDlg->m_sockListen is error!!");
            Sleep(1000);
            continue;
        }
        CMyPacket recvPkt;
        CMySocket sock(pMidDlg->m_sockMid);
        
        nRet = sock.RecvPacket(recvPkt);
        if (nRet == -1)
        {
            TRACE("ssc Mid sock.RecvPacket nRet == -1!");
            break;
        }
        
        pBuf = recvPkt.GetBuf();
        nDataSize = recvPkt.GetLength();
        TRACE("ssc Mid CGetSrvRet pBuf[0] is %d, nDataSize is %d", pBuf[0], nDataSize);
        switch (pBuf[0])
        {
        case SQL_RESULT: //服务端返回的SQL结果
            {
                TRACE("ssc Mid CGetSrvRet pBuf[0] is SQL_RESULT"); 
                //提取出客户端套接字
                pBuf++;
                SOCKET sClient = INVALID_SOCKET;
                memcpy(&sClient, pBuf, sizeof(sClient));
                TRACE("ssc Mid CGetSrvRet sClient is %d", sClient);
                pBuf += sizeof(SOCKET); //SRV_SQL_RESULT的大小加上SOCKET的大小
                nDataSize -= 1 + sizeof(SOCKET); //nDataSize同理，减去没用的大小
                TRACE("ssc Mid CGetSrvRet pBuf is %d , nDataSize is %d", pBuf, nDataSize);
                pMidDlg->GetSrvSelectSQL(sClient, pBuf, nDataSize); 
                break;
            }   
        case SQL_SUCCESS:
            {
                TRACE("ssc Mid CGetSrvRet pBuf[0] is SQL_SUCCESS"); 
                pBuf++;
                SOCKET sClient = INVALID_SOCKET;
                memcpy(&sClient, pBuf, sizeof(sClient));
                TRACE("ssc Mid CGetSrvRet sClient is %d", sClient);
                pBuf += sizeof(SOCKET); //SRV_SQL_RESULT的大小加上SOCKET的大小
                nDataSize -= 1 + sizeof(SOCKET); //nDataSize同理，减去没用的大小
                TRACE("ssc Mid CGetSrvRet pBuf is %d , nDataSize is %d", pBuf, nDataSize);

                //通知客户端执行成功
                CMySocket skt(sClient);
                CMyPacket sendPkt;
                
                BYTE btType = SQL_SUCCESS;
                int nBufSize = sizeof(BYTE);
                int nRet = 0;
                if (sendPkt.Append((char*)&btType, sizeof(btType)) == NULL )
                {
                    TRACE("ssc Mid sendPkt.Append failed!");
                    return FALSE;
                }
                
                skt.SendPacket(sendPkt);       
                break;
            }
        default:
            break;
        }
    }
    return TRUE;
}
