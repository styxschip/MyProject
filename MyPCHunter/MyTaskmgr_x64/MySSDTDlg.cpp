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
//     if (!DeviceIoControl(hDrvFile, 
//                         IOCTRL_GET_SSDTNUM, 
//                         (LPVOID)&dwSSDTNum, 
//                         sizeof(DWORD),
//                         (LPVOID)&dwSSDTNum,
//                         sizeof(DWORD),
//                         &dwBytes, 
//                         NULL))
//     {
//         //CloseHandle(hDrvFile);
//         wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_IDTLIMIT ErrorCode:%d\n", GetLastError());
//         AfxMessageBox(szBuf);
//         return 0;
//     }

    dwSSDTNum = 401; //不从内核中拿了 直接写死

    DWORD dwSSDTSize = dwSSDTNum * sizeof(ULONGLONG);
    ULONGLONG *pParams = new ULONGLONG[dwSSDTNum];
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

        wsprintf(szTmpBuf, "0xFFFFF8%010X", ((ULONGLONG *)pParams)[i]);
        m_LstSSDT.SetItemText(i, 2, szTmpBuf); //当前函数地址
        
        wsprintf(szTmpBuf, "%s", "-");
        m_LstSSDT.SetItemText(i, 3, szTmpBuf); //Hook
        
        wsprintf(szTmpBuf, "0xFFFFF8%010X", ((ULONGLONG *)pParams)[i]);
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
    "NtMapUserPhysicalPagesScatter",
    "NtWaitForSingleObject",
    "NtCallbackReturn",
    "NtReadFile",
    "NtDeviceIoControlFile",
    "NtWriteFile",
    "NtRemoveIoCompletion",
    "NtReleaseSemaphore",
    "NtReplyWaitReceivePort",
    "NtReplyPort",
    "NtSetInformationThread",
    "NtSetEvent",
    "NtClose",
    "NtQueryObject",
    "NtQueryInformationFile",
    "NtOpenKey",
    "NtEnumerateValueKey",
    "NtFindAtom",
    "NtQueryDefaultLocale",
    "NtQueryKey",
    "NtQueryValueKey",
    "NtAllocateVirtualMemory",
    "NtQueryInformationProcess",
    "NtWaitForMultipleObjects32",
    "NtWriteFileGather",
    "NtSetInformationProcess",
    "NtCreateKey",
    "NtFreeVirtualMemory",
    "NtImpersonateClientOfPort",
    "NtReleaseMutant",
    "NtQueryInformationToken",
    "NtRequestWaitReplyPort",
    "NtQueryVirtualMemory",
    "NtOpenThreadToken",
    "NtQueryInformationThread",
    "NtOpenProcess",
    "NtSetInformationFile",
    "NtMapViewOfSection",
    "NtAccessCheckAndAuditAlarm",
    "NtUnmapViewOfSection",
    "NtReplyWaitReceivePortEx",
    "NtTerminateProcess",
    "NtSetEventBoostPriority",
    "NtReadFileScatter",
    "NtOpenThreadTokenEx",
    "NtOpenProcessTokenEx",
    "NtQueryPerformanceCounter",
    "NtEnumerateKey",
    "NtOpenFile",
    "NtDelayExecution",
    "NtQueryDirectoryFile",
    "NtQuerySystemInformation",
    "NtOpenSection",
    "NtQueryTimer",
    "NtFsControlFile",
    "NtWriteVirtualMemory",
    "NtCloseObjectAuditAlarm",
    "NtDuplicateObject",
    "NtQueryAttributesFile",
    "NtClearEvent",
    "NtReadVirtualMemory",
    "NtOpenEvent",
    "NtAdjustPrivilegesToken",
    "NtDuplicateToken",
    "NtContinue",
    "NtQueryDefaultUILanguage",
    "NtQueueApcThread",
    "NtYieldExecution",
    "NtAddAtom",
    "NtCreateEvent",
    "NtQueryVolumeInformationFile",
    "NtCreateSection",
    "NtFlushBuffersFile",
    "NtApphelpCacheControl",
    "NtCreateProcessEx",
    "NtCreateThread",
    "NtIsProcessInJob",
    "NtProtectVirtualMemory",
    "NtQuerySection",
    "NtResumeThread",
    "NtTerminateThread",
    "NtReadRequestData",
    "NtCreateFile",
    "NtQueryEvent",
    "NtWriteRequestData",
    "NtOpenDirectoryObject",
    "NtAccessCheckByTypeAndAuditAlarm",
    "NtQuerySystemTime",
    "NtWaitForMultipleObjects",
    "NtSetInformationObject",
    "NtCancelIoFile",
    "NtTraceEvent",
    "NtPowerInformation",
    "NtSetValueKey",
    "NtCancelTimer",
    "NtSetTimer",
    "NtAcceptConnectPort",
    "NtAccessCheck",
    "NtAccessCheckByType",
    "NtAccessCheckByTypeResultList",
    "NtAccessCheckByTypeResultListAndAuditAlarm",
    "NtAccessCheckByTypeResultListAndAuditAlarmByHandle",
    "NtAddBootEntry",
    "NtAddDriverEntry",
    "NtAdjustGroupsToken",
    "NtAlertResumeThread",
    "NtAlertThread",
    "NtAllocateLocallyUniqueId",
    "NtAllocateReserveObject",
    "NtAllocateUserPhysicalPages",
    "NtAllocateUuids",
    "NtAlpcAcceptConnectPort",
    "NtAlpcCancelMessage",
    "NtAlpcConnectPort",
    "NtAlpcCreatePort",
    "NtAlpcCreatePortSection",
    "NtAlpcCreateResourceReserve",
    "NtAlpcCreateSectionView",
    "NtAlpcCreateSecurityContext",
    "NtAlpcDeletePortSection",
    "NtAlpcDeleteResourceReserve",
    "NtAlpcDeleteSectionView",
    "NtAlpcDeleteSecurityContext",
    "NtAlpcDisconnectPort",
    "NtAlpcImpersonateClientOfPort",
    "NtAlpcOpenSenderProcess",
    "NtAlpcOpenSenderThread",
    "NtAlpcQueryInformation",
    "NtAlpcQueryInformationMessage",
    "NtAlpcRevokeSecurityContext",
    "NtAlpcSendWaitReceivePort",
    "NtAlpcSetInformation",
    "NtAreMappedFilesTheSame",
    "NtAssignProcessToJobObject",
    "NtCancelIoFileEx",
    "NtCancelSynchronousIoFile",
    "NtCommitComplete",
    "NtCommitEnlistment",
    "NtCommitTransaction",
    "NtCompactKeys",
    "NtCompareTokens",
    "NtCompleteConnectPort",
    "NtCompressKey",
    "NtConnectPort",
    "NtCreateDebugObject",
    "NtCreateDirectoryObject",
    "NtCreateEnlistment",
    "NtCreateEventPair",
    "NtCreateIoCompletion",
    "NtCreateJobObject",
    "NtCreateJobSet",
    "NtCreateKeyTransacted",
    "NtCreateKeyedEvent",
    "NtCreateMailslotFile",
    "NtCreateMutant",
    "NtCreateNamedPipeFile",
    "NtCreatePagingFile",
    "NtCreatePort",
    "NtCreatePrivateNamespace",
    "NtCreateProcess",
    "NtCreateProfile",
    "NtCreateProfileEx",
    "NtCreateResourceManager",
    "NtCreateSemaphore",
    "NtCreateSymbolicLinkObject",
    "NtCreateThreadEx",
    "NtCreateTimer",
    "NtCreateToken",
    "NtCreateTransaction",
    "NtCreateTransactionManager",
    "NtCreateUserProcess",
    "NtCreateWaitablePort",
    "NtCreateWorkerFactory",
    "NtDebugActiveProcess",
    "NtDebugContinue",
    "NtDeleteAtom",
    "NtDeleteBootEntry",
    "NtDeleteDriverEntry",
    "NtDeleteFile",
    "NtDeleteKey",
    "NtDeleteObjectAuditAlarm",
    "NtDeletePrivateNamespace",
    "NtDeleteValueKey",
    "NtDisableLastKnownGood",
    "NtDisplayString",
    "NtDrawText",
    "NtEnableLastKnownGood",
    "NtEnumerateBootEntries",
    "NtEnumerateDriverEntries",
    "NtEnumerateSystemEnvironmentValuesEx",
    "NtEnumerateTransactionObject",
    "NtExtendSection",
    "NtFilterToken",
    "NtFlushInstallUILanguage",
    "NtFlushInstructionCache",
    "NtFlushKey",
    "NtFlushProcessWriteBuffers",
    "NtFlushVirtualMemory",
    "NtFlushWriteBuffer",
    "NtFreeUserPhysicalPages",
    "NtFreezeRegistry",
    "NtFreezeTransactions",
    "NtGetContextThread",
    "NtGetCurrentProcessorNumber",
    "NtGetDevicePowerState",
    "NtGetMUIRegistryInfo",
    "NtGetNextProcess",
    "NtGetNextThread",
    "NtGetNlsSectionPtr",
    "NtGetNotificationResourceManager",
    "NtGetPlugPlayEvent",
    "NtGetWriteWatch",
    "NtImpersonateAnonymousToken",
    "NtImpersonateThread",
    "NtInitializeNlsFiles",
    "NtInitializeRegistry",
    "NtInitiatePowerAction",
    "NtIsSystemResumeAutomatic",
    "NtIsUILanguageComitted",
    "NtListenPort",
    "NtLoadDriver",
    "NtLoadKey",
    "NtLoadKey2",
    "NtLoadKeyEx",
    "NtLockFile",
    "NtLockProductActivationKeys",
    "NtLockRegistryKey",
    "NtLockVirtualMemory",
    "NtMakePermanentObject",
    "NtMakeTemporaryObject",
    "NtMapCMFModule",
    "NtMapUserPhysicalPages",
    "NtModifyBootEntry",
    "NtModifyDriverEntry",
    "NtNotifyChangeDirectoryFile",
    "NtNotifyChangeKey",
    "NtNotifyChangeMultipleKeys",
    "NtNotifyChangeSession",
    "NtOpenEnlistment",
    "NtOpenEventPair",
    "NtOpenIoCompletion",
    "NtOpenJobObject",
    "NtOpenKeyEx",
    "NtOpenKeyTransacted",
    "NtOpenKeyTransactedEx",
    "NtOpenKeyedEvent",
    "NtOpenMutant",
    "NtOpenObjectAuditAlarm",
    "NtOpenPrivateNamespace",
    "NtOpenProcessToken",
    "NtOpenResourceManager",
    "NtOpenSemaphore",
    "NtOpenSession",
    "NtOpenSymbolicLinkObject",
    "NtOpenThread",
    "NtOpenTimer",
    "NtOpenTransaction",
    "NtOpenTransactionManager",
    "NtPlugPlayControl",
    "NtPrePrepareComplete",
    "NtPrePrepareEnlistment",
    "NtPrepareComplete",
    "NtPrepareEnlistment",
    "NtPrivilegeCheck",
    "NtPrivilegeObjectAuditAlarm",
    "NtPrivilegedServiceAuditAlarm",
    "NtPropagationComplete",
    "NtPropagationFailed",
    "NtPulseEvent",
    "NtQueryBootEntryOrder",
    "NtQueryBootOptions",
    "NtQueryDebugFilterState",
    "NtQueryDirectoryObject",
    "NtQueryDriverEntryOrder",
    "NtQueryEaFile",
    "NtQueryFullAttributesFile",
    "NtQueryInformationAtom",
    "NtQueryInformationEnlistment",
    "NtQueryInformationJobObject",
    "NtQueryInformationPort",
    "NtQueryInformationResourceManager",
    "NtQueryInformationTransaction",
    "NtQueryInformationTransactionManager",
    "NtQueryInformationWorkerFactory",
    "NtQueryInstallUILanguage",
    "NtQueryIntervalProfile",
    "NtQueryIoCompletion",
    "NtQueryLicenseValue",
    "NtQueryMultipleValueKey",
    "NtQueryMutant",
    "NtQueryOpenSubKeys",
    "NtQueryOpenSubKeysEx",
    "NtQueryPortInformationProcess",
    "NtQueryQuotaInformationFile",
    "NtQuerySecurityAttributesToken",
    "NtQuerySecurityObject",
    "NtQuerySemaphore",
    "NtQuerySymbolicLinkObject",
    "NtQuerySystemEnvironmentValue",
    "NtQuerySystemEnvironmentValueEx",
    "NtQuerySystemInformationEx",
    "NtQueryTimerResolution",
    "NtQueueApcThreadEx",
    "NtRaiseException",
    "NtRaiseHardError",
    "NtReadOnlyEnlistment",
    "NtRecoverEnlistment",
    "NtRecoverResourceManager",
    "NtRecoverTransactionManager",
    "NtRegisterProtocolAddressInformation",
    "NtRegisterThreadTerminatePort",
    "NtReleaseKeyedEvent",
    "NtReleaseWorkerFactoryWorker",
    "NtRemoveIoCompletionEx",
    "NtRemoveProcessDebug",
    "NtRenameKey",
    "NtRenameTransactionManager",
    "NtReplaceKey",
    "NtReplacePartitionUnit",
    "NtReplyWaitReplyPort",
    "NtRequestPort",
    "NtResetEvent",
    "NtResetWriteWatch",
    "NtRestoreKey",
    "NtResumeProcess",
    "NtRollbackComplete",
    "NtRollbackEnlistment",
    "NtRollbackTransaction",
    "NtRollforwardTransactionManager",
    "NtSaveKey",
    "NtSaveKeyEx",
    "NtSaveMergedKeys",
    "NtSecureConnectPort",
    "NtSerializeBoot",
    "NtSetBootEntryOrder",
    "NtSetBootOptions",
    "NtSetContextThread",
    "NtSetDebugFilterState",
    "NtSetDefaultHardErrorPort",
    "NtSetDefaultLocale",
    "NtSetDefaultUILanguage",
    "NtSetDriverEntryOrder",
    "NtSetEaFile",
    "NtSetHighEventPair",
    "NtSetHighWaitLowEventPair",
    "NtSetInformationDebugObject",
    "NtSetInformationEnlistment",
    "NtSetInformationJobObject",
    "NtSetInformationKey",
    "NtSetInformationResourceManager",
    "NtSetInformationToken",
    "NtSetInformationTransaction",
    "NtSetInformationTransactionManager",
    "NtSetInformationWorkerFactory",
    "NtSetIntervalProfile",
    "NtSetIoCompletion",
    "NtSetIoCompletionEx",
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
    "NtSetTimerEx",
    "NtSetTimerResolution",
    "NtSetUuidSeed",
    "NtSetVolumeInformationFile",
    "NtShutdownSystem",
    "NtShutdownWorkerFactory",
    "NtSignalAndWaitForSingleObject",
    "NtSinglePhaseReject",
    "NtStartProfile",
    "NtStopProfile",
    "NtSuspendProcess",
    "NtSuspendThread",
    "NtSystemDebugControl",
    "NtTerminateJobObject",
    "NtTestAlert",
    "NtThawRegistry",
    "NtThawTransactions",
    "NtTraceControl",
    "NtTranslateFilePath",
    "NtUmsThreadYield",
    "NtUnloadDriver",
    "NtUnloadKey",
    "NtUnloadKey2",
    "NtUnloadKeyEx",
    "NtUnlockFile",
    "NtUnlockVirtualMemory",
    "NtVdmControl",
    "NtWaitForDebugEvent",
    "NtWaitForKeyedEvent",
    "NtWaitForWorkViaWorkerFactory",
    "NtWaitHighEventPair",
    "NtWaitLowEventPair",
    "NtWorkerFactoryWorkerReady"
};