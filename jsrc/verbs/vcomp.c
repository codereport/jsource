/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Comparatives                                                     */

#include "j.h"
#include "ve.h"
#include "vcomp.h"

#define CMPGT(a,b) ((a)>(b)?1:0)
#define CMPLT(a,b) ((a)<(b)?1:0)
#define CMPGE(a,b) ((a)>=(b)?1:0)
#define CMPLE(a,b) ((a)<=(b)?1:0)
#define EQTEMPLATE APFX
#define NETEMPLATE APFX
#define CMPEQ(a,b) ((a)==(b)?1:0)
#define CMPNE(a,b) ((a)!=(b)?1:0)

D jttfloor(J jt,D v){D x; R (x=jround(v), x-TGT(x,v));}
D jttceil (J jt,D v){D x; R (x=jround(v), x+TLT(x,v));}

BPFX(eqBB, EQ,BEQ,EQ,BEQ, _mm256_xor_pd(bool256,_mm256_xor_pd(u256,v256)) , ,__m256d bool256=_mm256_castsi256_pd(_mm256_set1_epi64x(0x0101010101010101));)
BPFX(neBB, NE,BNE,NE,BNE, _mm256_xor_pd(u256,v256) , ,)
BPFX(ltBB, LT,BLT,GT,BGT, _mm256_andnot_pd(u256,v256) , ,)
BPFX(leBB, LE,BLE,GE,BGE, _mm256_xor_pd(bool256,_mm256_andnot_pd(v256,u256)) , ,__m256d bool256=_mm256_castsi256_pd(_mm256_set1_epi64x(0x0101010101010101));)
BPFX(geBB, GE,BGE,LE,BLE, _mm256_xor_pd(bool256,_mm256_andnot_pd(u256,v256)) , ,__m256d bool256=_mm256_castsi256_pd(_mm256_set1_epi64x(0x0101010101010101));)
BPFX(gtBB, GT,BGT,LT,BLT, _mm256_andnot_pd(v256,u256) , , )
 

                          EQTEMPLATE(eqBI, B,B,I, CMPEQ,, R EVOK;)  ACMP0(eqBD, B,B,D, TEQ, ==  )
EQTEMPLATE(eqIB, B,I,B, CMPEQ,, R EVOK;)    ACMP0(eqID, B,I,D, TEQ, ==  )
ACMP0(eqDB, B,D,B, TEQ, ==  )  ACMP0(eqDI, B,D,I, TEQ, ==  )  
APFX(eqZZ, B,Z,Z, zeq,, R EVOK;)   
APFX(eqXX, B,X,X, equ,, R EVOK;)   
APFX(eqQQ, B,Q,Q, QEQ,, R EVOK;)
BPFXAVX2(eqCC, CMPEQCC,x, CMPEQCC, x,
   (workarea=_mm256_castpd_si256(_mm256_xor_pd(u256,v256)), _mm256_castsi256_pd(_mm256_and_si256(_mm256_srli_epi64(_mm256_andnot_si256(workarea,_mm256_sub_epi8(workarea,bool256)),7),bool256))) ,
   I work; , __m256i workarea; __m256i bool256=_mm256_set1_epi64x(0x0101010101010101);
   )

                           EQTEMPLATE(eqCS, B,UC,US, CMPEQ,, R EVOK;)  EQTEMPLATE(eqSC, B,US,UC, CMPEQ,, R EVOK;)  EQTEMPLATE(eqSS, B,S,S, CMPEQ,, R EVOK;)
EQTEMPLATE(eqUU, B,C4,C4, CMPEQ,, R EVOK;)  EQTEMPLATE(eqUS, B,C4,US, CMPEQ,, R EVOK;)  EQTEMPLATE(eqSU, B,US,C4, CMPEQ,, R EVOK;)
EQTEMPLATE(eqCU, B,UC,C4, CMPEQ,, R EVOK;)  EQTEMPLATE(eqUC, B,C4,UC, CMPEQ,, R EVOK;)
APFX(eqAA, B,A,A, equ,, R EVOK;)

                          NETEMPLATE(neBI, B,B,I, CMPNE,, R EVOK;)  ACMP0(neBD, B,B,D, TNE, != )
