/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* SYS_ and friends                                                        */

#ifndef C_NA // noasm/asm
#define C_NA 1
#endif

#undef C_NA
#define C_NA 1   // scaf force noasm

#ifndef SY_GETTOD
#define SY_GETTOD
#endif

/* Inclusion of a system herein does not necessarily mean that the source  */
/* compiles or works under that system.                                    */

#define SYS_AMIGA           1L              /* DICE                        */
#define SYS_ARCHIMEDES      2L
#define SYS_ATARIST         4L              /* GCC                         */
#define SYS_ATT3B1          8L              /* System V C                  */
#define SYS_DEC5500         16L             /* GCC                         */
#define SYS_IBMRS6000       32L             /* AIX XL C                    */
#define SYS_MACINTOSH       64L             /* Think C                     */
#define SYS_MIPS            128L            /* GCC                         */
#define SYS_NEXT            256L            /* GCC                         */
#define SYS_OS2             512L
#define SYS_PC              1024L           /* Turbo C                     */
#define SYS_PCWIN           2048L           /* Watcom C 386                */
#define SYS_PC386           4096L           /* Watcom C 386                */
#define SYS_SGI             8192L           /* GCC                         */
#define SYS_SUN3            16384L          /* GCC                         */
#define SYS_SUN4            32768L          /* GCC                         */
#define SYS_VAX             65536L          /* GCC                         */
#define SYS_HPUX            131072L         /* C89                         */
#define SYS_LINUX           262144L         /* GCC                         */
#define SYS_FREEBSD         524288L         /* x86 only                    */
#define SYS_NETBSD          1048576L        /* GCC                         */
#define SYS_SUNSOL2         2097152L        /* GCC                         */
#define SYS_MACOSX          4194304L        /* GCC (CC)                    */

#define SY_LINUX            0    /* any linux intel version                */
#define SY_MAC              0    /* any macosx intel (once included ppc)   */
#define SY_MACPPC           0    /* macosx powerpc                         */

#define SYS_DOS             (SYS_PC + SYS_PC386 + SYS_PCWIN)

#define SYS_UNIX            (SYS_ATT3B1 + SYS_DEC5500 + SYS_IBMRS6000 + \
                             SYS_MIPS + SYS_NEXT + SYS_SGI + SYS_SUN3 + \
                             SYS_SUN4 + SYS_VAX + SYS_LINUX + SYS_MACOSX + \
                             SYS_FREEBSD + SYS_NETBSD + SYS_SUNSOL2 + SYS_HPUX)

#if defined(__FreeBSD__)
#define SYS SYS_FREEBSD
#endif

#if defined(__NetBSD__)
#define SYS SYS_NETBSD
#endif

#if defined(sparc) && ! defined(__svr4__)
#define SYS SYS_SUN4
#endif

#if defined(sparc) && defined(__svr4__)
#define SYS SYS_SUNSOL2
#endif

#if defined(__sgi__)
#define SYS SYS_SGI
#endif

#if defined (_AIX)
#define SYS SYS_IBMRS6000
#endif

#ifdef __linux__
#define SYS SYS_LINUX
#undef SY_LINUX
#define SY_LINUX 1
#endif

#if defined _PA_RISC1_1
#define SYS SYS_HPUX
#endif

#ifdef __MACH__
#ifdef __ppc__
#ifdef __GNUC__
#define SYS SYS_MACOSX // powerpc
#undef SY_MAC
#define SY_MAC 1
#undef SY_MACPPC
#def SY_MACPPC 1
#endif
#endif
#endif

#ifdef __MACH__
#ifndef __ppc__
#ifdef __GNUC__
#define SYS SYS_MACOSX // intel
#undef SY_MAC
#define SY_MAC 1

#endif
#endif
#endif


// SY_ALIGN 1 for compilers requiring strict alignment
//             e.g. if (I*)av is not allowed for arbitrary av of type C*
#define SY_ALIGN 1 // always use 1 so all use same code; required by nvr stack in parser

#ifndef SYS     /* must be defined */
 error: "SYS must be defined"
#endif

