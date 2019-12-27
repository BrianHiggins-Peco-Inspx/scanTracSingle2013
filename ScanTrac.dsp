# Microsoft Developer Studio Project File - Name="ScanTrac" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SCANTRAC - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ScanTrac.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ScanTrac.mak" CFG="SCANTRAC - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ScanTrac - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ScanTrac - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ScanTrac - Win32 ScanTrac Bounds Checker" (based on "Win32 (x86) Application")
!MESSAGE "ScanTrac - Win32 Demo" (based on "Win32 (x86) Application")
!MESSAGE "ScanTrac - Win32 Demo Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386 /mapinfo:lines
# SUBTRACT LINK32 /profile /debug

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /D /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ScanTrac___Win32_ScanTrac_Bounds_Checker"
# PROP BASE Intermediate_Dir "ScanTrac___Win32_ScanTrac_Bounds_Checker"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ScanTrac___Win32_ScanTrac_Bounds_Checker"
# PROP Intermediate_Dir "ScanTrac___Win32_ScanTrac_Bounds_Checker"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 mvtools.lib itimgutl.lib itimgmgr.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mvtools.lib itimgutl.lib itimgmgr.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ScanTrac___Win32_Demo"
# PROP BASE Intermediate_Dir "ScanTrac___Win32_Demo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ScanTrac___Win32_Demo"
# PROP Intermediate_Dir "ScanTrac___Win32_Demo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ScanTracDemo" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0xc0c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 mvtools.lib itimgutl.lib itimgmgr.lib /nologo /subsystem:windows /map /machine:I386 /mapinfo:lines
# SUBTRACT BASE LINK32 /profile /debug
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386 /mapinfo:lines
# SUBTRACT LINK32 /profile /debug

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ScanTrac___Win32_Demo_Debug0"
# PROP BASE Intermediate_Dir "ScanTrac___Win32_Demo_Debug0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ScanTrac___Win32_Demo_Debug0"
# PROP Intermediate_Dir "ScanTrac___Win32_Demo_Debug0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /D /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "ScanTracDemo" /FR /Yu"stdafx.h" /D /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 mvtools.lib itimgutl.lib itimgmgr.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept /mapinfo:lines
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 itimgutl.lib itimgmgr.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ScanTrac - Win32 Release"
# Name "ScanTrac - Win32 Debug"
# Name "ScanTrac - Win32 ScanTrac Bounds Checker"
# Name "ScanTrac - Win32 Demo"
# Name "ScanTrac - Win32 Demo Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ACPowerMoniter.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ACPowerMoniterLogFile.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AlphaEntryDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AutoSetupImageDisplayDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BackupHardDriveThread.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BodyTrigger.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CheckWeighDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureDirectoryNames.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureEdgePixelAdjustDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureEMailAddresses.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureInspectionsDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigurePasswordsDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureProduct.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureProductBodyTriggerDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureProductContaminantProcessesDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureProductContaminantReferenceDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureProductInspectionSettings.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureProductRegionOfInterestDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigureRadiationDoorDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Container.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Contaminant22EditThresholdsDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DetectorSetupDialog1.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DiagnosticCountersDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DiagnosticLineDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DigitalLine.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DisplayDialogThread.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DisplaySavedRejects.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EasySelectProductDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ejector.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExternalDetector.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FBWF_Info.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FindParameterHelpDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FrontEndDiagnosticDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HelpDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HelpIndexDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HistogramDisplayDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImageAquisitionThread.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImageData.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inspection.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionBottomContaminant.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionCheckWeigh.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionContaminant.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionDensity.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionGlassContaminant.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionKernel.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionSize.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionStandardDeviation.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspectionVoid.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspxHiddenDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspxHiddenSystemDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InspxHiddenXRayDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ITIPCDig.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\KeyBoardTestDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ListeningSocket.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mapimessages.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mapisession.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MoreDiagnosticsDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewConfigProductEjectorDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewConfigureProductExternalDetectorDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewConfigureSystemDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewConfigureSystemEjectorDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewConfigureSystemExternalDetectorDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewDiagnosticDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewEditProductDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewImageDisplayDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NoticeDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NumericEntryDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PasswordEntryDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ping.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PingTestThread.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlotDataDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProcessQuickLearnImagesThread.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Product.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProductDensityCompensationDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProductShapeHandlingDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ScanTrac.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ScanTrac.rc
# End Source File
# Begin Source File

