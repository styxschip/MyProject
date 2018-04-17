; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyCMD_srvDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyCMD_srv.h"

ClassCount=3
Class1=CMyCMD_srvApp
Class2=CMyCMD_srvDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYCMD_SRV_DIALOG

[CLS:CMyCMD_srvApp]
Type=0
HeaderFile=MyCMD_srv.h
ImplementationFile=MyCMD_srv.cpp
Filter=N

[CLS:CMyCMD_srvDlg]
Type=0
HeaderFile=MyCMD_srvDlg.h
ImplementationFile=MyCMD_srvDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=EDT_SHOW

[CLS:CAboutDlg]
Type=0
HeaderFile=MyCMD_srvDlg.h
ImplementationFile=MyCMD_srvDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYCMD_SRV_DIALOG]
Type=1
Class=CMyCMD_srvDlg
ControlCount=9
Control1=EDT_CMD,edit,1350635648
Control2=BTN_RUN,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=EDT_SHOW,edit,1352728708
Control5=IDC_START,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_IPADDRESS,SysIPAddress32,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=EDT_PORT,edit,1350631552

