/***************************************************************************************
* AUTHOR : schip
* DATE   : 2018-2-4
* MODULE : MySysX64.H
*
* IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2010 schip.
****************************************************************************************/

#ifndef CXX_MYSYSX64_H
#define CXX_MYSYSX64_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ntddk.h>
#include <devioctl.h>
#include "common.h"
#include "struct.h"

//
// TODO: Add your include here
//


//////////////////////////////////////////////////////////////////////////

//
// TODO: Add your struct,enum(private) here
//



//////////////////////////////////////////////////////////////////////////
//***************************************************************************************
//* NAME:			DriverEntry
//*
//* DESCRIPTION:	Registers dispatch routines.
//*					
//*	PARAMETERS:		pDriverObj						IN		
//*						Address of the DRIVER_OBJECT created by NT for this driver.
//*					pRegistryString					IN		
//*						UNICODE_STRING which represents this drivers KEY in the Registry.  	
//*
//*	IRQL:			IRQL_PASSIVE_LEVEL.
//*
//*	RETURNS:		NTSTATUS
//***************************************************************************************
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString);

//***************************************************************************************
//* NAME:			DriverUnload
//*
//* DESCRIPTION:	This routine is our dynamic unload entry point.
//*					
//*	PARAMETERS:		pDriverObj						IN		Address of our DRIVER_OBJECT.
//*
//*	IRQL:			IRQL_PASSIVE_LEVEL.
//*
//*	RETURNS:		None
//***************************************************************************************
VOID DriverUnload(IN PDRIVER_OBJECT pDriverObj);

//***************************************************************************************
//* NAME:			DispatchCreate, DispatchClose
//*
//* DESCRIPTION:	This two methods are the dispatch entry point for IRP_MJ_CREATE and IRP_MJ_CLOSE 
//*					routines.  This sample simply completes the requests with success.
//*					
//*	PARAMETERS:		pDevObj							IN		Address of our DRIVER_OBJECT.
//*					pIrp							IN		Address of the IRP.
//*
//*	IRQL:			IRQL_PASSIVE_LEVEL.
//*
//*	RETURNS:		STATUS_SUCCESS
//***************************************************************************************
NTSTATUS DispatchCreate(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);
NTSTATUS DispatchClose(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);

//***************************************************************************************
//* NAME:			DispatchDeviceControl
//*
//* DESCRIPTION:	This is the dispatch entry point for IRP_MJ_DEVICE_CONTROL.
//*					
//*	PARAMETERS:		pDevObj							IN		Address of our DRIVER_OBJECT.
//*					pIrp							IN		Address of the IRP.
//*
//*	IRQL:			IRQL_PASSIVE_LEVEL.
//*
//*	RETURNS:		NTSTATUS
//*
//*	NOTES:			IRP_MJ_DEVICE_CONTROL
//*					Parameters:
//*					Parameters.DeviceIoControl.OutputBufferLength	Length of OutBuffer 
//*					in bytes (length of buffer from GUI)
//*					Parameters.DeviceIoControl.InputBufferLength	Length of InBuffer 
//*					in bytes (length of buffer from DRIVER)
//*					Parameters.DeviceIoControl.ControlCode			I/O control code
//***************************************************************************************
NTSTATUS DispatchDeviceControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);

