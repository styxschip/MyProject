/***************************************************************************************
* AUTHOR : schip
* DATE   : 2018-1-18
* MODULE : MySys.C
* 
* Command: 
*	Source of IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2018 schip.
****************************************************************************************/

//#######################################################################################
//# I N C L U D E S
//#######################################################################################

#ifndef CXX_MYSYS_H
#	include "MySys.h"
#endif

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

    dprintf("[MySys] EasySys Sample Driver\r\n"
            "[MySys] Compiled %s %s\r\n[MySys] In DriverEntry : %wZ\r\n",
			__DATE__, __TIME__, pRegistryString);

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
		dprintf("[MySys] Error, IoCreateDevice = 0x%x\r\n", status);
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
		dprintf("[MySys] Error, IoCreateSymbolicLink = 0x%x\r\n", status);
		
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

	dprintf("[MySys] DriverEntry Success\r\n");

	return STATUS_SUCCESS;
}

VOID
DriverUnload(IN PDRIVER_OBJECT pDriverObj)
{	
	UNICODE_STRING strLink;

	// Unloading - no resources to free so just return.
	dprintf("[MySys] Unloading...\r\n");;	

	//
	// TODO: Add uninstall code here.
	//
	
	// Delete the symbolic link
	RtlInitUnicodeString(&strLink, SYMBOLIC_LINK_NAME);
	IoDeleteSymbolicLink(&strLink);

	// Delete the DeviceObject
	IoDeleteDevice(pDriverObj->DeviceObject);

	dprintf("[MySys] Unloaded Success\r\n");

	return;
}

