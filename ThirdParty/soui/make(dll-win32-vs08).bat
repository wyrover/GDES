rem DLL_SOUI 指示为SOUI产生DLL版本。
rem USING_MT 指示整个项目使用MT方式连接CRT
rem CAN_DEBUG 指示为生成的Release版本生产调试符号
call "%VS90COMNTOOLS%..\..\VC\vcvarsall.bat" x86
rem tools\qmake -tp vc -r -spec .\tools\mkspecs\win32-msvc2008 "CONFIG += USING_MT CAN_DEBUG"
tools\qmake -tp vc -r -spec .\tools\mkspecs\win32-msvc2008 "CONFIG += CAN_DEBUG"
rem soui.sln