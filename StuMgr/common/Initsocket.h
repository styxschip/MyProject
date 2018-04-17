// Initsocket.h: interface for the CInitsocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITSOCKET_H__804A8E4C_6E94_4F9B_B3D1_96C543B3F6AF__INCLUDED_)
#define AFX_INITSOCKET_H__804A8E4C_6E94_4F9B_B3D1_96C543B3F6AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Winsock2.h>  //winsock2.h必须放在windows.h的前面否则报错
#pragma comment(lib, "Ws2_32.lib")


class CInitsocket  
{
public:
	inline CInitsocket()
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        wVersionRequested = MAKEWORD( 2, 2 );
        err = WSAStartup( wVersionRequested, &wsaData );
        if ( err != 0 )
        {
            TRACE("ssc WSAStartup failed[%d]", WSAGetLastError());
            return;
        }
        if ( LOBYTE( wsaData.wVersion ) != 2 ||
            HIBYTE( wsaData.wVersion ) != 2 )
        {
            TRACE("ssc wsaData error[%d]", WSAGetLastError());
            WSACleanup( );
            return; 
        }
    }

	inline ~CInitsocket()
    {
        WSACleanup( );
    }
};

#endif // !defined(AFX_INITSOCKET_H__804A8E4C_6E94_4F9B_B3D1_96C543B3F6AF__INCLUDED_)
