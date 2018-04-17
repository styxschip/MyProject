/**************************************************************************************
* AUTHOR : schip
* DATE   : 2018-2-4
* MODULE : common.h
*
* Command: 
*	IOCTRL Common Header
*
* Description:
*	Common data for the IoCtrl driver and application
*
****************************************************************************************
* Copyright (C) 2010 schip.
****************************************************************************************/

#pragma once 

//#######################################################################################
// D E F I N E S
//#######################################################################################

#if DBG
#define dprintf DbgPrint
#else
#define dprintf
#endif

//不支持符号链接用户相关性
#define DEVICE_NAME                  L"\\Device\\devMySys"             // Driver Name
#define SYMBOLIC_LINK_NAME           L"\\DosDevices\\MySys"            // Symbolic Link Name
#define WIN32_LINK_NAME              "\\\\.\\MySys"                    // Win32 Link Name

//支持符号链接用户相关性
#define SYMBOLIC_LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\MySys"    // Symbolic Link Name

#define DATA_TO_APP                  "Hello World from Driver"

//
// Device IO Control Codes
//
#define IOCTL_BASE          0x800
#define MY_CTL_CODE(i)        \
    CTL_CODE                  \
    (                         \
        FILE_DEVICE_UNKNOWN,  \
        IOCTL_BASE + i,       \
        METHOD_BUFFERED,      \
        FILE_ANY_ACCESS       \
    )

#define IOCTRL_GET_GDT              MY_CTL_CODE(20)
#define IOCTRL_GET_GDTLIMIT         MY_CTL_CODE(21)
#define IOCTRL_READ_PROC_MEM        MY_CTL_CODE(22)
#define IOCTRL_WRIT_PROC_MEM        MY_CTL_CODE(23)
#define IOCTRL_GET_IDT              MY_CTL_CODE(24)
#define IOCTRL_GET_IDTLIMIT         MY_CTL_CODE(25)
#define IOCTRL_GET_SSDTNUM          MY_CTL_CODE(26)
#define IOCTRL_GET_SSDT             MY_CTL_CODE(27)
#define IOCTRL_GET_SHADOWNUM        MY_CTL_CODE(28)
#define IOCTRL_GET_SHADOW           MY_CTL_CODE(29)
#define IOCTRL_GET_PROC_INFO        MY_CTL_CODE(30)
#define IOCTRL_ENUM_MODULE          MY_CTL_CODE(31)


#define IOCTL_READ_KRNL_MM	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)	//read kernel
#define IOCTL_MODIFY_KN_MM	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)	//write kernel
#define IOCTL_SET_RWKM_ADR	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x809, METHOD_BUFFERED, FILE_ANY_ACCESS)	//set address
#define IOCTL_SET_RWKM_LEN	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80A, METHOD_BUFFERED, FILE_ANY_ACCESS)	//set length

#define IOCTL_GET_PN_BY_ET	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x7FF, METHOD_BUFFERED, FILE_ANY_ACCESS)	//set length
//
// TODO: Add your IOCTL define here
//



//
// TODO: Add your struct,enum(public) define here
//



/* EOF */

