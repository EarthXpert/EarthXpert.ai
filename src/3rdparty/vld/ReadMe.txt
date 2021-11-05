Visual Leak Detector for Visual C++ 2008-2015

Project Description

Visual Leak Detector is a free, robust, open-source memory leak detection system for Visual C++.

It's pretty easy to use. After installing it, you just need to tell Visual C++ where to find the included header and library file.

Then it can be used with any C/C++ project simply by adding the following line to your code:
#include <vld.h>

When you run your program under the Visual Studio debugger, Visual Leak Detector will output a memory leak report at the end of your debugging session. The leak report includes the full call stack showing how any leaked memory blocks were allocated. Double-click on a line in the call stack to jump to that file and line in the editor window.

It's a very effective way to quickly diagnose, and fix, memory leaks in C/C++ applications.

The main difference between the CRT Debug Library and VLD, is that Visual Leak Detector shows you the complete callstack used for memory allocation has led to the leak.

For example:

---------- Block 1199 at 0x04BE1058: 136 bytes ----------
Call Stack:
d:\Foobar\FooLog.cpp (26): FooLog::getInstance
d:\Foobar\FooMain.cpp (75): FooMain::init
f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c (578): __tmainCRTStartup
f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c (403): WinMainCRTStartup
0x759A3677 (File and line number not available): BaseThreadInitThunk
0x770C9D42 (File and line number not available): RtlInitializeExceptionChain
0x770C9D15 (File and line number not available): RtlInitializeExceptionChain
Data:
9C 33 2D 6B    74 2A 2D 6B    C8 11 BE 04    00 00 00 00     .3-kt*-k ........
00 00 00 00    70 14 BB 6C    70 14 BB 6C    00 00 00 00     ....p..l p..l....
00 00 00 00    68 14 BB 6C    68 14 BB 6C    00 00 00 00     ....h..l h..l....
00 00 00 00    6C 14 BB 6C    6C 14 BB 6C    20 12 BE 04     ....l..l l..l....
00 00 00 00    CD 00 CD CD    00 00 00 00    01 CD CD CD     ........ ........
68 14 BB 6C    78 33 2D 6B    00 00 00 00    00 00 00 00     h..lx3-k ........
00 00 00 00    01 02 00 00    06 00 00 00    00 00 00 00     ........ ........
00 00 00 00    00 00 00 00    88 11 BE 04    5C 10 BE 04     ........ ....\...
00 00 00 00    20 CD CD CD                                   ........ ........
This software is provided "AS IS" without warranty of any kind.

The project was originally developed by Dan Moulding, but they are no longer supported. Features of version 2.0 are implemented by Arkadiy Shapkin (me).

Links

Visual Leak Detector Previous Official Homepage http://sites.google.com/site/dmoulding/vld
CodeProject article: Visual Leak Detector - Enhanced Memory Leak Detection for Visual C++
Finding Memory Leaks Using the CRT Library
Visual Leak Detector: Investigate Memory Leaks in Visual C++