SOURCE=.\ScanTracConfigureDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ScanTracDlg.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SelectInspectionDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SelectItemDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SelectMultipleItemDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SerialPortBase.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SetupSystemExternalHardware.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SocketMessage1.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SpellmanDXMXRaySourcePort.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StartCheckDisk.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SystemConfigurationData.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SystemCountersDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SystemProductSettingsDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SystemTemperaturesDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SystemValuesDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TCPIPServerSocket.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\uControllerStatusDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UndoAddToLearnDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Win32SerialPortThread.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XRaySource.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XRaySourceStatusDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XRayTubeAlignmentDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\YesNoDialog.cpp

!IF  "$(CFG)" == "ScanTrac - Win32 Release"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 ScanTrac Bounds Checker"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo"

!ELSEIF  "$(CFG)" == "ScanTrac - Win32 Demo Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ACPowerMoniter.h
# End Source File
# Begin Source File

SOURCE=.\ACPowerMoniterLogFile.h
# End Source File
# Begin Source File

SOURCE=.\AlphaEntryDialog.h
# End Source File
# Begin Source File

SOURCE=.\AutoSetupImageDisplayDialog.h
# End Source File
# Begin Source File

SOURCE=.\BackupHardDriveThread.h
# End Source File
# Begin Source File

SOURCE=.\BodyTrigger.h
# End Source File
# Begin Source File

SOURCE=.\CheckWeighDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureDirectoryNames.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureEdgePixelAdjustDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureEMailAddresses.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureInspectionsDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigurePasswordsDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureProduct.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureProductBodyTriggerDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureProductContaminantProcessesDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureProductContaminantReferenceDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureProductInspectionSettings.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureProductRegionOfInterestDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureRadiationDoorDialog.h
# End Source File
# Begin Source File

SOURCE=.\Container.h
# End Source File
# Begin Source File

SOURCE=.\Contaminant22EditThresholdsDialog.h
# End Source File
# Begin Source File

SOURCE=.\DetectorSetupDialog1.h
# End Source File
# Begin Source File

SOURCE=.\DiagnosticCountersDialog.h
# End Source File
# Begin Source File

SOURCE=.\DiagnosticLineDialog.h
# End Source File
# Begin Source File

SOURCE=.\DigitalLine.h
# End Source File
# Begin Source File

SOURCE=.\DisplayDialogThread.h
# End Source File
# Begin Source File

SOURCE=.\DisplaySavedRejects.h
# End Source File
# Begin Source File

SOURCE=.\EasySelectProductDialog.h
# End Source File
# Begin Source File

SOURCE=.\Ejector.h
# End Source File
# Begin Source File

SOURCE=.\ExternalDetector.h
# End Source File
# Begin Source File

SOURCE=.\FBWF_Info.h
# End Source File
# Begin Source File

SOURCE=.\FindParameterHelpDialog.h
# End Source File
# Begin Source File

SOURCE=.\FrontEndDiagnosticDialog.h
# End Source File
# Begin Source File

SOURCE=.\HelpDialog.h
# End Source File
# Begin Source File

SOURCE=.\HelpIndexDialog.h
# End Source File
# Begin Source File

SOURCE=.\HistogramDisplayDialog.h
# End Source File
# Begin Source File

SOURCE=.\ImageAquisitionThread.h
# End Source File
# Begin Source File

SOURCE=.\ImageData.h
# End Source File
# Begin Source File

SOURCE=.\Inspection.h
# End Source File
# Begin Source File

SOURCE=.\InspectionBottomContaminant.h
# End Source File
# Begin Source File

SOURCE=.\InspectionCheckWeigh.h
# End Source File
# Begin Source File

SOURCE=.\InspectionContaminant.h
# End Source File
# Begin Source File

SOURCE=.\InspectionDensity.h
# End Source File
# Begin Source File

SOURCE=.\InspectionGlassContaminant.h
# End Source File
# Begin Source File

SOURCE=.\InspectionKernel.h
# End Source File
# Begin Source File

SOURCE=.\InspectionSize.h
# End Source File
# Begin Source File

SOURCE=.\InspectionStandardDeviation.h
# End Source File
# Begin Source File

SOURCE=.\InspectionVoid.h
# End Source File
# Begin Source File

SOURCE=.\InspxHiddenDialog.h
# End Source File
# Begin Source File

SOURCE=.\InspxHiddenSystemDialog.h
# End Source File
# Begin Source File

SOURCE=.\InspxHiddenXRayDialog.h
# End Source File
# Begin Source File

SOURCE=.\ITIPCDig.h
# End Source File
# Begin Source File

SOURCE=.\KeyBoardTestDialog.h
# End Source File
# Begin Source File

SOURCE=.\ListeningSocket.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\lutLog2_1024_16bit.h
# End Source File
# Begin Source File

SOURCE=.\mapimessages.h
# End Source File
# Begin Source File

SOURCE=.\mapisession.h
# End Source File
# Begin Source File

