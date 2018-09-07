﻿#ifndef HGL_PLATFORM_INCLUDE
#define HGL_PLATFORM_INCLUDE

#ifdef _DEBUG
	#define HGL_LIB_DEBUG_NAME	"Debug"
#else
	#define HGL_LIB_DEBUG_NAME	"Release"
#endif//

#define HGL_MERGE32(a,b,c,d)		(a|b<<8|c<<16|d<<24)

#define HGL_OS_Windows				HGL_MERGE32('W','i','n',' ')
#define HGL_OS_macOS				HGL_MERGE32('m','a','c',' ')
#define HGL_OS_iOS					HGL_MERGE32('i','O','S',' ')
#define HGL_OS_FreeBSD				HGL_MERGE32('F','B','S','D')
#define HGL_OS_NetBSD				HGL_MERGE32('N','B','S','D')
#define HGL_OS_OpenBSD				HGL_MERGE32('O','B','S','D')
#define HGL_OS_Linux				HGL_MERGE32('L','i','n','u')
#define HGL_OS_Cygwin				HGL_MERGE32('C','y','g','w')
#define HGL_OS_PSP2					HGL_MERGE32('P','S','P','2')
#define HGL_OS_PS4					HGL_MERGE32('P','S','4',' ')
#define HGL_OS_AIX					HGL_MERGE32('A','I','X',' ')
#define HGL_OS_Android              HGL_MERGE32('A','n','d','r')
#define HGL_OS_Wasm                 HGL_MERGE32('W','A','s','m')        //Web Assembly

#define HGL_CPU_X86_32				HGL_MERGE32('8','6','3','2')
#define HGL_CPU_X86_64				HGL_MERGE32('8','6','6','4')
#define HGL_CPU_MIPS				HGL_MERGE32('M','I','P','S')
#define HGL_CPU_MIPSel				HGL_MERGE32('M','I','e','l')
#define HGL_CPU_MIPS64              HGL_MERGE32('M','I','6','4')
#define HGL_CPU_PowerPC				HGL_MERGE32('P','P','C',' ')
#define HGL_CPU_PowerPC64           HGL_MERGE32('P','P','6','4')
#define HGL_CPU_Cell				HGL_MERGE32('C','E','L','L')
#define HGL_CPU_ARM7                HGL_MERGE32('A','R','M','7')
#define HGL_CPU_ARM64               HGL_MERGE32('A','R','6','4')

#define HGL_COMPILER_Microsoft		HGL_MERGE32('M','S','C',' ')
#define HGL_COMPILER_GNU			HGL_MERGE32('G','N','U',' ')
#define HGL_COMPILER_MINGW32		HGL_MERGE32('M','G','3','2')
#define HGL_COMPILER_Intel			HGL_MERGE32('I','n','t','e')
#define HGL_COMPILER_IBM			HGL_MERGE32('I','B','M',' ')
#define HGL_COMPILER_DigitalMars	HGL_MERGE32('D','i','M','a')
#define HGL_COMPILER_LLVM			HGL_MERGE32('L','L','V','M')
#define HGL_COMPILER_PGI			HGL_MERGE32('P','G','I',' ')
#define HGL_COMPILER_Wasm           HGL_MERGE32('W','A','s','m')        //Web Assembly

#define HGL_BIG_ENDIAN		'B'
#define HGL_LITTLE_ENDIAN	'L'

#if defined(__amd64__)	||defined(__amd64)	||	\
	defined(__x86_64__)	||defined(__x86_64)	||	\
	defined(_M_AMD64)	||defined(_M_X64)
	#define HGL_CPU						HGL_CPU_X86_64
	#define HGL_CPU_NAME				OS_TEXT("X86-64Bit")
	#define HGL_LIB_CPU_NAME			OS_TEXT("x64")
	#define HGL_MIN_MEMORY_ALLOC_BYTES	8
	#define HGL_ENDIAN					HGL_LITTLE_ENDIAN
#elif defined(i386) || defined(__i386__) || defined(__i386) ||defined(_M_IX86)
	#define HGL_CPU						HGL_CPU_X86_32
	#define HGL_CPU_NAME				OS_TEXT("X86-32Bit")
	#define HGL_LIB_CPU_NAME			OS_TEXT("x86")
	#define HGL_MIN_MEMORY_ALLOC_BYTES	4
	#define HGL_ENDIAN					HGL_LITTLE_ENDIAN
