/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Words: Control Words                                                    */

#include "j.h"
#include "w.h"

// CTESTB         (CIF+CELSEIF+CSELECT+CSELECTN+CWHILE+CWHILST+CFOR+CCASE+CFCASE)
#define CWCASE(x,y)    (x+256*y)
#define CWASSERT(b)    if(!(b))return i


static A jtcongotoblk(J jt,I n,CW*con){A z;CW*d=con;I i,j,k,*u,*v;
 GATV0(z,INT,2*n,2); v=AS(z); v[0]=n; v[1]=2;
 u=v=AV(z);
 for(i=j=0;i<n;++i,++d){
  *u++=-1; *u++=-1; 
  switch(d->type){
   case CEND: 
    v[k]=i; while(0<k&&0<v[k])k-=2; break;
   case CCASE: case CCATCH: case CDO: case CELSE: case CELSEIF: case CFCASE:
    v[k]=i;  /* fall thru */
   case CFOR: case CIF: case CSELECT: case CTRY: case CWHILE: case CWHILST:
    v[j]=i; k=1+j; j+=2;
 }}
 return z;
}    /* compute blocks for goto checking */

static I jtcongotochk(J jt,I i,I j,A x){I k,n,*v;
 n=AS(x)[0]; v=AV(x);
 for(k=0;k<n;++k,v+=2)if(BETWEENO(j,v[0],v[1])&&!BETWEENO(i,v[0],v[1]))return i;
 return -1;
}    /* i: goto; j: label; return -1 if ok or i if bad */

#define LABELEQU(m,s,e)    (CLABEL==e->type&&(x=lv[e->i],!memcmpne(s,6+CAV(x),m)))

static I jtcongoto(J jt,I n,CW*con,A*lv){A x,z;C*s;CW*d=con,*e;I i,j,k,m;
 RZ(z=jtcongotoblk(jt,n,con));
 for(i=0;i<n;++i,++d)
  if(CGOTO==d->type){
   x=lv[d->i]; s=5+CAV(x); m=0; while('.'!=s[m])++m; ++m;
   e=con-1; j=-1;
   DO(n, ++e; if(LABELEQU(m,s,e)){j=1+i; d->go=(US)j; break;});
   CWASSERT(0<=j);
   e=con+j-1;
   for(k=j;k<n;++k){++e; if(LABELEQU(m,s,e)){i=k; CWASSERT(0);}}
   if(0<=congotochk(i,j-1,z))return i;
  }
 return -1;
}    /* same result as conall */


/* conend - end encountered (but select end handled separately) */
/* same result as conall                                        */
/* b i p  end.                                                  */
/* c j q  1st word before end.                                  */
/* d k r  2nd word before end.                                  */
/* each triplet (b,i,p) is ptr, stack index, code (ptr->type)   */

// Each end. for for./while./select. is processed as it is encountered, and the break./continue. lines within it are processed.
// When we fill in a line, we change its go field from SMAX to a valid line#, which we use as an indicator that the field
// has been processed; thus we fill in each break./continue. only in the innermost containing loop.  If the innermost structure
// containing the break./continue. is a select., we change the type to BREAKS or CONTS to signal that we need to pop the
// select. off the stack, but we do not change the line number so that the line number will be filled in properly for the for. or while. .
// If the BREAKS turns out to be nested inside a for., it will be changed to BREAKF to cause the for. to be popped off the stack.

