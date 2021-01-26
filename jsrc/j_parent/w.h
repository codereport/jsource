/* Copyright 1990-2001, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Words                                                                   */

#define CBBLOCK        1   /* control word types */
#define CTBLOCK        2
#define CDO            3
#define CIF            4
#define CELSE          5
#define CEND           6
#define CWHILE         7
#define CWHILST        8
#define CELSEIF        9
#define CTRY           10
#define CCATCH         11
#define CBREAK         12
#define CCONT          13
#define CLABEL         14
#define CGOTO          15
#define CRETURN        16
#define CFOR           17
#define CDOF           18
#define CBREAKF        19
#define CSELECT        20
#define CCASE          21
#define CFCASE         22
#define CDOSEL         23
#define CENDSEL        24
#define CASSERT        25
#define CTHROW         26
#define CCATCHD        27
#define CCATCHT        28
#define CSELECTN       29
#define CBREAKS        30   // break. with select. as containing looping struct
#define CCONTS         31   // continue. with select. as containing looping struct
// following codes cannot be tested by a shift; they alias to lower values
#define CBBLOCKEND     (32+CBBLOCK)  // B followed by end. that falls through to NSI without other action