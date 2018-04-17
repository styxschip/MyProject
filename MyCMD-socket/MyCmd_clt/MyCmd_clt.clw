; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyCmd_cltDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyCmd_clt.h"

ClassCount=3
Class1=CMyCmd_cltApp
Class2=CMyCmd_cltDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYCMD_CLT_DIALOG

[CLS:CMyCmd_cltApp]
Type=0
HeaderFile=MyCmd_clt.h
ImplementationFile=MyCmd_clt.cpp
Filter=N

[CLS:CMyCmd_cltDlg]
Type=0
HeaderFile=MyCmd_cltDlg.h
ImplementationFile=MyCmd_cltDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMyCmd_cltDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MyCmd_cltDlg.h
ImplementationFile=MyCmd_cltDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYCMD_CLT_DIALOG]
Type=1
Class=CMyCmd_cltDlg
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=EDT_PORT,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BTNCONNECT,button,1342242816
Control5=IDC_IPADDRESS,SysIPAddress32,1342242816