// Process end. not associated with select. or try.
static I conend(I i,I j,I k,CW*b,CW*c,CW*d,I p,I q,I r){I e,m,t;
 e=1+i;    // set e to Next Sequential Instruction
 CWASSERT(c); c->go=(US)e;   // Set previous control to come here.  This could be the do. if if./do. or loop/do., or the else. of else./do.
 switch(CWCASE(r,q)){    // look at the stack-types of the antepenultimate and penultimate entries
  default:                   CWASSERT(0);  // if not known combination, give control error
  case CWCASE(CDO,CELSE):                  // if. ...do. ... end.  or do. ... else. ... end. - leave as is
  case CWCASE(CIF,CDO):      break;
  case CWCASE(CELSEIF,CDO):  CWASSERT(d); d->go=(US)e; break;  // if elseif./do./end., set elseif. to come to end.
  case CWCASE(CWHILST,CDO):  CWASSERT(d); d->go=(US)(1+j);   // set whilst. to start at end, and fall through to...
  case CWCASE(CWHILE,CDO):                                    // ...while. ... do.
   // Set the end. to point back to the post-while., and then scan the loop looking for break./continue.
   // that has not been processed before (we detect these by d->go==SMAX; so when there are nested loops
   // we will skip over break. for inner loops, which have already been processed). 
   CWASSERT(b&&d); b->go=(US)(1+k); m=i-k-1;   // get # cws between (after while.) and (before end.)
   // fill in break. to go after end., or continue. to go after while.; leave others unchanged
   DQ(m, ++d; t=d->type; if(SMAX==d->go)d->go=(((((I)1<<CBREAK)|((I)1<<CBREAKS))>>t)&1)?(US)e :(((((I)1<<CCONT)|((I)1<<CCONTS))>>(t&31))&1)?(US)(1+k):(US)SMAX;);
   break;
  case CWCASE(CFOR,CDOF):
   // for. is like while., but end. and continue. go back to the do., and break. is marked as BREAKF
   // to indicate that the for. must be popped off the execution stack.  breakf. is needed even if the block
   // was previously marked as in select.
   CWASSERT(b&&d); b->go=(US)j;   m=i-k-1;
   DQ(m, ++d; t=d->type; if(SMAX==d->go)d->go=(((((I)1<<CBREAK)|((I)1<<CBREAKS))>>t)&1)?(d->type=CBREAKF,(US)e):(((((I)1<<CCONT)|((I)1<<CCONTS))>>(t&31))&1)?(US)j:(US)SMAX;);
 }
 return -1;
}

// Fix up the stack after encountering the end. for a try.  e=address of end.
static I conendtry(I e,I top,I stack[],CW*con){CW*v;I c[3],d[4],i=-1,j,k=0,m,t=0;US ii;
 c[0]=c[1]=c[2]=-1; d[k++]=e;
 // fill d[] with end. line# followed by catchx. numbers, in descending order of line #s.  Error if repeated catch. type
 while(top&&t!=CTRY){
  j=stack[--top];
  switch(t=(j+con)->type){
   case CTRY:    break;
   case CCATCH:  CWASSERT(0>c[0]); c[0]=d[k++]=j; break;
   case CCATCHD: CWASSERT(0>c[1]); c[1]=d[k++]=j; break;
   case CCATCHT: CWASSERT(0>c[2]); c[2]=d[k++]=j; break;
   default:      CWASSERT(0);
 }}
 CWASSERT(t==CTRY&&1<k);   // verify at least one catchx.  j now points to try.
 (j+con)->go=(US)d[k-1];                         // point the try. to the first catchx.
 m=k; DQ(k-1, --m; (d[m]+con)->go=(US)d[m-1];);  // point each catchx. to the next catchx., and the last one to the end.
 (e+con)->go=(US)(1+e);                          // point the end. to NSI
 m=d[k-1];                                    // line# of first catchx.
 // if there is a catch./catchd., scan the lines from after-try. to the first catchx., and point
 // any hitherto not-processed throw. to go to the catch. (if it exists) or catchd. (if no catch.).
 // That way, unfielded throw. counts as an error that can be picked up in catch.
 // kludge if break/continue encountered:  while. do. try. break. catch. end. end.  leaves the break pointing past the outer end, and the try stack unpopped
 if     (0<=c[0]){ii=(US)(1+c[0]); v=j+con; DQ(m-j-1, ++v; if(SMAX==v->go&&!(((((I)1<<CCONT)|((I)1<<CCONTS)|((I)1<<CBREAK)|((I)1<<CBREAKS))>>(v->type&31))&1))v->go=ii;);}
 else if(0<=c[1]){ii=(US)(1+c[1]); v=j+con; DQ(m-j-1, ++v; if(SMAX==v->go&&!(((((I)1<<CCONT)|((I)1<<CCONTS)|((I)1<<CBREAK)|((I)1<<CBREAKS))>>(v->type&31))&1))v->go=ii;);}
 return top;  //return stack pointer with the try. ... end. removed
}    /* result is new value of top */

