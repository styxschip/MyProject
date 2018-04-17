; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRemoteMgr_SvrDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "RemoteMgr_Svr.h"

ClassCount=5
Class1=CRemoteMgr_SvrApp
Class2=CRemoteMgr_SvrDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=DLG_CLTSCREEN
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CShowCltScreen
Resource4=IDD_REMOTEMGR_SVR_DIALOG
Class5=CMyCMD_srvDlg
Resource5=IDD_MYCMD_SRV_DIALOG

[CLS:CRemoteMgr_SvrApp]
Type=0
HeaderFile=RemoteMgr_Svr.h
ImplementationFile=RemoteMgr_Svr.cpp
Filter=N

[CLS:CRemoteMgr_SvrDlg]
Type=0
HeaderFile=RemoteMgr_SvrDlg.h
ImplementationFile=RemoteMgr_SvrDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LISTCLT

[CLS:CAboutDlg]
Type=0
HeaderFile=RemoteMgr_SvrDlg.h
ImplementationFile=RemoteMgr_SvrDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_REMOTEMGR_SVR_DIALOG]
Type=1
Class=CRemoteMgr_SvrDlg
ControlCount=9
Control1=BTN_START,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_IPADDRESS,SysIPAddress32,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=EDT_PORT,edit,1350631552
Control6=BTN_GETSCREEN,button,1342242816
Control7=BTN_CMD,button,1342242816
Control8=IDC_LISTCLT,SysListView32,1350633485
Control9=IDC_STATIC,static,1342308352

[DLG:DLG_CLTSCREEN]
Type=1
Class=CShowCltScreen
ControlCount=0

[CLS:CShowCltScreen]
Type=0
HeaderFile=ShowCltScreen.h
ImplementationFile=ShowCltScreen.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CShowCltScreen

[DLG:IDD_MYCMD_SRV_DIALOG]
Type=1
Class=CMyCMD_srvDlg
ControlCount=4
Control1=EDT_CMD,edit,1350635648
Control2=BTN_RUN,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=EDT_SHOW,edit,1352730756

[CLS:CMyCMD_srvDlg]
Type=0
HeaderFile=MyCMD_srvDlg.h
ImplementationFile=MyCMD_srvDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMyCMD_srvDlg
VirtualFilter=dWC

