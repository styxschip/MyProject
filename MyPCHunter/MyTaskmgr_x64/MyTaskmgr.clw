; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMessageHookDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyTaskmgr.h"

ClassCount=12
Class1=CMyTaskmgrApp
Class2=CMyTaskmgrDlg
Class3=CAboutDlg

ResourceCount=13
Resource1=DLG_FILEMGR
Resource2=IDR_MAINFRAME
Resource3=DLG_IDT
Class4=CDialogReg
Class5=CMyRegTree
Resource4=DLG_KRNLMODULE
Resource5=DLG_SSDT
Resource6=DLG_SHADOWSSDT
Resource7=DLG_MYCMD
Class6=CMyGDT
Class7=CProcessMgrDlg
Resource8=IDD_ABOUTBOX
Class8=CMyIDT
Resource9=DLG_GDT
Class9=CMySSDTDlg
Resource10=DLG_REGEDIT
Class10=CShadowSSDTDlg
Resource11=IDD_MYTASKMGR_DIALOG
Class11=CKernelModuleDlg
Class12=CMessageHookDlg
Resource12=DLG_PROCESS
Resource13=DLG_MSGHOOK

[CLS:CMyTaskmgrApp]
Type=0
HeaderFile=MyTaskmgr.h
ImplementationFile=MyTaskmgr.cpp
Filter=N
LastObject=CMyTaskmgrApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CMyTaskmgrDlg]
Type=0
HeaderFile=MyTaskmgrDlg.h
ImplementationFile=MyTaskmgrDlg.cpp
Filter=D
LastObject=CMyTaskmgrDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MyTaskmgrDlg.h
ImplementationFile=MyTaskmgrDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYTASKMGR_DIALOG]
Type=1
Class=CMyTaskmgrDlg
ControlCount=1
Control1=IDC_TAB1,SysTabControl32,1342177280

[DLG:DLG_REGEDIT]
Type=1
Class=CDialogReg
ControlCount=2
Control1=IDC_TREE1,SysTreeView32,1350631431
Control2=IDC_LIST1,SysListView32,1350633473

[CLS:CDialogReg]
Type=0
HeaderFile=DialogReg.h
ImplementationFile=DialogReg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CMyRegTree]
Type=0
HeaderFile=MyRegTree.h
ImplementationFile=MyRegTree.cpp
BaseClass=CTreeCtrl
Filter=W
VirtualFilter=GWC
LastObject=CMyRegTree

[DLG:DLG_PROCESS]
Type=1
Class=CProcessMgrDlg
ControlCount=1
Control1=LST_PROCESS,SysListView32,1350633473

[DLG:DLG_MYCMD]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=EDT_ENTER,edit,1350635648
Control3=BTN_RUN,button,1342242816
Control4=EDT_CMDSHOW,edit,1352730756

[DLG:DLG_FILEMGR]
Type=1
Class=CMessageHookDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352

[DLG:DLG_GDT]
Type=1
Class=CMyGDT
ControlCount=1
Control1=LST_GDT,SysListView32,1350633473

[CLS:CMyGDT]
Type=0
HeaderFile=MyGDT.h
ImplementationFile=MyGDT.cpp
BaseClass=CDialog
Filter=D
LastObject=LST_GDT
VirtualFilter=dWC

[CLS:CProcessMgrDlg]
Type=0
HeaderFile=ProcessMgrDlg.h
ImplementationFile=ProcessMgrDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=LST_PROCESS

[DLG:DLG_IDT]
Type=1
Class=CMyIDT
ControlCount=1
Control1=LST_IDT,SysListView32,1350633473

[CLS:CMyIDT]
Type=0
HeaderFile=MyIDT.h
ImplementationFile=MyIDT.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=LST_IDT

[CLS:CMySSDTDlg]
Type=0
HeaderFile=MySSDTDlg.h
ImplementationFile=MySSDTDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=LST_SSDT

[DLG:DLG_SSDT]
Type=1
Class=?
ControlCount=1
Control1=LST_SSDT,SysListView32,1350633473

[DLG:DLG_SHADOWSSDT]
Type=1
Class=CShadowSSDTDlg
ControlCount=1
Control1=LST_SHADOW,SysListView32,1350633473

[CLS:CShadowSSDTDlg]
Type=0
HeaderFile=ShadowSSDTDlg.h
ImplementationFile=ShadowSSDTDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=LST_SHADOW

[DLG:DLG_KRNLMODULE]
Type=1
Class=CKernelModuleDlg
ControlCount=1
Control1=LST_MODULE,SysListView32,1350633473

[CLS:CKernelModuleDlg]
Type=0
HeaderFile=KernelModuleDlg.h
ImplementationFile=KernelModuleDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:CMessageHookDlg]
Type=0
HeaderFile=MessageHookDlg.h
ImplementationFile=MessageHookDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=LST_MSGHOOK
VirtualFilter=dWC

[DLG:DLG_MSGHOOK]
Type=1
Class=CMessageHookDlg
ControlCount=1
Control1=LST_MSGHOOK,SysListView32,1350633473