// Fix up the stack after encountering the end, for a select.  i=address of end.
static I conendsel(I endline,I top,I stack[],CW*con){I c=endline-1,d=0,j,ot=top,t;
 // Go through the stack in reverse order till we hit the select.
 // c will hold the cw one before the one to go to if the previous test fails (init to one before end.)
 while(1){
  j=stack[--top]; t=(j+con)->type;    // back up to next cw
  if(t==CSELECT||t==CSELECTN)break;                //when we hit select., we're done
  if(t==CDOSEL){d=j; (j+con)->go=(US)(1+c);}  // on do., remember line# of do. in d; point that do. to the failure position
  else{                            // must be case./fcase.
   c=j; (j+con)->go=(US)endline;          // set failed-compare point to be the case. test; point case. to the end. (end-of-case goes to end.)
   if(d==1+j)(d+con)->go=(US)(1+d);    // if empty case., set do. to fall through to next inst
   if(t==CFCASE&&top<ot-2)(stack[2+top]+con)->go=(US)(1+stack[3+top]);  // if fcase. (and not last case), point case. AFTER fcase. to go to the do. for that following case.
 }}
 (c+con)->go=(US)(1+c);  // set first case. to fall through to the first test
 // j points to the select. for this end.  Replace any hitherto unfilled break./continue. with BREAKS/CONTS
 DQ(endline-j-2, ++j; if(SMAX==con[j].go){if(CBREAK==con[j].type)con[j].type=CBREAKS;else if(CCONT==con[j].type)con[j].type=CCONTS;});
 return top;     // return stack with select. ... end. removed
}    /* result is new value of top */

