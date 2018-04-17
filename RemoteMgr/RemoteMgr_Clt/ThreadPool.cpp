// ThreadPool.cpp: implementation of the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <process.h>
#include "RemoteMgr_Clt.h"
#include "ThreadPool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CThreadPool::WorkThread(LPVOID lpParam)
{
    CThreadPool* pThis = (CThreadPool*)lpParam;
    HANDLE hHandleAry[2] = {NULL};
    hHandleAry[0] = pThis->m_hExitEvent;
    hHandleAry[1] = pThis->m_hSemaphore;
    
    BOOL bRet = FALSE;

    while (TRUE)
    {
        DWORD dwRet = WaitForMultipleObjects(2, hHandleAry, FALSE, INFINITE);
        if (dwRet == WAIT_FAILED)
        {
            TRACE("ssc WaitForMultipleObjects failed[%d]", GetLastError());
            return FALSE;
        }
        else if (dwRet == WAIT_TIMEOUT)
        {
            TRACE("ssc WaitForMultipleObjects WAIT_TIMEOUT[%d]", GetLastError());
            return FALSE;
        }
        else
        {
            int nIndex = dwRet - WAIT_OBJECT_0;
            switch (nIndex)
            {
            case 0: //此时应该结束线程
                return TRUE;
            case 1: //该干活了
                {
                    ICommand *pCmd = NULL;
                    bRet = pThis->m_TaskLst.GetCommand(pCmd);
                    if (bRet == FALSE)
                    {
                        TRACE("ssc pThis->m_TaskLst.GetCommand(pCmd) failed!");
                        continue;
                    }
                    pCmd->Exec();
                    pThis->m_TaskLst.RemoveHead();
                    continue;
                }
                break;
            }
        }
    }
    return TRUE;
}

CThreadPool::CThreadPool()
{
    m_nCount = 0;
    m_pThreadAry = NULL;
    m_hSemaphore = NULL;
    m_hExitEvent = NULL;
}

CThreadPool::~CThreadPool()
{

}

BOOL CThreadPool::Handle(ICommand *pCmd)
{
    if (pCmd == NULL)
    {
        TRACE("ssc pCmd is NULL!!");
        return FALSE;
    }

    //向任务队列添加
    m_TaskLst.AddTail(pCmd);
    
    
    //释放信号量让工作线程有机会执行
    BOOL bRet  = ReleaseSemaphore(m_hSemaphore, 1, NULL);
    if (bRet == FALSE)
    {
        TRACE("ssc ReleaseSemaphore failed[%d]", GetLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL CThreadPool::Create(int nCount)
{
    if (nCount <= 0)
    {
        TRACE("ssc nCount error!!");
        return FALSE;
    }
    m_nCount = nCount;

    m_hExitEvent = CreateEvent(NULL,
                                TRUE, //手动
                                FALSE,//初始时没信号
                                NULL);
    
    if (m_hExitEvent == NULL)
    {
        TRACE("ssc CreateEvent failed[%d]", GetLastError());
        return FALSE;
    }

    m_hSemaphore = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);
    if (m_hSemaphore == NULL)
    {
        TRACE("ssc CreateSemaphore failed[%d]", GetLastError());
        return FALSE;
    }

    m_pThreadAry = new HANDLE[m_nCount];
    if (m_pThreadAry == NULL)
    {
        TRACE("ssc new failed!!");
        return FALSE;
    }

    for (int i = 0; i < m_nCount; i++)
    {
        m_pThreadAry[i] = (HANDLE)_beginthreadex(NULL, 0,
                        (unsigned ( __stdcall*)(void*))WorkThread,
                        this,
                        0, 
                        NULL);
        if (m_pThreadAry[i] == NULL)
        {
            TRACE("ssc _beginthreadex failed[%d]!", GetLastError());
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CThreadPool::Destory()
{
    SetEvent(m_hExitEvent);

    //等待线程池中所有的线程退出
    DWORD dwRet = WaitForMultipleObjects(m_nCount,
                                        m_pThreadAry,
                                        TRUE,
                                        INFINITE);

    if (dwRet != WAIT_FAILED)
    {
        for (int i = 0; i < m_nCount; i++)
        {
            CloseHandle(m_pThreadAry[i]);
        }
        delete[] m_pThreadAry;
        m_pThreadAry = NULL;
    }

    m_nCount = 0;
    CloseHandle(m_hSemaphore);
    CloseHandle(m_hExitEvent);
    return TRUE;
}


