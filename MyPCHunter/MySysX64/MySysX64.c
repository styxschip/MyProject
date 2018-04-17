/***************************************************************************************
* AUTHOR : schip
* DATE   : 2018-2-4
* MODULE : MySysX64.C
* 
* Command: 
*	Source of IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2010 schip.
****************************************************************************************/

//#######################################################################################
//# I N C L U D E S
//#######################################################################################

#ifndef CXX_MYSYSX64_H
#	include "MySysX64.h"
#endif

#include <Ntstrsafe.h>
#include "struct.h"

//////////////////////////////////////////////////////////////////////////

//#######################################################################################
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@				D R I V E R   E N T R Y   P O I N T						 @@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//#######################################################################################
NTSTATUS
DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
	NTSTATUS		status = STATUS_SUCCESS;
	UNICODE_STRING  ustrLinkName;
	UNICODE_STRING  ustrDevName;  
	PDEVICE_OBJECT  pDevObj;
	int i = 0;

    // test data
    //QWORD qdGDTR = 0;
    //DWORD dwGETL = 0;

    //

    dprintf("[MySysX64] EasySys Sample Driver\r\n"
            "[MySysX64] Compiled %s %s\r\n[MySysX64] In DriverEntry : %wZ\r\n",
			__DATE__, __TIME__, pRegistryString);

    // test code
    //KdBreakPoint();
    //qdGDTR = GetGDTR();
    //dwGETL = GetGDTL();

    //


	// Register dispatch routines
/*
	for(i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObj->MajorFunction[i] = DispatchCommon;  
	}
*/
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;

	// Dispatch routine for communications
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;

	// Unload routine
	pDriverObj->DriverUnload = DriverUnload;

	// Initialize the device name.
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);

	// Create the device object and device extension
	status = IoCreateDevice(pDriverObj, 
				0,
				&ustrDevName, 
				FILE_DEVICE_UNKNOWN,
				0,
				FALSE,
				&pDevObj);

	if(!NT_SUCCESS(status))
	{
		dprintf("[MySysX64] Error, IoCreateDevice = 0x%x\r\n", status);
		return status;
	}

    //// Get a pointer to our device extension
    //deviceExtension = (PDEVICE_EXTENSION) deviceObject->DeviceExtension;

    //// Save a pointer to the device object
    //deviceExtension->DeviceObject = deviceObject;

	if(IoIsWdmVersionAvailable(1,0x10))
	{
		//如果是支持符号链接用户相关性的系统
		RtlInitUnicodeString(&ustrLinkName, SYMBOLIC_LINK_GLOBAL_NAME);
	}
	else
	{
		//不支持
		RtlInitUnicodeString(&ustrLinkName, SYMBOLIC_LINK_NAME);
	}
	
	// Create a symbolic link to allow USER applications to access it. 
	status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);  
	
	if(!NT_SUCCESS(status))
	{
		dprintf("[MySysX64] Error, IoCreateSymbolicLink = 0x%x\r\n", status);
		
		IoDeleteDevice(pDevObj); 
		return status;
	}	

	//
	//	TODO: Add initialization code here.
	//

    //// Tell the I/O Manger to do BUFFERED IO
    //deviceObject->Flags |= DO_BUFFERED_IO;

    //// Save the DeviveObject
    //deviceExtension->DeviceObject = deviceObject;

	dprintf("[MySysX64] DriverEntry Success\r\n");

	return STATUS_SUCCESS;
}

VOID
DriverUnload(IN PDRIVER_OBJECT pDriverObj)
{	
	UNICODE_STRING strLink;

	// Unloading - no resources to free so just return.
	dprintf("[MySysX64] Unloading...\r\n");;	

	//
	// TODO: Add uninstall code here.
	//
	
	// Delete the symbolic link
	RtlInitUnicodeString(&strLink, SYMBOLIC_LINK_NAME);
	IoDeleteSymbolicLink(&strLink);

	// Delete the DeviceObject
	IoDeleteDevice(pDriverObj->DeviceObject);

	dprintf("[MySysX64] Unloaded Success\r\n");

	return;
}

