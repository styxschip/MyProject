// MySSDTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "MySSDTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySSDTDlg dialog


CMySSDTDlg::CMySSDTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySSDTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySSDTDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMySSDTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySSDTDlg)
	DDX_Control(pDX, LST_SSDT, m_LstSSDT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySSDTDlg, CDialog)
	//{{AFX_MSG_MAP(CMySSDTDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySSDTDlg message handlers

BOOL CMySSDTDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    
    // TODO: Add extra initialization here
    //设置ListCtrl风格
    m_LstSSDT.SetExtendedStyle(m_LstSSDT.GetExtendedStyle() |
                                LVS_EX_GRIDLINES |
                                LVS_EX_FULLROWSELECT);
    //设置ListCtrl列名
    m_LstSSDT.InsertColumn(1, "序号", LVCFMT_LEFT, 60);
    m_LstSSDT.InsertColumn(2, "函数名称", LVCFMT_LEFT, 100);
    m_LstSSDT.InsertColumn(3, "当前函数地址", LVCFMT_LEFT, 200);
    m_LstSSDT.InsertColumn(4, "Hook", LVCFMT_LEFT, 100);
    m_LstSSDT.InsertColumn(5, "原始函数地址", LVCFMT_LEFT, 200);
    m_LstSSDT.InsertColumn(6, "当前函数所在模块", LVCFMT_LEFT, 200);
    
    UpdateData(FALSE);
    
    GetSSDTFromSys();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CMySSDTDlg::GetSSDTFromSys()
{
    char szBuf[MAXBYTE] = {0};
    DWORD dwSSDTNum = 0;
    DWORD dwBytes = 0;

    extern CMyTaskmgrApp theApp;

    HANDLE hDrvFile = theApp.m_hDrvFile;
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_SSDTNUM, 
                        (LPVOID)&dwSSDTNum, 
                        sizeof(DWORD),
                        (LPVOID)&dwSSDTNum,
                        sizeof(DWORD),
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_IDTLIMIT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        return 0;
    }
    DWORD dwSSDTSize = dwSSDTNum * sizeof(DWORD);
    char *pParams = new char[dwSSDTSize];
    if (pParams == NULL)
    {
        wsprintf(szBuf, "new CONTORL_PARAMS faild!");
        AfxMessageBox(szBuf);
        return FALSE;
    }
    ZeroMemory(pParams, dwSSDTSize);
    
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_SSDT, 
                        pParams, 
                        dwSSDTSize,
                        pParams, 
                        dwSSDTSize, 
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_IDT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        
        if (pParams != NULL)
        {
            delete pParams;
            pParams = NULL;
        }
        return 0;
    }
    
    char szTmpBuf[MAXBYTE] = {0};
    int j = 0;
    WORD wAttrib = 0;
    DWORD dwSelector = 0;
    DWORD dwOffset = 0;
    DWORD dwP = 0;
    UCHAR chType = 0;
    //解析后填充到ListCtrl
    for (DWORD i = 0; i < dwSSDTNum; i++)
    {        
        wsprintf(szTmpBuf, "%d", i);
        m_LstSSDT.InsertItem(i, szTmpBuf); //序号

        
        wsprintf(szTmpBuf, "%s", g_szSSDTFuncName[i]);
        m_LstSSDT.SetItemText(i, 1, szTmpBuf); //函数名称

        wsprintf(szTmpBuf, "0x%08X", ((DWORD *)pParams)[i]);
        m_LstSSDT.SetItemText(i, 2, szTmpBuf); //当前函数地址
        
        wsprintf(szTmpBuf, "%s", "-");
        m_LstSSDT.SetItemText(i, 3, szTmpBuf); //Hook
        
        wsprintf(szTmpBuf, "0x%08X", ((DWORD *)pParams)[i]);
        m_LstSSDT.SetItemText(i, 4, szTmpBuf); //原始函数地址;
        
        wsprintf(szTmpBuf, "%s", "ntoskrnl");
        m_LstSSDT.SetItemText(i, 5, szTmpBuf); //当前函数地址所在模块;
        
        j++;
    }
      
    if (pParams != NULL)
    {
        delete pParams;
        pParams = NULL;
    }
    
    return TRUE;
}

