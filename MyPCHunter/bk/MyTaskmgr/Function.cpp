// Function.cpp: implementation of the CFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Function.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Shlwapi.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFunction::CFunction()
{

}

CFunction::~CFunction()
{

}

/*
typedef struct _tagGDT 
{
    unsigned __int64 m_LimiteLow: 16;
    unsigned __int64 m_BaseLow: 24;
    
    //Attributes
    unsigned __int64 m_TYPE:  4; 
    unsigned __int64 m_DT1:   1;
    unsigned __int64 m_DPL:   2;
    unsigned __int64 m_P:     1;
    unsigned __int64 m_LimiteHigh:  4; 
    unsigned __int64 m_AVL:  1; 
    unsigned __int64 m_Saved:  1; 
    unsigned __int64 m_D:  1; 
    unsigned __int64 m_G:  1; 
    
      unsigned __int64 m_BaseHigh: 8;
}GDT, *PGDT;
*/

DWORD CFunction::GetBaseFormGDT(PGDT pValue, DWORD *pdwBase)
{
    if (pValue == NULL || pdwBase == NULL)
    {
        return FALSE;
    }
    DWORD dwBaseLow = (DWORD)(pValue->m_BaseLow);
    DWORD dwBaseHigh = (DWORD)((pValue->m_BaseHigh) << 24);

    *pdwBase = dwBaseLow + dwBaseHigh;

    return TRUE;
}

DWORD CFunction::GetLimiteFormGDT(PGDT pValue, DWORD *pwLimite)
{
    if (pValue == NULL || pwLimite == NULL)
    {
        return FALSE;
    }

    DWORD dwLimiteLow = (DWORD)(pValue->m_LimiteLow);
    DWORD dwLimiteHigh = (DWORD)((pValue->m_LimiteHigh) << 16);

    *pwLimite = dwLimiteLow + dwLimiteHigh;

    return TRUE;
}

DWORD CFunction::GetAttribFormGDT(PGDT pValue, WORD *pwAttrib)
{
    if (pValue == NULL || pwAttrib == NULL)
    {
        return FALSE;
    }

    *pwAttrib = *(WORD*)((char *)pValue + 5);

    return TRUE;
}

DWORD CFunction::GetDLP(WORD wAttrib)
{
    PATTRIB pAttrib = (PATTRIB)(&wAttrib);
    return pAttrib->m_DPL;
}

//用来判断是存储段(1)还是系统段(0)
DWORD CFunction::GetDT(WORD wAttrib)
{
    PATTRIB pAttrib = (PATTRIB)(&wAttrib);
    return pAttrib->m_DT1;
}
    

char *CFunction::GetG(WORD wAttrib)
{
    PATTRIB pAttrib = (PATTRIB)(&wAttrib);
    if (pAttrib->m_G == 1)
    {
        return "Page"; //4k
    }
    else
    {
        return "Byte"; 
    }
}

char *g_pDataszBuf[] = {
    "Read-Only",
    "Read-Only AC",
    "Read/Write",
    "Read/Write AC",
    "Read-Only ED",
    "Read-Only ED AC",
    "Read/Write ED",
    "Read/Write ED AC"  //实际上不带AC的根本不能显示
};

char *g_pCodeszBuf[] = {
    "Execute-Only",
    "Execute-Only AC",
    "Execute/Read",
    "Execute/Read AC",
    "Execute-Only conforming",
    "Execute-Only conforming AC",
    "Execute/Read-Only conforming",
    "Execute/Read-Only conforming AC" //实际上不带AC的根本不能显示     
};

char *g_pSysszBuf[] = {
    "Reserved",
    "16-Bit TSS (Available)",
    "LDT",
    "16-Bit TSS (Busy)",
    "16-Bit Call Gate",
    "Task Gate",
    "16-Bit Interrupt Gate",
    "16-Bit Trap Gate",
    "Reserved",
    "32-Bit TSS (Available)",
    "Reserved",
    "32-Bit TSS (Busy)",
    "32-Bit Call Gate",
    "Reserved",
    "32-Bit Interrupt Gate",
    "32-Bit Trap Gate"
};

char *CFunction::GetTypeString(WORD wAttrib)
{
    UCHAR chType = GetTypeValue(wAttrib);

    //先判断是系统段还是存储段
    DWORD dwType = GetDT(wAttrib);
    if (dwType == 0)
    {
        return g_pSysszBuf[GetTypeValue(wAttrib)];
    }
    
    //取 Type 的 第4位(下标3)
    DWORD dwDataOrCode = 0;
    dwDataOrCode = chType >> 3;

    //提取 Type 的描述信息
    DWORD dwDescription = 0;
    dwDescription = chType & 7;

    if (dwDataOrCode == 1) //代码段
    {
        return g_pCodeszBuf[dwDescription];
    }
    else //数据段
    {
        return g_pDataszBuf[dwDescription];
    }
}

UCHAR CFunction::GetTypeValue(WORD wAttrib)
{
    return wAttrib & 0xF;
}



DWORD CFunction::AnalyseGDT(const char *szGDTBuf,
                            DWORD *pdwBase,
                            DWORD *pwLimite,
                            WORD *pwAttributes)
{
    if (szGDTBuf == NULL)
    {
        return FALSE;
    }

    if (strlen(szGDTBuf) > 16)
    {
        return FALSE;
    }

    DWORD dwRet = 0;
    
    QWORD qwValue = 0;
    char szHexBuf[MAXBYTE] = {0};

    //为了转 16进制前面加上0x
    wsprintf(szHexBuf, "0x%s", szGDTBuf);
    dwRet = StrToInt64ExA(szHexBuf, STIF_SUPPORT_HEX, &qwValue);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    dwRet = GetBaseFormGDT((PGDT)&qwValue, pdwBase);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    dwRet = GetLimiteFormGDT((PGDT)&qwValue, pwLimite);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    dwRet = GetAttribFormGDT((PGDT)&qwValue, pwAttributes);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

void CFunction::ShowErrorMsg()
{
    //获取具体错误信息
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );//这段是MSDN中的例子，这里直接用了
    
    SetTip((char *)lpMsgBuf);
}

void CFunction::SetTip(char *szBuf)
{
    AfxMessageBox(szBuf);
}

DWORD CFunction::GetNumberOfProcessors()
{
    //获得CPU核心数
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    
    return SystemInfo.dwNumberOfProcessors;

}

//IDT中获取函数地址
DWORD CFunction::GetOffsetFormIDT(PIDT pValue, DWORD *pdwOffset)
{
    if (pValue == NULL || pdwOffset == NULL)
    {
        return FALSE;
    }

    DWORD  dwLow = (DWORD)(pValue->m_OffsetLow);
    DWORD  dwHigh = (DWORD)((pValue->m_OffsetHigh) << 16);
    *pdwOffset = dwLow + dwHigh;

    return TRUE;
}

//IDT中获取选择子
DWORD CFunction::GetSelectorFormIDT(PIDT pValue, DWORD *pdwSelector)
{
    if (pValue == NULL || pdwSelector == NULL)
    {
        return FALSE;
    }
    *pdwSelector = (DWORD)(pValue->m_Selector_Index);
    return TRUE;
}