NTSTATUS
DispatchCreate(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	dprintf("[MySys] IRP_MJ_CREATE\r\n");

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS
DispatchClose(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	dprintf("[MySys] IRP_MJ_CLOSE\r\n");

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	// Return success
	return STATUS_SUCCESS;
}

NTSTATUS
DispatchCommon(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0L;

	dprintf("[MySys] Common Dispatch\r\n");

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
            status = GetSSDTNum(pIoBuffer);
        }
        break;
    case IOCTRL_GET_SSDT:
        {
            status = GetSSDT(pIoBuffer);
        }
        break;
    case IOCTRL_GET_SHADOWNUM:
        {
            status = GetShadowSSDTNum(pIoBuffer);
        }
        break;

    case IOCTRL_GET_SHADOW:
        {
            status = GetShadowSSDT(pIoBuffer);
        }
        break;

    case IOCTRL_REC_FROM_APP:
        {
            // Receive data form Application
            dprintf("[MySys] IOCTRL_REC_FROM_APP\r\n");
            
            // Do we have any data?
            if( uInSize > 0 )
            {
                dprintf("[MySys] Get Data from App: %s\r\n", pIoBuffer);
            }
            
            // Return success
            status = STATUS_SUCCESS;
        }
        break;
        
    case IOCTRL_SEND_TO_APP:
        {
            // Send data to Application
            dprintf("[MySys] IOCTRL_SEND_TO_APP\r\n");
            
            // If we have enough room copy the data upto the App - note copy the terminating character as well...
            if( uOutSize >= strlen( DATA_TO_APP ) + 1 )
            {
                RtlCopyMemory(  pIoBuffer,
                    DATA_TO_APP, 
                    strlen( DATA_TO_APP ) + 1 );
                
                // Update the length for the App
                pIrp->IoStatus.Information = strlen( DATA_TO_APP ) + 1;
                
                dprintf("[MySys] Send Data to App: %s\r\n", pIoBuffer);
                
                // Return success
                status = STATUS_SUCCESS;
            }
        }
        break;
        
        //
        // TODO: Add execute code here.
        //
        
    default:
        {
            // Invalid code sent
            dprintf("[MySys] Unknown IOCTL: 0x%X (%04X,%04X)\r\n", 
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
    char szGDT[6] = {0};
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
        __asm
        {
            sgdt szGDT
        }
        KdPrint(("[MySys] GDT limit:%p GDT:%p\n", *(SHORT*)szGDT, *(int*)(szGDT + 2)));
        pGDT = (PCHAR)*(int*)(szGDT + 2);
        dwGDTL = *(SHORT*)szGDT + 1; //从0开始所以加1
        //dwMap = dwLimit / 8;
        RtlCopyMemory((PCHAR)pBuff + dwGDTL * dwCPUCount, pGDT, dwGDTL);
        ulMask <<= 1;

        KdPrint(("[MySys] pBuff + dwGDTL * dwCPUCount : %p\n", (PCHAR)pBuff + dwGDTL * dwCPUCount));
        dwCPUCount++;
    }
    //还原CPU属性
    KeSetSystemAffinityThread(ulProcessors);
    
    return TRUE;
}

BOOLEAN ReadGDTLimit(PVOID pBuff)
{
    char szGDT[6] = {0};

    __asm
    {
        sgdt szGDT
    }
    KdPrint(("[MySys] GDT limit:%p \n", *(SHORT*)szGDT));

    RtlCopyMemory((PCHAR)pBuff, szGDT, sizeof(SHORT));
    return TRUE;
}

NTSTATUS GetProcessDirBase(DWORD dwPID, PDWORD pDirBase)
{
    PEPROCESS Process;
    PEPROCESS CurProcess;
    CHAR  *pszImageName;
    DWORD dwCurPID;
    DWORD i;
    CHAR szBuf[256] = {0};
    
    __try
    {
        //遍历EPROCESS
        __asm 
        {
            mov eax, fs:[124h]  //ETHREAD
            mov eax, [eax+44h] //EPROCESS
            mov Process, eax
        }
        
        CurProcess = Process;
        i = 0;
        do 
        {
            pszImageName = (char*)CurProcess + 0x174;
            dwCurPID = (*(DWORD*)((char*)CurProcess + 0x084));
        
            dprintf("[MyReadProcessMemory] {%d} PID=%d ImageName:%s \r\n",
                i++, dwCurPID, pszImageName);
            
            if (dwCurPID == dwPID)
            {
                *pDirBase = (*(DWORD*)((char*)CurProcess + 0x018));
                dprintf("[MyReadProcessMemory] Find PID=%d DirBase:%p\r\n",
                    dwCurPID, pDirBase);
                return STATUS_SUCCESS;
            }
            CurProcess = (*(DWORD*)((char*)CurProcess + 0x088)) - 0x88; //下一个
        } 
        while (CurProcess != Process);
    }
    __except(EXCEPTION_EXECUTE_HANDLER )
    {
        dprintf("[MyReadProcessMemory] GetProcessDirBase __except \r\n");
    }
    
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS MyReadProcessMemory(DWORD dwPID, DWORD dwAdddress, 
                             DWORD dwSize, PVOID lpBuff, 
                             DWORD dwBufSize)
{
    DWORD dwDirBase;
    NTSTATUS status;
    DWORD dwOldDirBase;
    
    KdBreakPoint();
    
    __try
    {
        status = GetProcessDirBase(dwPID, &dwDirBase);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        
        
        __asm
        {
            cli              //屏蔽中断
            mov eax, cr0                //关闭内存保护
            and eax, not 10000h
            mov cr0, eax
            
            mov eax, cr3
            
            
            mov dwOldDirBase, eax
            
            //切换CR3
            mov eax, dwDirBase
            mov cr3, eax
        }
        
        //读取内存
        //ProbeForRead(dwAdddress, dwSize, 4);
        
        if ( dwSize > dwBufSize)
        {
            dwSize = dwBufSize;
        }
        
        RtlCopyMemory(lpBuff, dwAdddress, dwSize);
        
        __asm
        {
            mov eax, dwOldDirBase
            mov cr3, eax
            
            mov eax, cr0                //恢复内存保护
            or  eax,  10000h
            mov cr0, eax
            sti                         //恢复中断
        }
        
    }
    __except(EXCEPTION_EXECUTE_HANDLER )
    {
        dprintf("[MyReadProcessMemory] MyReadProcessMemory __except \r\n");
    }
    
    return STATUS_SUCCESS;
}

NTSTATUS ReadIDTLimit(PVOID pBuff)
{
    char szIDT[6] = {0};
    
    __asm
    {
        sidt szIDT
    }
    KdPrint(("[MySys] IDT limit:%p \n", *(SHORT*)szIDT));
    
    RtlCopyMemory((PCHAR)pBuff, szIDT, sizeof(SHORT));
    return STATUS_SUCCESS;
}


NTSTATUS ReadIDT(PVOID pBuff)
{
    char szIDT[6] = {0};
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
        __asm
        {
            sidt szIDT
        }
        KdPrint(("[MySys] IDT limit:%p IDT:%p\n", *(SHORT*)szIDT, *(int*)(szIDT + 2)));
        pIDT = (PCHAR)*(int*)(szIDT + 2);
        dwIDTL = *(SHORT*)szIDT + 1; //从0开始所以加1
        //dwMap = dwLimit / 8;
        RtlCopyMemory((PCHAR)pBuff + dwIDTL * dwCPUCount, pIDT, dwIDTL);
        ulMask <<= 1;
        
        KdPrint(("[MySys] pBuff + dwIDTL * dwCPUCount : %p\n", (PCHAR)pBuff + dwIDTL * dwCPUCount));
        dwCPUCount++;
    }
    //还原CPU属性
    KeSetSystemAffinityThread(ulProcessors);
    
    return STATUS_SUCCESS;
}

NTSTATUS GetBaseFromGDT(DWORD dwIndex, DWORD *pdwBase)
{
    char szGDT[6] = {0};
    DWORD dwDGT = NULL;
    DWORD dwBase = NULL;
    DWORD dwBaseLow = NULL;
    DWORD dwBaseHigh = NULL;
    PGDT pGDT = NULL;
    
    //取出GDT
    __asm
    {
        sgdt szGDT
    }

    dwDGT = *(DWORD *)(szGDT + 2);
    dprintf("GetBaseFromGDT GDT addr dwDGT is 0x%08X\n", dwDGT);
    if (dwDGT == NULL)
    {
        return STATUS_UNSUCCESSFUL;
    }

    //KdBreakPoint();

    //GDT每项8字节
    dwDGT = dwDGT + dwIndex * 8;

    pGDT = (PGDT)dwDGT;
    dprintf("GetBaseFromGDT pGDT is 0x%08X\n", pGDT);
    
    //拼出地址
    dwBaseLow = (DWORD)(pGDT->m_BaseLow);
    dwBaseHigh = (DWORD)((pGDT->m_BaseHigh) << 24);
    
    dwBase = dwBaseLow + dwBaseHigh;
    dprintf("GetBaseFromGDT dwBase is 0x%08X\n", dwBase);

    //检查得到的地址是否有效
    if(!MmIsAddressValid((PVOID)dwBase))
    {
        dprintf("GetBaseFromGDT MmIsAddressValid(dwBase) is invalid!!\n");
        *pdwBase = NULL;
        return STATUS_UNSUCCESSFUL;
    }

    *pdwBase = dwBase; 

    return STATUS_SUCCESS;
}

NTSTATUS GetSSDTDataBase(DWORD *pDataBase)
{
    DWORD dwFS = 0;
    DWORD dwGDTBase = 0;
    DWORD dwSSDTAddr = 0;

    //KdBreakPoint();
    __asm
    {
        xor eax, eax
        mov ax, fs
        mov dwFS, eax
    }
    dwFS = dwFS >> 3;
    if (STATUS_UNSUCCESSFUL == GetBaseFromGDT(dwFS, &dwGDTBase))
    {
        dprintf("GetSSDTDataBase GetBaseFromGDT failed!\n");
        return STATUS_UNSUCCESSFUL;
    }

    dwSSDTAddr = *(DWORD *)(PVOID)(dwGDTBase + 0x020); //得 _KPRCB 地址
    dwSSDTAddr = *(DWORD *)(PVOID)(dwSSDTAddr + 0x004); // 得 _KTHREAD 地址
    dwSSDTAddr = *(DWORD *)(PVOID)(dwSSDTAddr + 0x0e0); //得 ServiceTable 地址;

    //检查地址是否有效
    if(!MmIsAddressValid((PVOID)dwSSDTAddr))
    {
        dprintf("GetSSDTDataBase MmIsAddressValid(dwAddr) is invalid!!\n");
        return STATUS_UNSUCCESSFUL;
    }

    *pDataBase = dwSSDTAddr;

    return STATUS_SUCCESS;
}


NTSTATUS GetSSDTNum(PVOID pBuff)
{
    DWORD dwSSDTAddr = NULL;
    if (STATUS_UNSUCCESSFUL == GetSSDTDataBase(&dwSSDTAddr))
    {
        dprintf("GetSSDTDataBase failed!!\n");
        return STATUS_UNSUCCESSFUL;
    }

    *(DWORD *)pBuff = *(DWORD *)(dwSSDTAddr + 2 * sizeof(DWORD));
    return STATUS_SUCCESS; 
}

NTSTATUS GetSSDT(PVOID pBuff)
{
    DWORD dwNum = 0;
    DWORD dwSSDTAddr = NULL;
    PVOID pSSDTMap = NULL;

    if (STATUS_UNSUCCESSFUL == GetSSDTDataBase(&dwSSDTAddr))
    {
        dprintf("GetSSDT GetSSDTDataBase!!\n");
        return STATUS_UNSUCCESSFUL;
    }
    
    //拿SSDT个数  封装的不好
    dwNum = *(DWORD *)(dwSSDTAddr + 2 * sizeof(DWORD));

    pSSDTMap = (PVOID)(*(DWORD*)dwSSDTAddr);

    RtlCopyMemory(pBuff, pSSDTMap, dwNum * sizeof(PVOID));

    return STATUS_SUCCESS; 
}

NTSTATUS GetShadowSSDTNum(PVOID pBuff)
{
    //通过SSDT的位置找到ShadowSSDT
    //addr:8055b1e0  data: 804e36b8 00000000 0000011c 805110c8  804e36b8是SSDT
    //addr:8055b1f0  data: bf99d800 00000000 0000029b bf99e510  bf99d800是ShadowSSDT

    DWORD dwSSDTAddr = NULL;
    if (STATUS_UNSUCCESSFUL == GetSSDTDataBase(&dwSSDTAddr))
    {
        dprintf("GetSSDTDataBase failed!!\n");
        return STATUS_UNSUCCESSFUL;
    }
    
    *(DWORD *)pBuff = *(DWORD *)(dwSSDTAddr + 6 * sizeof(DWORD));
    return STATUS_SUCCESS; 
}

NTSTATUS GetShadowSSDT(PVOID pBuff)
{
    //ShadowSSDT 注意只有UI线程访问地址的内容才是有效的
    DWORD dwNum = 0;
    DWORD dwSSDTAddr = NULL;
    PVOID pShadowSSDTMap = NULL;

    KdPrint(("[MySys] GetShadowSSDT PID:%d TID:%d\n", PsGetCurrentProcessId(), PsGetCurrentThreadId()));
    
    if (STATUS_UNSUCCESSFUL == GetSSDTDataBase(&dwSSDTAddr))
    {
        dprintf("GetSSDT GetSSDTDataBase!!\n");
        return STATUS_UNSUCCESSFUL;
    }
    
    //通过SSDT的位置找到ShadowSSDT
    //addr:8055b1e0  data: 804e36b8 00000000 0000011c 805110c8  804e36b8是SSDT
    //addr:8055b1f0  data: bf99d800 00000000 0000029b bf99e510  bf99d800是ShadowSSDT
    //拿ShadowSSDT个数
    dwNum = *(DWORD *)(dwSSDTAddr + 6 * sizeof(DWORD));
    
    pShadowSSDTMap = (PVOID)(*(DWORD*)(dwSSDTAddr + 4 * sizeof(DWORD)));
    
    RtlCopyMemory(pBuff, pShadowSSDTMap, dwNum * sizeof(PVOID));
    
    return STATUS_SUCCESS; 
}


/* EOF */