SOURCE=.\MoreDiagnosticsDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewConfigProductEjectorDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewConfigureProductExternalDetectorDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewConfigureSystemDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewConfigureSystemEjectorDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewConfigureSystemExternalDetectorDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewDiagnosticDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewEditProductDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewImageDisplayDialog.h
# End Source File
# Begin Source File

SOURCE=.\NoticeDialog.h
# End Source File
# Begin Source File

SOURCE=.\NumericEntryDialog.h
# End Source File
# Begin Source File

SOURCE=.\PasswordEntryDialog.h
# End Source File
# Begin Source File

SOURCE=.\ping.h
# End Source File
# Begin Source File

SOURCE=.\PingTestThread.h
# End Source File
# Begin Source File

SOURCE=.\PlotDataDialog.h
# End Source File
# Begin Source File

SOURCE=.\ProcessQuickLearnImagesThread.h
# End Source File
# Begin Source File

SOURCE=.\Product.h
# End Source File
# Begin Source File

SOURCE=.\ProductDensityCompensationDialog.h
# End Source File
# Begin Source File

SOURCE=.\ProductShapeHandlingDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScanTrac.h
# End Source File
# Begin Source File

SOURCE=.\ScanTracConfigureDialog.h
# End Source File
# Begin Source File

SOURCE=.\ScanTracDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectInspectionDialog.h
# End Source File
# Begin Source File

SOURCE=.\SelectItemDialog.h
# End Source File
# Begin Source File

SOURCE=.\SelectMultipleItemDialog.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\SerialPortBase.h
# End Source File
# Begin Source File

SOURCE=.\SetupSystemExternalHardware.h
# End Source File
# Begin Source File

SOURCE=.\shiftSortedArray.h
# End Source File
# Begin Source File

SOURCE=.\SocketMessage1.h
# End Source File
# Begin Source File

SOURCE=.\SpellmanDXMXRaySourcePort.h
# End Source File
# Begin Source File

SOURCE=.\StartCheckDisk.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemConfigurationData.h
# End Source File
# Begin Source File

SOURCE=.\SystemCountersDialog.h
# End Source File
# Begin Source File

SOURCE=.\SystemProductSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\SystemTemperaturesDialog.h
# End Source File
# Begin Source File

SOURCE=.\SystemValuesDialog.h
# End Source File
# Begin Source File

SOURCE=.\TCPIPServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\uControllerStatusDialog.h
# End Source File
# Begin Source File

SOURCE=.\UndoAddToLearnDialog.h
# End Source File
# Begin Source File

SOURCE=.\Win32SerialPortThread.h
# End Source File
# Begin Source File

SOURCE=.\XRaySource.h
# End Source File
# Begin Source File

SOURCE=.\XRaySourceStatusDialog.h
# End Source File
# Begin Source File

SOURCE=.\XRayTubeAlignmentDialog.h
# End Source File
# Begin Source File

SOURCE=.\YesNoDialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\arrowdow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrowlef.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrowrig.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checkmar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\downarro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\green.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Greenck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_tran.ico
# End Source File
# Begin Source File

SOURCE=".\Inspx Large Red Simple.bmp"
# End Source File
# Begin Source File

SOURCE=".\Inspx Small Red Simple.bmp"
# End Source File
# Begin Source File

SOURCE=".\Inspx Tiny Red Simple.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\inspxgra.bmp
# End Source File
# Begin Source File

SOURCE=.\res\leftarro.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Pecobmp div 2 in Blue.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Pecobmp div 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Pecobmp div 4.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\rightarr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ScanTrac.ico
# End Source File
# Begin Source File

SOURCE=.\res\ScanTrac.rc2
# End Source File
# Begin Source File

SOURCE=.\ScanTrac.rgs
# End Source File
# Begin Source File

SOURCE=.\res\smallrig.bmp
# End Source File
# Begin Source File

SOURCE=.\res\uparrow1.bmp
# End Source File
# End Group
# End Target
# End Project
# Section ScanTrac : {F49AC0B2-DF74-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMapiMessages
# 	2:10:HeaderFile:mapimessages.h
# 	2:8:ImplFile:mapimessages.cpp
# End Section
# Section ScanTrac : {F49AC0B0-DF74-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMapiSession
# 	2:10:HeaderFile:mapisession.h
# 	2:8:ImplFile:mapisession.cpp
# End Section
# Section ScanTrac : {20C62CA0-15DA-101B-B9A8-444553540000}
# 	2:21:DefaultSinkHeaderFile:mapisession.h
# 	2:16:DefaultSinkClass:CMapiSession
# End Section
# Section ScanTrac : {20C62CAB-15DA-101B-B9A8-444553540000}
# 	2:21:DefaultSinkHeaderFile:mapimessages.h
# 	2:16:DefaultSinkClass:CMapiMessages
# End Section