#elif defined(_M_MIPS)||defined(_MIPS_ARCH)||defined(__mips__)||defined(__mips)||defined(mips)||defined(__MIPS__)
	#define HGL_MIN_MEMORY_ALLOC_BYTES	4
	#ifdef __MIPSEL__
		#define	HGL_CPU						HGL_CPU_MIPSel
		#define HGL_CPU_NAME				OS_TEXT("MIPSel")
		#define HGL_LIB_CPU_NAME			OS_TEXT("MIPSel")
		#define HGL_ENDIAN					HGL_LITTLE_ENDIAN
	#else
		#define	HGL_CPU						HGL_CPU_MIPS
		#define HGL_CPU_NAME				OS_TEXT("MIPS")
		#define HGL_LIB_CPU_NAME			OS_TEXT("MIPS")
		#define HGL_ENDIAN					HGL_BIG_ENDIAN
	#endif//__MIPSEL__
#elif defined(__mips64__)||defined(mips64)
        #define HGL_MIN_MEMORY_ALLOC_BYTES  8
        #define HGL_CPU                     HGL_CPU_MIPS64
        #define HGL_CPU_NAME                OS_TEXT("MIPS64")
        #define HGL_LIB_CPU_NAME            OS_TEXT("MIPS64")
        #define HGL_ENDIAN                  HGL_BIG_ENDIAN
#elif defined(__powerpc__) || defined(__powerpc)    \
     ||defined(__POWERPC__) \
     || defined(__ppc__) || defined(__PPC__) || defined(__ppc) \
     || defined(_M_PPC) ||defined(_M_MPPC) \
     || defined(_ARCH_PPC) || defined(_ARCH_PWR)
	#ifdef __PPU__
		#define HGL_CPU				HGL_CPU_Cell
		#define HGL_CPU_NAME		OS_TEXT("Cell")
		#define HGL_LIB_CPU_NAME	OS_TEXT("Cell")
	#else
        #ifdef defined(__ppc64__)||defined(__PPC64__)||defined(__powerpc64__)   \
             ||defined(_ARCH_PPC64)||defined(__64BIT__)
            #define HGL_CPU             HGL_CPU_PowerPC64
            #define HGL_CPU_NAME        OS_TEXT("PowerPC64")
            #define HGL_LIB_CPU_NAME    OS_TEXT("PowerPC64")
        #else
            #define HGL_CPU				HGL_CPU_PowerPC
            #define HGL_CPU_NAME		OS_TEXT("PowerPC")
            #define HGL_LIB_CPU_NAME	OS_TEXT("PowerPC")
        #endif//PowerPC64
	#endif

	#define HGL_MIN_MEMORY_ALLOC_BYTES	8
	#define HGL_ENDIAN					HGL_BIG_ENDIAN
#elif defined(__arm__)||defined(__arm)  \
    ||defined(_ARM) \
    ||defined(_M_ARM)  \
    ||defined(__ARM_ARCH__7__)  \
    ||defined(__ARM_ARCH__7A__) \
    ||defined(__ARM_ARCH__7R__) \
    ||defined(__ARM_ARCH__7M__) \
    ||defined(__ARM_ARCH__7S__)
        #define	HGL_CPU						HGL_CPU_ARM7
        #define HGL_CPU_NAME				OS_TEXT("ARM7")
        #define HGL_LIB_CPU_NAME			OS_TEXT("ARM7")
        #define HGL_MIN_MEMORY_ALLOC_BYTES	4
        #define HGL_ENDIAN					HGL_LITTLE_ENDIAN
#elif defined(__aarch64__)
    #define	HGL_CPU						HGL_CPU_ARM64
    #define HGL_CPU_NAME				OS_TEXT("ARM64")
    #define HGL_LIB_CPU_NAME			OS_TEXT("ARM64")
    #define HGL_MIN_MEMORY_ALLOC_BYTES	4
    #define HGL_ENDIAN					HGL_LITTLE_ENDIAN
#elif defined(__wasm__)

    #error can't support WebAssembly.please wait update......

    #define HGL_OS          HGL_OS_Wasm
    #define HGL_COMPILER    HGL_COMPILER_Wasm
#else
	#error Can't not support the cpu.
#endif

#if HGL_ENDIAN == HGL_BIG_ENDIAN
	#define HGL_CPU_ENDIAN		OS_TEXT("Big Endian")
#else
	#define HGL_CPU_ENDIAN		OS_TEXT("Little Endian")