char *g_szSSDTFuncName[] = {
    "NtAcceptConnectPort",
    "NtAccessCheck",
    "NtAccessCheckAndAuditAlarm",
    "NtAccessCheckByType",
    "NtAccessCheckByTypeAndAuditAlarm",
    "NtAccessCheckByTypeResultList",
    "NtAccessCheckByTypeResultListAndAuditAlarm",
    "NtAccessCheckByTypeResultListAndAuditAlarmByHandle",
    "NtAddAtom",
    "NtAddBootEntry",
    "NtAdjustGroupsToken",
    "NtAdjustPrivilegesToken",
    "NtAlertResumeThread",
    "NtAlertThread",
    "NtAllocateLocallyUniqueId",
    "NtAllocateUserPhysicalPages",
    "NtAllocateUuids",
    "NtAllocateVirtualMemory",
    "NtAreMappedFilesTheSame",
    "NtAssignProcessToJobObject",
    "NtCallbackReturn",
    "NtCancelDeviceWakeupRequest",
    "NtCancelIoFile",
    "NtCancelTimer",
    "NtClearEvent",
    "NtClose",
    "NtCloseObjectAuditAlarm",
    "NtCompactKeys",
    "NtCompareTokens",
    "NtCompleteConnectPort",
    "NtCompressKey",
    "NtConnectPort",
    "NtContinue",
    "NtCreateDebugObject",
    "NtCreateDirectoryObject",
    "NtCreateEvent",
    "NtCreateEventPair",
    "NtCreateFile",
    "NtCreateIoCompletion",
    "NtCreateJobObject",
    "NtCreateJobSet",
    "NtCreateKey",
    "NtCreateMailslotFile",
    "NtCreateMutant",
    "NtCreateNamedPipeFile",
    "NtCreatePagingFile",
    "NtCreatePort",
    "NtCreateProcess",
    "NtCreateProcessEx",
    "NtCreateProfile",
    "NtCreateSection",
    "NtCreateSemaphore",
    "NtCreateSymbolicLinkObject",
    "NtCreateThread",
    "NtCreateTimer",
    "NtCreateToken",
    "NtCreateWaitablePort",
    "NtDebugActiveProcess",
    "NtDebugContinue",
    "NtDelayExecution",
    "NtDeleteAtom",
    "NtDeleteBootEntry",
    "NtDeleteFile",
    "NtDeleteKey",
    "NtDeleteObjectAuditAlarm",
    "NtDeleteValueKey",
    "NtDeviceIoControlFile",
    "NtDisplayString",
    "NtDuplicateObject",
    "NtDuplicateToken",
    "NtEnumerateBootEntries",
    "NtEnumerateKey",
    "NtEnumerateSystemEnvironmentValuesEx",
    "NtEnumerateValueKey",
    "NtExtendSection",
    "NtFilterToken",
    "NtFindAtom",
    "NtFlushBuffersFile",
    "NtFlushInstructionCache",
    "NtFlushKey",
    "NtFlushVirtualMemory",
    "NtFlushWriteBuffer",
    "NtFreeUserPhysicalPages",
    "NtFreeVirtualMemory",
    "NtFsControlFile",
    "NtGetContextThread",
    "NtGetDevicePowerState",
    "NtGetPlugPlayEvent",
    "NtGetWriteWatch",
    "NtImpersonateAnonymousToken",
    "NtImpersonateClientOfPort",
    "NtImpersonateThread",
    "NtInitializeRegistry",
    "NtInitiatePowerAction",
    "NtIsProcessInJob",
    "NtIsSystemResumeAutomatic",
    "NtListenPort",
    "NtLoadDriver",
    "NtLoadKey",
    "NtLoadKey2",
    "NtLockFile",
    "NtLockProductActivationKeys",
    "NtLockRegistryKey",
    "NtLockVirtualMemory",
    "NtMakePermanentObject",
    "NtMakeTemporaryObject",
    "NtMapUserPhysicalPages",
    "NtMapUserPhysicalPagesScatter",
    "NtMapViewOfSection",
    "NtModifyBootEntry",
    "NtNotifyChangeDirectoryFile",
    "NtNotifyChangeKey",
    "NtNotifyChangeMultipleKeys",
    "NtOpenDirectoryObject",
    "NtOpenEvent",
    "NtOpenEventPair",
    "NtOpenFile",
    "NtOpenIoCompletion",
    "NtOpenJobObject",
    "NtOpenKey",
    "NtOpenMutant",
    "NtOpenObjectAuditAlarm",
    "NtOpenProcess",
    "NtOpenProcessToken",
    "NtOpenProcessTokenEx",
    "NtOpenSection",
    "NtOpenSemaphore",
    "NtOpenSymbolicLinkObject",
    "NtOpenThread",
    "NtOpenThreadToken",
    "NtOpenThreadTokenEx",
    "NtOpenTimer",
    "NtPlugPlayControl",
    "NtPowerInformation",
    "NtPrivilegeCheck",
    "NtPrivilegeObjectAuditAlarm",
    "NtPrivilegedServiceAuditAlarm",
    "NtProtectVirtualMemory",
    "NtPulseEvent",
    "NtQueryAttributesFile",
    "NtQueryBootEntryOrder",
    "NtQueryBootOptions",
    "NtQueryDebugFilterState",
    "NtQueryDefaultLocale",
    "NtQueryDefaultUILanguage",
    "NtQueryDirectoryFile",
    "NtQueryDirectoryObject",
    "NtQueryEaFile",
    "NtQueryEvent",
    "NtQueryFullAttributesFile",
    "NtQueryInformationAtom",
    "NtQueryInformationFile",
    "NtQueryInformationJobObject",
    "NtQueryInformationPort",
    "NtQueryInformationProcess",
    "NtQueryInformationThread",
    "NtQueryInformationToken",
    "NtQueryInstallUILanguage",
    "NtQueryIntervalProfile",
    "NtQueryIoCompletion",
    "NtQueryKey",
    "NtQueryMultipleValueKey",
    "NtQueryMutant",
    "NtQueryObject",
    "NtQueryOpenSubKeys",
    "NtQueryPerformanceCounter",
    "NtQueryQuotaInformationFile",
    "NtQuerySection",
    "NtQuerySecurityObject",
    "NtQuerySemaphore",
    "NtQuerySymbolicLinkObject",
    "NtQuerySystemEnvironmentValue",
    "NtQuerySystemEnvironmentValueEx",
    "NtQuerySystemInformation",
    "NtQuerySystemTime",
    "NtQueryTimer",
    "NtQueryTimerResolution",
    "NtQueryValueKey",
    "NtQueryVirtualMemory",
    "NtQueryVolumeInformationFile",
    "NtQueueApcThread",
    "NtRaiseException",
    "NtRaiseHardError",
    "NtReadFile",
    "NtReadFileScatter",
    "NtReadRequestData",
    "NtReadVirtualMemory",
    "NtRegisterThreadTerminatePort",
    "NtReleaseMutant",
    "NtReleaseSemaphore",
    "NtRemoveIoCompletion",
    "NtRemoveProcessDebug",
    "NtRenameKey",
    "NtReplaceKey",
    "NtReplyPort",
    "NtReplyWaitReceivePort",
    "NtReplyWaitReceivePortEx",
    "NtReplyWaitReplyPort",
    "NtRequestDeviceWakeup",
    "NtRequestPort",
    "NtRequestWaitReplyPort",
    "NtRequestWakeupLatency",
    "NtResetEvent",
    "NtResetWriteWatch",
    "NtRestoreKey",
    "NtResumeProcess",
    "NtResumeThread",
    "NtSaveKey",
    "NtSaveKeyEx",
    "NtSaveMergedKeys",
    "NtSecureConnectPort",
    "NtSetBootEntryOrder",
    "NtSetBootOptions",
    "NtSetContextThread",
    "NtSetDebugFilterState",
    "NtSetDefaultHardErrorPort",
    "NtSetDefaultLocale",
    "NtSetDefaultUILanguage",
    "NtSetEaFile",
    "NtSetEvent",
    "NtSetEventBoostPriority",
    "NtSetHighEventPair",
    "NtSetHighWaitLowEventPair",
    "NtSetInformationDebugObject",
    "NtSetInformationFile",
    "NtSetInformationJobObject",
    "NtSetInformationKey",
    "NtSetInformationObject",
    "NtSetInformationProcess",
    "NtSetInformationThread",
    "NtSetInformationToken",
    "NtSetIntervalProfile",
    "NtSetIoCompletion",
    "NtSetLdtEntries",
    "NtSetLowEventPair",
    "NtSetLowWaitHighEventPair",
    "NtSetQuotaInformationFile",
    "NtSetSecurityObject",
    "NtSetSystemEnvironmentValue",
    "NtSetSystemEnvironmentValueEx",
    "NtSetSystemInformation",
    "NtSetSystemPowerState",
    "NtSetSystemTime",
    "NtSetThreadExecutionState",
    "NtSetTimer",
    "NtSetTimerResolution",
    "NtSetUuidSeed",
    "NtSetValueKey",
    "NtSetVolumeInformationFile",
    "NtShutdownSystem",
    "NtSignalAndWaitForSingleObject",
    "NtStartProfile",
    "NtStopProfile",
    "NtSuspendProcess",
    "NtSuspendThread",
    "NtSystemDebugControl",
    "NtTerminateJobObject",
    "NtTerminateProcess",
    "NtTerminateThread",
    "NtTestAlert",
    "NtTraceEvent",
    "NtTranslateFilePath",
    "NtUnloadDriver",
    "NtUnloadKey",
    "NtUnloadKeyEx",
    "NtUnlockFile",
    "NtUnlockVirtualMemory",
    "NtUnmapViewOfSection",
    "NtVdmControl",
    "NtWaitForDebugEvent",
    "NtWaitForMultipleObjects",
    "NtWaitForSingleObject",
    "NtWaitHighEventPair",
    "NtWaitLowEventPair",
    "NtWriteFile",
    "NtWriteFileGather",
    "NtWriteRequestData",
    "NtWriteVirtualMemory",
    "NtYieldExecution",
    "NtCreateKeyedEvent",
    "NtOpenKeyedEvent",
    "NtReleaseKeyedEvent",
    "NtWaitForKeyedEvent",
    "NtQueryPortInformationProcess"
};