// CustomClose.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "tlhelp32.h"

//我只想用StrStrI
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

//1000D1B3 和 1000903F 这两个地址都可以Hook
//1000D1B3  |  E8 17740000     call    100145CF
//1000903F  |  E8 38410000     call    1000D17C

#define FUNOFFSET1 0xD1B3
#define FUNOFFSET2 0x903F

//获得wise释放的GLCxxx.tmp的句柄
//GLCxxx.tmp就是wise安装目录下的WISE0132.dll
//窗口相关代码都写在这个dll中了
HANDLE GetGLCHandle()
{
    HANDLE  hProcessSnap = NULL;
    HANDLE  hRet = NULL;
    MODULEENTRY32 me32 = { 0 };
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    
    me32.dwSize = sizeof(MODULEENTRY32);
    
    if (Module32First(hProcessSnap, &me32))
    {
        do
        {
            if (StrStrI(me32.szModule, "GLC") != NULL)
            {
                return me32.hModule;
            }
        } while (Module32Next(hProcessSnap, &me32));
    }
    else
    {
        hRet = FALSE;  
    } 
    CloseHandle(hProcessSnap);
    return hRet;
}

void MyFun()
{
    MessageBox(NULL, "Fuck!!!!", NULL, MB_OK);
    ExitProcess(0);
    //如果需要更复杂的功能，可以：
    //LoadLibrary
    //GetProcAddress()
}

DWORD SetNewAddr(char *pOld, char *pNew)
{
    BOOL bRet = FALSE;
    char szBuf[MAXBYTE] = {0};

    //修改内存页属性为可读可写可执行
    DWORD OldProtect = 0;
    bRet = VirtualProtect((LPVOID)pOld, 
                            0x1000, 
                            PAGE_EXECUTE_READWRITE, 
                            &OldProtect);

    if (bRet == FALSE)
    {
        wsprintf(szBuf, "VirtualProtect failed [%d]", GetLastError());
        OutputDebugString(szBuf);
        return FALSE;
    }
    
    //addr = 目标地址 - (原地址 + 5 )
    DWORD dwNewOffset = (DWORD)pNew - ((DWORD)pOld + 5);
    //pOld[0] = 0xE8; //第一个字节就是CALL不用管

    //把新的偏移地址写到 call 指令后面
    memcpy(&pOld[1], &dwNewOffset, sizeof(DWORD));
    

    //还原内存页属性
    bRet = VirtualProtect((LPVOID)pOld, 
                            0x1000, 
                            OldProtect, 
                            &OldProtect);
    
    if (bRet == FALSE)
    {
        wsprintf(szBuf, "VirtualProtect failed [%d]", GetLastError());
        OutputDebugString(szBuf);
        return FALSE;
    }

    return TRUE;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    char szBuf[MAXBYTE] = {0};

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        wsprintf(szBuf, "dll load!!");
        OutputDebugString(szBuf);

        //计算需要改写的函数地址
        HANDLE hGLC = GetGLCHandle();
        DWORD dwFunAddr = (DWORD)hGLC + FUNOFFSET1;

        //设置新地址
        SetNewAddr((char *)dwFunAddr, (char *)MyFun);

    }
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        wsprintf(szBuf, "dll free!!");
        OutputDebugString(szBuf);
	}

    return TRUE;
}

