#if !defined(AFX_MYDEF_H__0E5E0B98_89D6_4FDF_88D7_7C4CCA0BE8B7__INCLUDED_)
#define AFX_MYDEF_H__0E5E0B98_89D6_4FDF_88D7_7C4CCA0BE8B7__INCLUDED_

#define DRV_SVC_NAME "MySys"

#define DRV_FILE DRV_SVC_NAME##".sys"
#define DRV_FILE_X64 DRV_SVC_NAME##"X64"##".sys"

typedef struct _CONTORL_PARAMS
{
    ULONG CPUNumber;
    DWORD dwSave;
}CONTORL_PARAMS, *PCONTORL_PARAMS;

typedef struct _KERNELMODULE_INFO
{
    char m_szDriverName[MAXBYTE]; //驱动名
    DWORD m_dwBaseAddress;
    DWORD m_dwSize;
    DWORD m_DeviceObject;
    char m_szDriverPath[MAXBYTE]; //驱动路径
    char m_szServiceName[MAXBYTE]; //驱动路径
    DWORD m_dwLoadIndex; //加载顺序
    char m_szFileCompany[MAXBYTE]; //文件厂商
}KERNELMODULE_INFO, *PKERNELMODULE_INFO;

#endif // !defined(AFX_MYDEF_H__0E5E0B98_89D6_4FDF_88D7_7C4CCA0BE8B7__INCLUDED_)