// GetSrvRet.h: interface for the CGetSrvRet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETSRVRET_H__31A51034_BEB2_4D74_88B1_A4555D8966DC__INCLUDED_)
#define AFX_GETSRVRET_H__31A51034_BEB2_4D74_88B1_A4555D8966DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\ICommand.h"

class CGetSrvRet  : public ICommand
{
public:
	CGetSrvRet(LPVOID lParam);
	virtual ~CGetSrvRet();

    virtual BOOL Exec();
private:
    LPVOID m_lParam;

};

#endif // !defined(AFX_GETSRVRET_H__31A51034_BEB2_4D74_88B1_A4555D8966DC__INCLUDED_)