NTSTATUS
DispatchCreate(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	dprintf("[MySysX64] IRP_MJ_CREATE\r\n");

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS
DispatchClose(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	dprintf("[MySysX64] IRP_MJ_CLOSE\r\n");

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	// Return success
	return STATUS_SUCCESS;
}

NTSTATUS
DispatchCommon(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0L;

	dprintf("[MySysX64] Common Dispatch\r\n");

	IoCompleteRequest( pIrp, 0 );

	// Return success
	return STATUS_SUCCESS;
}

NTSTATUS 
DispatchDeviceControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS status               = STATUS_INVALID_DEVICE_REQUEST;	 // STATUS_UNSUCCESSFUL
	PIO_STACK_LOCATION pIrpStack  = IoGetCurrentIrpStackLocation(pIrp);
	ULONG uIoControlCode          = 0;
	PVOID pIoBuffer				  = NULL;
	ULONG uInSize                 = 0;
	ULONG uOutSize                = 0;

	// Get the IoCtrl Code
	uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;

	pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch(uIoControlCode)
	{			
		//
		// TODO: Add execute code here.
		//
    case IOCTRL_GET_GDT:
        {
            if (TRUE == ReadGDT(pIoBuffer))
            {
                status = STATUS_SUCCESS;
            }
            else
            {
                status = STATUS_UNSUCCESSFUL;
            }
        }
        break;
    case IOCTRL_GET_GDTLIMIT:
        {
            ReadGDTLimit(pIoBuffer);
            status = STATUS_SUCCESS;
        }
        break;  
    case IOCTRL_READ_PROC_MEM:
        {
            status = STATUS_UNSUCCESSFUL;
        }
        break;
    case IOCTRL_WRIT_PROC_MEM:
        {
            status = STATUS_UNSUCCESSFUL;
        }
        break;
    case IOCTRL_GET_IDT:
        {
            status = ReadIDT(pIoBuffer);
        }
        break;
        
    case IOCTRL_GET_IDTLIMIT:
        {
            status = ReadIDTLimit(pIoBuffer);
        }
        break;
    case IOCTRL_GET_SSDTNUM:
        {
            //status = GetSSDTNum(pIoBuffer);
            status = STATUS_SUCCESS;
        }
        break;
    case IOCTRL_GET_SSDT:
        {
            status = GetSSDT(pIoBuffer);
        }
        break;
    case IOCTRL_GET_SHADOWNUM:
        {
            //status = GetShadowSSDTNum(pIoBuffer);
            status = STATUS_SUCCESS;
        }
        break;
        
    case IOCTRL_GET_SHADOW:
        {
            status = GetShadowSSDT(pIoBuffer);
        }
        break;
    case IOCTRL_GET_PROC_INFO:
        {
            status = EnumProcess(pIoBuffer);
        }
        break;
    case IOCTRL_ENUM_MODULE:
        {
            status = EnumModuleFunc(pIoBuffer);
        }
        break;

    case IOCTL_SET_RWKM_ADR:
        {
            rwkm_adr=*(UINT64 *)pIoBuffer;
            DbgPrint("rwkm_adr: %lld\n",rwkm_adr);
            status = STATUS_SUCCESS;
            break;
        }
    case IOCTL_SET_RWKM_LEN:
        {
            rwkm_len=*(UINT64 *)pIoBuffer;
            DbgPrint("rwkm_len: %lld\n",rwkm_len);
            status = STATUS_SUCCESS;
            break;
        }
    case IOCTL_READ_KRNL_MM:
        {
            VxkCopyMemory(pIoBuffer,(PVOID)rwkm_adr,(SIZE_T)rwkm_len);
            status = STATUS_SUCCESS;
            break;
        }
    case IOCTL_MODIFY_KN_MM:
        {
            VxkCopyMemory((PVOID)rwkm_adr,pIoBuffer,(SIZE_T)rwkm_len);
            status = STATUS_SUCCESS;
            break;
        }
    case IOCTL_GET_PN_BY_ET:
        {
            PETHREAD et;
            PEPROCESS ep;
            PUCHAR pn;
            et=*(PETHREAD *)pIoBuffer;
            ep=IoThreadToProcess(et);
            pn=PsGetProcessImageFileName(ep);
            memcpy(pIoBuffer,pn,16);
            status = STATUS_SUCCESS;
            break;
		}

		default:
			{
				// Invalid code sent
				dprintf("[MySysX64] Unknown IOCTL: 0x%X (%04X,%04X)\r\n", 
                                          uIoControlCode,
                                          DEVICE_TYPE_FROM_CTL_CODE(uIoControlCode),
                                          IoGetFunctionCodeFromCtlCode(uIoControlCode));
				status = STATUS_INVALID_PARAMETER;	
			}
			break;
	}

	if(status == STATUS_SUCCESS)
	{
		pIrp->IoStatus.Information = uOutSize;
	}
	else
	{
		pIrp->IoStatus.Information = 0;
	}

	// Complete the I/O Request
	pIrp->IoStatus.Status = status;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}


