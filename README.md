# scanTracSolo

1. (probably don't have to do this, now that we include compiled results in repo...)  Install IFCVersion 5.9 and MVTools 6.4.4.0 from libDependencies by running Setup.exe within each of the folders.

1.5 (probably don't have to do this, now that we include compiled results in repo...)  Download and Install boost 
  - Download boost 1.58: http://sourceforge.net/projects/boost/files/boost/1.58.0/ 
  - Compile boost per http://www.boost.org/doc/libs/1_58_0/more/getting_started/windows.html ( took relatively long for me)

2. Download the MFC MBCS DLL from : https://msdn.microsoft.com/en-us/library/dn251007(v=vs.120).aspx

3. Open solution in Visual Studio, Compile and Run

4. In AFX.H in the beginning add "#define NO_WARN_MBCS_MFC_DEPRECATION" so 
it will not give many warnings about 
Multi Byte Character Set being used instead of Unicode

5. add #define _WIN32_WINNT _WIN32_WINNT_MAXVER to the top of afxwin.h
c:\Program Files\Microsoft Visual Studio 12.0\VC\atlmfc\include

The Following things are done in the Project Properties, so you don't have to do them again unless starting a new project or porting an old project from VS 2010 or earlier

1. On each project, in Solution Explorer, right click on the project and select properties.  Make sure All Configurations is selected at the top.  
Under Configuration Properties, General change Platform Toolset 
to Visual Studio 2103 - Widows XP (v120-xp) so it will run under Windows XP.
2. Under Project, Properties, C++, Code Generation, Runtime Library select /MT for Multi-Threaded
3. Under Project, Properties, Configuration Properties, 
General, Use of MFC to Use MFC in a Static Library
4. Under Project, Properties, Configuration Properties, Linker, Command Line add: /SAFESEH:NO /verbose /NODEFAULTLIB:msvcrt.lib
5. Add "C:\Program Files\Microsoft Visual Studio 12.0\VC\bin" to Properties VC++ Directories, executable.
