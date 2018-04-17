; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyCMDDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyCMD.h"

ClassCount=3
Class1=CMyCMDApp
Class2=CMyCMDDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYCMD_DIALOG

[CLS:CMyCMDApp]
Type=0
HeaderFile=MyCMD.h
ImplementationFile=MyCMD.cpp
Filter=N

[CLS:CMyCMDDlg]
Type=0
HeaderFile=MyCMDDlg.h
ImplementationFile=MyCMDDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=EDT_SHOW

[CLS:CAboutDlg]
Type=0
HeaderFile=MyCMDDlg.h
ImplementationFile=MyCMDDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYCMD_DIALOG]
Type=1
Class=CMyCMDDlg
ControlCount=5
Control1=EDT_ENTER,edit,1350635648
Control2=BTN_RUN,button,1342242817
Control3=BTN_STOP,button,1476460544
Control4=EDT_SHOW,edit,1352730756
Control5=IDC_STATIC,static,1342308352