//
// TODO: Add your module definitions here.
//
BOOLEAN ReadGDT(PVOID pBuff)
{
    QWORD qwGDT = 0;
    ULONG ulProcessors = 0;
    ULONG ulMask = 1;
    PCHAR pGDT = NULL;
    DWORD dwCPUCount = 0; //CPU核心数
    //DWORD dwMap = 0; //GDT表项数
    DWORD dwGDTL = 0; //GDT表长度
    
    KdPrint(("[MySys] ReadGDT PID:%d TID:%d\n", PsGetCurrentProcessId(), PsGetCurrentThreadId()));
    
    //获取核心数
    ulProcessors = KeQueryActiveProcessors();
    
    while (ulMask != 0x80000000)
    {        
        if (!(ulMask & ulProcessors))
        {
            break;
        }
        //指定CPU执行线程
        KeSetSystemAffinityThread(ulProcessors & ulMask);
        qwGDT = GetGDTR();
        dwGDTL = GetGDTL();
        
        dprintf("[MySys] GDT:%p\n  limit::%p\n", qwGDT, dwGDTL);
        RtlCopyMemory((PCHAR)pBuff + dwGDTL * dwCPUCount, qwGDT, dwGDTL);
        ulMask <<= 1;
        
        dprintf("[MySys] pBuff + dwGDTL * dwCPUCount : %p\n", (PCHAR)pBuff + dwGDTL * dwCPUCount);
        dwCPUCount++;
    }
    //还原CPU属性
    KeSetSystemAffinityThread(ulProcessors);
    
    return TRUE;
}

BOOLEAN ReadGDTLimit(PVOID pBuff)
{
    DWORD dwGDTL = 0;
    dwGDTL = GetGDTL();
    dprintf("[MySys] GDT limit:%p \n", dwGDTL);
    
    RtlCopyMemory((PCHAR)pBuff, &dwGDTL, sizeof(SHORT));
    return TRUE;
}

NTSTATUS ReadIDTLimit(PVOID pBuff)
{
    DWORD dwIDTL = 0;
    dwIDTL = GetIDTL();
    dprintf("[MySys] IDT limit:%p \n", dwIDTL);
    
    RtlCopyMemory((PCHAR)pBuff, &dwIDTL, sizeof(SHORT));
    return STATUS_SUCCESS;
}


