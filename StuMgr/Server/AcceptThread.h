// AcceptThread.h: interface for the CAcceptThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCEPTTHREAD_H__CC013AED_23DD_4FBA_9C66_CC82C3676C69__INCLUDED_)
#define AFX_ACCEPTTHREAD_H__CC013AED_23DD_4FBA_9C66_CC82C3676C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\ICommand.h"

class CAcceptThread  : public ICommand
{
public:
	CAcceptThread(LPVOID lParam);
	virtual ~CAcceptThread();

    virtual BOOL Exec();
private:
    //SOCKET m_sockListen;
    LPVOID m_lParam;
};

#endif // !defined(AFX_ACCEPTTHREAD_H__CC013AED_23DD_4FBA_9C66_CC82C3676C69__INCLUDED_)