// audit the control sequence in con, making replacement in places
// n is number of control words, con points to them
static I jtconall(J jt,I n,CW*con){A y;CW*b=0,*c=0,*d=0;I e,i,j,k,p=0,q,r,*stack,tb=0,top=0,wb=0;
 // We process the words through a stack to match up control sequences.  Initialize to empty
 GATV0(y,INT,n,1); stack=AV(y);
 for(i=0;i<n;++i){
  // top is the top of the stack (i. e. the place to add the next entry)
  // b, c, d -> con entries, p, q, r are cw-types   for current cw, previous cw on stack, 2d-previous cw on stack
  // e is the next sequential instruction (following instruction i, the current line)
  // tb counts the number of unclosed if. statements.  If this is nonzero, every sentence becomes a T-block sentence
  q=r=0; e=1+i;             b=i+con; p=b->type;
  if(0<top){j=stack[top-1]; c=j+con; q=c->type;}
  if(1<top){k=stack[top-2]; d=k+con; r=d->type;}
  switch(p){
   case CBBLOCK:
     if(tb)b->type=CTBLOCK;   // In case of  if. if. x do. y end. do. end., y is NOT eligible to be the result, because
     // it is executed  in a T block; so we switch B to T inside ANY T block
     break;
   case CLABEL:  b->go=(US)e;           break;  // label goes to NSI
   case CTRY:
   case CCATCH:
   case CCATCHD:
   case CCATCHT: stack[top++]=i;        break;  // try./catch.: push address of control word
   case CCONT: case CBREAK: case CCONTS: case CBREAKS:  CWASSERT(wb);          break;  // continue/break: verify in a loop
   case CFOR:
   case CWHILE:  
   case CWHILST: ++wb;                          // for./while./whilst.: increment nested-loop count, fall through to...
   case CIF:     stack[top++]=i; ++tb;  break;   // (if. too) ...push addr of cw, increment t-block count
   case CCASE:
   case CFCASE:                                 // case./fcase.
    CWASSERT(q==CSELECT||q==CSELECTN||q==CDOSEL);           // verify in select.
    stack[top++]=i; if(q==CDOSEL)++tb;        // push address of cw; if not first case, increment T-block count (for the first,
                                              // we inherit the T-block status from the select.)
    break;
   case CSELECT:                              // select. - start T-block and check select. nesting
    // The execution-time code creates a stack-frame for select. that must be popped if break. or continue. is
    // encountered.  If select. blocks are nested, all nested blocks back to the looping structure must be
    // popped.  But the execution-time stack does not create a stack-frame for while., so looking at the
    // run-time stack cannot indicate where the top-level select. is.  Instead, we must analyze it here.
    // we change a nested select. to SELECTN.  This is needed only if we are in a loop.
    if(wb) {   // analyze only if in loop
     // Go back through the stack till we hit a loop.  If we find a SELECT along the way, change this to a SELECTN
     I s;  // stack pointer
     for(s=top-1;con[stack[s]].type!=CFOR && con[stack[s]].type!=CWHILE && con[stack[s]].type!=CWHILST;--s) {
      if(con[stack[s]].type==CSELECT){b->type=CSELECTN;break;}
     } 
    }
    stack[top++]=i; ++tb;  break;            // push addr of cw, increment t-block count
   case CDO:                                   // do. - classify according to what structure we are in
    CWASSERT(((((I)1<<CIF)|((I)1<<CELSEIF)|((I)1<<CSELECT)|((I)1<<CWHILE)|((I)1<<CWHILST)|((I)1<<CFOR)|((I)1<<CCASE)|((I)1<<CFCASE))>>(q&31))&1);   // first, make sure do. is allowed here
     // classify the do. based on struct: if for. do., call it DOF; if [f]case. do, call it DOSEL;
     // otherwise (if./elseif., while./whilst.) leave it as DO
    b->type=q==CFOR?CDOF:q==CCASE||q==CFCASE?CDOSEL:CDO;
    stack[top++]=i; --tb;                    // push the line# of the DO line; and note that this ends a T block
    break;
   case CELSEIF:                             // elseif. - like end. + if.
    CWASSERT(q==CDO);                        // verify in if.  (but it could be while. ??)
    c->go=(US)e;                             // set previous do. go to instruction after elseif.
    if(r==CELSEIF)d->go=(US)i;               // if struct is elseif. .. do.  elseif., point the previous elseif. to this one
    top-=2; stack[top++]=i; ++tb;            // remove the previous if. ... do. or elseif. ... do., replace with this elseif.; and
                                             // note that the previous block was a B and we have now moved to a T
    break;
   case CELSE:                               // else.
    CWASSERT((r==CIF||r==CELSEIF)&&q==CDO);                // verify part of if./elseif. ... do. ... else.
    c->go=(US)e;                             // set if. to jump to NSI
    if(r==CELSEIF)d->go=(US)i;               // if struct is elseif. .. do.  else., point the previous elseif. to this else.
    stack[top-2]=stack[top-1]; stack[top-1]=i;  // replace if./elseif. ... do. on stack with do. ... else.
    break;
   case CEND:                                // end. run a conend... routine to update pointers in the cws. q->the do., r->the starting cw of the structure
    switch(q){
    case CDOSEL:                            // if [f]case. ... do. ... end. 
     top=conendsel(i,top,stack,con); CWASSERT(0<=top); b->type=CENDSEL; break;  // end the select., and change the cw to ENDSEL
    case CCATCH: case CCATCHD: case CCATCHT:  // if a catch?.
     CWASSERT(1<=top);
     top=conendtry(i,top,stack,con); CWASSERT(0<=top);                  break;  // end the catch, verify validity
    default:                                // if if./elseif./else. or while./whilst./for. ...
     top-=2;                                // pop the starting cw and the do.
     if(r==CWHILE||r==CWHILST||r==CFOR)--wb;  // if this ends a loop, decrement the nested-loop count
     CWASSERT(0>conend(i,j,k,b,c,d,p,q,r));   // update the controls matching this end.
    }
    // if the END was not ENDSEL and goes to NSI, change the preceding block from BBLOCK to BBLOCKEND
    if(i+1<n&&b->go==i+1&&q!=CDOSEL&&con[i-1].type==CBBLOCK)con[i-1].type=CBBLOCKEND;
  }
}
 // when it's over, the stack should be empty.  If not, return the index of the top control on the stack
 if(top)return stack[top-1];
 // Fill in the canend field, which tells whether the previous B-block result can become the overall result.  It is used only
 // in T blocks and end./continue./break.
 // Clear canend to 0, meaning 'don't know'.  1=must return, 2=won't return, 4/8=provisional values of same
 DO(n, con[i].canend=0;);
 // Go through in reverse order, filling in a line if we have the status of its successors
 // Repeat until there are no more changes
 I madechange;  // set if we made a change
 do{
  madechange=0;  // init to no change
  for(i=n-1;i>=0;--i){
   I origcanend=con[i].canend;
   if(!(origcanend&3)){  // if the value is filled in with a final already, we won't change it
    switch(con[i].type) {
     case CBBLOCK: case CBBLOCKEND: case CTHROW:
      con[i].canend = 8+2; break;  // These are by definition not an end
     case CRETURN:
      con[i].canend = 4+1; break;  // These by definition ARE an end
     case CFOR: case CSELECTN: case CSELECT: case CLABEL:
      // These blocks inherit only from NSI
      if(i>=n-1)con[i].canend = 4+1;  // The last line of the function is the end
      else con[i].canend = con[i+1].canend;  // Only successor is NSI, use that
      break;
     case CTBLOCK: case CASSERT:
      // These blocks inherit from NSI only, but if go is NOT off the end, they also inherit from go, which is
      // catch. or the like.  If go is off the end, the function is taking an error and the result is immaterial
      if(i>=n-1)con[i].canend = 4+1;  // The last line of the function is the end
      else if(con[i].go>=n)con[i].canend = con[i+1].canend;   // if go is off the end, use NSI
      else con[i].canend = con[con[i].go].canend & con[i+1].canend;  // otherwise, make 2 only if both successors are 2
      break;
     case CTRY: case CCATCH: case CCATCHD: case CCATCHT: case CDOF: case CDOSEL: case CDO:
      // These blocks inherit either from NSI or from go
      if(i==n-1)con[i].canend = 4+1;  // The last line of the function is the end
      else if(con[i].go>=n)con[i].canend = 4+1;   // if go is off the end, use that
      else con[i].canend = con[con[i].go].canend & con[i+1].canend;  // otherwise, make 2 only if both successors are 2
      break;
     case CENDSEL: case CBREAK: case CCONT: case CBREAKS: case CCONTS: case CBREAKF: case CCASE: case CFCASE:
     case CIF: case CELSE: case CWHILE: case CWHILST: case CELSEIF: case CGOTO: case CEND:
       // These blocks inherit only from GO, but on a backward branch they provisionally inherit from NSI
      if(con[i].go>=n)con[i].canend = 4+1;  // If jump off the end, that's end
      else if(con[i].go>=i)con[i].canend = con[con[i].go].canend;  // Only successor is go, use that
      else if(i==n-1)con[i].canend = 4+1;  // backward branch but NSI is off the end: end
      else con[i].canend = (con[i+1].canend&0xc) | ((con[i+1].canend&con[con[i].go].canend)>>2); // backward branch: provisionally accept NSI; ratify it if go matches
      break;
    }
    madechange|=origcanend^con[i].canend;  // remember if we made a change
   }
  }
 }while(madechange);

 return -1;
}    /* modifies con; return -1 if OK or index of bad con entry */

