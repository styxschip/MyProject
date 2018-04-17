// MyPacket.cpp: implementation of the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPacket.h"
#include "OutputDbgMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPacket::CMyPacket()
{
    m_pBuf = NULL;
    m_nLength = 0;
    m_BufSize = 64;

    m_pBuf = new char[m_BufSize];
    if (m_pBuf != NULL)
    {
        memset(m_pBuf, 0, m_BufSize);
    }
    else
    {        
        m_nLength = 0;
        m_BufSize = 0;
        MYTRACE("ssc new failed!");
    }    
}

CMyPacket::~CMyPacket()
{
    m_nLength = 0;
    m_BufSize = 0;
    if (m_pBuf != NULL)
    {
        delete[] m_pBuf;
        m_pBuf = NULL;
    }
}

char* CMyPacket::Append(char *pDataBuf, int nLength)
{
    if (pDataBuf == NULL || nLength <= 0)
    {
        return m_pBuf;
    }

    if (nLength + m_nLength <= m_BufSize)
    {
        memcpy(m_pBuf + m_nLength, pDataBuf, nLength);
        m_nLength += nLength;
    }
    else
    {
        char *pNewBuf = new char[m_nLength + nLength];
        if (pNewBuf == NULL)
        {
            MYTRACE("ssc new failed!");
            return m_pBuf;
        }
        //拷贝原数据
        if (m_pBuf != NULL)
        {
            memcpy(pNewBuf, m_pBuf, m_nLength);
        }
        
        //拷贝新数据
        memcpy(pNewBuf + m_nLength, pDataBuf, nLength);

        m_nLength += nLength;
        m_BufSize = m_nLength;

        if (m_pBuf != NULL)
        {
            delete[] m_pBuf;
            m_pBuf = NULL;
        }

        m_pBuf = pNewBuf;
    }

    return m_pBuf;
}


CMySocket::CMySocket(SOCKET s)
{
    m_socket = s;
}
CMySocket::~CMySocket()
{

}

//发送数据包CMyPacket& pkt
int CMySocket::SendPacket(CMyPacket& pkt)
{
    if (pkt.GetBuf() == NULL)
    {
        MYTRACE("ssc pkt.GetBuf() == NULL");
        return -1;
    }
    int nRet = 0;
    int nSize = pkt.GetLength();

    if (nSize <= 0)
    {
        MYTRACE("ssc pkt nSize <= 0!");
        return -1;
    }

    nRet = SendData((char*)&nSize, sizeof(int));
    if (nRet == -1)
    {
        MYTRACE("ssc SendData failed!");
        return nRet;
    }

    nRet = SendData(pkt.GetBuf(), nSize);
    if (nRet == -1)
    {
        MYTRACE("ssc SendData failed!");
        return nRet;
    }

    return nRet;
}

//发送数据
int CMySocket::SendData(char *pBuf, int nLength)
{
    if (pBuf == NULL || nLength <= 0)
    {
        MYTRACE("ssc SendData agrs error!");
        TRACE("pBuf = [%p] nLength = [%d]", pBuf, nLength);
        return -1;
    }

    int nSendSize = 0; //每次发送的大小
    int nTotaltSize = 0; //发送的大小
    while (nTotaltSize < nLength)
    {
        nSendSize = send(m_socket, pBuf + nTotaltSize, nLength - nTotaltSize, 0);
        if (nSendSize != SOCKET_ERROR)
        {
            nTotaltSize += nSendSize;
        }
        else
        {
            MYTRACE("ssc send SOCKET_ERROR!");
            return -1;
        }
    }   
    return 0;
}

//接收数据包
int CMySocket::RecvPacket(CMyPacket& pkt)
{
    int nRet = 0;
    int nSize = 0;
    nRet = RecvData((char*)&nSize, sizeof(int));
    if (nRet == -1)
    {
        MYTRACE("ssc RecvData failed!");
        return -1;
    }

    char *pBuf  = NULL;
    pBuf = new char[nSize];
    if (pBuf == NULL)
    {
        MYTRACE("ssc new failed!");
        return -1;
    }
    nRet = RecvData(pBuf, nSize);
    if (nRet == -1)
    {
        delete[] pBuf;
        return nRet;
    }

    pkt.Append(pBuf, nSize);
    delete[] pBuf;
    pBuf = NULL;

    return nRet;
}

//接收数据
int CMySocket::RecvData(char *pBuf, int nLength)
{
    if (pBuf == NULL || nLength <= 0)
    {
        MYTRACE("ssc RecvData agrs error!");
        TRACE("pBuf = [%p] nLength = [%d]", pBuf, nLength);
        return -1;
    }

    int nRecvSize = 0; //每次接收的数据大小
    int nTotaltSize = 0; //接收的总大小

    while (nTotaltSize < nLength)
    {
        nRecvSize = recv(m_socket, pBuf + nTotaltSize, nLength - nTotaltSize, 0);
        if (nRecvSize != SOCKET_ERROR && nRecvSize > 0)
        {
            nTotaltSize += nRecvSize;
        }
        else if (nRecvSize == 0)
        {
            MYTRACE("ssc recv nRecvSize == 0!");
            return -1;
        }
        else
        {
            MYTRACE("ssc recv SOCKET_ERROR!");
            return -1;
        }
    }
    return 0;
}