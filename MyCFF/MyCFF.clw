; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgReloc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyCFF.h"

ClassCount=5
Class1=CMyCFFApp
Class2=CMyCFFDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_MYCFF_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=DlgINT
Resource4=IDD_INT
Class5=CDlgReloc
Resource5=DLG_RELOC

[CLS:CMyCFFApp]
Type=0
HeaderFile=MyCFF.h
ImplementationFile=MyCFF.cpp
Filter=N

[CLS:CMyCFFDlg]
Type=0
HeaderFile=MyCFFDlg.h
ImplementationFile=MyCFFDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMyCFFDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MyCFFDlg.h
ImplementationFile=MyCFFDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYCFF_DIALOG]
Type=1
Class=CMyCFFDlg
ControlCount=13
Control1=ID_OPENEXE,button,1342242817
Control2=EDT_EXEPATH,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=LST_STRUCT,SysListView32,1350633997
Control5=IDC_TREE1,SysTreeView32,1350631431
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=EDT_VA,edit,1350631560
Control10=EDT_RVA,edit,1350631560
Control11=EDT_FILEOFFSET,edit,1350631560
Control12=IDC_STATIC,static,1342308352
Control13=EDT_IMAGEBASE,edit,1350639752

[DLG:IDD_INT]
Type=1
Class=DlgINT
ControlCount=1
Control1=LST_IMPORTTABLE,SysListView32,1350633989

[CLS:DlgINT]
Type=0
HeaderFile=DlgINT.h
ImplementationFile=DlgINT.cpp
BaseClass=CDialog
Filter=D
LastObject=DlgINT
VirtualFilter=dWC

[DLG:DLG_RELOC]
Type=1
Class=CDlgReloc
ControlCount=1
Control1=LST_RELOC,SysListView32,1350633485

[CLS:CDlgReloc]
Type=0
HeaderFile=DlgReloc.h
ImplementationFile=DlgReloc.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgReloc

