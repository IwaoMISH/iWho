; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CIWhoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "iWho.h"

ClassCount=3
Class1=CIWhoApp
Class2=CIWhoDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_IWHO_DIALOG

[CLS:CIWhoApp]
Type=0
HeaderFile=iWho.h
ImplementationFile=iWho.cpp
Filter=N
LastObject=CIWhoApp

[CLS:CIWhoDlg]
Type=0
HeaderFile=iWhoDlg.h
ImplementationFile=iWhoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CIWhoDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=iWhoDlg.h
ImplementationFile=iWhoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_IWHO_DIALOG]
Type=1
Class=CIWhoDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_INPUT,edit,1352728644
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_TARGET,edit,1350568064
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_RESULT,edit,1352665284
Control7=IDOK,button,1342177281
Control8=IDCANCEL,button,1073807360