#endif//HGL_ENDIAN == HGL_BIG_ENDIAN

#if defined(__WIN32__)||defined(_WIN32)||defined(WIN32)||defined(__WINDOWS__)||defined(__WIN64__)||defined(_WIN64)||defined(WIN64)
	#define HGL_OS 			HGL_OS_Windows
#elif defined(__APPLE__)||defined(__MAC__)||defined(macintosh)||defined(__APPLE_CC__)
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE
//		#if TARGET_IPHONE_SIMULATOR
//			#define HGL_OS	HGL_OS_iOS_Simulator
//		#else
			#define HGL_OS	HGL_OS_iOS
//		#endif//TARGET_IPHONE_SIMULATOR
	#elif TARGET_OS_MAC
		#define HGL_OS		HGL_OS_macOS
	#endif//
#elif defined(__ANDROID__)
    #define HGL_OS          HGL_OS_Android
#elif defined(__FreeBSD)||defined(__FreeBSD__)
	#define HGL_OS 			HGL_OS_FreeBSD
#elif defined(__NetBSD)||defined(__NetBSD__)
	#define HGL_OS 			HGL_OS_NetBSD
#elif defined(__OPENBSD)||defined(__OpenBSD__)
	#define HGL_OS 			HGL_OS_OpenBSD
#elif defined(__linux__)||defined(__LINUX__)||defined(linux)||defined(__linux)
	#define HGL_OS			HGL_OS_Linux
#elif defined(__CYGWIN__)
	#define HGL_OS			HGL_OS_Cygwin
#elif defined(SN_TARGET_PSP2)||defined(__PSV__)||defined(__psv__)||defined(_PSV)||defined(__PSVita__)||defined(__PSVita)
	#define HGL_OS			HGL_OS_PSP2
#elif defined(SN_TARGET_ORBIS)
	#define HGL_OS			HGL_OS_PS4
#elif defined(_AIX)||defined(__AIX)||defined(__AIX__)||defined(__aix)||defined(__aix__)
	#define HGL_OS			HGL_OS_AIX
#endif//

#if defined(__clang__)
	#define HGL_COMPILER	HGL_COMPILER_LLVM
#elif defined(__INTEL_COMPILER)||defined(__ECL)||defined(__ICL)||defined(__ICC)
	#define HGL_COMPILER	HGL_COMPILER_Intel
#elif defined(__IBMC__)||defined(__IBMCPP__)
	#define HGL_COMPILER	HGL_COMPILER_IBM
#elif defined(__DMC__)||defined(__SC__)||defined(__ZTC__)
	#define HGL_COMPILER	HGL_COMPILER_DigitalMars
#elif defined(_MSC_VER)
	#define HGL_COMPILER	HGL_COMPILER_Microsoft
#elif defined(__PGI)
	#define HGL_COMPILER	HGL_COMPILER_PGI
#elif defined(__MINGW32__)
	#define HGL_COMPILER	HGL_COMPILER_MINGW32
#elif defined(__GNUC__)
	#define HGL_COMPILER	HGL_COMPILER_GNU
#else
	#error Can not support the Compiler.
#endif//

#if defined(SetEventCall)||defined(SafeCallEvent)||defined(CallEvent)||defined(DefEvent)
	#error SetEventCall,SafeCallEvent,CallEvent,DefEvent 其中之一已经定义
#endif//

#if defined(Property)||defined(PropertyRead)||defined(PropertyWrite)
	#error Property,PropertyRead,PropertyWrite 其中之一已经定义
#endif//

// #ifndef UNICODE
// 	#define UNICODE                     //定义使用UNICODE编码
// #endif//UNICODE

#if HGL_OS == HGL_OS_Windows

	#include<hgl/platform/os/MSWindows.h>

	#if HGL_COMPILER == HGL_COMPILER_Microsoft
		#include<hgl/platform/compiler/Microsoft.h>
	#elif HGL_COMPILER == HGL_COMPILER_Intel
		#include<hgl/platform/compiler/Intel.h>
	#elif HGL_COMPILER == HGL_COMPILER_IBM
		#include<hgl/platform/compiler/IBM.h>
	#elif HGL_COMPILER == HGL_COMPILER_MINGW32
		#include<hgl/platform/compiler/GNU.h>
	#elif HGL_COMPILER == HGL_COMPILER_GNU
		#include<hgl/platform/compiler/GNU.h>
	#else
		#error Unrecognized compiler
	#endif

