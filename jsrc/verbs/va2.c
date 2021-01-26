/* Copyright 1990-2011, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Atomic (Scalar) Dyadic                                           */

#include "j.h"
#include "ve.h"
#include "vcomp.h"

// reduce/prefix/suffix routines
// first word is the maximum valid precision bit index, followed by that many+1 routines for reduce, and then for prefix and suffix.
// the last routine is always 0 to indicate invalid
// if there are integer-overflow routine, they comes after the others, in the order rps
VARPSA rpsnull = {0, {0}};

static VARPSA rpsbw0000 = {INTX+1 , { {(VARPSF)bw0000insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0000insI,VCVTIP+VI},
                             {(VARPSF)bw0000pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw0000pfxI,VCVTIP+VI},
                             {(VARPSF)bw0000sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0000sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw0001 = {INTX+1 , { {(VARPSF)bw0001insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0001insI,VCVTIP+VI},
                             {(VARPSF)bw0001pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw0001pfxI,VCVTIP+VI},
                             {(VARPSF)bw0001sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0001sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw0010 = {INTX+1 , { {(VARPSF)bw0010insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0010insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw0010sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0010sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw0011 = {INTX+1 , { {(VARPSF)bw0011insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0011insI,VCVTIP+VI},
                             {(VARPSF)bw0011pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw0011pfxI,VCVTIP+VI},
                             {(VARPSF)bw0011sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0011sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw0100 = {INTX+1 , { {(VARPSF)bw0100insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0100insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw0100sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0100sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw0101 = {INTX+1 , { {(VARPSF)bw0101insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0101insI,VCVTIP+VI},
                             {(VARPSF)bw0101pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw0101pfxI,VCVTIP+VI},
                             {(VARPSF)bw0101sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0101sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw0110 = {INTX+1 , { {(VARPSF)bw0110insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0110insI,VCVTIP+VI},
                             {(VARPSF)bw0110pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw0110pfxI,VCVTIP+VI},
                             {(VARPSF)bw0110sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0110sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw0111 = {INTX+1 , { {(VARPSF)bw0111insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0111insI,VCVTIP+VI},
                             {(VARPSF)bw0111pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw0111pfxI,VCVTIP+VI},
                             {(VARPSF)bw0111sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw0111sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1000 = {INTX+1 , { {(VARPSF)bw1000insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1000insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw1000sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1000sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1001 = {INTX+1 , { {(VARPSF)bw1001insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1001insI,VCVTIP+VI},
                             {(VARPSF)bw1001pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw1001pfxI,VCVTIP+VI},
                             {(VARPSF)bw1001sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1001sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1010 = {INTX+1 , { {(VARPSF)bw1010insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1010insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw1010sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1010sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1011 = {INTX+1 , { {(VARPSF)bw1011insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1011insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw1011sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1011sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1100 = {INTX+1 , { {(VARPSF)bw1100insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1100insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw1100sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1100sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1101 = {INTX+1 , { {(VARPSF)bw1101insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1101insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw1101sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1101sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1110 = {INTX+1 , { {(VARPSF)bw1110insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1110insI,VCVTIP+VI},
                             {0,0}, {0,0}, {0,0},
                             {(VARPSF)bw1110sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1110sfxI,VCVTIP+VI},
                             }};
static VARPSA rpsbw1111 = {INTX+1 , { {(VARPSF)bw1111insI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1111insI,VCVTIP+VI},
                             {(VARPSF)bw1111pfxI,VCVTIP+VI+VII}, {0,0}, {(VARPSF)bw1111pfxI,VCVTIP+VI},
                             {(VARPSF)bw1111sfxI,VCVTIP+VI+VII}, {0}, {(VARPSF)bw1111sfxI,VCVTIP+VI},
                             }};

static VARPSA rpsne = {B01X+1 , { {(VARPSF)neinsB,VCVTIP+VB},
                         {(VARPSF)nepfxB,VCVTIP+VB},
                         {(VARPSF)nesfxB,VCVTIP+VB},
                         }};
static VARPSA rpsnor = {B01X+1 , { {(VARPSF)norinsB,VCVTIP+VB},
                         {(VARPSF)norpfxB,VCVTIP+VB},
                         {(VARPSF)norsfxB,VCVTIP+VB},
                         }};
static VARPSA rpsor = {B01X+1 , { {(VARPSF)orinsB,VCVTIP+VB},
                         {(VARPSF)orpfxB,VCVTIP+VB},
                         {(VARPSF)orsfxB,VCVTIP+VB},
                         }};
static VARPSA rpseq = {B01X+1 , { {(VARPSF)eqinsB,VCVTIP+VB},
                         {(VARPSF)eqpfxB,VCVTIP+VB},
                         {(VARPSF)eqsfxB,VCVTIP+VB},
                         }};
static VARPSA rpsand = {B01X+1 , { {(VARPSF)andinsB,VCVTIP+VB},
                         {(VARPSF)andpfxB,VCVTIP+VB},
                         {(VARPSF)andsfxB,VCVTIP+VB},
                         }};
static VARPSA rpsnand = {B01X+1 , { {(VARPSF)nandinsB,VCVTIP+VB},
                         {(VARPSF)nandpfxB,VCVTIP+VB},
                         {(VARPSF)nandsfxB,VCVTIP+VB},
                         }};
static VARPSA rpsge = {B01X+1 , { {(VARPSF)geinsB,VCVTIP+VB},
                         {(VARPSF)gepfxB,VCVTIP+VB},
                         {(VARPSF)gesfxB,VCVTIP+VB},
                         }};
static VARPSA rpsle = {B01X+1 , { {(VARPSF)leinsB,VCVTIP+VB},
                         {(VARPSF)lepfxB,VCVTIP+VB},
                         {(VARPSF)lesfxB,VCVTIP+VB},
                         }};
static VARPSA rpsgt = {B01X+1 , { {(VARPSF)gtinsB,VCVTIP+VB},
                         {(VARPSF)gtpfxB,VCVTIP+VB},
                         {(VARPSF)gtsfxB,VCVTIP+VB},
                         }};
static VARPSA rpslt = {B01X+1 , { {(VARPSF)ltinsB,VCVTIP+VB},
                         {(VARPSF)ltpfxB,VCVTIP+VB},
                         {(VARPSF)ltsfxB,VCVTIP+VB},
                         }};

static VARPSA rpsdiv = {CMPXX+1 , { {(VARPSF)divinsD,VCVTIP+VD+VDD}, {0}, {(VARPSF)divinsD,VCVTIP+VD+VDD}, {(VARPSF)divinsD,VCVTIP+VD}, {(VARPSF)divinsZ,VCVTIP+VZ},
                          {(VARPSF)divpfxD,VCVTIP+VD+VDD}, {0}, {(VARPSF)divpfxD,VCVTIP+VD+VDD}, {(VARPSF)divpfxD,VCVTIP+VD}, {(VARPSF)divpfxZ,VCVTIP+VZ},
                          {(VARPSF)divsfxD,VCVTIP+VD+VDD}, {0}, {(VARPSF)divsfxD,VCVTIP+VD+VDD}, {(VARPSF)divsfxD,VCVTIP+VD}, {(VARPSF)divsfxZ,VCVTIP+VZ},
                         }};

static VARPSA rpsminus = {RATX+1 , {
{(VARPSF)minusinsB,VCVTIP+VI}, {0}, {(VARPSF)minusinsI,VCVTIP+VI}, {(VARPSF)minusinsD,VCVTIP+VD}, {(VARPSF)minusinsZ,VCVTIP+VZ},        {0}, {0}, {0},
{(VARPSF)minuspfxB,VCVTIP+VI}, {0}, {(VARPSF)minuspfxI,VCVTIP+VI}, {(VARPSF)minuspfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)minuspfxZ,VCVTIP+VZ}, {0}, {(VARPSF)minuspfxX,VCVTIP+VX}, {(VARPSF)minuspfxQ,VCVTIP+VQ},
{(VARPSF)minussfxB,VCVTIP+VI}, {0}, {(VARPSF)minussfxI,VCVTIP+VI}, {(VARPSF)minussfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)minussfxZ,VCVTIP+VZ},  {0}, {0}, {0},
{(VARPSF)minusinsO,VCVTIP+VD},{(VARPSF)minuspfxO,VCVTIP+VD},{(VARPSF)minussfxO,VCVTIP+VD},  // integer-overflow routines
}};
static VARPSA rpsplus = {RATX+1 , {
{(VARPSF)plusinsB,VCVTIP+VI}, {0}, {(VARPSF)plusinsI,VCVTIP+VI}, {(VARPSF)plusinsD,VCVTIP+VD}, {(VARPSF)plusinsZ,VCVTIP+VZ},        {0}, {0}, {0},
{(VARPSF)pluspfxB,VCVTIP+VI}, {0}, {(VARPSF)pluspfxI,VCVTIP+VI}, {(VARPSF)pluspfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)pluspfxZ,VCVTIP+VZ}, {0}, {(VARPSF)pluspfxX,VCVTIP+VX}, {(VARPSF)pluspfxQ,VCVTIP+VQ},
{(VARPSF)plussfxB,VCVTIP+VI}, {0}, {(VARPSF)plussfxI,VCVTIP+VI}, {(VARPSF)plussfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)plussfxZ,VCVTIP+VZ}, {0}, {(VARPSF)plussfxX,VCVTIP+VX}, {(VARPSF)plussfxQ,VCVTIP+VQ},
{(VARPSF)plusinsO,VCVTIP+VD},{(VARPSF)pluspfxO,VCVTIP+VD},{(VARPSF)plussfxO,VCVTIP+VD},  // integer-overflow routines
}};
static VARPSA rpstymes = {RATX+1 , {
{(VARPSF)andinsB,VCVTIP+VB}, {0}, {(VARPSF)tymesinsI,VCVTIP+VI}, {(VARPSF)tymesinsD,VCVTIP+VD}, {(VARPSF)tymesinsZ,VCVTIP+VZ},        {0}, {0}, {0},
{(VARPSF)andpfxB,VCVTIP+VB}, {0}, {(VARPSF)tymespfxI,VCVTIP+VI}, {(VARPSF)tymespfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)tymespfxZ,VCVTIP+VZ}, {0}, {(VARPSF)tymespfxX,VCVTIP+VX}, {(VARPSF)tymespfxQ,VCVTIP+VQ},
{(VARPSF)andsfxB,VCVTIP+VB}, {0}, {(VARPSF)tymessfxI,VCVTIP+VI}, {(VARPSF)tymessfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)tymessfxZ,VCVTIP+VZ}, {0}, {(VARPSF)tymessfxX,VCVTIP+VX}, {(VARPSF)tymessfxQ,VCVTIP+VQ},
{(VARPSF)tymesinsO,VCVTIP+VD},{(VARPSF)tymespfxO,VCVTIP+VD},{(VARPSF)tymessfxO,VCVTIP+VD},  // integer-overflow routines
}};

static VARPSA rpsmin = {SBTX+1 , {
{(VARPSF)andinsB,VCVTIP+VB}, {0}, {(VARPSF)mininsI,VCVTIP+VI}, {(VARPSF)mininsD,VCVTIP+VD}, {(VARPSF)mininsD,VCVTIP+VD+VDD}, {0}, {(VARPSF)mininsX,VCVTIP+VX}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {(VARPSF)mininsS,VCVTIP+VSB},
{(VARPSF)andpfxB,VCVTIP+VB}, {0}, {(VARPSF)minpfxI,VCVTIP+VI+VIPOKW}, {(VARPSF)minpfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)minpfxD,VCVTIP+VD+VDD}, {0}, {(VARPSF)minpfxX,VCVTIP+VX}, {(VARPSF)minpfxQ,VCVTIP+VQ}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {(VARPSF)minpfxS,VCVTIP+VSB},
{(VARPSF)andsfxB,VCVTIP+VB}, {0}, {(VARPSF)minsfxI,VCVTIP+VI+VIPOKW}, {(VARPSF)minsfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)minsfxD,VCVTIP+VD+VDD}, {0}, {(VARPSF)minsfxX,VCVTIP+VX}, {(VARPSF)minsfxQ,VCVTIP+VQ}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {(VARPSF)minsfxS,VCVTIP+VSB},
}};
static VARPSA rpsmax = {SBTX+1 , {
{(VARPSF)orinsB,VCVTIP+VB}, {0}, {(VARPSF)maxinsI,VCVTIP+VI}, {(VARPSF)maxinsD,VCVTIP+VD}, {(VARPSF)maxinsD,VCVTIP+VD+VDD}, {0}, {(VARPSF)maxinsX,VCVTIP+VX}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {(VARPSF)maxinsS,VCVTIP+VSB},
{(VARPSF)orpfxB,VCVTIP+VB}, {0}, {(VARPSF)maxpfxI,VCVTIP+VI+VIPOKW}, {(VARPSF)maxpfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)maxpfxD,VCVTIP+VD+VDD}, {0}, {(VARPSF)maxpfxX,VCVTIP+VX}, {(VARPSF)maxpfxQ,VCVTIP+VQ}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {(VARPSF)maxpfxS,VCVTIP+VSB},
{(VARPSF)orsfxB,VCVTIP+VB}, {0}, {(VARPSF)maxsfxI,VCVTIP+VI+VIPOKW}, {(VARPSF)maxsfxD,VCVTIP+VD+VIPOKW}, {(VARPSF)maxsfxD,VCVTIP+VD+VDD}, {0}, {(VARPSF)maxsfxX,VCVTIP+VX}, {(VARPSF)maxsfxQ,VCVTIP+VQ}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {(VARPSF)maxsfxS,VCVTIP+VSB},
}};


VA va[]={
/* non-atomic functions      */ {
 {{0,0}, {0,0}, {0,0},                                /* BB BI BD              */
  {0,0}, {0,0}, {0,0},                                /* IB II ID              */
  {0,0}, {0,0}, {0,0},                                /* DB DI DD              */
  {0,0}, {0,0}, {0,0}, {0,0}},                        /* ZZ XX QQ Symb         */
  &rpsnull},

/* 10    */ {
 {{(VF)bw0000II,VCVTIP+VI+VII+VIP}, {(VF)bw0000II,VCVTIP+VI+VII+VIP}, {(VF)bw0000II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0000II,VCVTIP+VI+VII+VIP}, {(VF)bw0000II,VCVTIP+VI+VIP},     {(VF)bw0000II,VCVTIP+VI+VII+VIP},
  {(VF)bw0000II,VCVTIP+VI+VII+VIP}, {(VF)bw0000II,VCVTIP+VI+VII+VIP}, {(VF)bw0000II,VCVTIP+VI+VII+VIP},
  {(VF)bw0000II,VCVTIP+VI+VII+VIP}, {(VF)bw0000II,VCVTIP+VI+VII+VIP}, {(VF)bw0000II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0000},

/* 11    */ {
 {{(VF)bw0001II,VCVTIP+VI+VII+VIP}, {(VF)bw0001II,VCVTIP+VI+VII+VIP}, {(VF)bw0001II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0001II,VCVTIP+VI+VII+VIP}, {(VF)bw0001II,VCVTIP+VI+VIP},     {(VF)bw0001II,VCVTIP+VI+VII+VIP},
  {(VF)bw0001II,VCVTIP+VI+VII+VIP}, {(VF)bw0001II,VCVTIP+VI+VII+VIP}, {(VF)bw0001II,VCVTIP+VI+VII+VIP},
  {(VF)bw0001II,VCVTIP+VI+VII+VIP}, {(VF)bw0001II,VCVTIP+VI+VII+VIP}, {(VF)bw0001II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0001},

/* 12    */ {
 {{(VF)bw0010II,VCVTIP+VI+VII+VIP}, {(VF)bw0010II,VCVTIP+VI+VII+VIP}, {(VF)bw0010II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0010II,VCVTIP+VI+VII+VIP}, {(VF)bw0010II,VCVTIP+VI+VIP},     {(VF)bw0010II,VCVTIP+VI+VII+VIP},
  {(VF)bw0010II,VCVTIP+VI+VII+VIP}, {(VF)bw0010II,VCVTIP+VI+VII+VIP}, {(VF)bw0010II,VCVTIP+VI+VII+VIP},
  {(VF)bw0010II,VCVTIP+VI+VII+VIP}, {(VF)bw0010II,VCVTIP+VI+VII+VIP}, {(VF)bw0010II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0010},

/* 13    */ {
 {{(VF)bw0011II,VCVTIP+VI+VII+VIP}, {(VF)bw0011II,VCVTIP+VI+VII+VIP}, {(VF)bw0011II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0011II,VCVTIP+VI+VII+VIP}, {(VF)bw0011II,VCVTIP+VI+VIP},     {(VF)bw0011II,VCVTIP+VI+VII+VIP},
  {(VF)bw0011II,VCVTIP+VI+VII+VIP}, {(VF)bw0011II,VCVTIP+VI+VII+VIP}, {(VF)bw0011II,VCVTIP+VI+VII+VIP},
  {(VF)bw0011II,VCVTIP+VI+VII+VIP}, {(VF)bw0011II,VCVTIP+VI+VII+VIP}, {(VF)bw0011II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0011},

/* 14    */ {
 {{(VF)bw0100II,VCVTIP+VI+VII+VIP}, {(VF)bw0100II,VCVTIP+VI+VII+VIP}, {(VF)bw0100II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0100II,VCVTIP+VI+VII+VIP}, {(VF)bw0100II,VCVTIP+VI+VIP},     {(VF)bw0100II,VCVTIP+VI+VII+VIP},
  {(VF)bw0100II,VCVTIP+VI+VII+VIP}, {(VF)bw0100II,VCVTIP+VI+VII+VIP}, {(VF)bw0100II,VCVTIP+VI+VII+VIP},
  {(VF)bw0100II,VCVTIP+VI+VII+VIP}, {(VF)bw0100II,VCVTIP+VI+VII+VIP}, {(VF)bw0100II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0100},

/* 15    */ {
 {{(VF)bw0101II,VCVTIP+VI+VII+VIP}, {(VF)bw0101II,VCVTIP+VI+VII+VIP}, {(VF)bw0101II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0101II,VCVTIP+VI+VII+VIP}, {(VF)bw0101II,VCVTIP+VI+VIP},     {(VF)bw0101II,VCVTIP+VI+VII+VIP},
  {(VF)bw0101II,VCVTIP+VI+VII+VIP}, {(VF)bw0101II,VCVTIP+VI+VII+VIP}, {(VF)bw0101II,VCVTIP+VI+VII+VIP},
  {(VF)bw0101II,VCVTIP+VI+VII+VIP}, {(VF)bw0101II,VCVTIP+VI+VII+VIP}, {(VF)bw0101II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0101},

/* 16    */ {
 {{(VF)bw0110II,VCVTIP+VI+VII+VIP}, {(VF)bw0110II,VCVTIP+VI+VII+VIP}, {(VF)bw0110II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0110II,VCVTIP+VI+VII+VIP}, {(VF)bw0110II,VCVTIP+VI+VIP},     {(VF)bw0110II,VCVTIP+VI+VII+VIP},
  {(VF)bw0110II,VCVTIP+VI+VII+VIP}, {(VF)bw0110II,VCVTIP+VI+VII+VIP}, {(VF)bw0110II,VCVTIP+VI+VII+VIP},
  {(VF)bw0110II,VCVTIP+VI+VII+VIP}, {(VF)bw0110II,VCVTIP+VI+VII+VIP}, {(VF)bw0110II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0110},

/* 17    */ {
 {{(VF)bw0111II,VCVTIP+VI+VII+VIP}, {(VF)bw0111II,VCVTIP+VI+VII+VIP}, {(VF)bw0111II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw0111II,VCVTIP+VI+VII+VIP}, {(VF)bw0111II,VCVTIP+VI+VIP},     {(VF)bw0111II,VCVTIP+VI+VII+VIP},
  {(VF)bw0111II,VCVTIP+VI+VII+VIP}, {(VF)bw0111II,VCVTIP+VI+VII+VIP}, {(VF)bw0111II,VCVTIP+VI+VII+VIP},
  {(VF)bw0111II,VCVTIP+VI+VII+VIP}, {(VF)bw0111II,VCVTIP+VI+VII+VIP}, {(VF)bw0111II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw0111},

/* 18    */ {
 {{(VF)bw1000II,VCVTIP+VI+VII+VIP}, {(VF)bw1000II,VCVTIP+VI+VII+VIP}, {(VF)bw1000II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1000II,VCVTIP+VI+VII+VIP}, {(VF)bw1000II,VCVTIP+VI+VIP},     {(VF)bw1000II,VCVTIP+VI+VII+VIP},
  {(VF)bw1000II,VCVTIP+VI+VII+VIP}, {(VF)bw1000II,VCVTIP+VI+VII+VIP}, {(VF)bw1000II,VCVTIP+VI+VII+VIP},
  {(VF)bw1000II,VCVTIP+VI+VII+VIP}, {(VF)bw1000II,VCVTIP+VI+VII+VIP}, {(VF)bw1000II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1000},

/* 19    */ {
 {{(VF)bw1001II,VCVTIP+VI+VII+VIP}, {(VF)bw1001II,VCVTIP+VI+VII+VIP}, {(VF)bw1001II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1001II,VCVTIP+VI+VII+VIP}, {(VF)bw1001II,VCVTIP+VI+VIP},     {(VF)bw1001II,VCVTIP+VI+VII+VIP},
  {(VF)bw1001II,VCVTIP+VI+VII+VIP}, {(VF)bw1001II,VCVTIP+VI+VII+VIP}, {(VF)bw1001II,VCVTIP+VI+VII+VIP},
  {(VF)bw1001II,VCVTIP+VI+VII+VIP}, {(VF)bw1001II,VCVTIP+VI+VII+VIP}, {(VF)bw1001II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1001},

/* 1a    */ {
 {{(VF)bw1010II,VCVTIP+VI+VII+VIP}, {(VF)bw1010II,VCVTIP+VI+VII+VIP}, {(VF)bw1010II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1010II,VCVTIP+VI+VII+VIP}, {(VF)bw1010II,VCVTIP+VI+VIP},     {(VF)bw1010II,VCVTIP+VI+VII+VIP},
  {(VF)bw1010II,VCVTIP+VI+VII+VIP}, {(VF)bw1010II,VCVTIP+VI+VII+VIP}, {(VF)bw1010II,VCVTIP+VI+VII+VIP},
  {(VF)bw1010II,VCVTIP+VI+VII+VIP}, {(VF)bw1010II,VCVTIP+VI+VII+VIP}, {(VF)bw1010II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1010},

/* 1b    */ {
 {{(VF)bw1011II,VCVTIP+VI+VII+VIP}, {(VF)bw1011II,VCVTIP+VI+VII+VIP}, {(VF)bw1011II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1011II,VCVTIP+VI+VII+VIP}, {(VF)bw1011II,VCVTIP+VI+VIP},     {(VF)bw1011II,VCVTIP+VI+VII+VIP},
  {(VF)bw1011II,VCVTIP+VI+VII+VIP}, {(VF)bw1011II,VCVTIP+VI+VII+VIP}, {(VF)bw1011II,VCVTIP+VI+VII+VIP},
  {(VF)bw1011II,VCVTIP+VI+VII+VIP}, {(VF)bw1011II,VCVTIP+VI+VII+VIP}, {(VF)bw1011II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1011},

/* 1c    */ {
 {{(VF)bw1100II,VCVTIP+VI+VII+VIP}, {(VF)bw1100II,VCVTIP+VI+VII+VIP}, {(VF)bw1100II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1100II,VCVTIP+VI+VII+VIP}, {(VF)bw1100II,VCVTIP+VI+VIP},     {(VF)bw1100II,VCVTIP+VI+VII+VIP},
  {(VF)bw1100II,VCVTIP+VI+VII+VIP}, {(VF)bw1100II,VCVTIP+VI+VII+VIP}, {(VF)bw1100II,VCVTIP+VI+VII+VIP},
  {(VF)bw1100II,VCVTIP+VI+VII+VIP}, {(VF)bw1100II,VCVTIP+VI+VII+VIP}, {(VF)bw1100II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1100},

/* 1d    */ {
 {{(VF)bw1101II,VCVTIP+VI+VII+VIP}, {(VF)bw1101II,VCVTIP+VI+VII+VIP}, {(VF)bw1101II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1101II,VCVTIP+VI+VII+VIP}, {(VF)bw1101II,VCVTIP+VI+VIP},     {(VF)bw1101II,VCVTIP+VI+VII+VIP},
  {(VF)bw1101II,VCVTIP+VI+VII+VIP}, {(VF)bw1101II,VCVTIP+VI+VII+VIP}, {(VF)bw1101II,VCVTIP+VI+VII+VIP},
  {(VF)bw1101II,VCVTIP+VI+VII+VIP}, {(VF)bw1101II,VCVTIP+VI+VII+VIP}, {(VF)bw1101II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1101},

/* 1e    */ {
 {{(VF)bw1110II,VCVTIP+VI+VII+VIP}, {(VF)bw1110II,VCVTIP+VI+VII+VIP}, {(VF)bw1110II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1110II,VCVTIP+VI+VII+VIP}, {(VF)bw1110II,VCVTIP+VI+VIP},     {(VF)bw1110II,VCVTIP+VI+VII+VIP},
  {(VF)bw1110II,VCVTIP+VI+VII+VIP}, {(VF)bw1110II,VCVTIP+VI+VII+VIP}, {(VF)bw1110II,VCVTIP+VI+VII+VIP},
  {(VF)bw1110II,VCVTIP+VI+VII+VIP}, {(VF)bw1110II,VCVTIP+VI+VII+VIP}, {(VF)bw1110II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1110},

/* 1f    */ {
 {{(VF)bw1111II,VCVTIP+VI+VII+VIP}, {(VF)bw1111II,VCVTIP+VI+VII+VIP}, {(VF)bw1111II,VCVTIP+VI+VII+VIP}, 
  {(VF)bw1111II,VCVTIP+VI+VII+VIP}, {(VF)bw1111II,VCVTIP+VI+VIP},     {(VF)bw1111II,VCVTIP+VI+VII+VIP},
  {(VF)bw1111II,VCVTIP+VI+VII+VIP}, {(VF)bw1111II,VCVTIP+VI+VII+VIP}, {(VF)bw1111II,VCVTIP+VI+VII+VIP},
  {(VF)bw1111II,VCVTIP+VI+VII+VIP}, {(VF)bw1111II,VCVTIP+VI+VII+VIP}, {(VF)bw1111II,VCVTIP+VI+VII+VIP}, {0,0}}, 
  &rpsbw1111},

/* 95 ~: */ {
 {{(VF)neBB,VCVTIP+VB+VIP}, {(VF)neBI,VCVTIP+VB+VIPOKA}, {(VF)neBD,VCVTIP+VB+VIPOKA},
  {(VF)neIB,VCVTIP+VB+VIPOKW}, {(VF)neII,VCVTIP+VB}, {(VF)neID,VCVTIP+VB},
  {(VF)neDB,VCVTIP+VB+VIPOKW}, {(VF)neDI,VCVTIP+VB}, {(VF)neDD,VCVTIP+VB}, 
  {(VF)neZZ,VCVTIP+VB+VZZ}, {(VF)neXX,VCVTIP+VB+VXEQ}, {(VF)neQQ,VCVTIP+VB+VQQ}, {0,0}},
  &rpsne},

/* 25 %  */ {
 {{(VF)divBB,VCVTIP+VD}, {(VF)divBI,VCVTIP+VD+VIP0I}, {(VF)divBD,VCVTIP+VD+VIPOKW},
  {(VF)divIB,VCVTIP+VD+VIPI0}, {(VF)divII,VCVTIP+VD+VIPI0+VIP0I}, {(VF)divID,VCVTIP+VD+VIPID},
  {(VF)divDB,VCVTIP+VD+VIPOKA}, {(VF)divDI,VCVTIP+VD+VIPDI}, {(VF)divDD,VCVTIP+VD+VIP+VCANHALT}, 
  {(VF)divZZ,VCVTIP+VZ+VZZ+VIP}, {(VF)divXX,VCVTIP+VX+VXX}, {(VF)divQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpsdiv},

/* 89 +: */ {
 {{(VF)norBB,VCVTIP+VB+VIP    }, {(VF)norBB,VCVTIP+VB+VBB+VIP}, {(VF)norBB,VCVTIP+VB+VBB+VIP},
  {(VF)norBB,VCVTIP+VB+VBB+VIP}, {(VF)norBB,VCVTIP+VB+VBB+VIP}, {(VF)norBB,VCVTIP+VB+VBB+VIP},
  {(VF)norBB,VCVTIP+VB+VBB+VIP}, {(VF)norBB,VCVTIP+VB+VBB+VIP}, {(VF)norBB,VCVTIP+VB+VBB+VIP}, 
  {(VF)norBB,VCVTIP+VB+VBB+VIP}, {(VF)norBB,VCVTIP+VB+VBB+VIP}, {(VF)norBB,VCVTIP+VB+VBB+VIP}, {0,0}},
  &rpsnor},

/* 88 +. */ {
 {{(VF)orBB,VCVTIP+VB+VIP     }, {(VF)gcdII,VCVTIP+VI+VII}, {(VF)gcdDD,VCVTIP+VD+VDD+VIP},
  {(VF)gcdII,VCVTIP+VI+VII}, {(VF)gcdII,VCVTIP+VI    }, {(VF)gcdDD,VCVTIP+VD+VDD+VIP},
  {(VF)gcdDD,VCVTIP+VD+VDD+VIP}, {(VF)gcdDD,VCVTIP+VD+VDD+VIP}, {(VF)gcdDD,VCVTIP+VD+VIP+VCANHALT}, 
  {(VF)gcdZZ,VCVTIP+VZ+VZZ}, {(VF)gcdXX,VCVTIP+VX+VXX}, {(VF)gcdQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpsor},

/* 2d -  */ {
 {{(VF)minusBB,VCVTIP+VI    }, {(VF)minusBI,VCVTIP+VI+VIPOKW}, {(VF)minusBD,VCVTIP+VD+VIPOKW}, 
  {(VF)minusIB,VCVTIP+VI+VIPOKA}, {(VF)minusII,VCVTIP+VI+VIP}, {(VF)minusID,VCVTIP+VD+VIPID},
  {(VF)minusDB,VCVTIP+VD+VIPOKA    }, {(VF)minusDI,VCVTIP+VD+VIPDI    }, {(VF)minusDD,VCVTIP+VD+VIP+VCANHALT}, 
  {(VF)minusZZ,VCVTIP+VZ+VZZ+VIP}, {(VF)minusXX,VCVTIP+VX+VXX}, {(VF)minusQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpsminus},

   // For Booleans, VIP means 'inplace if rank not specified and there is no frame'
/* 3c <  */ {
 {{(VF)ltBB,VCVTIP+VB+VIP}, {(VF)ltBI,VCVTIP+VB+VIPOKA}, {(VF)ltBD,VCVTIP+VB+VIPOKA},
  {(VF)ltIB,VCVTIP+VB+VIPOKW}, {(VF)ltII,VCVTIP+VB}, {(VF)ltID,VCVTIP+VB},
  {(VF)ltDB,VCVTIP+VB+VIPOKW}, {(VF)ltDI,VCVTIP+VB}, {(VF)ltDD,VCVTIP+VB}, 
  {(VF)ltDD,VCVTIP+VB+VDD+VIP}, {(VF)ltXX,VCVTIP+VB+VXFC}, {(VF)ltQQ,VCVTIP+VB+VQQ}, {(VF)ltSS,VCVTIP+VB}},
  &rpslt},

/* 3d =  */ {
 {{(VF)eqBB,VCVTIP+VB+VIP}, {(VF)eqBI,VCVTIP+VB+VIPOKA}, {(VF)eqBD,VCVTIP+VB+VIPOKA},
  {(VF)eqIB,VCVTIP+VB+VIPOKW}, {(VF)eqII,VCVTIP+VB}, {(VF)eqID,VCVTIP+VB},
  {(VF)eqDB,VCVTIP+VB+VIPOKW}, {(VF)eqDI,VCVTIP+VB}, {(VF)eqDD,VCVTIP+VB}, 
  {(VF)eqZZ,VCVTIP+VB+VZZ}, {(VF)eqXX,VCVTIP+VB+VXEQ}, {(VF)eqQQ,VCVTIP+VB+VQQ}, {(VF)eqII,VCVTIP+VB}},
  &rpseq},

/* 3e >  */ {
 {{(VF)gtBB,VCVTIP+VB+VIP}, {(VF)gtBI,VCVTIP+VB+VIPOKA}, {(VF)gtBD,VCVTIP+VB+VIPOKA},
  {(VF)gtIB,VCVTIP+VB+VIPOKW}, {(VF)gtII,VCVTIP+VB}, {(VF)gtID,VCVTIP+VB},
  {(VF)gtDB,VCVTIP+VB+VIPOKW}, {(VF)gtDI,VCVTIP+VB}, {(VF)gtDD,VCVTIP+VB}, 
  {(VF)gtDD,VCVTIP+VB+VDD+VIP}, {(VF)gtXX,VCVTIP+VB+VXCF}, {(VF)gtQQ,VCVTIP+VB+VQQ}, {(VF)gtSS,VCVTIP+VB}},
  &rpsgt},

/* 8a *. */ {
 {{(VF)andBB,VCVTIP+VB+VIP    }, {(VF)lcmII,VCVTIP+VI+VII}, {(VF)lcmDD,VCVTIP+VD+VDD+VIP},
  {(VF)lcmII,VCVTIP+VI+VII}, {(VF)lcmII,VCVTIP+VI    }, {(VF)lcmDD,VCVTIP+VD+VDD+VIP},
  {(VF)lcmDD,VCVTIP+VD+VDD+VIP}, {(VF)lcmDD,VCVTIP+VD+VDD+VIP}, {(VF)lcmDD,VCVTIP+VD+VIP+VCANHALT}, 
  {(VF)lcmZZ,VCVTIP+VZ+VZZ}, {(VF)lcmXX,VCVTIP+VX+VXX}, {(VF)lcmQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpsand},

/* 8b *: */ {
 {{(VF)nandBB,VCVTIP+VB+VIP},     {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {(VF)nandBB,VCVTIP+VB+VBB+VIP},
  {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {(VF)nandBB,VCVTIP+VB+VBB+VIP},
  {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {(VF)nandBB,VCVTIP+VB+VBB+VIP}, 
  {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {(VF)nandBB,VCVTIP+VB+VBB+VIP}, {0,0}},
  &rpsnand},

/* 85 >: */ {
 {{(VF)geBB,VCVTIP+VB+VIP}, {(VF)geBI,VCVTIP+VB+VIPOKA}, {(VF)geBD,VCVTIP+VB+VIPOKA},
  {(VF)geIB,VCVTIP+VB+VIPOKW}, {(VF)geII,VCVTIP+VB}, {(VF)geID,VCVTIP+VB},
  {(VF)geDB,VCVTIP+VB+VIPOKW}, {(VF)geDI,VCVTIP+VB}, {(VF)geDD,VCVTIP+VB}, 
  {(VF)geDD,VCVTIP+VB+VDD+VIP}, {(VF)geXX,VCVTIP+VB+VXFC}, {(VF)geQQ,VCVTIP+VB+VQQ}, {(VF)geSS,VCVTIP+VB}},
  &rpsge},

/* 83 <: */ {
 {{(VF)leBB,VCVTIP+VB+VIP}, {(VF)leBI,VCVTIP+VB+VIPOKA}, {(VF)leBD,VCVTIP+VB+VIPOKA},
  {(VF)leIB,VCVTIP+VB+VIPOKW}, {(VF)leII,VCVTIP+VB}, {(VF)leID,VCVTIP+VB},
  {(VF)leDB,VCVTIP+VB+VIPOKW}, {(VF)leDI,VCVTIP+VB}, {(VF)leDD,VCVTIP+VB}, 
  {(VF)leDD,VCVTIP+VB+VDD+VIP}, {(VF)leXX,VCVTIP+VB+VXCF}, {(VF)leQQ,VCVTIP+VB+VQQ}, {(VF)leSS,VCVTIP+VB}},
  &rpsle},

/* 82 <. */ {
 {{(VF)andBB,VCVTIP+VB+VIP}, {(VF)minBI,VCVTIP+VI+VIPOKW}, {(VF)minBD,VCVTIP+VD+VIPOKW},
  {(VF)minIB,VCVTIP+VI+VIPOKA}, {(VF)minII,VCVTIP+VI+VIP}, {(VF)minID,VCVTIP+VD+VIPID},
  {(VF)minDB,VCVTIP+VD+VIPOKA}, {(VF)minDI,VCVTIP+VD+VIPDI}, {(VF)minDD,VCVTIP+VD+VIP}, 
  {(VF)minDD,VCVTIP+VD+VDD+VIP}, {(VF)minXX,VCVTIP+VX+VXX}, {(VF)minQQ,VCVTIP+VQ+VQQ}, {(VF)minSS,VCVTIP+VSB}},  // always VIP a forced conversion
  &rpsmin},

/* 84 >. */ {
 {{(VF)orBB,VCVTIP+VB+VIP}, {(VF)maxBI,VCVTIP+VI+VIPOKW}, {(VF)maxBD,VCVTIP+VD+VIPOKW},
  {(VF)maxIB,VCVTIP+VI+VIPOKA}, {(VF)maxII,VCVTIP+VI+VIP}, {(VF)maxID,VCVTIP+VD+VIPID},
  {(VF)maxDB,VCVTIP+VD+VIPOKA}, {(VF)maxDI,VCVTIP+VD+VIPDI}, {(VF)maxDD,VCVTIP+VD+VIP}, 
  {(VF)maxDD,VCVTIP+VD+VDD+VIP}, {(VF)maxXX,VCVTIP+VX+VXX}, {(VF)maxQQ,VCVTIP+VQ+VQQ}, {(VF)maxSS,VCVTIP+VSB}},
  &rpsmax},

/* 2b +  */ {
 {{(VF)plusBB,VCVTIP+VI    }, {(VF)plusBI,VCVTIP+VI+VIPOKW}, {(VF)plusBD,VCVTIP+VD+VIPOKW}, 
  {(VF)plusIB,VCVTIP+VI+VIPOKA}, {(VF)plusII,VCVTIP+VI+VIP}, {(VF)plusID,VCVTIP+VD+VIPID}, 
  {(VF)plusDB,VCVTIP+VD+VIPOKA    }, {(VF)plusDI,VCVTIP+VD+VIPDI    }, {(VF)plusDD,VCVTIP+VD+VIP+VCANHALT}, 
  {(VF)plusZZ,VCVTIP+VZ+VZZ+VIP}, {(VF)plusXX,VCVTIP+VX+VXX}, {(VF)plusQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpsplus},

/* 2a *  */ {
 {{(VF)andBB,VCVTIP+VB+VIP}, {(VF)tymesBI,VCVTIP+VI+VIPOKW}, {(VF)tymesBD,VCVTIP+VD+VIPOKW},
  {(VF)tymesIB,VCVTIP+VI+VIPOKA}, {(VF)tymesII,VCVTIP+VI+VIP}, {(VF)tymesID,VCVTIP+VD+VIPID},
  {(VF)tymesDB,VCVTIP+VD+VIPOKA}, {(VF)tymesDI,VCVTIP+VD+VIPDI}, {(VF)tymesDD,VCVTIP+VD+VIP}, 
  {(VF)tymesZZ,VCVTIP+VZ+VZZ+VIP}, {(VF)tymesXX,VCVTIP+VX+VXX}, {(VF)tymesQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpstymes},

/* 5e ^  */ {   // may produce complex numbers
 {{(VF)geBB,VCVTIP+VB+VIP}, {(VF)powBI,VCVTIP+VD}, {(VF)powBD,VCVTIP+VD},
  {(VF)powIB,VCVTIP+VI}, {(VF)powII,VCVTIP+VD}, {(VF)powID,VCVTIP+VD+VCANHALT},
  {(VF)powDB,VCVTIP+VD}, {(VF)powDI,VCVTIP+VD}, {(VF)powDD,VCVTIP+VD+VCANHALT}, 
  {(VF)powZZ,VCVTIP+VZ+VZZ}, {(VF)powXX,VCVTIP+VX+VXX}, {(VF)powQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpsge},

/* 7c |  */ {
 {{(VF)ltBB,VCVTIP+VB+VIP    }, {(VF)remII,VCVTIP+VI+VII+VIP}, {(VF)remDD,VCVTIP+VD+VDD+VIP},
  {(VF)remII,VCVTIP+VI+VII+VIP}, {(VF)remII,VCVTIP+VI+VIP    }, {(VF)remID,VCVTIP+VI+VCANHALT    },   // remID can 'overflow' if result is nonintegral
  {(VF)remDD,VCVTIP+VD+VDD+VIP}, {(VF)remDD,VCVTIP+VD+VDD+VIP}, {(VF)remDD,VCVTIP+VD+VIP+VCANHALT}, 
  {(VF)remZZ,VCVTIP+VZ+VZZ}, {(VF)remXX,VCVTIP+VX+VXX}, {(VF)remQQ,VCVTIP+VQ+VQQ}, {0,0}},
  &rpslt},

/* 21 !  */ {
 {{(VF)leBB,VCVTIP+VB+VIP            }, {(VF)binDD,VCVTIP+VD+VDD+VRI+VIP}, {(VF)binDD,VCVTIP+VD+VDD+VIP}, 
  {(VF)binDD,VCVTIP+VD+VDD+VRI+VIP}, {(VF)binDD,VCVTIP+VD+VDD+VRI+VIP}, {(VF)binDD,VCVTIP+VD+VDD+VIP}, 
  {(VF)binDD,VCVTIP+VD+VDD+VIP    }, {(VF)binDD,VCVTIP+VD+VDD+VIP    }, {(VF)binDD,VCVTIP+VD+VIP    }, 
  {(VF)binZZ,VCVTIP+VZ+VZZ}, {(VF)binXX,VCVTIP+VX+VXX}, {(VF)binQQ,VCVTIP+VX+VQQ}, {0,0}}, 
  &rpsle},

/* d1 o. */ {
 {{(VF)cirDD,VCVTIP+VD+VDD}, {(VF)cirDD,VCVTIP+VD+VDD}, {(VF)cirBD,VCVTIP+VD},
  {(VF)cirDD,VCVTIP+VD+VDD}, {(VF)cirDD,VCVTIP+VD+VDD}, {(VF)cirID,VCVTIP+VD},
  {(VF)cirDD,VCVTIP+VD+VDD}, {(VF)cirDD,VCVTIP+VD+VDD}, {(VF)cirDD,VCVTIP+VD}, 
  {(VF)cirZZ,VCVTIP+VZ+VZZ+VRD}, {(VF)cirDD,VCVTIP+VD+VDD}, {(VF)cirDD,VCVTIP+VD+VDD}, {0,0}},
  &rpsnull},
};

A jtcvz(J jt,I cv,A w){I t;
 t=AT(w);
 if(cv&VRD&&!(t&FL) )R pcvt(FL,w);  // convert if possible
 if(cv&VRI&&!(t&INT))R icvt(w);  // convert to integer if possible
 R w;
}    /* convert result */


// Table converting operand types to slot numbers in a va[] entry
// Employed when one arg is known to be CMPX/XNUM/RAT.  Indexed by
// bitmask of RAT,CMPX,FL.  Entry 9=CMPX; otherwise entry 8=some FL, otherwise
// entry 10 for XNUM, 11 for some RAT 
// static UC xnumpri[] = {10 ,8 ,9 ,9 ,11 ,8 ,9 ,9};
#define xnumpri 0x998B998AU   // we use shift to select 4-bit sections

// repair routines for 
static VF repairip[4] = {plusBIO, plusIIO, minusBIO, minusIIO};

// All dyadic arithmetic verbs f enter here, and also f"n.  a and w are the arguments, id
// is the pseudocharacter indicating what operation is to be performed.  self is the block for this primitive,
// allranks is (ranks of a and w),(verb ranks)
static A jtva2(J jt,AD * RESTRICT a,AD * RESTRICT w,AD * RESTRICT self,UI allranks){  // allranks is argranks/ranks
 A z;I m,mf,n,nf,zn;VA2 adocv,*aadocv;UI fr;  // fr will eventually be frame/rank  nf (and mf) change roles during execution  fr/shortr use all bits and shift
 I aawwzk[5];  // a outer/only, a inner, w outer/only, w inner, z
 F2PREFIP;
 {I at=AT(a);
  I wt=AT(w);
  if(likely(!(((I)jtinplace&(JTRETRY|JTEMPTY))+((UNSAFE(at|wt))&(NOUN&~(B01|INT|FL)))))){  // no error, bool/int/fl args, no empties
   // Here for the fast and important case, where the arguments are both B01/INT/FL
   VA *vainfo=(VA*)FAV(self)->localuse.lvp[0];  // extract table line from the primitive
   // The index into va is atype*3 + wtype, calculated sneakily.  We test here to avoid the call overhead
   jt->mulofloloc = 0;  // Reinit multiplier-overflow count, in case we hit overflow.  Needed only on integer multiply, but there's no better place
   aadocv=&vainfo->p2[(UNSAFE(at)>>(INTX-1))+((UNSAFE(at)+UNSAFE(wt))>>INTX)];
  }else{

   // If an operand is empty, turn it to Boolean, and if the OTHER operand is non-numeric, turn that to Boolean too (leaving
   //  rank and shape untouched).  This change to the other operand is notional only - we won't actually convert
   // when there is an empty - but it guarantees that execution on an empty never fails.
   // If we switch a sparse nonnumeric matrix to boolean, that may be a space problem; but we don't
   // support nonnumeric sparse now
   // if an operand is sparse, replace its type with the corresponding non-sparse type, for purposes of testing operand precisions
   if(unlikely(((at|wt)&SPARSE)!=0)){
    at=(SPARSE&at)?DTYPE(at):at;
    wt=(SPARSE&wt)?DTYPE(wt):wt;
    jtinplace=0;  // We use jtinplace==0 as a flag meaning 'sparse'
   }
   // We could allocate the result block here & avoid the test after the allocation later.  But we would have to check for agreement etc
   if(AN(a)==0){at=B01;if(!(wt&NUMERIC))wt=B01;}  // switch empty arg to Boolean & ensure compatibility with other arg
   if(AN(w)==0){wt=B01;if(!(at&NUMERIC))at=B01;}

   // Figure out the result type.  Don't signal the error from it yet, because domain has lower priority than agreement
   adocv=var(self,at,wt); aadocv=&adocv;
  }
 }

 // finish up the computation of sizes.  We have to defer this till after var() because
 // if we are retrying the operation, we may be in error state until var clears it; and prod and mult can fail,
 // so we have to RE when we call them

// I ar = AR(a);
// I wr = AR(w);
 // Analyze the rank and calculate cell shapes, counts, and sizes.
 // We detect agreement error before domain error
 {//I *as = AS(a); I *ws = AS(w);
  if(likely((allranks&RANK2TMSK)==0)){ // rank 0 0 means no outer frames, sets up faster
   fr=allranks>>(3*RANKTX); UI shortr=(allranks>>(2*RANKTX))&RANKTMSK;  // fr,shortr = ar,wr to begin with.  Changes later
   // No rank specified.  Since all these verbs have rank 0, that simplifies quite a bit.  ak/wk/zk are not needed and are garbage
   // n is not needed for sparse, but we start it early to get it finished
   if(likely(jtinplace!=0)){
    I an=AN(a); m=zn=AN(w);
    I raminusw=fr-shortr;   // ar-wr, neg if WISLONG
    zn=raminusw<0?zn:an; m=raminusw<0?an:m;  // zn=# atoms in bigger operand, m=#atoms in smaller
    jtinplace = (J)(((I)jtinplace&3)+aadocv->cv+(raminusw&VIPWCRLONG));  // inplaceability plus routine flags  aadocv free
    mf=REPSGN(raminusw);  // mf=-1 if w has longer frame, means cannot inplace a
    raminusw=-raminusw;   // now wr-ar
    nf=REPSGN(raminusw);  // nf=-1 if a has longer frame, means cannot inplace w
    raminusw=raminusw&mf; fr+=raminusw; shortr-=raminusw;  // if ar is the longer one, change nothing; otherwise transfer aw-ar from shortr to r.  f (high part of fr) is 0
    PRODRNK(n,fr-shortr,AS((I)jtinplace&VIPWCRLONG?w:a)+shortr);  // treat the entire operands as one big cell; get the rest of the values needed
   // notionally we now repurpose fr to be frame/rank, with the frame 0
    shortr=((I)jtinplace>>VIPOKWX);  // only if supported by routine
    shortr=a==w?0:shortr;  // not if args equal
    // Non-sparse setup for copy loop, no rank
      // get number of inner cells
    nf= 3 + mf*2 + nf;  // set inplaceability here: only if nonrepeated cell
    nf&=shortr;  // we use shortr to shorten dependency chain on nf
    nf+=4*nf-16;  // make 2 copies of the 2 bits  This is a long dependency chain through nf
    jtinplace = (J)((I)jtinplace&nf);  // bit 2-3=routine/rank/arg inplaceable, 0-1=routine/rank/arg/input inplaceable
    n^=REPSGN(1-n)&mf;  // encode 'w has long frame, so a is repeated' as complementary n; but if n<2, leave it alone.  Since n=1 when frames are equal, mf in that case is N/C
    mf=1;  // suppress the outer loop, leaving only the loop over m and n
   }else{
    // Sparse setup: move the block-local variables to longer-lived ones.  We are trying to reduce register pressure
    // repurpose ak/wk/mf/nf to hold acr/wcr/af/wf, which we will pass into vasp.  This allows acr/wcr/af/wf to be block-local
    aawwzk[0]=fr; aawwzk[1]=shortr; fr=shortr>fr?shortr:fr; mf=0; nf=0;
   }
  }else{I ak,wk;UI acr,wcr;  // fr, shortr are left/right verb rank here
   // Here, a rank was specified.  That means there must be a frame, according to the IRS rules
   {I af,wf;
    // Heavy register pressure here.  Seemingly trivial changes to allranks cause early spill of a in next-to-last PRODRNK
    // When this was written with 6 names it barely fit in registers, so I rewrote it with 4 names.  Compiler's ideas about handling allranks fill spare register
    acr=0; af=allranks>>2*RANKTX;  // allranks = anr/wnr/avr/wvr fr==af=anr/wnr  wcr will hold verbrank
    wcr=(UI4)allranks; af|=RANKTMSK; af-=wcr; af=((I)af<0)?acr:af; af&=~RANKTMSK;  // fr is 0/0/anr/wnr -> 0/0/anr/ffff -> 0/0/afr/x -> clamp at 0 -> 0/0/afr/0
    wf=allranks>>2*RANKTX; wcr&=RANKTMSK; wf&=RANKTMSK; wf-=wcr; wf=((I)wf<0)?acr:wf;    // wf is 0/0/anr/wnr -> 0/0/0/anr ->  0/0/0/afr -> clamp at 0
    acr=allranks>>2*RANKTX; acr-=af; acr-=wf;  // acr = ar/wr/acr/wcr
    wcr=acr; wcr&=RANKTMSK; wf<<=2*RANKTX+1; wcr|=wf;   // wcr = wfr/0/wcr  final value
    acr>>=RANKTX; af<<=RANKTX+1; acr|=af;  // acr = afr/0/acr    final value
   }
   if(likely(jtinplace!=0)){  // If not sparse... This block isn't needed for sparse arguments, and may fail on them.
    jtinplace = (J)((I)jtinplace&3);  // remove all but the inplacing bits
    jtinplace = (J)((I)jtinplace+aadocv->cv);  // insert flag bits for routine (always has bits 0-3=0xc); set bits 2-3 (converted inplacing bits) aadocv free
    jtinplace = (J)((I)jtinplace+((acr+((I)1<<(2*RANKTX))-wcr)&(VIPWFLONG|VIPWCRLONG)));  // set flag for 'w has longer cell-rank' and 'w has longer frame (wrt verb)'.  The 1 is a carry blocker
    PRODRNK(ak,acr, AS(a)+(acr>>(2*RANKTX+1))); PRODRNK(wk,wcr,AS(w)+(wcr>>(2*RANKTX+1)));   // left/right #atoms/cell  length is assigned first af free
       // note: the prod above can never fail, because it gives the actual # cells of an existing noun
    // m=#atoms in cell with shorter rank; n=#times shorter-rank cells must be repeated; r=larger of cell-ranks
    // fr has the longer cell-rank
    // if looping required, calculate the strides for input & output.  Needed only if mf or nf>1, but not worth testing, since presumably one will, else why use rank?
    // zk=result-cell size in bytes; ak,wk=left,right arg-cell size in bytes.  Not needed if not looping
    // bits 0-1 of jtinplace are combined input+local; 2-3 just local; 4+ hold adocv.cv; sign set if ak==0. output type is always set to show non-sparse
    // 0-1 are set if operand is inplaceable according to prim & input inplaceability; 2-3 from prim only.  We use 0-1 unless we convert; then we use 2-3
     // jtinplace VIPWFLONG set if wf>af, bit VIPWCRLONG set if wcr>acr
    zn=(I)jtinplace&VIPWCRLONG?wk:ak;    // zn=#atoms in cell with larger rank
    m=(I)jtinplace&VIPWCRLONG?ak:wk;  // m=#atoms in common inner cell, i. e. the smaller
    aawwzk[4]=zn<<bplg(rtype((I)jtinplace));  // calc result-cell size and move it out of registers
    ak<<=bplg(AT(a)); wk<<=bplg(AT(w));  // convert cell sizes to bytes
    aawwzk[0]=ak; aawwzk[2]=wk; ak=((I)jtinplace&VIPWFLONG)?0:ak; wk=((I)jtinplace&VIPWFLONG)?wk:0; aawwzk[1]=ak; aawwzk[3]=wk;  // set inner cell size for last followed by non-last.  Last is 0 for a repeated cell ak/wk free
    I shortr=(I)jtinplace&VIPWCRLONG?acr:wcr; fr=(I)jtinplace&VIPWCRLONG?wcr:acr; // shortr=frame(short cell)/0/cellrank(short cell)  fr=frame(long cell)/0/cellrank(long cell)
    shortr&=RANKTMSK;  // cellrank(short cell)
    shortr*=0x20000ffff;   //   cellrank(short cell)/cellrank(short cell)/-cellrank(short cell)  200000000+10000+ffffffffffffffff
    shortr+=fr;  // frame(long cell)+cellrank(short cell)/cellrank(short cell)/cellrank(long cell)-cellrank(short cell)
                 //  offset to excess frame, for calc n  / length for agreement / length for calc n,(# intracell repeats) - final value
    acr>>=2*RANKTX+1; wcr>>=2*RANKTX+1;  // NOW ACR/WCR HAVE BEEN REPURPOSED TO MEAN AF/WF; acr/wcr gone
    // fr will be (frame(long cell))  /  (shorter frame len)   /  (longer frame len)                      /   (longer frame len+longer celllen)
    //  (offset to store cellshape to)  / for #outer cells mf  / length of frame to copy, also to calc nf / ranks that = this have no repeats, can inplace (also used to figure cellen for shape copy)
    // on 32-bit systems, the top 2 fields have to go into names scellf and f
    I f=((I)jtinplace&VIPWFLONG)?wcr:acr;    // f=#long frame len
    fr*=0x8001; fr&=(0xffff000000007fffU);  // encode start of cell-shape in the argument where the longer cell-shape resides  frame(long cell)/0/0/cellrank(long cell)
    f*=0xffffffff00010001; fr+=f; f=(acr+wcr)<<(2*RANKTX); fr+=f;  // cry/-longframe/longframe/longframe   frame(long cell)+cry/-longframe/longframe/longframe+cellrank(long cell) 0 + ffffffff00000000 + 10000 + 1
        //   frame(long cell)/shortframe/longframe/longframe+cellrank(long cell) - final value   f free
    // register pressure is excruciating here.  Only one register allowed, and no 64-bit immediates
    f=fr&RANKTMSK; f^=RANKTMSK; allranks>>=2*RANKTX; allranks^=f; f<<=RANKTX; allranks^= f;  // now allranks has 2 bytes, with 00 indicating OK to inplace  i. e. argrank^f+fr  f free
    allranks+=(VIPOKRNKA-1)-((1LL<<(2*RANKTX))-1)+(1LL<<RANKTX); // 0x00010000  set VIPOKRNKA  (by carry into 0) if a byte of allranks is 00
    allranks&=~((VIPOKRNKW<<1)-(1LL<<RANKTX)); allranks+=(VIPOKRNKW-(1LL<<RANKTX))+1;  // 0x3fff0001  set VIPOKRNKW (by carry into 0) if w byte of allranks is 00
    allranks>>=(VIPOKRNKAX-VIPOKAX); allranks&=~VIPOKW; allranks+=VIPOKW>>1; allranks|=~(VIPOKA+VIPOKW); jtinplace=(J)((I)jtinplace&allranks);  // close up gap betw RNKA and RNKW  allranks free
    ASSERTAGREE(AS(a)+acr, AS(w)+wcr, (shortr>>RANKTX)&RANKTMSK)  // offset to each cellshape, and cellrank(short cell) acr wcr free but were actually spilled earlier
    PRODRNK(n,shortr,AS((I)jtinplace&VIPWCRLONG?w:a)+(shortr>>=(2*RANKTX+1)));  // n is #atoms in excess frame of inner cells, length assigned first shortr free
    // if the cell-shapes don't match, that's an agreement error UNLESS the frame contains 0; in that case it counts as
    // 'error executing on the cell of fills' and produces a scalar 0 as the result for that cell, which we handle by changing the result-cell rank to 0
    // Nonce: continue giving the error even when frame contains 0 - remove 1|| in the next line to conform to fill-cell rules
// this shows the fix   if(ICMP(as+af,ws+wf,MIN(acr,wcr))){if(1||zn)ASSERT(0,EVLENGTH)else r = 0;}
    n^=REPSGN((1-n)&SGNIF((I)jtinplace,VIPWCRLONGX));  // encode 'w has long frame, so a is repeated' as complementary n; but if n<2, leave it alone
    nf=((I)jtinplace>>VIPOKWX)&3;  // extract inplaceability from operation and ranks
    nf=a==w?0:nf;  // not inplaceable if args identical
    nf+=4*nf-16;  // make 2 copies of the 2 bits protect high bits of jtinplace.  This is a long dependency chain through nf but it will overlap the PRODs coming up
    jtinplace = (J)((I)jtinplace&nf);  // bit 2-3=routine/rank/arg inplaceable, 0-1=routine/rank/arg/input inplaceable   nf free
    f=(fr>>(2*RANKTX))&RANKTMSK;  // recover (shorter frame len) from upper fx
    PRODRNK(nf,((fr>>RANKTX)-f),f+AS(((I)jtinplace&VIPWFLONG)?w:a));    // nf=#times shorter-frame cell must be repeated;  offset is (shorter frame len), i. e. loc of excess frame
         // length is (longer frame len)-(shorter frame len)  i. e. length of excess frame
    PRODRNK(mf,fr>>(2*RANKTX),AS(w));  //  mf=#cells in common frame [either arg ok]   f is (shorter frame len)      f free now
    
    // Now nf=outer repeated frame  mf=outer common frame  n=inner repeated frame  m=inner common frame
    //    leading axes --------------------------------------------------------------> trailing axes
    // loop migration: if the outer loop can be subsumed into the inner loop do that to make the faster inner loops more effective
    // the cases we see are:
    // 1: m=1 and n=1: move mf->m, nf->n, and complement n if ak==0 (i. e. x arg is repeated) example:  list *"0 _ atom
    // 2: n=1 and nf=1: multiply m by mf, leave n  example: (shape 4 5) *"1 (shape 4 5 or 1 4 5)
    // 3: m=1 and nf=1: multiply m by mf, leave n  example: (shape 4 5) *"1 0 (shape 5)
    if(unlikely(((nf==1)+(n==1)+(m==1))>1)){  // 2 values=1, can lose a loop
     // migration is possible
     n=(n*nf)^REPSGN(SGNIF(jtinplace,VIPWFLONGX)&(1-nf)); m*=mf;   // propagate mf and nf down; if n is not 1, complement if af<wf
     DPMULDE(nf,mf,mf);  // mf is total # iterations
     DPMULDE(zn,mf,zn)  // total # atoms in result
     mf=1;  // no outer loops.  nf immaterial.  zk does not need to change since it will not be used
    }else{
     // normal case
     DPMULDE(nf,mf,mf); --nf; // mf is total # iterations; nf is outer loop repeat count-1
     DPMULDE(zn,mf,zn)  // total # atoms in result
    }
   }else{
    I af=acr>>(2*RANKTX+1), wf=wcr>>(2*RANKTX+1); acr&=RANKTMSK; wcr&=RANKTMSK;   // separate cr and f for sparse
    fr=acr<wcr?wcr:acr; I f=(af<wf)?wf:af; fr+=(f<<RANKTX)+f; aawwzk[0]=acr; aawwzk[1]=wcr; mf=af; nf=wf;
   }
   // For sparse, repurpose ak/wk/mf/nf to hold acr/wcr/af/wf, which we will pass into vasp.  This allows acr/wcr/af/wf to be block-local
   // Note: this code passed the test suite even when fr was garbage
  }
  // TODO: for 64-bit, we could move f and r into upper jtinplace; use bit 4 of jtinplace for testing below; make f/r block-local; extract f/r below as needed
 }

 RESETRANK;  // Ranks are required for xnum/rat/sparse, which call IRS-enabled routines internally.  We could suppress this for mainline types, perhaps in var().  Anyone who sets this must set it back,
             // so it's OK that we don't clear it if we have error

 // Signal domain error if appropriate. Must do this after agreement tests
 ASSERT(aadocv->f,EVDOMAIN);
 if(likely(jtinplace!=0)){   // if not sparse...
  // Not sparse.

   // If op specifies forced input conversion AND if both arguments are non-sparse: convert them to the selected type.
   // Incompatible arguments were detected in var().  If there is an empty operand, skip conversions which
   // might fail because the type in t is incompatible with the actual type in a.  t is rare.
   //
   // Because of the priority of errors we mustn't check the type until we have verified agreement above
   if(unlikely(((I)jtinplace&VARGMSK)!=0))if(likely(zn>0)){I t=atype((I)jtinplace);  // input conversion required (rare), and the result is not empty
    // Conversions to XNUM use a routine that pushes/sets/pops jt->mode, which controls the
    // type of conversion to XNUM in use.  Any result of the conversion is automatically inplaceable.  If type changes, change the cell-size too, possibly larger or smaller
    // bits 2-3 of jtinplace indicate whether inplaceability is allowed by the op, the ranks, and the addresses
    if(TYPESNE(t,AT(a))){aawwzk[0]=(aawwzk[0]>>bplg(AT(a)))<<bplg(t); aawwzk[1]=(aawwzk[1]>>bplg(AT(a)))<<bplg(t); RZ(a=cvt(t|((I)jtinplace&VARGCVTMSKF),a)); jtinplace = (J)(intptr_t)((I)jtinplace | (((I)jtinplace>>2)&JTINPLACEA));}
    if(TYPESNE(t,AT(w))){aawwzk[2]=(aawwzk[2]>>bplg(AT(w)))<<bplg(t); aawwzk[3]=(aawwzk[3]>>bplg(AT(w)))<<bplg(t); RZ(w=cvt(t|((I)jtinplace&VARGCVTMSKF),w)); jtinplace = (J)(intptr_t)((I)jtinplace | (((I)jtinplace>>2)&JTINPLACEW));}
   }  // It might be better to do the conversion earlier, and defer the error
      // until here.  We will have to look at the generated code when we can use all the registers

   // From here on we have possibly changed the address of a and w, but we are still using shape pointers
   // in the original input block.  That's OK.

   // Allocate a result area of the right type, and copy in its cell-shape after the frame
  // If an argument can be overwritten, use it rather than allocating a new one
  // Argument can be overwritten if: action routine allows it; flagged in jtinplace; usecount 1 or zombie; rank equals (length of longer frame)+(length of longer cell)
  // If the argument has rank that large, and the arguments agree, the argument MUST have the same number of atoms as the result, because all shape is accounted for.
  // rank = rank of result (the rank of the result is the sum of (the longer frame-length) plus (the larger cell-rank))
  // Also, if the operation is one that may abort, we suppress inplacing it if the user can't handle early assignment.
  // Finally, if a==w suppress inplacing, in case the operation must be retried (we could check which ones but they are
  // just not likely to be used reflexively)
  // 
  // If the # atoms is same as result, each atom must be read only once & can be written after it has been read
  // This holds true even if the atoms have different types, as long as they have the same size (this is encoded in the IPOK bits of cv)
  // use the inplacing from the action routine to qualify the caller's inplacing, EXCEPT
  // for Boolean inputs: since we do the operations a word at a time, they may overrun the output area and
  // are thus not inplaceable.  The exception is if there is only one loop through the inputs; that's always inplaceable
// less regs, more comp   {I inplaceallow = (adocv.cv>>VIPOKWX) & (((a==w)|((f!=0)&zt))-1) & ((((zn^an)|(ar^(f+r)))==0)*2 + (((zn^wn)|(wr^(f+r)))==0));
   
      // also turn off inplacing if a==w  or if Boolean with repeated cells   uses B01==1
      // (mf|nf)>1 is a better test than f!=0, because it handles frame of all 1s, but it's slower in the normal case

   // Establish the result area z; if we're reusing an argument, make sure the type is updated to the result type
   // If the operation is one that can fail partway through, don't allow it to overwrite a zombie input unless so enabled by the user
  // The ordering here assumes that jtinplace will usually be set
// alternative  if(((I)jtinplace&1) && (AC(w)<ACUC1 || AC(w)==ACUC1&&jt->assignsym&&jt->assignsym->val==w&&!(adocv.cv&VCANHALT && jt->asgzomblevel<2))){z=w; if(TYPESNE(AT(w),zt))MODBLOCKTYPE(z,zt)  //  Uses JTINPLACEW==1
// alternative  }else if(((I)jtinplace&2) && (AC(a)<ACUC1 || AC(a)==ACUC1&&jt->assignsym&&jt->assignsym->val==a&&!(adocv.cv&VCANHALT && jt->asgzomblevel<2))){z=a; if(TYPESNE(AT(a),zt))MODBLOCKTYPE(z,zt)  //  Uses JTINPLACEA==2
  if(ASGNINPLACESGN(SGNIF(jtinplace,JTINPLACEWX),w)){z=w; I zt=rtype((I)jtinplace); if(unlikely(TYPESNE(AT(w),zt)))MODBLOCKTYPE(z,zt)  //  Uses JTINPLACEW==1
  }else if(ASGNINPLACESGN(SGNIF(jtinplace,JTINPLACEAX),a)){z=a; I zt=rtype((I)jtinplace); if(unlikely(TYPESNE(AT(a),zt)))MODBLOCKTYPE(z,zt)  //  Uses JTINPLACEA==2
#define scell AS((I)jtinplace&VIPWCRLONG?w:a)+(fr>>(3*RANKTX))  // address of start of cell shape     shape of long cell+frame(long cell)
    // fr is (frame(long cell))  /  (shorter frame len)   /  (longer frame len)                      /   (longer frame len+longer celllen)
  }else{GA(z,rtype((I)jtinplace),zn,(RANKT)fr,0); MCISH(AS(z),AS(((I)jtinplace&VIPWFLONG)?w:a),(fr>>RANKTX)&RANKTMSK); MCISH(AS(z)+((fr>>RANKTX)&RANKTMSK),scell,(fr&RANKTMSK)-((fr>>RANKTX)&RANKTMSK));} 
//                                                 frame loc     shape of long frame             len of long frame           cellshape loc              cellshape     longer cellen 
  // fr free
  if(unlikely(zn==0)){RETF(z);}  // If the result is empty, the allocated area says it all
  // zn  NOT USED FROM HERE ON

  // End of setup phase.  The execution phase:

  // The compiler thinks that because ak/wk/zk are used in the loop they should reside in registers.  We do better to keep a and w in registers.  So we
  // force the compiler to spill aawwzk by using address arithmetic.
  {I rc=EVOK;
   {  // lowest rc from the executed sections
    C *av=CAV(a); C *wv=CAV(w); C *zv=CAV(z);  // point to the data
    // Call the action routines: nf,mf, etc must be preserved in case of repair
     // mf has the total number of calls.  nf is 1 less than the number of calls with a repeated cell.  aawwzk[0,1] are the cell-size of a for the outer loop, aawwzk[2,3] are for w;
     // but aawwzk[1,3] have 0 in a repeated argument.  aawwzk[1,3] are added for each inner iteration, aawwzk[0,2] at the end of an inner cycle
     // m is the number of outer loops the caller will run
     // n is the number of times the inner-loop atom is repeated for each outer loop: n=1 means no inner loop needed; n>1 means each atom of y is repeated n times; n<0 means each atom of x is repeated ~n times.  n*m cannot=0. 
   I i=mf; I jj=nf; lp000: {I lrc=((AHDR2FN*)aadocv->f)(n,m,av,wv,zv,jt); rc=lrc<rc?lrc:rc; if(unlikely(--i!=0)){ I jj1=--jj; jj=jj<0?nf:jj; av+=aawwzk[1+REPSGN(jj1)]; wv+=aawwzk[3+REPSGN(jj1)]; zv+=aawwzk[4]; goto lp000; }}  // jj1 is -1 on the last inner iter, where we use outer incr
   }

   // The work has been done.  If there was no error, check for optional conversion-if-possible or -if-necessary
   if(likely(rc==EVOK)){if(unlikely((I)jtinplace&VRI+VRD))z=cvz((I)jtinplace,z); RETF(z);  // normal return is here.  The rest is error recovery
   }else if(rc-EWOVIP>=0){A zz;C *zzv;I zzk;
    // Here for overflow that can be corrected in place
// not yet    if(rc==EVOKCLEANUP){jt->mulofloloc=0; RETF(z);}  // if multiply that did not overflow, clear the oflo position for next time, and return
    // If the original result block cannot hold the D result, allocate a separate result area
    if(sizeof(D)==sizeof(I)){zz=z; MODBLOCKTYPE(zz,FL); zzk=aawwzk[4];   // shape etc are already OK
    }else{GATV(zz,FL,AN(z),AR(z),AS(z)); zzk=aawwzk[4]*(sizeof(D)/sizeof(I));}
    // restore pointers to beginning of arguments
    zzv=CAV(zz);  // point to new-result data
    // Set up pointers etc for the overflow handling.  Set b=1 if w is taken for the x argument to repair
    if(rc==EWOVIP+EWOVIPMULII){D *zzvd=(D*)zzv; I *zvi=IAV(z);
     // Multiply repair.  We have to convert all the pre-overflow results to float, and then finish the multiplies
     jt->mulofloloc = ~jt->mulofloloc;  // make length positive
     DQ(jt->mulofloloc, *zzvd++=(D)*zvi++;);  // convert the multiply results to float.  mulofloloc is known negative, and must be complemented
     // Now repeat the processing.  Unlike with add/subtract overflow, we have to match up all the argument atoms
     {C *av=CAV(a); C *wv=CAV(w);
      I i=mf; I jj=nf; while(1){tymesIIO(n,m,(I*)av,(I*)wv,(D*)zzv,jt); if(!--i)break; zzv+=zzk; I jj1=--jj; jj=jj<0?nf:jj; av+=aawwzk[1+REPSGN(jj1)]; wv+=aawwzk[3+REPSGN(jj1)];}  // jj1 is -1 on the last inner iter, where we use outer incr
     }
// not yet     jt->mulofloloc=0;  // reinit for next time
    } else {   // not multiply repair, but something else to do inplace
     adocv.f = repairip[(rc-EWOVIP)&3];   // fetch ep from table
     I nipw = ((z!=w) & (rc-EWOVIP)) ^ (((rc-EWOVIP)>>2) & 1);  // nipw from z!=w if bits2,0==01; 1 if 10; 0 if 00
     // nipw means 'use w as not-in-place'; c means 'repeat cells of a'; so if nipw!=c we repeat cells of not-in-place, if nipw==c we set nf to 1
     // if we are repeating cells of the not-in-place, we leave the repetition count in nf, otherwise subsume it in mf
     // b means 'repeat atoms inside a'; so if nipw!=b we repeat atoms of not-in-place, if nipw==b we set n to 1
     {C *av, *zv=CAV(z);
      // zv and zzv process without repeats; they contain all the information for the in-place argument (if any).
      // av may have repeats.  Repeats before the function call are handled exactly as the first time through, by using aawwzk.
      // repeats inside the function call (from n) must appear only on a, i. e. n<0 to repeat a, or n==1 for no repeat
      // if the original internal repeat was on the argument that is now z (e. g. nipw is set, meaning original a is going to z, and n is negative, meaning repeat inside a)
      // we must multiply out the repeat to leave n=1.
      av=CAV(nipw?w:a);  // point to the not-in-place argument
      I nsgn=SGNTO0(n); n^=REPSGN(n); if(nipw==nsgn){m *= n; n = 1;} n^=-nipw;  // force n to <=1; make n flag indicate whether args were switched
      I i=mf; I jj=nf; while(1){((AHDR2FN*)adocv.f)(n,m,av,zv,zzv,jt); if(!--i)break; zv+=aawwzk[4]; zzv+=zzk; I jj1=--jj; jj=jj<0?nf:jj; av+=aawwzk[2*nipw+1+REPSGN(jj1)];}  // jj1 is -1 on the last inner iter, where we use outer incr
     }
    }
    R zz;  // Return the result after overflow has been corrected
   }
   // retry required, not inplaceable.  Signal the error code to the caller.  If the error is not retryable, set the error message
   if(rc<=NEVM)jsignal(rc);else jt->jerr=(UC)rc;
  }
 }else{z=vasp(a,w,FAV(self)->id,aadocv->f,aadocv->cv,atype(aadocv->cv),rtype(aadocv->cv),mf,aawwzk[0],nf,aawwzk[1],fr>>RANKTX,(RANKT)fr-(fr>>RANKTX)); if(!jt->jerr)R z;}  // handle sparse arrays separately.
 R 0;  // return to the caller, who will retry any retryable errors
}    /* scalar fn primitive and f"r main control */


// 4-nested loop for dot-products.  Handles repeats for inner and outer frame.  oneprod is the code for calculating a single vector inner product *zv++ = *av++ dot *wv++
// If there is inner frame, it is the a arg that is repeated
// LIT is set in it if it is OK to use 2x2 operations (viz a has no inner frame & w has no outer frame)
#define SUMATLOOP2(ti,to,oneprod2,oneprod1) \
  {ti * RESTRICT av=avp,* RESTRICT wv=wvp; to * RESTRICT zv=zvp; \
   _mm256_zeroupper(VOIDARG); \
   __m256i endmask = _mm256_loadu_si256((__m256i*)(validitymask+((-dplen)&(NPAR-1))));  /* mask for 00=1111, 01=1000, 10=1100, 11=1110 */ \
   __m256d acc000; __m256d acc010; __m256d acc100; __m256d acc110; \
   __m256d acc001; __m256d acc011; __m256d acc101; __m256d acc111; \
   DQ(nfro, I jj=nfri; ti *ov0=it&BOX?av:wv; \
    while(1){  \
     DQ(ndpo, I j=ndpi; ti *av0=av; /* i is how many a's are left, j is how many w's*/ \
      while(1){ \
       if(it&LIT&&jj>1){ \
        ti * RESTRICT wv1=wv+dplen; wv1=j==1?wv:wv1; \
        oneprod2  \
        if(j>1){--j; _mm_storeu_pd(zv,_mm256_castpd256_pd128 (acc000)); _mm_storeu_pd(zv+ndpi,_mm256_castpd256_pd128 (acc100)); wv+=dplen; zv +=2;} \
        else{_mm_storel_pd(zv,_mm256_castpd256_pd128 (acc000)); _mm_storel_pd(zv+ndpi,_mm256_castpd256_pd128 (acc100));  zv+=1;} \
       }else{ \
        oneprod1  \
        _mm_storel_pd(zv,_mm256_castpd256_pd128 (acc000)); \
        zv+=1; \
       } \
       if(!--j)break; \
       av=av0;  \
      } \
      if(it&LIT&&jj>1){--i; av+=dplen; zv+=ndpi;} \
     ) \
     if((jj-=(((it&LIT)>>1)+1))<=0)break; \
     if(it&BOX)av=ov0;else wv=ov0; \
    } \
   ) \
  }

#define SUMATLOOP(ti,to,oneprod) \
  {ti * RESTRICT av=avp,* RESTRICT wv=wvp; to * RESTRICT zv=zvp; \
   DQ(nfro, I jj=nfri; ti *ov0=it&BOX?av:wv; \
    while(1){  \
     DQ(ndpo, I j=ndpi; ti *av0=av; /* i is how many a's are left, j is how many w's*/ \
      while(1){oneprod if(!--j)break; av=av0;} \
     ) \
     if(!--jj)break; \
     if(it&BOX)av=ov0;else wv=ov0; } \
   ) \
  }

// 

#define ONEPRODD D total0=0.0; D total1=0.0; if(dplen&1)total1=(D)*av++*(D)*wv++; DQ(dplen>>1, total0+=(D)*av++*(D)*wv++; total1+=(D)*av++*(D)*wv++;); *zv++=total0+total1;

// routine to do the dot-product calculations.  Brought out to help the compiler allocate registers
// it=type of input, a,w=args dplen=len of each dot-product
// ndpi is the number of times to repeat each list of a within the inner loop, i. e. # dps to repeat it on
// ndpo is */ inner frame of w, i. e. the number of times to repeat the inner loop
// nfri is the number of times to repeat the short-frame operand for the outer loop
// nfri is */ surplus outer frame
// it&BOX is set if a has the shorter outer frame
// w is never repeated in the inner loop (i. e. you can have multiple w but not multiple a; exchange args to ensure this
// if LIT is set in it, it is OK to use 2x2 operations (viz inner frame of a and outer frame of w are empty)
// 'repeata' flag comes from it&BOX
I jtsumattymesprods(J jt,I it,void *avp, void *wvp,I dplen,I nfro,I nfri,I ndpo,I ndpi,void *zvp){
 if(it&FL){
  NAN0;
  SUMATLOOP(D,D,ONEPRODD)
  if(NANTEST){  // if there was an error, it might be 0 * _ which we will turn to 0.  So rerun, checking for that.
   NAN0;
   SUMATLOOP(D,D,D total=0.0; DQ(dplen, D u=*av++; D v=*wv++; if(u&&v)total+=dmul2(u,v);); *zv++=total;)
   NAN1;
  }
 }else if(it&INT){
  SUMATLOOP(I,D,D total0=0.0; D total1=0.0; if(dplen&1)total1=(D)*av++*(D)*wv++; DQ(dplen>>1, total0+=(D)*av++*(D)*wv++; total1+=(D)*av++*(D)*wv++;); *zv++=total0+total1;)
 }else{
  SUMATLOOP(B,I,
   I total=0; I k=dplen; I *avi=(I*)av; I *wvi=(I*)wv;
    while(k>>LGSZI){I kn=MIN(255,k>>LGSZI); k-=kn<<LGSZI; I total2=0; DQ(kn, total2+=*avi++&*wvi++;) ADDBYTESINI(total2); total+=total2;} av=(B*)avi; wv=(B*)wvi; DQ(k, total+=*av++&*wv++;)
   *zv++=total;
  )
 }
 R 1;
}



// +/@:*"1 with IRS
DF2(jtsumattymes1){
 ARGCHK2(a,w);
 I ar=AR(a); I wr=AR(w); I acr=jt->ranks>>RANKTX; I wcr=jt->ranks&RMAX;
 // get the cell-ranks to use 
 acr=ar<acr?ar:acr;   // r=left rank of verb, acr=effective rank
 wcr=wr<wcr?wr:wcr;  // r=right rank of verb, wcr=effective rank
     // note: the prod above can never fail, because it gives the actual # cells of an existing noun
   // Now that we have used the rank info, clear jt->ranks.  All verbs start with jt->ranks=RMAXX unless they have "n applied
   // we do this before we generate failures
 RESETRANK;  // This is required if we go to slower code
 // if an argument is empty, sparse, has cell-rank 0, or not a fast arithmetic type, revert to the code for f/@:g atomic
 if(((-((AT(a)|AT(w))&(NOUN&~(B01|INT|FL))))|(AN(a)-1)|(AN(w)-1)|(acr-1)|(wcr-1))<0) { // test for all unusual cases
  R rank2ex(a,w,FAV(self)->fgh[0],MIN(acr,1),MIN(wcr,1),acr,wcr,jtfslashatg);
 }
 // We can handle it here, and both ranks are at least 1.

 // If there is no additional rank (i. e. highest cell-rank is 1), ignore the given rank (which must be 1 1) and use the argument rank
 // This promotes the outer loops to inner loops
 {I rankgiven = (acr|wcr)-1; acr=rankgiven?acr:ar; wcr=rankgiven?wcr:wr;}

 // Exchange if needed to make the cell-rank of a no greater than that of w.  That way we know that w will never repeat in the inner loop
 if(acr>wcr){A t=w; I tr=wr; I tcr=wcr; w=a; wr=ar; wcr=acr; a=t; ar=tr; acr=tcr;}

 // Convert arguments as required
 I it=MAX(AT(a),AT(w));  // if input types are dissimilar, convert to the larger
 if(it!=(AT(w)|AT(a))){
  if(TYPESNE(it,AT(a))){RZ(a=cvt(it,a));}  // convert to common input type
  else if(TYPESNE(it,AT(w))){RZ(w=cvt(it,w));}
 }

 // Verify inner frames match
 ASSERTAGREE(AS(a)+ar-acr, AS(w)+wr-wcr, acr-1) ASSERT(AS(a)[ar-1]==AS(w)[wr-1],EVLENGTH);  // agreement error if not prefix match

 // calculate inner repeat amounts and result shape
 I dplen = AS(a)[ar-1];  // number of atoms in 1 dot-product
 I ndpo; PROD(ndpo,acr-1,AS(w)+wr-wcr);  // number of cells of a = # 2d-level loops
 I ndpi; PROD(ndpi,wcr-acr,AS(w)+wr-wcr+acr-1);  // number of times each cell of a must be repeated (= excess frame of w)
 I zn=ndpo*ndpi;  // number of results from 1 inner cell.  This can't overflow since frames agree and operands are not empty

 A z; 
 // if there is frame, create the outer loop values
 I nfro,nfri;  // outer loop counts, and which arg is repeated
 if(((ar-acr)|(wr-wcr))==0){  // normal case
  nfro=nfri=1;  // no outer loops, repeata immaterial
  GA(z,FL>>(it&B01),ndpo*ndpi,wcr-1,AS(w));  // type is INT if inputs booleans, otherwise FL
 }else{
  // There is frame, analyze and check it
  I af=ar-acr; I wf=wr-wcr; I commonf=wf; I *as=AS(a), *ws=AS(w); I *longs=as;
  it|=(ndpo==1)>wf?LIT:0;  // if there is no inner frame for a, and no outer frame for w, signal OK to use 2x2 multiplies.  Mainly this is +/@:*"1/
  commonf=wf>=af?af:commonf; longs=wf>=af?ws:longs; it|=wf>=af?BOX:0;  // repeat flag, length of common frame, pointer to long shape
  af+=wf; af-=2*commonf;  // repurpose af to be length of surplus frame
  ASSERTAGREE(as,ws,commonf)  // verify common frame
  PROD(nfri,af,longs+commonf); PROD(nfro,commonf,longs);   // number of outer loops, number of repeats
  I zn = ndpo*ndpi*nfro; RE(zn=mult(zn,nfri));  // no error possible till we extend the shape
  GA(z,FL>>(it&B01),zn,af+commonf+wcr-1,0); I *zs=AS(z);  // type is INT if inputs booleans, otherwise FL
  // install the shape
  MCISH(zs,longs,af+commonf); MCISH(zs+af+commonf,ws+wr-wcr,wcr-1);
 }

 RZ(jtsumattymesprods(jt,it,voidAV(a),voidAV(w),dplen,nfro,nfri,ndpo,ndpi,voidAV(z)));  // eval, check for error
 RETF(z);
}


static A jtsumattymes(J jt, A a, A w, I b, I t, I m, I n, I nn, I r, I *s, I zn){A z;
 ARGCHK2(a,w);
 switch(UNSAFE(t)){
 case B01:  // the aligned cases are handled elsewhere, a word at a time
  {B*av=BAV(a),u,*wv=BAV(w);I*zu,*zv;
   GATV(z,INT,zn,r-1,1+s); zu=AV(z);
   if(1==n){
             zv=zu; DQ(m,                     *zv++ =*av++**wv++;);
    DQ(nn-1, zv=zu; DQ(m,                     *zv+++=*av++**wv++;););
   }else{if(!b){B* tv=av; av=wv; wv=tv;}
             zv=zu; DQ(m, u=*av++;      DQ(n, *zv++ =u**wv++;););
    DQ(nn-1, zv=zu; DQ(m, u=*av++; if(u)DQ(n, *zv+++=u**wv++;) else wv+=n;););
   }
  }
  break;
 case FL:   
  {D*av=DAV(a),u,v,*wv=DAV(w),*zu,*zv;
   GATV(z,FL,zn,r-1,1+s); zu=DAV(z);
   NAN0;
   // First, try without testing for 0*_ .  If we hit it, it will raise NAN
   if(1==n){
             zv=zu; DQ(m, u=*av++;            v=*wv++; *zv++ =u*v; );
    DQ(nn-1, zv=zu; DQ(m, u=*av++;            v=*wv++; *zv+++=u*v; ););
   }else{if(!b){zv=av; av=wv; wv=zv;}
             zv=zu; DQ(m, u=*av++; DQ(n, v=*wv++; *zv++ =u*v;););
    DQ(nn-1, zv=zu; DQ(m, u=*av++; DQ(n, v=*wv++; *zv+++=u*v;)););
   }
   if(NANTEST){av-=m*nn;wv-=m*nn*n; // try again, testing for 0*_
    NAN0;
    if(1==n){
              zv=zu; DQ(m, u=*av++;            v=*wv++; *zv++ =u&&v?dmul2(u,v):0;  );
     DQ(nn-1, zv=zu; DQ(m, u=*av++;            v=*wv++; *zv+++=u&&v?dmul2(u,v):0;  ););
    }else{   // don't swap again
              zv=zu; DQ(m, u=*av++;      DQ(n, v=*wv++; *zv++ =u&&v?dmul2(u,v):0;););
     DQ(nn-1, zv=zu; DQ(m, u=*av++; if(u)DQ(n, v=*wv++; *zv+++=   v?dmul2(u,v):0;) else wv+=n;););
    }
    NAN1;
   }
  }
 }
 RETF(z);
}    /* a +/@:* w for non-scalar a and w */


#define SUMBFLOOPW(BF)     \
 {DO(q, memset(tv,C0,p); DO(255, DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);); DO(zn,zv[i]+=tu[i];));  \
        memset(tv,C0,p); DO(r,   DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);); DO(zn,zv[i]+=tu[i];) ;  \
 }
#define SUMBFLOOPX(BF)     \
 {DO(q, memset(tv,C0,p); DO(255, DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);                           \
                               av+=zn; u=(UI*)av; wv+=zn; v=(UI*)wv;); DO(zn,zv[i]+=tu[i];));  \
        memset(tv,C0,p); DO(r,   DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);                           \
                               av+=zn; u=(UI*)av; wv+=zn; v=(UI*)wv;); DO(zn,zv[i]+=tu[i];) ;  \
 }
#if SY_ALIGN
#define SUMBFLOOP(BF)   SUMBFLOOPW(BF)
#else
#define SUMBFLOOP(BF)   if(zn%SZI)SUMBFLOOPX(BF) else SUMBFLOOPW(BF)
#endif

static A jtsumatgbool(J jt,A a,A w,C id){A t,z;B* RESTRICTI av,* RESTRICTI wv;I dw,n,p,q,r,*s,zn,* RESTRICT zv;UC* RESTRICT tu;UI* RESTRICTI tv,*u,*v;
 ARGCHK2(a,w);
 s=AS(w); n=*s;
 zn=AN(w)/n; dw=(zn+SZI-1)>>LGSZI; p=dw*SZI;
 q=n/255; r=n%255;
 GATV(z,INT,zn,AR(w)-1,1+s); zv=AV(z); memset(zv,C0,zn*SZI);
 GATV0(t,INT,dw,1); tu=UAV(t); tv=(UI*)tu;
 av=BAV(a); u=(UI*)av; 
 wv=BAV(w); v=(UI*)wv;
 switch(id){
  case CMIN: 
  case CSTAR: 
  case CSTARDOT: SUMBFLOOP(AND ); break;
  case CMAX:
  case CPLUSDOT: SUMBFLOOP(OR  ); break;
  case CEQ:      SUMBFLOOP(EQ  ); break;
  case CNE:      SUMBFLOOP(NE  ); break;
  case CSTARCO:  SUMBFLOOP(NAND); break;
  case CPLUSCO:  SUMBFLOOP(NOR ); break;
  case CLT:      SUMBFLOOP(LT  ); break;
  case CLE:      SUMBFLOOP(LE  ); break;
  case CGT:      SUMBFLOOP(GT  ); break;
  case CGE:      SUMBFLOOP(GE  ); break;
 }
 RETF(z);
}    /* a +/@:g w  for boolean a,w where a-:&(* /@$)w; see also plusinsB */

DF2(jtfslashatg){A fs,gs,y,z;B b,sb=0;C*av,c,d,*wv;I ak,an,ar,*as,at,m,
     n,nn,r,rs,*s,t,wk,wn,wr,*ws,wt,yt,zn,zt;VA2 adocv,adocvf;
 ARGCHK3(a,w,self);
 an=AN(a); ar=AR(a); as=AS(a); at=AT(a); at=an?at:B01;
 wn=AN(w); wr=AR(w); ws=AS(w); wt=AT(w); wt=wn?wt:B01;
 b=ar<=wr; r=b?wr:ar; rs=b?ar:wr; s=b?ws:as; nn=s[0]; nn=r?nn:1;  // b='w has higher rank'; r=higher rank rs=lower rank s->longer shape  nn=#items in longer-shape arg
 ASSERTAGREE(as,ws,MIN(ar,wr));
 {I isfork=CFORK==FAV(self)->id; fs=FAV(self)->fgh[0+isfork]; gs=FAV(self)->fgh[1+isfork];}   // b=0 if @:, 1 if fork; take fs,gs accordingly
 if(SPARSE&(at|wt)||!an||!wn||2>nn){ R df1(z,df2(y,a,w,gs),fs);}  // if sparse or empty, or just 1 item, do it the old-fashioned way
 rs=MAX(1,rs); PROD(m,rs-1,s+1); PROD(n,r-rs,s+rs); zn=m*n;   // zn=#atoms in _1-cell of longer arg = #atoms in result; m=#atoms in _1-cell of shorter arg  n=#times to repeat shorter arg  (*/ surplus longer shape)
   // if the short-frame arg is an atom, move its rank to 1 so we get the lengths of the _1-cells of the replicated arguments
 y=FAV(fs)->fgh[0]; c=ID(y); d=ID(gs);
 if(c==CPLUS){
  // +/@:g is special if args are boolean, length is integral number of I, and g is boolean or *
  if((((at&wt&(n==1))>(zn&(SZI-1)))||!SY_ALIGN)){   //  relies on B01==1
#define sumbfvalues(w) CCM(w,CGE)+CCM(w,CLE)+CCM(w,CGT)+CCM(w,CLT)+CCM(w,CPLUSCO)+CCM(w,CSTARCO)+CCM(w,CNE)+CCM(w,CEQ)+ \
 CCM(w,CSTARDOT)+CCM(w,CPLUSDOT)+CCM(w,CMIN)+CCM(w,CMAX)+CCM(w,CSTAR)
   CCMWDS(sumbf) CCMCAND(sumbf,cand,d) if(CCMTST(cand,d))R sumatgbool(a,w,d);   // quickly handle verbs that have primitive inverses
  }
  if(d==CSTAR){
   if(!ar||!wr){  // if either argument is atomic, apply the distributive property to save multiplies
    A z0; z=!ar?tymes(a,df1(z0,w,fs)):tymes(w,df1(z0,a,fs));
    if(jt->jerr==EVNAN)RESETERR else R z;
   }else if(TYPESEQ(at,wt)&&at&B01+FL)R jtsumattymes(jt,a,w,b,at,m,n,nn,r,s,zn);  // +/@:*
  }
 }
 adocv=var(gs,at,wt); ASSERT(adocv.f,EVDOMAIN); yt=rtype(adocv.cv ); t=atype(adocv.cv);
 adocvf=var(y,yt,yt); ASSERT(adocvf.f,EVDOMAIN); zt=rtype(adocvf.cv);
 sb=yt&(c==CPLUS);  // +/@:g where g produces Boolean.
 if(!(sb||TYPESEQ(yt,zt)))R df1(z,df2(y,a,w,gs),fs);
 if(t){
  if(TYPESNE(t,at))RZ(a=cvt(t|(adocv.cv&VARGCVTMSKF),a));
  if(TYPESNE(t,wt))RZ(w=cvt(t|(adocv.cv&VARGCVTMSKF),w));
 }
 ak=b?m:zn; wk=b?zn:m; ak=an<nn?0:ak; wk=wn<nn?0:wk; ak<<=bplg(AT(a));wk<<=bplg(AT(w));
 GA(y,yt,zn,1,0);  // allocate one item for result of g
 GA(z,zt,zn,r-1,1+s);  // allocate main output area for final result from f/
 n^=-b; n=(n==~1)?1:n;  // encode b flag in sign of n
 if(sb){A t;I j,tn,*zv;UC*tc;UI*ti,*yv;  /* +/@:g for boolean-valued g */
  av=CAV(a); wv=CAV(w); yv=(UI*)AV(y); zv=AV(z); memset(zv,C0,zn*SZI);
  tn=(zn+SZI-1)>>LGSZI; GATV0(t,INT,tn,1); tc=UAV(t); ti=(UI*)tc;
  // Run g in batches of up to 255, accumulating the result bytewise.  NOTE: there may be garbage at the end of yv, but because
  // we are supporting littleendian only, it will not affect the result
  for(j=nn;0<j;j-=255){
   memset(ti,C0,tn*SZI); 
   DO(MIN(j,255), ((AHDR2FN*)adocv.f)(n,m,av,wv,yv,jt); av+=ak; wv+=wk; DO(tn,ti[i]+=yv[i];););
   DO(zn, zv[i]+=tc[i];);
  }
 }else{A z1;B p=0;C*yv,*zu,*zv;  // general f/@:g.  Do not run g on entire y; instead run one cell at a time
  av=CAV(a)+ak*(nn-1); wv=CAV(w)+wk*(nn-1); yv=CAV(y); zv=CAV(z);
  GA(z1,zt,zn,r-1,1+s); zu=CAV(z1);  // allocate ping-pong output area for f/
  I rc;  // accumulate error returns
  rc=((AHDR2FN*)adocv.f)(n,m,av,wv,zv,jt);  // create first result-cell of g
  DQ(nn-1, av-=ak; wv-=wk; I lrc; lrc=((AHDR2FN*)adocv.f)(n,m,av,wv,yv,jt); rc=lrc<rc?lrc:rc; lrc=((AHDR2FN*)adocvf.f)((I)1,zn,yv,p?zu:zv,p?zv:zu,jt); rc=lrc<rc?lrc:rc; p^=1;);  // p==1 means result goes to ping buffer zv
  if(NEVM<(rc&255)){df1(z,df2(y,a,w,gs),fs);}else{if(rc&255)jsignal(rc); z=p?z1:z;}  // if overflow, revert to old-fashioned way.  If p points to ping, prev result went to pong, make pong the result
 }
 RE(0); RETF(z);
}    /* a f/@:g w where f and g are atomic*/

// Consolidated entry point for ATOMIC2 verbs.  These can be called with self pointing either to a rank block or to the block for
// the atomic.  If the block is a rank block, we will switch self over to the block for the atomic.
// Rank can be passed in via jt->ranks, or in the rank for self.  jt->ranks has priority.
// This entry point supports inplacing
DF2(jtatomic2){A z;
 A realself=FAV(self)->fgh[0];  // if rank operator, this is nonzero and points to the left arg of rank
 RANK2T selfranks=FAV(self)->lrr;  // get left & right rank from rank/primitive
 self=realself?realself:self;  // if this is a rank block, move to the primitive.  u b. or any atomic primitive has f clear
 F2PREFIP;ARGCHK2(a,w);
 RANK2T jtranks=jt->ranks;  // fetch IRS ranks if any
 UI ar=AR(a), wr=AR(w), awr=(ar<<RANKTX)+wr; I awm1=(AN(a)-1)|(AN(w)-1);
 selfranks=jtranks==(RANK2T)~0?selfranks:jtranks;
 // check for singletons
 if(!(awm1|((AT(a)|AT(w))&(NOUN&UNSAFE(~(B01+INT+FL)))))){
  z=jtssingleton(jtinplace,a,w,self,(RANK2T)awr,selfranks);
  if(likely(z!=0)){RETF(z);}  // normal case is good return
  if(unlikely(jt->jerr<=NEVM)){RETF(z);}   // if error is unrecoverable, don't retry
  // if retryable error, fall through.  The retry will not be through the singleton code
  jtinplace=(J)((I)jtinplace|JTRETRY);  // indicate that we are retrying the operation.  We must, because jt->jerr is set with the retry code
 }
 // while it's convenient, check for empty result
 jtinplace=(J)((I)jtinplace+(((SGNTO0(awm1)))<<JTEMPTYX));
 // find frame
 I af=(I)(ar-((UI)selfranks>>RANKTX)); af=af<0?0:af;  // framelen of a
 I wf=(I)(wr-((UI)selfranks&RANKTMSK)); wf=wf<0?0:wf;  // framelen of w
 // if there is no frame wrt rank, shift down to working on frame wrt 0.  Set selfranks=0 to signal that case.  It uses simpler setup
 selfranks=af+wf==0?0:selfranks; af=af+wf==0?ar:af; wf=selfranks==0?wr:wf;  // the conditions had to be like this to prevent a jmp
 af=af<wf?af:wf;   // now af is short frame
 ASSERTAGREE(AS(a),AS(w),af);  // outermost (or only) agreement check
 // Run the full dyad, retrying if a retryable error is returned
 z=jtva2(jtinplace,a,w,self,(awr<<RANK2TX)+selfranks);  // execute the verb
 if(likely(z!=0)){RETF(z);}  // normal case is good return
 if(unlikely(jt->jerr<=NEVM)){RETF(z);}   // if error is unrecoverable, don't retry
 R z=jtva2((J)((I)jtinplace|JTRETRY),a,w,self,(awr<<RANK2TX)+selfranks);  // execute the verb
}

DF2(jtexpn2  ){F2PREFIP; ARGCHK2(a,w); if(unlikely(((((I)AR(w)-1)&SGNIF(AT(w),FLX))<0)))if(unlikely(0.5==DAV(w)[0]))R sqroot(a);  R jtatomic2(jtinplace,a,w,self);}  // use sqrt hardware for sqrt.  Only for atomic w. 
DF2(jtresidue){F2PREFIP; ARGCHK2(a,w); I intmod; if(!((AT(a)|AT(w))&(NOUN&~INT)|AR(a))&&(intmod=IAV(a)[0], (intmod&-intmod)+(intmod<=0)==0))R intmod2(w,intmod); R jtatomic2(jtinplace,a,w,self);}


// These are the unary ops that are implemented using a canned argument
// NOTE that they pass through inplaceability

// Shift the w-is-inplaceable flag to a.  Bit 1 is known to be 0 in any call to a monad
#define IPSHIFTWA (jt = (J)(intptr_t)(((I)jt+JTINPLACEW)&-JTINPLACEA))

// We use the right type of singleton so that we engage AVX loops
#define SETCONPTR(n) A conptr=num(n); A conptr2=zeroionei(n); A conptr3=numvr(n); conptr=AT(w)&INT?conptr2:conptr; conptr=AT(w)&FL?conptr3:conptr;  // for 0 or 1 only
#define SETCONPTR2(n) A conptr=num(n); A conptr3=numvr(n); conptr=AT(w)&FL?conptr3:conptr;   // used for 2, when the only options are INT/FL

F1(jtnot   ){ARGCHK1(w); SETCONPTR(1) R AT(w)&B01+SB01?eq(num(0),w):minus(conptr,w);}
F1(jtnegate){ARGCHK1(w); SETCONPTR(0) R minus(conptr,w);}
F1(jtdecrem){ARGCHK1(w); SETCONPTR(1) IPSHIFTWA; R minus(w,conptr);}
F1(jtincrem){ARGCHK1(w); SETCONPTR(1) R plus(conptr,w);}
F1(jtduble ){ARGCHK1(w); SETCONPTR2(2) R tymes(conptr,w);}
F1(jtsquare){ARGCHK1(w); R tymes(w,w);}   // leave inplaceable in w only  ?? never inplaces
F1(jtrecip ){ARGCHK1(w); SETCONPTR(1) R divide(conptr,w);}
F1(jthalve ){ARGCHK1(w); if(!(AT(w)&XNUM+RAT))R tymes(onehalf,w); IPSHIFTWA; R divide(w,num(2));} 

static AHDR2(zeroF,B,void,void){memset(z,C0,m*(n^REPSGN(n)));R EVOK;}
static AHDR2(oneF,B,void,void){memset(z,C1,m*(n^REPSGN(n)));R EVOK;}

// table of routines to handle = ~:
static VF eqnetbl[2][16] = {
//    11        12        14        BX        21        22        24       x       41        42        44        x      x      x      SB      INHOMO  // char len of aw, or HOMO SB BX
{ (VF)eqCC, (VF)eqCS, (VF)eqCU, (VF)eqAA, (VF)eqSC, (VF)eqSS, (VF)eqSU, (VF)0, (VF)eqUC, (VF)eqUS, (VF)eqUU, (VF)0, (VF)0, (VF)0, (VF)eqII, (VF)zeroF },
{ (VF)neCC, (VF)neCS, (VF)neCU, (VF)neAA, (VF)neSC, (VF)neSS, (VF)neSU, (VF)0, (VF)neUC, (VF)neUS, (VF)neUU, (VF)0, (VF)0, (VF)0, (VF)neII, (VF)oneF },
};

// Analyze the verb and arguments and come up with *ado, address of the routine to handle one
// list of arguments producing a list of results; and *cv, the conversion control which specifies
// the precision inputs must be converted to, and what the result type will be.
// The flags in cv have doubled letters (e.g. VDD) for input precision, single letters (e. g. VD) for result
// result is a VA2 struct  containing ado and cv.  If failure, ado is 0 and the caller should signal domain error

VA2 jtvar(J jt,A self,I at,I wt){I t;
 // If there is a pending error, it might be one that can be cured with a retry; for example, fixed-point
 // overflow, where we will convert to float.  If the error is one of those, get the routine and conversion
 // for it, and return.
 if(!jt->jerr){
  // Normal case where we are not retrying: here for numeric arguments
  // vaptr converts the character pseudocode into an entry in va;
  // that entry contains 34 (ado,cv) pairs, indexed according to verb/argument types.
  // the first 9 entries [0-8] are a 3x3 array of the combinations of the main numeric types
  // B,I,D; then [9] CMPX [10] XINT (but not RAT) [11] RAT [12] SBT (symbol)
  // then [13-19] are for verb/, with precisions B I D Z X Q Symb
  // [20-26] for verb\, and [27-33] for verb\.
  VA *vainfo=(VA*)FAV(self)->localuse.lvp[0];  // extract table line from the primitive
  if(!((t=UNSAFE(at|wt))&(NOUN&~(B01|INT|FL)))){
   // Here for the fast and important case, where the arguments are both B01/INT/FL
   // The index into va is atype*3 + wtype, calculated sneakily
   jt->mulofloloc = 0;  // Reinit multiplier-overflow count, in case we hit overflow
   R vainfo->p2[(UNSAFE(at)>>(INTX-1))+((UNSAFE(at)+UNSAFE(wt))>>INTX)];
  }else if(!(t&(NOUN&~NUMERIC))) {
   // Here one of the arguments is CMPX/RAT/XNUM  (we don't support XD and XZ yet)
   // They are in priority order CMPX, FL, RAT, XNUM.  Extract those bits and look up
   // the type to use
   I  prix=(xnumpri>>(((t&(FL+CMPX))>>(FLX-2))+((t&RAT)>>(RATX-4))))&15; // routine index, FL/CMPX/XNUM/RAT   bits: RAT CMPX FL
   VA2 selva2 = vainfo->p2[prix];  // 
   // Entries specify no input conversion in the (DD,DD) slot, if they can accept FL arguments directly.  But if we select the FL line,
   // one input is FL and the other must be RAT or XNUM, 
   // we'd better specify an input conversion of VDD, unless the verb is one like +. or bitwise that forces conversion to integer/boolean
   if((prix==8)&&!(selva2.cv&(VBB|VII|VDD|VZZ))){selva2.cv = (selva2.cv&(~VARGMSK))|VDD;}   // This is part of where XNUM/RAT is promoted to FL
   R selva2;
  }else{
   // Normal case, but something is nonnumeric.  This will be a domain error except for = and ~:, and a few symbol operations
   VA2 retva2;  retva2.cv=VB; // where we build the return value   cv indicates no input conversion, boolean result
    if(likely(((UC)FAV(self)->id&~1)==CEQ)){I opcode;  // CEQ or CNE
    // = or ~:, possibly inhomogeneous
    if(likely(HOMO(at,wt))){
     opcode=((at>>(C2TX-2))+(wt>>C2TX))|(3*(5&(((t>>(SBTX-(BOXX+2)))+t)>>BOXX))); // bits are a4 a2 w4 w2 if char, 1100 if symbol, 0011 if box.  symbol is 1110, coming from a and symb shift
    }else opcode=15;  // inhomogeneous line
    retva2.f=eqnetbl[(UC)FAV(self)->id&1][opcode];  // return the comparison
    R retva2;
   }
   // not = ~:, better be a symbol
   if(at&wt&SBT)R vainfo->p2[12];  // symbol on symbol - process it through the optbl
   retva2.f=0; R retva2;  // if not symbol, return not found
  }
 }else{VA2 retva2;
  // Here there was an error in a previous run.  We see if we have a way to retry the operation
  retva2.f=0;  // error if not filled in
  switch((UC)FAV(self)->id){
  case CCIRCLE: if(jt->jerr==EWIMAG){retva2.f=(VF)cirZZ; retva2.cv=VZ+VZZ+VRD;} break;
  case CEXP: if(jt->jerr==EWIMAG){retva2.f=(VF)powZZ; retva2.cv=VZ+VZZ+VRD;}
             else if(jt->jerr==EWRAT){retva2.f=(VF)powQQ; retva2.cv=VQ+VQQ;}
             else if(jt->jerr==EWIRR){retva2.f=(VF)powDD; retva2.cv=VD+VDD;} break;
  case CBANG: if(jt->jerr==EWIRR){retva2.f=(VF)binDD; retva2.cv=VD+VDD;} break;
  case CDIV: if(jt->jerr==EWRAT){retva2.f=(VF)divQQ; retva2.cv=VQ+VQQ;}
             else if(jt->jerr==EWDIV0){retva2.f=(VF)divDD; retva2.cv=VD+VDD;} break;
  case CPLUS: if(jt->jerr==EWOVIP+EWOVIPPLUSII||jt->jerr==EWOVIP+EWOVIPPLUSBI||jt->jerr==EWOVIP+EWOVIPPLUSIB){retva2.f=(VF)plusIO; retva2.cv=VD+VII;} break;
  case CMINUS: if(jt->jerr==EWOVIP+EWOVIPMINUSII||jt->jerr==EWOVIP+EWOVIPMINUSBI||jt->jerr==EWOVIP+EWOVIPMINUSIB){retva2.f=(VF)minusIO; retva2.cv=VD+VII;} break;
  case CSTAR: if(jt->jerr==EWOVIP+EWOVIPMULII){retva2.f=(VF)tymesIO; retva2.cv=VD+VII;} break;
  case CPLUSDOT: if(jt->jerr==EWOV){retva2.f=(VF)gcdIO; retva2.cv=VD+VII;} break;
  case CSTARDOT: if(jt->jerr==EWOV){retva2.f=(VF)lcmIO; retva2.cv=VD+VII;} break;
  case CSTILE: if(jt->jerr==EWOV){retva2.f=(VF)remDD; retva2.cv=VD+VDD+VIP;} break;
  }
  if(retva2.f){RESETERR}else{if(jt->jerr>NEVM){RESETERR jsignal(EVSYSTEM);}}  // system error if unhandled exception.  Otherwise reset error only if we handled it
  R retva2;
 }
}    /* function and control for rank */