// common dispatch
//***************************************************
//* #define IRP_MJ_CREATE                   0x00
//* #define IRP_MJ_CREATE_NAMED_PIPE        0x01
//* #define IRP_MJ_CLOSE                    0x02
//* #define IRP_MJ_READ                     0x03
//* #define IRP_MJ_WRITE                    0x04
//* #define IRP_MJ_QUERY_INFORMATION        0x05
//* #define IRP_MJ_SET_INFORMATION          0x06
//* #define IRP_MJ_QUERY_EA                 0x07
//* #define IRP_MJ_SET_EA                   0x08
//* #define IRP_MJ_FLUSH_BUFFERS            0x09
//* #define IRP_MJ_QUERY_VOLUME_INFORMATION 0x0a
//* #define IRP_MJ_SET_VOLUME_INFORMATION   0x0b
//* #define IRP_MJ_DIRECTORY_CONTROL        0x0c
//* #define IRP_MJ_FILE_SYSTEM_CONTROL      0x0d
//* #define IRP_MJ_DEVICE_CONTROL           0x0e
//* #define IRP_MJ_INTERNAL_DEVICE_CONTROL  0x0f
//* #define IRP_MJ_SHUTDOWN                 0x10
//* #define IRP_MJ_LOCK_CONTROL             0x11
//* #define IRP_MJ_CLEANUP                  0x12
//* #define IRP_MJ_CREATE_MAILSLOT          0x13
//* #define IRP_MJ_QUERY_SECURITY           0x14
//* #define IRP_MJ_SET_SECURITY             0x15
//* #define IRP_MJ_POWER                    0x16
//* #define IRP_MJ_SYSTEM_CONTROL           0x17
//* #define IRP_MJ_DEVICE_CHANGE            0x18
//* #define IRP_MJ_QUERY_QUOTA              0x19
//* #define IRP_MJ_SET_QUOTA                0x1a
//* #define IRP_MJ_PNP                      0x1b
//* #define IRP_MJ_PNP_POWER                IRP_MJ_PNP      // Obsolete....
//* #define IRP_MJ_MAXIMUM_FUNCTION         0x1b -->
//***************************************************************************************
NTSTATUS DispatchCommon (IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);

//////////////////////////////////////////////////////////////////////////

#ifdef ALLOC_PRAGMA
    // Allow the DriverEntry routine to be discarded once initialization is completed
#pragma alloc_text(INIT, DriverEntry)
    // 
#pragma alloc_text(PAGE, DriverUnload)
#pragma alloc_text(PAGE, DispatchCreate)
#pragma alloc_text(PAGE, DispatchClose)
#pragma alloc_text(PAGE, DispatchDeviceControl)
#pragma alloc_text(PAGE, DispatchCommon)
#endif // ALLOC_PRAGMA

//////////////////////////////////////////////////////////////////////////

//
// TODO: Add your module declarations here
//


ULONG64 LdrInPebOffset = 0x018;		//peb.ldr
ULONG64 ModListInPebOffset = 0x010;	//peb.ldr.InLoadOrderModuleList

QWORD GetGDTR();
DWORD GetGDTL();

QWORD GetIDTR();
DWORD GetIDTL();

BOOLEAN ReadGDT(PVOID pBuff);
BOOLEAN ReadGDTLimit(PVOID pBuff);

NTSTATUS ReadIDT(PVOID pBuff);
NTSTATUS ReadIDTLimit(PVOID pBuff);

//SSDT和ShadowSSDT都用
NTSTATUS GetSSDTDataBase(DWORD *pDataBase);

//获取SSDT
ULONGLONG GetKeServiceDescriptorTable64();

NTSTATUS GetSSDTNum(PVOID pBuff);
NTSTATUS GetSSDT(PVOID pBuff);


NTSTATUS EnumModuleFunc(PVOID pBuff);


#pragma intrinsic(__readmsr)
//获取Shadow SSDT
ULONGLONG GetKeServiceDescriptorTableShadow64();
ULONGLONG GetSSSDTFuncCurAddr64(ULONG64 Index);

NTSTATUS GetShadowSSDTNum(PVOID pBuff);
NTSTATUS GetShadowSSDT(PVOID pBuff);

//获取进程完整路径
VOID GetImagePath(PEPROCESS Process, UCHAR *szBuf);

NTSTATUS EnumProcess(PVOID pBuff);
PEPROCESS LookupProcess(HANDLE Pid);


//声明API
NTKERNELAPI UCHAR* PsGetProcessImageFileName( IN PEPROCESS Process );
NTKERNELAPI HANDLE PsGetProcessInheritedFromUniqueProcessId( IN PEPROCESS Process );
NTKERNELAPI PPEB PsGetProcessPeb(PEPROCESS Process);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(HANDLE Id, PEPROCESS *Process);
NTKERNELAPI NTSTATUS PsLookupThreadByThreadId(HANDLE Id, PETHREAD *Thread);
NTKERNELAPI PEPROCESS IoThreadToProcess(PETHREAD Thread);
NTKERNELAPI VOID NTAPI KeAttachProcess(PEPROCESS Process);
NTKERNELAPI VOID NTAPI KeDetachProcess();
NTKERNELAPI VOID NTAPI KeStackAttachProcess(PEPROCESS Process, PKAPC_STATE ApcState);
NTKERNELAPI VOID NTAPI KeUnstackDetachProcess(PKAPC_STATE ApcState);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeToMultiByteN(
    __out_bcount_part(MaxBytesInMultiByteString, *BytesInMultiByteString) PCHAR MultiByteString,
    __in ULONG MaxBytesInMultiByteString,
    __out_opt PULONG BytesInMultiByteString,
    __in_bcount(BytesInUnicodeString) PCWCH UnicodeString,
    __in ULONG BytesInUnicodeString
    );