NTSTATUS ReadIDT(PVOID pBuff)
{
    QWORD qwIDT = 0;
    ULONG ulProcessors = 0;
    ULONG ulMask = 1;
    PCHAR pIDT = NULL;
    DWORD dwCPUCount = 0; //CPU核心数
    //DWORD dwMap = 0; //IDT表项数
    DWORD dwIDTL = 0; //IDT表长度
    
    KdPrint(("[MySys] ReadIDT PID:%d TID:%d\n", PsGetCurrentProcessId(), PsGetCurrentThreadId()));
    //KdBreakPoint();
    //获取核心数
    ulProcessors = KeQueryActiveProcessors();
    
    while (ulMask != 0x80000000)
    {        
        if (!(ulMask & ulProcessors))
        {
            break;
        }
        //指定CPU执行线程
        KeSetSystemAffinityThread(ulProcessors & ulMask);
        qwIDT = GetIDTR();
        dwIDTL = GetIDTL();
        dprintf("[MySys] IDT limit:%p IDT:%p\n", dwIDTL, qwIDT);

        //dwMap = dwLimit / 8;
        RtlCopyMemory((PCHAR)pBuff + dwIDTL * dwCPUCount, qwIDT, dwIDTL);
        ulMask <<= 1;
        
        KdPrint(("[MySys] pBuff + dwIDTL * dwCPUCount : %p\n", (PCHAR)pBuff + dwIDTL * dwCPUCount));
        dwCPUCount++;
    }
    //还原CPU属性
    KeSetSystemAffinityThread(ulProcessors);
    
    return STATUS_SUCCESS;
}


PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable = NULL;

ULONGLONG GetSSDTFuncCurAddr(ULONG id)
{
    LONG dwtmp = 0;
    PULONG ServiceTableBase = NULL;
    
    KeServiceDescriptorTable = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTable64();

    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    dwtmp = ServiceTableBase[id];
    dwtmp = dwtmp >> 4;
    return dwtmp + (ULONGLONG)ServiceTableBase;
}

ULONG GetOffsetAddress(ULONGLONG FuncAddr)
{
    LONG dwtmp = 0;
    PULONG ServiceTableBase = NULL;
    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    dwtmp = (LONG)(FuncAddr - (ULONGLONG)ServiceTableBase);
    return dwtmp << 4;
}


NTSTATUS GetSSDTNum(PVOID pBuff)
{
    return STATUS_SUCCESS; 
}

ULONGLONG GetKeServiceDescriptorTable64() 
{
    //研究下
    //特征码
    char KiSystemServiceStart_pattern[13] = "\x8B\xF8\xC1\xEF\x07\x83\xE7\x20\x25\xFF\x0F\x00\x00";	
    ULONGLONG CodeScanStart = (ULONGLONG)&_strnicmp;
    ULONGLONG CodeScanEnd = (ULONGLONG)&KdDebuggerNotPresent;
    UNICODE_STRING Symbol;
    ULONGLONG i, tbl_address, b;
    for (i = 0; i < CodeScanEnd - CodeScanStart; i++)
    {
        if (!memcmp((char*)(ULONGLONG)CodeScanStart +i, (char*)KiSystemServiceStart_pattern,13))
        { 
            for (b = 0; b < 50; b++)
            {
                tbl_address = ((ULONGLONG)CodeScanStart+i+b);
                if (*(USHORT*) ((ULONGLONG)tbl_address ) == (USHORT)0x8d4c)
                    return ((LONGLONG)tbl_address +7) + *(LONG*)(tbl_address +3);
            }
        }
    }
    return 0;
}

ULONGLONG GetKeServiceDescriptorTableShadow64()
{
    PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
    PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
    PUCHAR i = NULL;
    UCHAR b1 = 0,b2 = 0, b3 = 0;
    ULONG templong = 0;
    ULONGLONG addr = 0;
    for(i = StartSearchAddress; i < EndSearchAddress;i++)
    {
        if( MmIsAddressValid(i) && MmIsAddressValid(i+1) && MmIsAddressValid(i+2) )
        {
            b1=*i;
            b2=*(i+1);
            b3=*(i+2);
            if( b1==0x4c && b2==0x8d && b3==0x1d ) //4c8d1d
            {
                memcpy(&templong,i+3,4);
                addr = (ULONGLONG)templong + (ULONGLONG)i + 7;
                return addr;
            }
        }
    }
    return 0;
}

