// CommandMgr.cpp: implementation of the CCommandMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandMgr::CCommandMgr()
{

}

CCommandMgr::~CCommandMgr()
{

}

BOOL CCommandMgr::AddTail(ICommand *pCmd)
{
    m_cs.Lock();
    m_Lst.AddTail(pCmd);
    m_cs.Unlock();
    return TRUE;
}
BOOL CCommandMgr::RemoveHead()
{
    m_cs.Lock();
    if (!m_Lst.IsEmpty())
    {
        m_Lst.RemoveHead();
    }
    m_cs.Unlock();
    return TRUE;
}

BOOL CCommandMgr::GetCommand(ICommand* &pCMd)
{
    if (m_Lst.GetCount() <= 0)
    {
        pCMd = NULL;
        return FALSE;
    }
    m_cs.Lock();
    pCMd = m_Lst.RemoveHead();
    m_cs.Unlock();

    return TRUE;
}
