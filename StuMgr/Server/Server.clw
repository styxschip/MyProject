; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CServerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Server.h"

ClassCount=3
Class1=CServerApp
Class2=CServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SERVER_DIALOG

[CLS:CServerApp]
Type=0
HeaderFile=Server.h
ImplementationFile=Server.cpp
Filter=N

[CLS:CServerDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=EDT_PORT

[CLS:CAboutDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERVER_DIALOG]
Type=1
Class=CServerDlg
ControlCount=19
Control1=IDC_IPADDRESS,SysIPAddress32,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=EDT_DBPORT,edit,1350631552
Control5=BTN_CONNECT,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=EDT_DBNAME,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CONSTATE,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=EDT_USER,edit,1350631552
Control13=EDT_PASWORD,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=BTN_MAKEDB,button,1208025088
Control16=IDC_STATIC,button,1342177287
Control17=IDC_LIST,SysListView32,1350631429
Control18=IDC_STATIC,static,1342308352
Control19=EDT_SRVPORT,edit,1350631552

