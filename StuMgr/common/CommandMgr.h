// CommandMgr.h: interface for the CCommandMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDMGR_H__E145F528_8A71_41B8_8D98_A219B6E8CEFF__INCLUDED_)
#define AFX_COMMANDMGR_H__E145F528_8A71_41B8_8D98_A219B6E8CEFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <AFXTEMPL.H>
#include <afxmt.h>
#include "ICommand.h"


class CCommandMgr  
{
public:
    CCommandMgr();
    virtual ~CCommandMgr();
    BOOL AddTail(ICommand *pCmd);
    BOOL RemoveHead();
    BOOL GetCommand(ICommand* &pCMd);
private:
    CCriticalSection m_cs;
    CList<ICommand*, ICommand*> m_Lst;
};

#endif // !defined(AFX_COMMANDMGR_H__E145F528_8A71_41B8_8D98_A219B6E8CEFF__INCLUDED_)
