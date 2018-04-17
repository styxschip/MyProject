// Function.h: interface for the CFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTION_H__8F48A8CF_7516_4ADE_AC07_4273FB566808__INCLUDED_)
#define AFX_FUNCTION_H__8F48A8CF_7516_4ADE_AC07_4273FB566808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef __int64 QWORD;


// Flags for StrToIntEx
#define STIF_DEFAULT        0x00000000L
#define STIF_SUPPORT_HEX    0x00000001L

#define LWSTDAPI_(type)   EXTERN_C DECLSPEC_IMPORT type STDAPICALLTYPE

LWSTDAPI_(BOOL)     StrToInt64ExA(LPCSTR pszString, DWORD dwFlags, LONGLONG * pllRet);
LWSTDAPI_(BOOL)     StrToInt64ExW(LPCWSTR pszString, DWORD dwFlags, LONGLONG * pllRet);

//傻逼
//包含Shlwapi.h后还是提示没有定义
//要么我傻逼 要么MS傻逼

typedef VOID (WINAPI *PFN_GetNativeSystemInfo)(
                      OUT LPSYSTEM_INFO lpSystemInfo
                      ); //我错了改用loadLibrary。。



#pragma pack(push, 1)
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

    unsigned __int64 m_BaseMid: 8;

    unsigned __int64 m_BaseHigh: 32;
    
    unsigned __int64 m_ReservedIGN: 8;

    unsigned __int64 m_SaveZero: 5;

    unsigned __int64 m_ReservedIGN2: 19;

}GDT, *PGDT;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _tagAttrib 
{   
    //Attributes
    WORD m_TYPE:  4; 
    WORD m_DT1:   1;
    WORD m_DPL:   2;
    WORD m_P:     1;
    WORD m_LimiteHigh:  4; 
    WORD m_AVL:  1; 
    WORD m_Saved:  1; 
    WORD m_D:  1; 
    WORD m_G:  1; 

}ATTRIB, *PATTRIB;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _tagIDT
{
    unsigned __int64 m_OffsetLow: 16;
    unsigned __int64 m_Selector_RPL: 2; //Requested Privilege Level
    unsigned __int64 m_Selector_TI: 1; //Table Indicator  0 = GDT 1 = LDT
    unsigned __int64 m_Selector_Index: 13;
    unsigned __int64 m_Reserved:  5; 
    unsigned __int64 m_Fixed1:   3; //这3位固定3个0
    unsigned __int64 m_Fixed2:   3; //这3位固定1 1 0
    unsigned __int64 m_D:   1; //D Size of gate: 1 = 32 bits; 0 = 16 bits
    unsigned __int64 m_Fixed3:  1; // 固定0
    unsigned __int64 m_DPL:   2;
    unsigned __int64 m_P:     1;
    unsigned __int64 m_OffsetMid: 16;
    unsigned __int64 m_OffsetHigh: 32;
    unsigned __int64 m_Reserved1: 32;  //填充0x00
}IDT, *PIDT;
#pragma pack(pop)


class CFunction  
{
public:
	CFunction();
	virtual ~CFunction();

public:
    
    static void SetTip(char *szBuf);
    static void ShowErrorMsg();

    static DWORD GetBaseFormGDT(PGDT pValue, __int64 *pdwBase);
    static DWORD GetLimiteFormGDT(PGDT pValue, DWORD *pdwLimite);
    static DWORD GetAttribFormGDT(PGDT pValue, WORD *pwAttrib);

    //描述R0 或者 R3
    static DWORD GetDLP(WORD wAttrib);

    //用来判断是存储段(1)还是系统段(0)
    static DWORD GetDT(WORD wAttrib); 

    //颗粒度 0说明LIMIT粒度为字节,1为4K字节
    static char *GetG(WORD wAttrib);

    //按字符串显示属性的Type信息
    static char *GetTypeString(WORD wAttrib);

    //按数值显示属性的Type信息
    static UCHAR GetTypeValue(WORD wAttrib);

    //解析GDT
    static DWORD AnalyseGDT(const char *szGDTBuf,
                            __int64 *pdwBase,
                            DWORD *pdwLimite,
                            WORD *pwAttributes);


    //获得CPU核心数
    static DWORD GetNumberOfProcessors();

    //IDT中获取函数地址
    static DWORD GetOffsetFormIDT(PIDT pValue, __int64 *pqwOffset);

    //IDT中获取选择子
    static DWORD GetSelectorFormIDT(PIDT pValue, DWORD *pdwSelector);

};

#endif // !defined(AFX_FUNCTION_H__8F48A8CF_7516_4ADE_AC07_4273FB566808__INCLUDED_)
