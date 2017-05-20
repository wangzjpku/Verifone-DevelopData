# Microsoft Developer Studio Project File - Name="LABSDS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=LABSDS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LABSDS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LABSDS.mak" CFG="LABSDS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LABSDS - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "LABSDS - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "LABSDS - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /f LABSDS.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "LABSDS.exe"
# PROP BASE Bsc_Name "LABSDS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "NMAKE /f LAB.smk"
# PROP Rebuild_Opt "/a"
# PROP Target_File "LABSDS.exe"
# PROP Bsc_Name "LABSDS.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "LABSDS - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /f LABSDS.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "LABSDS.exe"
# PROP BASE Bsc_Name "LABSDS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "NMAKE /f LAB.smk"
# PROP Rebuild_Opt "/a"
# PROP Target_File "LABSDS.exe"
# PROP Bsc_Name "LABSDS.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "LABSDS - Win32 Release"
# Name "LABSDS - Win32 Debug"

!IF  "$(CFG)" == "LABSDS - Win32 Release"

!ELSEIF  "$(CFG)" == "LABSDS - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\..\Source\count.txt
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\LAB.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\LAB.fst
# End Source File
# Begin Source File

SOURCE=.\LAB.smk
# End Source File
# End Target
# End Project