NTSTATUS GetSSDT(PVOID pBuff)
{
    int i = 0;

    ((ULONGLONG *)pBuff)[i] = 0x987654321;

    for (i = 0; i < 401; i++)
    {
        ((ULONGLONG *)pBuff)[i] = GetSSDTFuncCurAddr(i);
    }
    return STATUS_SUCCESS; 
}

NTSTATUS GetShadowSSDTNum(PVOID pBuff)
{
    return STATUS_SUCCESS; 
}

PSYSTEM_SERVICE_TABLE KeServiceDescriptorTableShadow = NULL;
ULONG64	ul64W32pServiceTable = NULL;

ULONGLONG GetSSSDTFuncCurAddr64(ULONG64 Index)
{
    ULONGLONG				W32pServiceTable=0, qwTemp=0;
    LONG 					dwTemp=0;
    PSYSTEM_SERVICE_TABLE	pWin32k;
    pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)KeServiceDescriptorTableShadow + sizeof(SYSTEM_SERVICE_TABLE));
    W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
    ul64W32pServiceTable = W32pServiceTable;
    qwTemp = W32pServiceTable + 4 * (Index-0x1000);	//这里是获得偏移地址的位置，要HOOK的话修改这里即可
    dwTemp = *(PLONG)qwTemp;
    dwTemp = dwTemp >> 4;
    qwTemp = W32pServiceTable + (LONG64)dwTemp;
    return qwTemp;
}


NTSTATUS GetShadowSSDT(PVOID pBuff)
{     
    int i = 0;
    ((ULONGLONG *)pBuff)[i] = 0x987654321;

    KeServiceDescriptorTableShadow = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTableShadow64();

    for (i = 0; i < 827; i++)
    {
        ((ULONGLONG *)pBuff)[i] = GetSSSDTFuncCurAddr64(i);
    }

    return STATUS_SUCCESS; 
}

//获取进程完整路径
VOID GetImagePath(PEPROCESS Process, UCHAR *szBuf)
{
    int nLen = 0;

    SIZE_T Peb = 0;
    SIZE_T Ldr = 0;
    PLIST_ENTRY ModListHead = 0;
    PLIST_ENTRY Module = 0;
    ANSI_STRING AnsiString;
    KAPC_STATE ks;
    //EPROCESS地址无效则退出
    if( !MmIsAddressValid(Process) )
    {
        return;
    }
    //获取PEB地址
    Peb = (SIZE_T)PsGetProcessPeb(Process);
    //PEB地址无效则退出
    if (!Peb)
    {
        return;
    }
    //依附进程
    KeStackAttachProcess(Process, &ks);
    __try
    {
        //获得LDR地址
        Ldr = Peb + (SIZE_T)LdrInPebOffset;
        //测试是否可读，不可读则抛出异常退出
        ProbeForRead((CONST PVOID)Ldr, 8, 8);
        //获得链表头
        ModListHead = (PLIST_ENTRY)(*(PULONG64)Ldr + ModListInPebOffset);
        //再次测试可读性
        ProbeForRead((CONST PVOID)ModListHead, 8, 8);
        //获得第一个模块的信息
        Module = ModListHead->Flink;

        //KdBreakPoint();

        RtlUnicodeToMultiByteN(szBuf,
                              MAXBYTE,
                              &nLen,
                              (((PLDR_DATA_TABLE_ENTRY)Module)->FullDllName).Buffer,
                              (((PLDR_DATA_TABLE_ENTRY)Module)->FullDllName).Length);




    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        DbgPrint("[EnumModule]__except (EXCEPTION_EXECUTE_HANDLER)");
    }
    //取消依附进程
    KeUnstackDetachProcess(&ks);
}

