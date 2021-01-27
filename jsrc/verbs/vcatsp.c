/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: ,. on Sparse Arguments of rank 2 or less                         */

#include "j.h"


F2(jtstitchsp2){I ar,wr;
 ar=AR(a); wr=AR(w);
 return irs2(a,w,0L,ar?ar-1:0,wr?wr-1:0,jtover);
}    /* sparse arrays with rank 2 or less */
