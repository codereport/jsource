/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* SYS_ and friends                                                        */

/* Inclusion of a system herein does not necessarily mean that the source  */
/* compiles or works under that system.                                    */

#define SYS_LINUX           262144L         /* GCC                         */
#define SYS_MACOSX          4194304L        /* GCC (CC)                    */

#define SY_LINUX            0    /* any linux intel version                */
#define SY_MAC              0    /* any macosx intel (once included ppc)   */
#define SY_MACPPC           0    /* macosx powerpc                         */

#define SYS_UNIX            (SYS_LINUX + SYS_MACOSX)

#ifdef __linux__
#define SYS SYS_LINUX
#undef SY_LINUX
#define SY_LINUX 1
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

#ifndef SYS     /* must be defined */
 error: "SYS must be defined"
#endif