#elif HGL_OS == HGL_OS_Cygwin

	#include<hgl/platform/os/Cygwin.h>

	#if HGL_COMPILER == HGL_COMPILER_LLVM
		#include<hgl/platform/compiler/LLVM.h>
	#elif HGL_COMPILER == HGL_COMPILER_GNU
		#include<hgl/platform/compiler/GNU.h>
	#else
		#error Unrecognized compiler
	#endif//GNU

#elif (HGL_OS == HGL_OS_FreeBSD)||(HGL_OS == HGL_OS_NetBSD)||(HGL_OS == HGL_OS_OpenBSD)

	#include<hgl/platform/os/BSD.h>

	#if HGL_COMPILER == HGL_COMPILER_LLVM
		#include<hgl/platform/compiler/LLVM.h>
	#elif HGL_COMPILER == HGL_COMPILER_GNU
		#include<hgl/platform/compiler/GNU.h>
	#else
		#error Unrecognized compiler
	#endif//GNU

#elif HGL_OS == HGL_OS_Linux

	#include<hgl/platform/os/Linux.h>

	#if HGL_COMPILER == HGL_COMPILER_CBuilder
		#include<hgl/platform/compiler/CBuilder.h>
	#elif HGL_COMPILER == HGL_COMPILER_Intel
		#include<hgl/platform/compiler/Intel.h>
	#elif HGL_COMPILER == HGL_COMPILER_IBM
		#include<hgl/platform/compiler/IBM.h>
	#elif HGL_COMPILER == HGL_COMPILER_Watcom
		#include<hgl/platform/compiler/Watcom.h>
	#elif HGL_COMPILER == HGL_COMPILER_LLVM
		#include<hgl/platform/compiler/LLVM.h>
	#elif HGL_COMPILER == HGL_COMPILER_GNU
		#include<hgl/platform/compiler/GNU.h>
	#else
		#error Unrecognized compiler
	#endif

#elif HGL_OS == HGL_OS_macOS

	#include<hgl/platform/os/MacOS.h>

	#if HGL_COMPILER == HGL_COMPILER_Intel
		#include<hgl/platform/compiler/Intel.h>
	#elif HGL_COMPILER == HGL_COMPILER_LLVM
		#include<hgl/platform/compiler/LLVM.h>
	#else
		#error Unrecognized compiler
	#endif

#elif HGL_OS == HGL_OS_iOS

	#include<hgl/platform/os/MacOS.h>
	#include<hgl/platform/compiler/LLVM.h>

#elif HGL_OS == HGL_OS_PSP2

	#include<hgl/platform/os/psp2.h>

	#if HGL_COMPILER == HGL_COMPILER_GNU
		#include<hgl/platform/compiler/GNU.h>
	#else
		#error Unrecognized compiler
	#endif

#elif HGL_OS == HGL_OS_AIX

	#include<hgl/platform/os/aix.h>

	#if HGL_COMPILER == HGL_COMPILER_IBM
		#include<hgl/platform/compiler/IBM.h>
	#elif HGL_COMPILER == HGL_COMPILER_SGI
		#include<hgl/platform/compiler/SGI.h>
	#elif HGL_COMPILER == HGL_COMPILER_GNU
		#include<hgl/platform/compiler/GNU.h>
	#else
		#error Unrecognized compiler
	#endif

#elif HGL_OS == HGL_OS_Android

	#include<hgl/platform/os/Android.h>

	#if HGL_COMPILER == HGL_COMPILER_LLVM
		#include<hgl/platform/compiler/LLVM.h>
	#elif HGL_COMPILER == HGL_COMPILER_GNU
		#include<hgl/platform/compiler/GNU.h>
	#else
		#error Unrecognized compiler
	#endif//GNU
#endif//HGL_OS

namespace hgl
{
    #define HGL_WINDOW_DIRECTORY_SEPARATOR  OS_TEXT('\\')
    #define HGL_UNIX_DIRECTORY_SPEARATOR    OS_TEXT('/')

    #define enum_int(name)	enum name:int
    #define enum_uint(name)	enum name:uint

	using void_pointer=void *;
}//namespace hgl

using uchar     = unsigned char;    ///< 无符号字符型
using ushort    = unsigned short;   ///< 无符号短整型
using uint      = unsigned int;     ///< 无符号整型
using ulong     = unsigned long;    ///< 无符号长整型

#endif//HGL_PLATFORM_INCLUDE