//根据进程ID返回进程EPROCESS，失败返回NULL
PEPROCESS LookupProcess(HANDLE Pid)
{
    PEPROCESS eprocess = NULL;
    if( NT_SUCCESS(PsLookupProcessByProcessId(Pid, &eprocess)) )
    {
        return eprocess;
    }
    else
    {
        return NULL;
    }
}

//枚举进程
NTSTATUS EnumProcess(PVOID pBuff)
{
    ULONG i = 0;
    ULONG j = 0;
    DWORD dwPID = 0;
    PEPROCESS eproc = NULL;
    UCHAR *szProcName = NULL;
    UCHAR szTmp[MAXBYTE] = {0};


    
    for(i = 4; i  < 100000; i = i + 4) //100000 应该没有这么大的PID 以后改进
    {
        eproc = LookupProcess((HANDLE)i);
        if(eproc != NULL)
        {
            ObDereferenceObject(eproc);
            
            szProcName = PsGetProcessImageFileName(eproc);
            
            RtlCopyMemory((PCHAR)(((PProcessInfo)pBuff)[j].m_szBuf), 
                                    szProcName,
                                    strlen(szProcName)); //危险

            RtlZeroMemory(szTmp, MAXBYTE);
            GetImagePath(eproc, szTmp);
            RtlCopyMemory((PCHAR)(((PProcessInfo)pBuff)[j].m_szFullPath), 
                                    szTmp,
                                    MAXBYTE);

                
            dwPID = (DWORD)PsGetProcessId(eproc);
            RtlCopyMemory((PCHAR)&(((PProcessInfo)pBuff)[j].m_dwPID), 
                            &dwPID,
                            sizeof(DWORD)); 
            
            RtlCopyMemory((PCHAR)&(((PProcessInfo)pBuff)[j].qwEPROCESS), 
                            &eproc,
                            sizeof(PEPROCESS)); 
            j++;
        }
    }
    return STATUS_SUCCESS;
}

NTSTATUS EnumModuleFunc(PVOID pBuff)
{
    NTSTATUS Status;
    PRTL_PROCESS_MODULES            Modules;
    RTL_PROCESS_MODULE_INFORMATION  *ModuleInfo;
    PVOID Buffer;
    ULONG BufferSize = 4096;
    ULONG ReturnLength;
    ULONG i;
    PEnumModule LPMoudle;
    
retry:
    Buffer = ExAllocatePoolWithTag(NonPagedPool, BufferSize, 'test');
    
    if (!Buffer)
    {
        return STATUS_NO_MEMORY;
    }
    Status = ZwQuerySystemInformation( SystemModuleInformation,
                                        Buffer,
                                        BufferSize,
                                        &ReturnLength);
    
    if (Status == STATUS_INFO_LENGTH_MISMATCH)
    {
        ExFreePool(Buffer);
        BufferSize = ReturnLength;
        goto retry;
    }
    
    if (NT_SUCCESS(Status)) 
    {
        Modules = (PRTL_PROCESS_MODULES) Buffer;
        for (i = 0, ModuleInfo = &(Modules->Modules[0]);
        i < Modules->NumberOfModules;
        i++, ModuleInfo++) 
        {
//             dprintf("[EnumModule] Base=%p Size=%p Path=%s\n",  
//                 ModuleInfo->ImageBase,
//                 ModuleInfo->ImageSize, 
//                 ModuleInfo->FullPathName);

            //KdBreakPoint();
            LPMoudle = (PEnumModule)pBuff;
            RtlCopyMemory(&(LPMoudle[i].m_qwImageBase), &(ModuleInfo->ImageBase), sizeof(ULONGLONG));
            RtlCopyMemory(&(LPMoudle[i].m_ImageSize), &(ModuleInfo->ImageSize), sizeof(ULONG));
            RtlCopyMemory(LPMoudle[i].szFullPathName, ModuleInfo->FullPathName, 255);
        }
        
    } 
    
    ExFreePool(Buffer);
    return Status;
}

/* EOF */