NETEMPLATE(neIB, B,I,B, CMPNE,, R EVOK;)    ACMP0(neID, B,I,D, TNE, != )
ACMP0(neDB, B,D,B, TNE, != )  ACMP0(neDI, B,D,I, TNE, != )  
APFX(neZZ, B,Z,Z, !zeq,, R EVOK;)  
APFX(neXX, B,X,X, !equ,, R EVOK;)
APFX(neQQ, B,Q,Q, !QEQ,, R EVOK;)
BPFXAVX2(neCC, CMPNECC,x, CMPNECC, x,
   (workarea=_mm256_castpd_si256(_mm256_xor_pd(u256,v256)), _mm256_castsi256_pd(_mm256_andnot_si256(_mm256_srli_epi64(_mm256_andnot_si256(workarea,_mm256_sub_epi8(workarea,bool256)),7),bool256))) ,
   I work; , __m256i workarea; __m256i bool256=_mm256_set1_epi64x(0x0101010101010101);
   )
                           NETEMPLATE(neCS, B,UC,US, CMPNE,, R EVOK;)  NETEMPLATE(neSC, B,US,UC, CMPNE,, R EVOK;)  NETEMPLATE(neSS, B,S,S, CMPNE,, R EVOK;)
NETEMPLATE(neUU, B,C4,C4, CMPNE,, R EVOK;)  NETEMPLATE(neUS, B,C4,US, CMPNE,, R EVOK;)  NETEMPLATE(neSU, B,US,C4, CMPNE,, R EVOK;)
NETEMPLATE(neCU, B,UC,C4, CMPNE,, R EVOK;)  NETEMPLATE(neUC, B,C4,UC, CMPNE,, R EVOK;)
APFX(neAA, B,A,A, !equ,, R EVOK;)

                          APFX(ltBI, B,B,I, CMPLT,, R EVOK;)  ACMP0(ltBD, B,B,D, TLT, <  )
APFX(ltIB, B,I,B, CMPLT,, R EVOK;)    ACMP0(ltID, B,I,D, TLT, <  )
ACMP0(ltDB, B,D,B, TLT, <  )  ACMP0(ltDI, B,D,I, TLT, <  )  
APFX(ltXX, B,X,X, -1==xcompare,, R EVOK;)
APFX(ltQQ, B,Q,Q, QLT,, R EVOK;)
APFX(ltSS, B,SB,SB, SBLT,, R EVOK;)

                          APFX(leBI, B,B,I, CMPLE,, R EVOK;)  ACMP0(leBD, B,B,D, TLE, <=  )
APFX(leIB, B,I,B, CMPLE,, R EVOK;)   ACMP0(leID, B,I,D, TLE, <=  )
ACMP0(leDB, B,D,B, TLE, <=  )  ACMP0(leDI, B,D,I, TLE, <=  )  
APFX(leXX, B,X,X,  1!=xcompare,, R EVOK;)
APFX(leQQ, B,Q,Q, QLE,, R EVOK;)
APFX(leSS, B,SB,SB, SBLE,, R EVOK;)

                          APFX(geBI, B,B,I, CMPGE,, R EVOK;)  ACMP0(geBD, B,B,D, TGE, >=  )
APFX(geIB, B,I,B, CMPGE,, R EVOK;)    ACMP0(geID, B,I,D, TGE, >=  )
ACMP0(geDB, B,D,B, TGE, >=  )  ACMP0(geDI, B,D,I, TGE, >=  )  
APFX(geXX, B,X,X, -1!=xcompare,, R EVOK;)
APFX(geQQ, B,Q,Q, QGE,, R EVOK;)
APFX(geSS, B,SB,SB, SBGE,, R EVOK;)

                           APFX(gtBI, B,B,I, CMPGT,, R EVOK;)  ACMP0(gtBD, B,B,D, TGT, >  )
APFX(gtIB, B,I,B, CMPGT,, R EVOK;)     ACMP0(gtID, B,I,D, TGT, >  )
ACMP0(gtDB, B,D,B, TGT, >  )   ACMP0(gtDI, B,D,I, TGT, > )  
APFX(gtXX, B,X,X,  1==xcompare,, R EVOK;)
APFX(gtQQ, B,Q,Q, QGT,, R EVOK;)
APFX(gtSS, B,SB,SB, SBGT,, R EVOK;)


ACMP0(geDD, B,D,D, TGE, >=  )
ACMP0(gtDD, B,D,D, TGT, >  )
ACMP0(leDD, B,D,D, TLE, <=  )
ACMP0(ltDD, B,D,D, TLT, <  )
ACMP0(neDD, B,D,D, TNE, != )
ACMP0(eqDD, B,D,D, TEQ, ==  )

APFX(gtII, B,I,I, CMPGT,,R EVOK;)
APFX(geII, B,I,I, CMPGE,,R EVOK;)
APFX(leII, B,I,I, CMPLE,,R EVOK;)
APFX(ltII, B,I,I, CMPLT,,R EVOK;)
NETEMPLATE(neII, B,I,I, CMPNE,,R EVOK;)
EQTEMPLATE(eqII, B,I,I, CMPEQ,,R EVOK;)