A jtspellcon(J jt,I c){
 switch(c){
  default:      ASSERTSYS(0,"spellcon");
  case CASSERT: return cstr("assert.");
  case CBBLOCK: case CBBLOCKEND: return cstr("bblock.");
  case CBREAK: case CBREAKF:  case CBREAKS: return cstr("break.");
  case CCASE:   return cstr("case.");
  case CCATCH:  return cstr("catch.");
  case CCATCHD: return cstr("catchd.");
  case CCATCHT: return cstr("catcht.");
  case CCONT: case CCONTS: return cstr("continue.");
  case CDO:
  case CDOF:    
  case CDOSEL:  return cstr("do.");
  case CELSE:   return cstr("else.");
  case CELSEIF: return cstr("elseif.");
  case CEND:    
  case CENDSEL: return cstr("end.");
  case CFCASE:  return cstr("fcase.");
  case CFOR:    return cstr("for.");
  case CGOTO:   return cstr("goto_.");
  case CIF:     return cstr("if.");
  case CLABEL:  return cstr("label_.");
  case CRETURN: return cstr("return.");
  case CSELECTN:
  case CSELECT: return cstr("select.");
  case CTBLOCK: return cstr("tblock.");
  case CTHROW:  return cstr("throw.");
  case CTRY:    return cstr("try.");
  case CWHILE:  return cstr("while.");
  case CWHILST: return cstr("whilst.");
}}

