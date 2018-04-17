#include <windows.h>

#if !defined(AFX_OUTPUTDBGMSG_H__0543937F_8584_49E3_BD5E_50583236E487__INCLUDED_)
#define AFX_OUTPUTDBGMSG_H__0543937F_8584_49E3_BD5E_50583236E487__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//输出Debug信息
//用法OutputDebugMsg(__LINE__, __FILE__, "ssc falied!");
//或者MYTRACE("ssc falied!");
void inline OutputDebugMsg(int nLine, char *szFile, const TCHAR *szFormat, ...)
{
    DWORD dwError = GetLastError();
    LPVOID lpMsgBuf = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                    FORMAT_MESSAGE_FROM_SYSTEM | 
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    dwError,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR)&lpMsgBuf,
                    0,
                    NULL 
                    );//MSDN中的例子

    
    TCHAR szError[MAXBYTE] = {0};
    wsprintf(szError, " line:%d cpp:%s error[%d] %s",                      
                      nLine,
                      szFile,
                      dwError,
                      lpMsgBuf);

    static TCHAR szBuf[1024] = {0};
    va_list args;
    va_start (args, szFormat);
    wvsprintf(szBuf, szFormat, args);
    va_end (args);
    strcat(szBuf, szError);
    OutputDebugString(szBuf); 
    
    LocalFree(lpMsgBuf);
}

#define MYTRACE(s) OutputDebugMsg(__LINE__, __FILE__, s)

#endif // !defined(AFX_OUTPUTDBGMSG_H__0543937F_8584_49E3_BD5E_50583236E487__INCLUDED_)