typedef struct _SYSTEM_SERVICE_TABLE{
    PVOID  		ServiceTableBase; 
    PVOID  		ServiceCounterTableBase; 
    ULONGLONG  	NumberOfServices; 
    PVOID  		ParamTableBase; 
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE{
    SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe (native api)
    SYSTEM_SERVICE_TABLE win32k;    // win32k.sys   (gdi/user)
    SYSTEM_SERVICE_TABLE Table3;    // not used
    SYSTEM_SERVICE_TABLE Table4;    // not used
}SERVICE_DESCRIPTOR_TABLE,*PSERVICE_DESCRIPTOR_TABLE;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,
    SystemProcessorInformation,             // obsolete...delete
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemMirrorMemoryInformation,
    SystemPerformanceTraceInformation,
    SystemObsolete0,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemVerifierAddDriverInformation,
    SystemVerifierRemoveDriverInformation,
    SystemProcessorIdleInformation,
    SystemLegacyDriverInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation,
    SystemTimeSlipNotification,
    SystemSessionCreate,
    SystemSessionDetach,
    SystemSessionInformation,
    SystemRangeStartInformation,
    SystemVerifierInformation,
    SystemVerifierThunkExtend,
    SystemSessionProcessInformation,
    SystemLoadGdiDriverInSystemSpace,
    SystemNumaProcessorMap,
    SystemPrefetcherInformation,
    SystemExtendedProcessInformation,
    SystemRecommendedSharedDataAlignment,
    SystemComPlusPackage,
    SystemNumaAvailableMemory,
    SystemProcessorPowerInformation,
    SystemEmulationBasicInformation,
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation,
    SystemLostDelayedWriteInformation,
    SystemBigPoolInformation,
    SystemSessionPoolTagInformation,
    SystemSessionMappedViewInformation,
    SystemHotpatchInformation,
    SystemObjectSecurityMode,
    SystemWatchdogTimerHandler,
    SystemWatchdogTimerInformation,
    SystemLogicalProcessorInformation,
    SystemWow64SharedInformation,
    SystemRegisterFirmwareTableInformationHandler,
    SystemFirmwareTableInformation,
    SystemModuleInformationEx,
    SystemVerifierTriageInformation,
    SystemSuperfetchInformation,
    SystemMemoryListInformation,
    SystemFileCacheInformationEx,
    MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS;

NTSTATUS
ZwQuerySystemInformation (
                          __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
                          __out_bcount_opt(SystemInformationLength) PVOID SystemInformation,
                          __in ULONG SystemInformationLength,
                          __out_opt PULONG ReturnLength);

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    ULONG Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    CHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;


typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;


BOOLEAN VxkCopyMemory( PVOID pDestination, PVOID pSourceAddress, SIZE_T SizeOfCopy )
{
    PMDL pMdl = NULL;
    PVOID pSafeAddress = NULL;
    pMdl = IoAllocateMdl( pSourceAddress, (ULONG)SizeOfCopy, FALSE, FALSE, NULL );
    if( !pMdl ) return FALSE;
    __try
    {
        MmProbeAndLockPages( pMdl, KernelMode, IoReadAccess );
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        IoFreeMdl( pMdl );
        return FALSE;
    }
    pSafeAddress = MmGetSystemAddressForMdlSafe( pMdl, NormalPagePriority );
    if( !pSafeAddress ) return FALSE;
    RtlCopyMemory( pDestination, pSafeAddress, SizeOfCopy );
    MmUnlockPages( pMdl );
    IoFreeMdl( pMdl );
    return TRUE;
}

NTKERNELAPI PUCHAR PsGetProcessImageFileName( IN PEPROCESS Process );
NTKERNELAPI PEPROCESS IoThreadToProcess( IN PETHREAD Thread );

ULONG64 rwkm_adr=0, rwkm_len=0;

#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////

#endif	//CXX_MYSYSX64_H
/* EOF */
