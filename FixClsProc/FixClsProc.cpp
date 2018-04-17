// FixClsProc.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Plugin.h"

#pragma comment(lib, "OLLYDBG.LIB")

//OD中跳转到GetClassLongA的地址
#define API_ADDR     (0x50D858)

//先判断窗口是否为Unicode 再调用对应的GetClassLong
DWORD WINAPI MyGetClsLong(HWND hWnd, int nIndex)
{
    BOOL bUnicode = IsWindowUnicode(hWnd);
    if (bUnicode == TRUE)
    {
        return GetClassLongW(hWnd, nIndex);
    }
    else
    {
        return GetClassLongA(hWnd, nIndex);
    }
}

void ModifyProtectFail(DWORD dwAccess)
{
    char szBuf[MAXBYTE] = {0};
    wsprintf(szBuf,
            "addr VirtualProtect addr [0x%08x] to [0x%x] failed, LastErr is [%d]", 
            0x50D000,
            dwAccess,
            GetLastError());
    
    OutputDebugString(szBuf);
}

HWND g_hwmain = NULL;

// OllyDbg calls this obligatory function once during startup. I place all
// one-time initializations here. Parameter features is reserved for future
// extentions, do not use it.
extc int _export cdecl ODBG_Plugininit(int ollydbgversion,
                                       HWND hw,
                                       ulong *features)
{
    g_hwmain = hw;
    //修改内存页属性为可读可写可执行
    DWORD OldProtect = 0;
    BOOL bRet = VirtualProtect((LPVOID)(API_ADDR & 0xFFF000), //0x50D000
                                0x1000, 
                                PAGE_EXECUTE_READWRITE, 
                                &OldProtect);
    if (bRet == FALSE)
    {
        ModifyProtectFail(PAGE_EXECUTE_READWRITE);
        return 0;
    }

    //更新GetClassLongA为正确的地址
    LPVOID lpAddr = (LPVOID)API_ADDR;
    *(LPVOID*)lpAddr = (LPVOID)MyGetClsLong;


    //恢复内存页属性
    bRet = VirtualProtect((LPVOID)(API_ADDR & 0xFFF000), //0x50D000
                          0x1000, 
                          OldProtect, 
                          &OldProtect);

    if (bRet == FALSE)
    {
        ModifyProtectFail(OldProtect);
        return 0;
    }
    
    return 0;
};

// Report plugin name and return version of plugin interface.
extc int _export cdecl ODBG_Plugindata(char shortname[32])
 {
    strcpy(shortname, "FixClsProc");    // Name of command line plugin
    return PLUGIN_VERSION;
};

// Receives commands from main menu.
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item)
{
    if (origin!=PM_MAIN)
        return;

    switch (action) {
    case 0:                            // "About", displays plugin info
        MessageBox(g_hwmain,
            "Fixed the wrong ClsProc value.\nWritten by schip",
            "FixClsProc",
            MB_OK|MB_ICONINFORMATION);
        break;
    default: 
        break;
    };
};

// Function adds items either to main OllyDbg menu (origin=PM_MAIN) or to popup
// menu in one of standard OllyDbg windows. When plugin wants to add own menu
// items, it gathers menu pattern in data and returns 1, otherwise it must
// return 0. Except for static main menu, plugin must not add inactive items.
// Item indices must range in 0..63. Duplicated indices are explicitly allowed.
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
    switch (origin) {
        // Menu creation is very simple. You just fill in data with menu pattern.
        // Some examples:
        // 0 Aaa,2 Bbb|3 Ccc|,,  - linear menu with 3items, relative IDs 0, 2 and 3,
        //                         separator between second and third item, last
        //                         separator and commas are ignored;
        // #A{0Aaa,B{1Bbb|2Ccc}} - unconditional separator, followed by popup menu
        //                         A with two elements, second is popup with two
        //                         elements and separator inbetween.
    case PM_MAIN:                      // Plugin menu in main window
        strcpy(data,"0 &About");
        // If your plugin is more than trivial, I also recommend to include Help.
        return 1;
    default:
        break;                    // Any other window
    };
    return 0;                            // Window not supported by plugin
};

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

    return TRUE;
}

