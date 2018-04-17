// RecvThread.h: interface for the CRecvThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECVTHREAD_H__FD2FA857_694B_476A_82A1_57FE2DB67F1D__INCLUDED_)
#define AFX_RECVTHREAD_H__FD2FA857_694B_476A_82A1_57FE2DB67F1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\ICommand.h"

class CRecvThread  : public ICommand
{
public:
	CRecvThread(LPVOID lParam, SOCKET socket);
	virtual ~CRecvThread();

    virtual BOOL Exec();
private:
    SOCKET m_sockMid;
    LPVOID m_lParam;
};

#endif // !defined(AFX_RECVTHREAD_H__FD2FA857_694B_476A_82A1_57FE2DB67F1D__INCLUDED_)
