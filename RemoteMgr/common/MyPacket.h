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
    //服务端启动CMD窗口
    SVR_STARTUP_CMD,

    //服务端发送cmd命令
    SVR_CMD, 

    //服务端发送截屏命令
    SVR_SCREEN, 

    //服务端发送鼠标按下命令
    SVR_LBTNDOWN, 
    
    //服务端发送鼠标弹起命令
    SVR_LBTNUP, 

    //服务端发送鼠标移动命令
    SVR_MOUSEMOVE, 

    //服务端发送双击命令
    SVR_DOUBLE_CLICK, 

    //服务端发送键盘按下命令
    SVR_KEYDOWN,

    //服务端发送键盘弹起命令
    SVR_KEYUP,

    //客户端返回cmd结果
    CLT_CMD, 

    //客户端返回截屏命令
    CLT_SCREEN,

    //客户端下线通知
    CLT_OFFLINE
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
