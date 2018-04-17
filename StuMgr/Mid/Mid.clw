; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMidDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Mid.h"

ClassCount=3
Class1=CMidApp
Class2=CMidDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MID_DIALOG

[CLS:CMidApp]
Type=0
HeaderFile=Mid.h
ImplementationFile=Mid.cpp
Filter=N

[CLS:CMidDlg]
Type=0
HeaderFile=MidDlg.h
ImplementationFile=MidDlg.cpp
Filter=D
LastObject=EDT_MIDPORT
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MidDlg.h
ImplementationFile=MidDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MID_DIALOG]
Type=1
Class=CMidDlg
ControlCount=11
Control1=BTN_START,button,1342242817
Control2=IDC_LIST,SysListView32,1350631425
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_IPADDRESS,SysIPAddress32,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=EDT_SRVPORT,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATE,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=EDT_MIDPORT,edit,1350631552

