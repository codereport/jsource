/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Macros and Defined Constants for !:                              */


#define XCC             127
#define XC(m,n)         (n+XCC*m)

/* adding suffix _O to avoid possible name conflict with fcntl.h */
#define FAPPEND_O         "ab"
#define FREAD_O           "rb"
#define FUPDATE_O         "r+b"
#define FUPDATEC_O        "w+b"
#define FWRITE_O          "wb"

#ifndef SEEK_SET
#define SEEK_SET        0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR        1
#endif
#ifndef SEEK_END
#define SEEK_END        2
#endif

#ifdef L_tmpnam
#undef L_tmpnam
#endif
#define L_tmpnam        300  /* full path name can be very long */