static I jtconword(J jt,I n,C*s){
 if(2<n&&'.'==s[n-1])switch(*s){
  case 'a': if(!strncmp(s,"assert.",  n))return CASSERT;  break;
  case 'b': if(!strncmp(s,"break.",   n))return CBREAK;   break;
  case 'c': if(!strncmp(s,"case.",    n))return CCASE;
            if(!strncmp(s,"continue.",n))return CCONT;
            if(!strncmp(s,"catch.",   n))return CCATCH;
            if(!strncmp(s,"catchd.",  n))return CCATCHD;
            if(!strncmp(s,"catcht.",  n))return CCATCHT;  break;
  case 'd': if(!strncmp(s,"do.",      n))return CDO;      break;
  case 'e': if(!strncmp(s,"end.",     n))return CEND;
            if(!strncmp(s,"else.",    n))return CELSE;
            if(!strncmp(s,"elseif.",  n))return CELSEIF;  break;
  case 'f': if(!strncmp(s,"for.",     n))return CFOR;
            if(!strncmp(s,"for_",    4L)){ASSERTN(jtvnm(jt,n-5,4+s),EVILNAME,jtnfs(jt,n-5,4+s)); return CFOR;}
            if(!strncmp(s,"fcase.",   n))return CFCASE;   break;
  case 'g': if(!strncmp(s,"goto_",   5L))return CGOTO;    break;
  case 'i': if(!strncmp(s,"if.",      n))return CIF;      break;
  case 'l': if(!strncmp(s,"label_",  6L))return CLABEL;   break;
  case 'r': if(!strncmp(s,"return.",  n))return CRETURN;  break;
  case 's': if(!strncmp(s,"select.",  n))return CSELECT;  break;
  case 't': if(!strncmp(s,"throw.",   n))return CTHROW;
            if(!strncmp(s,"try.",     n))return CTRY;     break;
  case 'w': if(!strncmp(s,"while.",   n))return CWHILE;
            if(!strncmp(s,"whilst.",  n))return CWHILST;  break;
 }
 return 0;
}

// w is string, result is list of boxed strings, one per sentence in string (delimited by control words)
static A jtgetsen(J jt, A w){A y,z,*z0,*zv;C*s;I i,j,k=-1,m,n,*v;
 RZ(y=wordil(w)); ASSERT(AM(y)>=0,EVOPENQ) // split string into words - result block has special format
 v=AV(y);   // v-> (index,end+1) for each word; #words neg if last is NB.
 n=AM(y)<<1;  // number of words not including any final NB., times 2 to get  # start/end+1 slots
 GATV0(z,BOX,n>>1,1); z0=zv=AAV(z);  // allocate one box per word
 s=CAV(w);                         // s-> original text
 for(i=0;i<n;i+=2){     // for each word...
  j=v[i]; m=v[1+i]-j;         // j=index, m=length of word
  if(0>k)k=j;              // k=index of start of sentence, set at start or when we have processed a control word
  if(jtconword(jt,m,j+s)){     // when we hit a control word...
   if(k<j)RZ(*zv++=incorp(jtstr(jt,j-k,k+s)));  // if a sentence was in progress, emit it
   RZ(*zv++=incorp(jtstr(jt,m,j+s)));           // then emit the control word
   k=-1;           // reset start-of-sentence search
 }}
 if(0<=k)RZ(*zv++=incorp(jtstr(jt,j+m-k,k+s))); // if there was a final sentence in progress, append it
 return vec(BOX,zv-z0,z0);  // keep only the boxes that we used
}    /* partition by controls */

/* preparse - return tokenized lines and control information     */
/* argument is a list of boxed explicit defn lines               */
/* result is 1 iff try is seen                                   */
/* zl - list of lines of tokens                                  */
/* zc - corresp list of control info                             */
// zs - local symbol table, to use and copy
/* control info has 3 I values for each line                     */
/* control info values - type, goto linenum, source linenum      */

#define ASSERTCW(b,j)  {if(!(b)){I jj=(j); jsignal3(EVCTRL,wv[jj],jj); return 0;}}

