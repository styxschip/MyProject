// ICommand.h: interface for the ICommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICOMMAND_H__0652F8E8_3F91_4EF6_A3A9_43AFE0529043__INCLUDED_)
#define AFX_ICOMMAND_H__0652F8E8_3F91_4EF6_A3A9_43AFE0529043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ICommand  
{
public:
    virtual BOOL Exec() = 0;
	ICommand();
	virtual ~ICommand();
};

#endif // !defined(AFX_ICOMMAND_H__0652F8E8_3F91_4EF6_A3A9_43AFE0529043__INCLUDED_)
