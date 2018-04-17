// MyPacket.h: interface for the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKET_H__E12DB514_A0B3_4CB4_B3E4_B8E9721E7C31__INCLUDED_)
#define AFX_MYPACKET_H__E12DB514_A0B3_4CB4_B3E4_B8E9721E7C31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
  包:
     ----------
     数据长度
     ----------
     消息类型
     ----------
     内容
     ----------
*/


enum
{
    //客户端上线通知
    CLT_ONLINE,

    //客户端下线通知
    CLT_OFFLINE,

    //中转上线通知
    MID_ONLINE,

    //中转下线通知
    MID_OFFLINE,

    //查询语句
    SQL_SELECT,

    //删除语句
    SQL_DEL,

    //增加语句
    SQL_ADD,

    //更新语句
    SQL_UPDATE,

    //返回的查询结果
    SQL_RESULT,

    //增删改等执行成功
    SQL_SUCCESS
};

class CMyPacket  
{
public:
    CMyPacket();
    virtual ~CMyPacket();
    inline char* GetBuf() const;
    inline int GetLength() const;
    char* Append(char *pNewBuf, int nLength);
private:
    //缓冲区地址
    char *m_pBuf;

    //当前使用大小
    int m_nLength;

    //缓冲区总大小
    int m_BufSize;
};

inline char* CMyPacket::GetBuf() const
{
    return m_pBuf;
}

inline int CMyPacket::GetLength() const
{
    return m_nLength;
}

class CMySocket
{
public:
    CMySocket(SOCKET s);
    virtual ~CMySocket();
    
    //发送数据包CMyPacket& pkt
    int SendPacket(CMyPacket& pkt);

    //发送数据
    int SendData(char *pBuf, int nLength);

    //接收数据包
    int RecvPacket(CMyPacket& pkt);

    //接收数据
    int RecvData(char *pBuf, int nLength);
private:
    SOCKET m_socket;
};

#endif // !defined(AFX_MYPACKET_H__E12DB514_A0B3_4CB4_B3E4_B8E9721E7C31__INCLUDED_)
