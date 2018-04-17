// ThreadPool.h: interface for the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADPOOL_H__56807E44_FE46_4013_84E5_16F2FFDCD18E__INCLUDED_)
#define AFX_THREADPOOL_H__56807E44_FE46_4013_84E5_16F2FFDCD18E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ICommand.h"
#include "CommandMgr.h"

class CThreadPool  
{
public:
	CThreadPool();
	virtual ~CThreadPool();
    static BOOL WorkThread(LPVOID lpParam);
    BOOL Handle(ICommand *pCmd);
    BOOL Create(int nCount);
    BOOL Destory();
private:
    int m_nCount;
    HANDLE *m_pThreadAry;
    HANDLE m_hSemaphore;
    HANDLE m_hExitEvent;
    CCommandMgr m_TaskLst;
};

#endif // !defined(AFX_THREADPOOL_H__56807E44_FE46_4013_84E5_16F2FFDCD18E__INCLUDED_)