B jtpreparse(J jt,A w,A*zl,A*zc){PROLOG(0004);A c,l,*lv,*v,w0,w1,*wv,x,y;B b=0,try=0;
     C*s;CW*d,*cv;I as=0,i,j,k,m,n,p,q,yn;
 p=AN(w); wv=AAV(w);  // p=#lines, wv->line 0 (a line is a boxed string)
 ASSERT(p<SMAX,EVLIMIT);
 RZ(c=exta(CONW,1L,1L,3*p)); cv=(CW*)AV(c); n=0;  // allocate result area, cv->start of block of CWs, n=#cws encountered
 RZ(l=exta(BOX, 1L,1L,5*p)); lv=    AAV(l); m=0;  // allocate list of boxed words, lv->&A for first word; m=#words
 for(i=0;i<p;++i){   // loop for each line
  // split the line into a sequence of sentences, splitting on each control word.  Result is a list of boxed strings, each one sentence
  RZ(y=getsen(wv[i])); yn=AN(y); v=AAV(y);  // split string into sentences; yn=#sentences on line, v->block for first sentence
  for(j=0;j<yn;++j){   // for each sentence on the line...
   if(n==AN(c)){RZ(c=jtext(jt,0,c)); cv=(CW*)AV(c);}  // if result buffer is full, reallocate it, reset pointer to first CW
   w0=v[j];                             // w0 is A block for sentence j
   RZ(w1=wordil(w0)); ASSERT(AM(w1)>=0,EVOPENQ)  // w1 is A block for (# words), (index,end+1) pairs
   s=CAV(str0(w0));                           // s->start of sentence after appending final NUL,  why?
   k=jtconword(jt,AV(w1)[1]-AV(w1)[0],s);         // classify first word, using its length.  0 means 'not CW', otherwise control type
   if(k==CTRY)try=1;                    // remember if we see a try.
   if(k==CASSERT){ASSERTCW(!as,i  ); as=1;}   // if assert., verify not preceded by assert.; go to post-assert. state
   else if(1==as){ASSERTCW(!k, i); as=2; --n;}   // verify assert. not followed by cw; back up to overwrite assert. block; go to post-post-assert. state
   d=n+cv;                              // d->CW block for this sentence
   d->type=k?(C)k:2==as?CASSERT:CBBLOCK;  // install type; if not cw, call it BBLOCK unless it's the block after assert.
   d->source=(US)i;                     /* source line number for this sentence   */
   // If this cw ends the verb, set a special goto line number; otherwise point to NSI
   d->go= (((((I)1<<0)|((I)1<<CCONT)|((I)1<<CBREAK)|((I)1<<CCONTS)|((I)1<<CBREAKS)|((I)1<<CTHROW))>>k)&1) ? (US)SMAX : k==CRETURN ? (US)SMAX-1 : (US)(1+n);
   b|=k==CGOTO;                         // remember if we see a goto_.
   // if not cw (ie executable sentence), turn words into an executable queue.  If cw, check for cw with data.  Set x to queue/cw, or 1 if cw w/o data
   if(!k){RZ(x=enqueue(w1,w0,2))}else{x=k==CLABEL||k==CGOTO||k==CFOR?w0:0L;}  // FOR must always go out; the length of the name is always needed, even if 0
   q=k?1&&x:AN(x);   // q=#words in sentence (1 if cw w/o data; 1 if cw w/data (eg for_x.); #words in sentence otherwise
   ASSERT(q<SMAX,EVLIMIT);
   // append the words (which are a queue or a cw) to the list of words
   if(x){                               // set unless the control word is not needed (it usually isn't)
    while(AN(l)<m+q){RZ(l=jtext(jt,0,l)); lv=AAV(l);}  // if word buffer filled, extend it & refresh data pointer
    if(k)lv[m]=incorp(x); else ICPY(m+lv,AAV(x),q);   // install word(s): the cw, or the words of the queue
   }
   // Now that the words have been moved, install the index to them, and their number, into the cw info; step word pointer over the words added (even if empty spaces)
   d->i=m; d->n=(US)q; m+=q;
   if(2==as)as=0;  // get out of post-post-assert. state if we are in it
   ++n;
 }}
 RE(0);
 ASSERTCW(!as,p-1);
 ASSERTCW(!b||0>(i=congoto(n,cv,lv)),(i+cv)->source);
 // Audit control structures and point the go line correctly
 ASSERTCW(    0>(i= jtconall(jt,n,cv   )),(i+cv)->source);
 // Install the number of words and cws into the return blocks, and return those blocks
 AN(l)=AS(l)[0]=m; *zl=incorp(l);
 AN(c)=AS(c)[0]=n; *zc=incorp(c);
 return try;
}
