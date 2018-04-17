/**************************************************************************************
* AUTHOR : schip
* DATE   : 2018-1-18
* MODULE : common.h
*
* Command: 
*	IOCTRL Common Header
*
* Description:
*	Common data for the IoCtrl driver and application
*
****************************************************************************************
* Copyright (C) 2018 schip.
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
#define DEVICE_NAME                  L"\\Device\\MySys"             // Driver Name
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

#define IOCTRL_GET_GDT              MY_CTL_CODE(0)
#define IOCTRL_GET_GDTLIMIT         MY_CTL_CODE(1)
#define IOCTRL_READ_PROC_MEM        MY_CTL_CODE(2)
#define IOCTRL_WRIT_PROC_MEM        MY_CTL_CODE(3)
#define IOCTRL_GET_IDT              MY_CTL_CODE(4)
#define IOCTRL_GET_IDTLIMIT         MY_CTL_CODE(5)
#define IOCTRL_GET_SSDTNUM          MY_CTL_CODE(6)
#define IOCTRL_GET_SSDT             MY_CTL_CODE(7)
#define IOCTRL_GET_SHADOWNUM        MY_CTL_CODE(8)
#define IOCTRL_GET_SHADOW           MY_CTL_CODE(9)


#define IOCTRL_REC_FROM_APP         MY_CTL_CODE(21)
#define IOCTRL_SEND_TO_APP          MY_CTL_CODE(22)


//
// TODO: Add your IOCTL define here
//



//
// TODO: Add your struct,enum(public) define here
//



/* EOF */

