|Path|Macro Name|Function|Regex|Replace|
|---|---|---|---|---|
|`jsrc\ja.h`|#define BfromI(x,y)                 jtBfromI(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|BfromI|jtBfromI|`(^\|[ \t]+\|[^\d\w_])BfromI\((?=([^,]+?),([^)]+?)\))`|`\1jtBfromI(jt,`|
|`jsrc\ja.h`|#define BfromX(x,y)                 jtBfromX(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|BfromX|jtBfromX|`(^\|[ \t]+\|[^\d\w_])BfromX\((?=([^,]+?),([^)]+?)\))`|`\1jtBfromX(jt,`|
|`jsrc\ja.h`|#define C1fromC2(x,y)               jtC1fromC2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|C1fromC2|jtC1fromC2|`(^\|[ \t]+\|[^\d\w_])C1fromC2\((?=([^,]+?),([^)]+?)\))`|`\1jtC1fromC2(jt,`|
|`jsrc\ja.h`|#define C2fromC1(x,y)               jtC2fromC1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|C2fromC1|jtC2fromC1|`(^\|[ \t]+\|[^\d\w_])C2fromC1\((?=([^,]+?),([^)]+?)\))`|`\1jtC2fromC1(jt,`|
|`jsrc\ja.h`|#define C1fromC4(x,y)               jtC1fromC4(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|C1fromC4|jtC1fromC4|`(^\|[ \t]+\|[^\d\w_])C1fromC4\((?=([^,]+?),([^)]+?)\))`|`\1jtC1fromC4(jt,`|
|`jsrc\ja.h`|#define C2fromC4(x,y)               jtC2fromC4(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|C2fromC4|jtC2fromC4|`(^\|[ \t]+\|[^\d\w_])C2fromC4\((?=([^,]+?),([^)]+?)\))`|`\1jtC2fromC4(jt,`|
|`jsrc\ja.h`|#define C4fromC1(x,y)               jtC4fromC1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|C4fromC1|jtC4fromC1|`(^\|[ \t]+\|[^\d\w_])C4fromC1\((?=([^,]+?),([^)]+?)\))`|`\1jtC4fromC1(jt,`|
|`jsrc\ja.h`|#define C4fromC2(x,y)               jtC4fromC2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|C4fromC2|jtC4fromC2|`(^\|[ \t]+\|[^\d\w_])C4fromC2\((?=([^,]+?),([^)]+?)\))`|`\1jtC4fromC2(jt,`|
|`jsrc\ja.h`|#define DfromQ(x,y)                 jtDfromQ(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|DfromQ|jtDfromQ|`(^\|[ \t]+\|[^\d\w_])DfromQ\((?=([^,]+?),([^)]+?)\))`|`\1jtDfromQ(jt,`|
|`jsrc\ja.h`|#define DfromX(x,y)                 jtDfromX(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|DfromX|jtDfromX|`(^\|[ \t]+\|[^\d\w_])DfromX\((?=([^,]+?),([^)]+?)\))`|`\1jtDfromX(jt,`|
|`jsrc\ja.h`|#define IfromX(x,y)                 jtIfromX(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|IfromX|jtIfromX|`(^\|[ \t]+\|[^\d\w_])IfromX\((?=([^,]+?),([^)]+?)\))`|`\1jtIfromX(jt,`|
|`jsrc\ja.h`|#define QfromX(x,y)                 jtQfromX(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|QfromX|jtQfromX|`(^\|[ \t]+\|[^\d\w_])QfromX\((?=([^,]+?),([^)]+?)\))`|`\1jtQfromX(jt,`|
|`jsrc\ja.h`|#define XfromB(x,y)                 jtXfromB(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|XfromB|jtXfromB|`(^\|[ \t]+\|[^\d\w_])XfromB\((?=([^,]+?),([^)]+?)\))`|`\1jtXfromB(jt,`|
|`jsrc\ja.h`|#define XfromI(x,y)                 jtXfromI(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|XfromI|jtXfromI|`(^\|[ \t]+\|[^\d\w_])XfromI\((?=([^,]+?),([^)]+?)\))`|`\1jtXfromI(jt,`|
|`jsrc\ja.h`|#define XfromQ(x,y)                 jtXfromQ(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|XfromQ|jtXfromQ|`(^\|[ \t]+\|[^\d\w_])XfromQ\((?=([^,]+?),([^)]+?)\))`|`\1jtXfromQ(jt,`|
|`jsrc\ja.h`|#define ZfromD(x,y)                 jtZfromD(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|ZfromD|jtZfromD|`(^\|[ \t]+\|[^\d\w_])ZfromD\((?=([^,]+?),([^)]+?)\))`|`\1jtZfromD(jt,`|
|`jsrc\ja.h`|#define abase2(x,y)                 jtabase2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cc.c` `jsrc\verbs\ve.c` `jsrc\verbs\vf.c` `jsrc\verbs\vp.c` `jsrc\verbs\vs.c`|abase2|jtabase2|`(^\|[ \t]+\|[^\d\w_])abase2\((?=([^,]+?),([^)]+?)\))`|`\1jtabase2(jt,`|
|`jsrc\ja.h`|#define afi(x,y)                    jtafi(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfrom.c`|afi|jtafi|`(^\|[ \t]+\|[^\d\w_])afi\((?=([^,]+?),([^)]+?)\))`|`\1jtafi(jt,`|
|`jsrc\ja.h`|#define afrom(x,y)                  jtafrom(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfrom.c`|afrom|jtafrom|`(^\|[ \t]+\|[^\d\w_])afrom\((?=([^,]+?),([^)]+?)\))`|`\1jtafrom(jt,`|
|`jsrc\ja.h`|#define afzrndID(x,y)               jtafzrndID(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|afzrndID|jtafzrndID|`(^\|[ \t]+\|[^\d\w_])afzrndID\((?=([^,]+?),([^)]+?)\))`|`\1jtafzrndID(jt,`|
|`jsrc\ja.h`|#define amend(x,y)                  jtamend(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define amp(x,y)                    jtamp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cu.c` `jsrc\verbs\vb.c` `jsrc\verbs\vfrom.c` `jsrc\xenos\xfmt.c`|amp|jtamp|`(^\|[ \t]+\|[^\d\w_])amp\((?=([^,]+?),([^)]+?)\))`|`\1jtamp(jt,`|
|`jsrc\ja.h`|#define ampco(x,y)                  jtampco(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cu.c`|ampco|jtampco|`(^\|[ \t]+\|[^\d\w_])ampco\((?=([^,]+?),([^)]+?)\))`|`\1jtampco(jt,`|
|`jsrc\ja.h`|#define ascan(x,y)                  jtascan(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\ch.c` `jsrc\conjunctions\cv.c`|ascan|jtascan|`(^\|[ \t]+\|[^\d\w_])ascan\((?=([^,]+?),([^)]+?)\))`|`\1jtascan(jt,`|
|`jsrc\ja.h`|#define aslash(x,y)                 jtaslash(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\ch.c` `jsrc\conjunctions\cv.c` `jsrc\verbs\v0.c` `jsrc\verbs\vb.c` `jsrc\verbs\ve.c` `jsrc\verbs\vrep.c`|aslash|jtaslash|`(^\|[ \t]+\|[^\d\w_])aslash\((?=([^,]+?),([^)]+?)\))`|`\1jtaslash(jt,`|
|`jsrc\ja.h`|#define aslash1(x,y)                jtaslash1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c` `jsrc\verbs\visp.c`|aslash1|jtaslash1|`(^\|[ \t]+\|[^\d\w_])aslash1\((?=([^,]+?),([^)]+?)\))`|`\1jtaslash1(jt,`|
|`jsrc\ja.h`|#define atco(x,y)                   jtatco(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c` `jsrc\conjunctions\cc.c` `jsrc\xenos\xfmt.c`|atco|jtatco|`(^\|[ \t]+\|[^\d\w_])atco\((?=([^,]+?),([^)]+?)\))`|`\1jtatco(jt,`|
|`jsrc\ja.h`|#define atomic(x,y)                 jtatomic(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define atop(x,y)                   jtatop(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\px.c` `jsrc\adverbs\ai.c` `jsrc\adverbs\ap.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cip.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\cu.c` `jsrc\verbs\ve.c` `jsrc\verbs\vg.c` `jsrc\xenos\xfmt.c`|atop|jtatop|`(^\|[ \t]+\|[^\d\w_])atop\((?=([^,]+?),([^)]+?)\))`|`\1jtatop(jt,`|
|`jsrc\ja.h`|#define axbytes(x,y)                jtaxbytes(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vs.c`|axbytes|jtaxbytes|`(^\|[ \t]+\|[^\d\w_])axbytes\((?=([^,]+?),([^)]+?)\))`|`\1jtaxbytes(jt,`|
|`jsrc\ja.h`|#define axtally(x,y)                jtaxtally(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vs.c`|axtally|jtaxtally|`(^\|[ \t]+\|[^\d\w_])axtally\((?=([^,]+?),([^)]+?)\))`|`\1jtaxtally(jt,`|
|`jsrc\ja.h`|#define base2(x,y)                  jtbase2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\amn.c` `jsrc\verbs\vp.c` `jsrc\verbs\vs.c` `jsrc\words\wn.c`|base2|jtbase2|`(^\|[ \t]+\|[^\d\w_])base2\((?=([^,]+?),([^)]+?)\))`|`\1jtbase2(jt,`|
|`jsrc\ja.h`|#define bcvt(x,y)                   jtbcvt(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c` `jsrc\adverbs\ar.c` `jsrc\words\wn.c`|bcvt|jtbcvt|`(^\|[ \t]+\|[^\d\w_])bcvt\((?=([^,]+?),([^)]+?)\))`|`\1jtbcvt(jt,`|
|`jsrc\ja.h`|#define bigdeal(x,y)                jtbigdeal(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vrand.c`|bigdeal|jtbigdeal|`(^\|[ \t]+\|[^\d\w_])bigdeal\((?=([^,]+?),([^)]+?)\))`|`\1jtbigdeal(jt,`|
|`jsrc\ja.h`|#define bitmatch(x,y)               jtbitmatch(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define cancel(x,y)                 jtcancel(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\ch.c`|cancel|jtcancel|`(^\|[ \t]+\|[^\d\w_])cancel\((?=([^,]+?),([^)]+?)\))`|`\1jtcancel(jt,`|
|`jsrc\ja.h`|#define cant2(x,y)                  jtcant2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\a.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\adverbs\ar.c` `jsrc\conjunctions\cc.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vs.c`|cant2|jtcant2|`(^\|[ \t]+\|[^\d\w_])cant2\((?=([^,]+?),([^)]+?)\))`|`\1jtcant2(jt,`|
|`jsrc\ja.h`|#define canta(x,y)                  jtcanta(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vcant.c`|canta|jtcanta|`(^\|[ \t]+\|[^\d\w_])canta\((?=([^,]+?),([^)]+?)\))`|`\1jtcanta(jt,`|
|`jsrc\ja.h`|#define cdinsert(x,y)               jtcdinsert(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\x15.c`|cdinsert|jtcdinsert|`(^\|[ \t]+\|[^\d\w_])cdinsert\((?=([^,]+?),([^)]+?)\))`|`\1jtcdinsert(jt,`|
|`jsrc\ja.h`|#define cdot2(x,y)                  jtcdot2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ao.c`|cdot2|jtcdot2|`(^\|[ \t]+\|[^\d\w_])cdot2\((?=([^,]+?),([^)]+?)\))`|`\1jtcdot2(jt,`|
|`jsrc\ja.h`|#define cdparse(x,y)                jtcdparse(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\x15.c`|cdparse|jtcdparse|`(^\|[ \t]+\|[^\d\w_])cdparse\((?=([^,]+?),([^)]+?)\))`|`\1jtcdparse(jt,`|
|`jsrc\ja.h`|#define celloffset(x,y)             jtcelloffset(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am.c` `jsrc\verbs\vfrom.c`|celloffset|jtcelloffset|`(^\|[ \t]+\|[^\d\w_])celloffset\((?=([^,]+?),([^)]+?)\))`|`\1jtcelloffset(jt,`|
|`jsrc\ja.h`|#define colon(x,y)                  jtcolon(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\px.c` `jsrc\parsing\pv.c`|colon|jtcolon|`(^\|[ \t]+\|[^\d\w_])colon\((?=([^,]+?),([^)]+?)\))`|`\1jtcolon(jt,`|
|`jsrc\ja.h`|#define compare(x,y)                jtcompare(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vgcomp.c` `jsrc\verbs\vi.c` `jsrc\verbs\viix.c`|compare|jtcompare|`(^\|[ \t]+\|[^\d\w_])compare\((?=([^,]+?),([^)]+?)\))`|`\1jtcompare(jt,`|
|`jsrc\ja.h`|#define conall(x,y)                 jtconall(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\words\wc.c`|conall|jtconall|`(^\|[ \t]+\|[^\d\w_])conall\((?=([^,]+?),([^)]+?)\))`|`\1jtconall(jt,`|
|`jsrc\ja.h`|#define congotoblk(x,y)             jtcongotoblk(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\words\wc.c`|congotoblk|jtcongotoblk|`(^\|[ \t]+\|[^\d\w_])congotoblk\((?=([^,]+?),([^)]+?)\))`|`\1jtcongotoblk(jt,`|
|`jsrc\ja.h`|#define connum(x,y)                 jtconnum(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\words\w.c`|connum|jtconnum|`(^\|[ \t]+\|[^\d\w_])connum\((?=([^,]+?),([^)]+?)\))`|`\1jtconnum(jt,`|
|`jsrc\ja.h`|#define constr(x,y)                 jtconstr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\words\w.c`|constr|jtconstr|`(^\|[ \t]+\|[^\d\w_])constr\((?=([^,]+?),([^)]+?)\))`|`\1jtconstr(jt,`|
|`jsrc\ja.h`|#define conword(x,y)                jtconword(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\words\wc.c`|conword|jtconword|`(^\|[ \t]+\|[^\d\w_])conword\((?=([^,]+?),([^)]+?)\))`|`\1jtconword(jt,`|
|`jsrc\ja.h`|#define crc2(x,y)                   jtcrc2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xcrc.c`|crc2|jtcrc2|`(^\|[ \t]+\|[^\d\w_])crc2\((?=([^,]+?),([^)]+?)\))`|`\1jtcrc2(jt,`|
|`jsrc\ja.h`|#define createcycliciterator(x,y)   jtcreatecycliciterator(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ap.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cr.c`|createcycliciterator|jtcreatecycliciterator|`(^\|[ \t]+\|[^\d\w_])createcycliciterator\((?=([^,]+?),([^)]+?)\))`|`\1jtcreatecycliciterator(jt,`|
|`jsrc\ja.h`|#define crcvalidate(x,t)            jtcrcvalidate(jt,(x),(t))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xcrc.c`|crcvalidate|jtcrcvalidate|`(^\|[ \t]+\|[^\d\w_])crcvalidate\((?=([^,]+?),([^)]+?)\))`|`\1jtcrcvalidate(jt,`|
|`jsrc\ja.h`|#define csize(x,y)                  jtcsize(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\amn.c`|csize|jtcsize|`(^\|[ \t]+\|[^\d\w_])csize\((?=([^,]+?),([^)]+?)\))`|`\1jtcsize(jt,`|
|`jsrc\ja.h`|#define cut(x,y)                    jtcut(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\io.c` `jsrc\conjunctions\cc.c` `jsrc\debugging\d.c` `jsrc\verbs\v2.c` `jsrc\verbs\vb.c` `jsrc\verbs\visp.c`|cut|jtcut|`(^\|[ \t]+\|[^\d\w_])cut\((?=([^,]+?),([^)]+?)\))`|`\1jtcut(jt,`|
|`jsrc\ja.h`|#define cvt(x,y)                    jtcvt(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\jtype.h` `jsrc\k.c` `jsrc\result.h` `jsrc\sl.c` `jsrc\sn.c` `jsrc\u.c` `jsrc\adverbs\a.c` `jsrc\adverbs\ab.c` `jsrc\adverbs\ai.c` `jsrc\adverbs\am.c` `jsrc\adverbs\ao.c` `jsrc\adverbs\ap.c` `jsrc\adverbs\ar.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\ca.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cg.c` `jsrc\conjunctions\ch.c` `jsrc\conjunctions\cip.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\crs.c` `jsrc\conjunctions\cv.c` `jsrc\conjunctions\cx.c` `jsrc\format\f2.c` `jsrc\verbs\v.c` `jsrc\verbs\v0.c` `jsrc\verbs\v1.c` `jsrc\verbs\v2.c` `jsrc\verbs\va.h` `jsrc\verbs\va1.c` `jsrc\verbs\va2.c` `jsrc\verbs\va2s.c` `jsrc\verbs\vb.c` `jsrc\verbs\vcat.c` `jsrc\verbs\vd.c` `jsrc\verbs\ve.c` `jsrc\verbs\vf.c` `jsrc\verbs\vfrom.c` `jsrc\verbs\vgauss.c` `jsrc\verbs\vgcomp.c` `jsrc\verbs\vi.c` `jsrc\verbs\viix.c` `jsrc\verbs\visp.c` `jsrc\verbs\vm.c` `jsrc\verbs\vo.c` `jsrc\verbs\vp.c` `jsrc\verbs\vrand.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c` `jsrc\verbs\vsb.c` `jsrc\verbs\vt.c` `jsrc\verbs\vx.c` `jsrc\words\wn.c` `jsrc\xenos\x15.c` `jsrc\xenos\xa.c` `jsrc\xenos\xb.c` `jsrc\xenos\xfmt.c` `jsrc\xenos\xt.c`|cvt|jtcvt|`(^\|[ \t]+\|[^\d\w_])cvt\((?=([^,]+?),([^)]+?)\))`|`\1jtcvt(jt,`|
|`jsrc\ja.h`|#define cvz(x,y)                    jtcvz(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ap.c` `jsrc\adverbs\ar.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\cc.c` `jsrc\verbs\va1.c` `jsrc\verbs\va2.c` `jsrc\verbs\va2s.c`|cvz|jtcvz|`(^\|[ \t]+\|[^\d\w_])cvz\((?=([^,]+?),([^)]+?)\))`|`\1jtcvz(jt,`|
|`jsrc\ja.h`|#define daxis(x,y)                  jtdaxis(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vs.c`|daxis|jtdaxis|`(^\|[ \t]+\|[^\d\w_])daxis\((?=([^,]+?),([^)]+?)\))`|`\1jtdaxis(jt,`|
|`jsrc\ja.h`|#define db1b2(x,y)                  jtdb1b2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v2.c`|db1b2|jtdb1b2|`(^\|[ \t]+\|[^\d\w_])db1b2\((?=([^,]+?),([^)]+?)\))`|`\1jtdb1b2(jt,`|
|`jsrc\ja.h`|#define dbin(x,y)                   jtdbin(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vbang.c`|dbin|jtdbin|`(^\|[ \t]+\|[^\d\w_])dbin\((?=([^,]+?),([^)]+?)\))`|`\1jtdbin(jt,`|
|`jsrc\ja.h`|#define dbrr(x,y)                   jtdbrr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\dsusp.c`|dbrr|jtdbrr|`(^\|[ \t]+\|[^\d\w_])dbrr\((?=([^,]+?),([^)]+?)\))`|`\1jtdbrr(jt,`|
|`jsrc\ja.h`|#define dbstop(x,y)                 jtdbstop(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\dsusp.c`|dbstop|jtdbstop|`(^\|[ \t]+\|[^\d\w_])dbstop\((?=([^,]+?),([^)]+?)\))`|`\1jtdbstop(jt,`|
|`jsrc\ja.h`|#define dbsig(x,y)                  jtdbsig(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\d.c`|dbsig|jtdbsig|`(^\|[ \t]+\|[^\d\w_])dbsig\((?=([^,]+?),([^)]+?)\))`|`\1jtdbsig(jt,`|
|`jsrc\ja.h`|#define dcase(x,y)                  jtdcase(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\parsing\pv.c`|dcase|jtdcase|`(^\|[ \t]+\|[^\d\w_])dcase\((?=([^,]+?),([^)]+?)\))`|`\1jtdcase(jt,`|
|`jsrc\ja.h`|#define dcube(x,y)                  jtdcube(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c`|dcube|jtdcube|`(^\|[ \t]+\|[^\d\w_])dcube\((?=([^,]+?),([^)]+?)\))`|`\1jtdcube(jt,`|
|`jsrc\ja.h`|#define ddtokens(x,y)               jtddtokens(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\io.c` `jsrc\px.c` `jsrc\conjunctions\cx.c` `jsrc\xenos\xs.c`|ddtokens|jtddtokens|`(^\|[ \t]+\|[^\d\w_])ddtokens\((?=([^,]+?),([^)]+?)\))`|`\1jtddtokens(jt,`|
|`jsrc\ja.h`|#define deal(x,y)                   jtdeal(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vrand.c`|deal|jtdeal|`(^\|[ \t]+\|[^\d\w_])deal\((?=([^,]+?),([^)]+?)\))`|`\1jtdeal(jt,`|
|`jsrc\ja.h`|#define decorate(x,y)               jtdecorate(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\rl.c`|decorate|jtdecorate|`(^\|[ \t]+\|[^\d\w_])decorate\((?=([^,]+?),([^)]+?)\))`|`\1jtdecorate(jt,`|
|`jsrc\ja.h`|#define det(x,y)                    jtdet(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cip.c`|det|jtdet|`(^\|[ \t]+\|[^\d\w_])det\((?=([^,]+?),([^)]+?)\))`|`\1jtdet(jt,`|
|`jsrc\ja.h`|#define detxm(x,y)                  jtdetxm(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cip.c` `jsrc\verbs\vgauss.c`|detxm|jtdetxm|`(^\|[ \t]+\|[^\d\w_])detxm\((?=([^,]+?),([^)]+?)\))`|`\1jtdetxm(jt,`|
|`jsrc\ja.h`|#define dfc(x,y)                    jtdfc(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vp.c`|dfc|jtdfc|`(^\|[ \t]+\|[^\d\w_])dfc\((?=([^,]+?),([^)]+?)\))`|`\1jtdfc(jt,`|
|`jsrc\ja.h`|#define dfs1(x,y)                   jtdfs1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\dsusp.c`|dfs1|jtdfs1|`(^\|[ \t]+\|[^\d\w_])dfs1\((?=([^,]+?),([^)]+?)\))`|`\1jtdfs1(jt,`|
|`jsrc\ja.h`|#define dgrade2(x,y)                jtdgrade2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|dgrade2|jtdgrade2|`(^\|[ \t]+\|[^\d\w_])dgrade2\((?=([^,]+?),([^)]+?)\))`|`\1jtdgrade2(jt,`|
|`jsrc\ja.h`|#define dhead(x,y)                  jtdhead(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\d.c`|dhead|jtdhead|`(^\|[ \t]+\|[^\d\w_])dhead\((?=([^,]+?),([^)]+?)\))`|`\1jtdhead(jt,`|
|`jsrc\ja.h`|#define diag(x,y)                   jtdiag(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c`|diag|jtdiag|`(^\|[ \t]+\|[^\d\w_])diag\((?=([^,]+?),([^)]+?)\))`|`\1jtdiag(jt,`|
|`jsrc\ja.h`|#define disp(x,y)                   jtdisp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\d.c`|disp|jtdisp|`(^\|[ \t]+\|[^\d\w_])disp\((?=([^,]+?),([^)]+?)\))`|`\1jtdisp(jt,`|
|`jsrc\ja.h`|#define dllsymaddr(x,y)             jtdllsymaddr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\s.c`|dllsymaddr|jtdllsymaddr|`(^\|[ \t]+\|[^\d\w_])dllsymaddr\((?=([^,]+?),([^)]+?)\))`|`\1jtdllsymaddr(jt,`|
|`jsrc\ja.h`|#define dot(x,y)                    jtdot(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|dot|jtdot|`(^\|[ \t]+\|[^\d\w_])dot\((?=([^,]+?),([^)]+?)\))`|`\1jtdot(jt,`|
|`jsrc\ja.h`|#define dpone(x,y)                  jtdpone(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|dpone|jtdpone|`(^\|[ \t]+\|[^\d\w_])dpone\((?=([^,]+?),([^)]+?)\))`|`\1jtdpone(jt,`|
|`jsrc\ja.h`|#define drop(x,y)                   jtdrop(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\as.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cip.c` `jsrc\conjunctions\cp.c` `jsrc\representations\rl.c` `jsrc\verbs\vd.c` `jsrc\verbs\vt.c` `jsrc\xenos\xs.c`|drop|jtdrop|`(^\|[ \t]+\|[^\d\w_])drop\((?=([^,]+?),([^)]+?)\))`|`\1jtdrop(jt,`|
|`jsrc\ja.h`|#define dropr(x,y)                  jtdropr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\crs.c` `jsrc\verbs\v.c` `jsrc\verbs\vf.c` `jsrc\verbs\visp.c`|dropr|jtdropr|`(^\|[ \t]+\|[^\d\w_])dropr\((?=([^,]+?),([^)]+?)\))`|`\1jtdropr(jt,`|
|`jsrc\ja.h`|#define ebar(x,y)                   jtebar(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vb.c`|ebar|jtebar|`(^\|[ \t]+\|[^\d\w_])ebar\((?=([^,]+?),([^)]+?)\))`|`\1jtebar(jt,`|
|`jsrc\ja.h`|#define ebarmat(x,y)                jtebarmat(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vb.c`|ebarmat|jtebarmat|`(^\|[ \t]+\|[^\d\w_])ebarmat\((?=([^,]+?),([^)]+?)\))`|`\1jtebarmat(jt,`|
|`jsrc\ja.h`|#define ebarvec(x,y)                jtebarvec(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vb.c`|ebarvec|jtebarvec|`(^\|[ \t]+\|[^\d\w_])ebarvec\((?=([^,]+?),([^)]+?)\))`|`\1jtebarvec(jt,`|
|`jsrc\ja.h`|#define efmt(x,y)                   jtefmt(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\d.c`|efmt|jtefmt|`(^\|[ \t]+\|[^\d\w_])efmt\((?=([^,]+?),([^)]+?)\))`|`\1jtefmt(jt,`|
|`jsrc\ja.h`|#define ep(x,y)                     jtep(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\d.c`|ep|jtep|`(^\|[ \t]+\|[^\d\w_])ep\((?=([^,]+?),([^)]+?)\))`|`\1jtep(jt,`|
|`jsrc\ja.h`|#define eps(x,y)                    jteps(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\af.c` `jsrc\adverbs\ai.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\cx.c` `jsrc\verbs\v.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vi.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c` `jsrc\words\wn.c` `jsrc\xenos\xa.c`|eps|jteps|`(^\|[ \t]+\|[^\d\w_])eps\((?=([^,]+?),([^)]+?)\))`|`\1jteps(jt,`|
|`jsrc\ja.h`|#define eputq(x,y)                  jteputq(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\d.c`|eputq|jteputq|`(^\|[ \t]+\|[^\d\w_])eputq\((?=([^,]+?),([^)]+?)\))`|`\1jteputq(jt,`|
|`jsrc\ja.h`|#define eqf(x,y)                    jteqf(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v1.c`|eqf|jteqf|`(^\|[ \t]+\|[^\d\w_])eqf\((?=([^,]+?),([^)]+?)\))`|`\1jteqf(jt,`|
|`jsrc\ja.h`|#define eva(x,y)                    jteva(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c`|eva|jteva|`(^\|[ \t]+\|[^\d\w_])eva\((?=([^,]+?),([^)]+?)\))`|`\1jteva(jt,`|
|`jsrc\ja.h`|#define even(x,y)                   jteven(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define every(x,y)                  jtevery(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\sl.c` `jsrc\adverbs\af.c` `jsrc\adverbs\am1.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cg.c` `jsrc\conjunctions\cl.c` `jsrc\conjunctions\cx.c` `jsrc\debugging\d.c` `jsrc\representations\r.c` `jsrc\representations\rt.c` `jsrc\verbs\vfrom.c` `jsrc\verbs\vfromsp.c`|every|jtevery|`(^\|[ \t]+\|[^\d\w_])every\((?=([^,]+?),([^)]+?)\))`|`\1jtevery(jt,`|
|`jsrc\ja.h`|#define everysp(x,y)                jteverysp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cu.c`|everysp|jteverysp|`(^\|[ \t]+\|[^\d\w_])everysp\((?=([^,]+?),([^)]+?)\))`|`\1jteverysp(jt,`|
|`jsrc\ja.h`|#define evger(x,y)                  jtevger(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ar.c`|evger|jtevger|`(^\|[ \t]+\|[^\d\w_])evger\((?=([^,]+?),([^)]+?)\))`|`\1jtevger(jt,`|
|`jsrc\ja.h`|#define expand(x,y)                 jtexpand(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c`|expand|jtexpand|`(^\|[ \t]+\|[^\d\w_])expand\((?=([^,]+?),([^)]+?)\))`|`\1jtexpand(jt,`|
|`jsrc\ja.h`|#define exprndID(x,y)               jtexprndID(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|exprndID|jtexprndID|`(^\|[ \t]+\|[^\d\w_])exprndID\((?=([^,]+?),([^)]+?)\))`|`\1jtexprndID(jt,`|
|`jsrc\ja.h`|#define ext(x,y)                    jtext(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c` `jsrc\s.h` `jsrc\sn.c` `jsrc\conjunctions\cl.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\cx.c` `jsrc\parsing\p.c` `jsrc\verbs\vb.c` `jsrc\verbs\vp.c` `jsrc\verbs\vsb.c` `jsrc\words\w.c` `jsrc\words\wc.c` `jsrc\xenos\x15.c` `jsrc\xenos\xd.c` `jsrc\xenos\xf.c` `jsrc\xenos\xl.c` `jsrc\xenos\xo.c` `jsrc\xenos\xs.c`|ext|jtext|`(^\|[ \t]+\|[^\d\w_])ext\((?=([^,]+?),([^)]+?)\))`|`\1jtext(jt,`|
|`jsrc\ja.h`|#define fit(x,y)                    jtfit(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cv.c` `jsrc\representations\rl.c`|fit|jtfit|`(^\|[ \t]+\|[^\d\w_])fit\((?=([^,]+?),([^)]+?)\))`|`\1jtfit(jt,`|
|`jsrc\ja.h`|#define fix(x,y)                    jtfix(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\sn.c` `jsrc\adverbs\ai.c` `jsrc\adverbs\au.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cu.c` `jsrc\conjunctions\cv.c` `jsrc\conjunctions\cx.c`|fix|jtfix|`(^\|[ \t]+\|[^\d\w_])fix\((?=([^,]+?),([^)]+?)\))`|`\1jtfix(jt,`|
|`jsrc\ja.h`|#define fixa(x,y)                   jtfixa(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\af.c`|fixa|jtfixa|`(^\|[ \t]+\|[^\d\w_])fixa\((?=([^,]+?),([^)]+?)\))`|`\1jtfixa(jt,`|
|`jsrc\ja.h`|#define fixrecursive(x,y)           jtfixrecursive(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\af.c`|fixrecursive|jtfixrecursive|`(^\|[ \t]+\|[^\d\w_])fixrecursive\((?=([^,]+?),([^)]+?)\))`|`\1jtfixrecursive(jt,`|
|`jsrc\ja.h`|#define fmt02(x,y)                  jtfmt02(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|fmt02|jtfmt02|`(^\|[ \t]+\|[^\d\w_])fmt02\((?=([^,]+?),([^)]+?)\))`|`\1jtfmt02(jt,`|
|`jsrc\ja.h`|#define fmt12(x,y)                  jtfmt12(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|fmt12|jtfmt12|`(^\|[ \t]+\|[^\d\w_])fmt12\((?=([^,]+?),([^)]+?)\))`|`\1jtfmt12(jt,`|
|`jsrc\ja.h`|#define fmt22(x,y)                  jtfmt22(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|fmt22|jtfmt22|`(^\|[ \t]+\|[^\d\w_])fmt22\((?=([^,]+?),([^)]+?)\))`|`\1jtfmt22(jt,`|
|`jsrc\ja.h`|#define fmtD(x,y)                   jtfmtD(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\format\f.c`|fmtD|jtfmtD|`(^\|[ \t]+\|[^\d\w_])fmtD\((?=([^,]+?),([^)]+?)\))`|`\1jtfmtD(jt,`|
|`jsrc\ja.h`|#define fmtprecomp(x,y)             jtfmtprecomp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|fmtprecomp|jtfmtprecomp|`(^\|[ \t]+\|[^\d\w_])fmtprecomp\((?=([^,]+?),([^)]+?)\))`|`\1jtfmtprecomp(jt,`|
|`jsrc\ja.h`|#define fong(x,y)                   jtfong(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c`|fong|jtfong|`(^\|[ \t]+\|[^\d\w_])fong\((?=([^,]+?),([^)]+?)\))`|`\1jtfong(jt,`|
|`jsrc\ja.h`|#define foreign(x,y)                jtforeign(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c` `jsrc\representations\r.c` `jsrc\xenos\xfmt.c`|foreign|jtforeign|`(^\|[ \t]+\|[^\d\w_])foreign\((?=([^,]+?),([^)]+?)\))`|`\1jtforeign(jt,`|
|`jsrc\ja.h`|#define forinit(x,y)                jtforinit(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cx.c`|forinit|jtforinit|`(^\|[ \t]+\|[^\d\w_])forinit\((?=([^,]+?),([^)]+?)\))`|`\1jtforinit(jt,`|
|`jsrc\ja.h`|#define from(x,y)                   jtfrom(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\a.c` `jsrc\adverbs\ab.c` `jsrc\adverbs\am.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\adverbs\ao.c` `jsrc\adverbs\ap.c` `jsrc\adverbs\ar.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cg.c` `jsrc\conjunctions\ch.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\crs.c` `jsrc\conjunctions\cu.c` `jsrc\conjunctions\cx.c` `jsrc\debugging\dc.c` `jsrc\parsing\p.c` `jsrc\representations\rl.c` `jsrc\verbs\v.c` `jsrc\verbs\v0.c` `jsrc\verbs\v1.c` `jsrc\verbs\v2.c` `jsrc\verbs\vcant.c` `jsrc\verbs\vcat.c` `jsrc\verbs\vf.c` `jsrc\verbs\vfrom.c` `jsrc\verbs\vg.c` `jsrc\verbs\vgsp.c` `jsrc\verbs\vp.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c` `jsrc\verbs\vt.c` `jsrc\words\w.c`|from|jtfrom|`(^\|[ \t]+\|[^\d\w_])from\((?=([^,]+?),([^)]+?)\))`|`\1jtfrom(jt,`|
|`jsrc\ja.h`|#define frombs(x,y)                 jtfrombs(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfrom.c`|frombs|jtfrombs|`(^\|[ \t]+\|[^\d\w_])frombs\((?=([^,]+?),([^)]+?)\))`|`\1jtfrombs(jt,`|
|`jsrc\ja.h`|#define fromis(x,y)                 jtfromis(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfrom.c`|fromis|jtfromis|`(^\|[ \t]+\|[^\d\w_])fromis\((?=([^,]+?),([^)]+?)\))`|`\1jtfromis(jt,`|
|`jsrc\ja.h`|#define fromr(x,y)                  jtfromr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vcant.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c`|fromr|jtfromr|`(^\|[ \t]+\|[^\d\w_])fromr\((?=([^,]+?),([^)]+?)\))`|`\1jtfromr(jt,`|
|`jsrc\ja.h`|#define fromsd(x,y)                 jtfromsd(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfrom.c`|fromsd|jtfromsd|`(^\|[ \t]+\|[^\d\w_])fromsd\((?=([^,]+?),([^)]+?)\))`|`\1jtfromsd(jt,`|
|`jsrc\ja.h`|#define fromss(x,y)                 jtfromss(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfrom.c`|fromss|jtfromss|`(^\|[ \t]+\|[^\d\w_])fromss\((?=([^,]+?),([^)]+?)\))`|`\1jtfromss(jt,`|
|`jsrc\ja.h`|#define fxchar(x,y)                 jtfxchar(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\r.c`|fxchar|jtfxchar|`(^\|[ \t]+\|[^\d\w_])fxchar\((?=([^,]+?),([^)]+?)\))`|`\1jtfxchar(jt,`|
|`jsrc\ja.h`|#define fxeach(x,y)                 jtfxeach(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cg.c` `jsrc\representations\r.c` `jsrc\representations\rl.c` `jsrc\representations\rt.c`|fxeach|jtfxeach|`(^\|[ \t]+\|[^\d\w_])fxeach\((?=([^,]+?),([^)]+?)\))`|`\1jtfxeach(jt,`|
|`jsrc\ja.h`|#define fxeachv(x,y)                jtfxeachv(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ao.c` `jsrc\adverbs\ap.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cg.c` `jsrc\conjunctions\cr.c`|fxeachv|jtfxeachv|`(^\|[ \t]+\|[^\d\w_])fxeachv\((?=([^,]+?),([^)]+?)\))`|`\1jtfxeachv(jt,`|
|`jsrc\ja.h`|#define gadv(x,y)                   jtgadv(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am.c`|gadv|jtgadv|`(^\|[ \t]+\|[^\d\w_])gadv\((?=([^,]+?),([^)]+?)\))`|`\1jtgadv(jt,`|
|`jsrc\ja.h`|#define gah(x,y)                    jtgah(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\x15.c`|gah|jtgah|`(^\|[ \t]+\|[^\d\w_])gah\((?=([^,]+?),([^)]+?)\))`|`\1jtgah(jt,`|
|`jsrc\ja.h`|#define gc(x,y)                     jtgc(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\j.h` `jsrc\m.c` `jsrc\adverbs\ar.c` `jsrc\conjunctions\cg.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cx.c` `jsrc\verbs\v2.c` `jsrc\verbs\vfromsp.c`|gc|jtgc|`(^\|[ \t]+\|[^\d\w_])gc\((?=([^,]+?),([^)]+?)\))`|`\1jtgc(jt,`|
|`jsrc\ja.h`|#define gerfrom(x,y)                jtgerfrom(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cg.c`|gerfrom|jtgerfrom|`(^\|[ \t]+\|[^\d\w_])gerfrom\((?=([^,]+?),([^)]+?)\))`|`\1jtgerfrom(jt,`|
|`jsrc\ja.h`|#define grade1p(x,y)                jtgrade1p(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ar.c`|grade1p|jtgrade1p|`(^\|[ \t]+\|[^\d\w_])grade1p\((?=([^,]+?),([^)]+?)\))`|`\1jtgrade1p(jt,`|
|`jsrc\ja.h`|#define grade2(x,y)                 jtgrade2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\sl.c` `jsrc\sn.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\debugging\dc.c` `jsrc\verbs\v2.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vg.c` `jsrc\verbs\vi.c` `jsrc\verbs\vs.c` `jsrc\xenos\xl.c` `jsrc\xenos\xo.c`|grade2|jtgrade2|`(^\|[ \t]+\|[^\d\w_])grade2\((?=([^,]+?),([^)]+?)\))`|`\1jtgrade2(jt,`|
|`jsrc\ja.h`|#define grd2sp(x,y)                 jtgrd2sp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vg.c`|grd2sp|jtgrd2sp|`(^\|[ \t]+\|[^\d\w_])grd2sp\((?=([^,]+?),([^)]+?)\))`|`\1jtgrd2sp(jt,`|
|`jsrc\ja.h`|#define hia(x,y)                    jthia(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vi.c`|hia|jthia|`(^\|[ \t]+\|[^\d\w_])hia\((?=([^,]+?),([^)]+?)\))`|`\1jthia(jt,`|
|`jsrc\ja.h`|#define hook(x,y)                   jthook(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\af.c` `jsrc\adverbs\ai.c` `jsrc\conjunctions\cu.c` `jsrc\parsing\p.c` `jsrc\parsing\pv.c` `jsrc\representations\r.c`|hook|jthook|`(^\|[ \t]+\|[^\d\w_])hook\((?=([^,]+?),([^)]+?)\))`|`\1jthook(jt,`|
|`jsrc\ja.h`|#define hook1(x,y)                  jthook1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cf.c`|hook1|jthook1|`(^\|[ \t]+\|[^\d\w_])hook1\((?=([^,]+?),([^)]+?)\))`|`\1jthook1(jt,`|
|`jsrc\ja.h`|#define ifb(x,y)                    jtifb(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\as.c` `jsrc\conjunctions\cf.c` `jsrc\conjunctions\crs.c` `jsrc\verbs\v.c` `jsrc\verbs\v1.c` `jsrc\verbs\va2s.c` `jsrc\verbs\vcant.c` `jsrc\verbs\vcat.c` `jsrc\verbs\vf.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vgsp.c` `jsrc\verbs\visp.c` `jsrc\verbs\vo.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c`|ifb|jtifb|`(^\|[ \t]+\|[^\d\w_])ifb\((?=([^,]+?),([^)]+?)\))`|`\1jtifb(jt,`|
|`jsrc\ja.h`|#define ifrom(x,y)                  jtifrom(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfromsp.c`|ifrom|jtifrom|`(^\|[ \t]+\|[^\d\w_])ifrom\((?=([^,]+?),([^)]+?)\))`|`\1jtifrom(jt,`|
|`jsrc\ja.h`|#define ifxi(x,y)                   jtifxi(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ap.c`|ifxi|jtifxi|`(^\|[ \t]+\|[^\d\w_])ifxi\((?=([^,]+?),([^)]+?)\))`|`\1jtifxi(jt,`|
|`jsrc\ja.h`|#define iindx(x,y)                  jtiindx(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c`|iindx|jtiindx|`(^\|[ \t]+\|[^\d\w_])iindx\((?=([^,]+?),([^)]+?)\))`|`\1jtiindx(jt,`|
|`jsrc\ja.h`|#define indexof(x,y)                jtindexof(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ab.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\adverbs\ao.c` `jsrc\conjunctions\ch.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\cu.c` `jsrc\representations\rl.c` `jsrc\verbs\v.c` `jsrc\verbs\v1.c` `jsrc\verbs\vb.c` `jsrc\verbs\vcant.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vi.c` `jsrc\verbs\visp.c` `jsrc\verbs\vp.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c` `jsrc\words\w.c` `jsrc\words\wn.c` `jsrc\xenos\xb.c` `jsrc\xenos\xl.c` `jsrc\xenos\xo.c` `jsrc\xenos\xs.c` `jsrc\xenos\xt.c`|indexof|jtindexof|`(^\|[ \t]+\|[^\d\w_])indexof\((?=([^,]+?),([^)]+?)\))`|`\1jtindexof(jt,`|
|`jsrc\ja.h`|#define intdiv(x,y)                 jtintdiv(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\ca.c`|intdiv|jtintdiv|`(^\|[ \t]+\|[^\d\w_])intdiv\((?=([^,]+?),([^)]+?)\))`|`\1jtintdiv(jt,`|
|`jsrc\ja.h`|#define intmod2(x,y)                jtintmod2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\va2.c`|intmod2|jtintmod2|`(^\|[ \t]+\|[^\d\w_])intmod2\((?=([^,]+?),([^)]+?)\))`|`\1jtintmod2(jt,`|
|`jsrc\ja.h`|#define ioe(x,y)                    jtioe(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\visp.c`|ioe|jtioe|`(^\|[ \t]+\|[^\d\w_])ioe\((?=([^,]+?),([^)]+?)\))`|`\1jtioe(jt,`|
|`jsrc\ja.h`|#define ioev(x,y)                   jtioev(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\visp.c`|ioev|jtioev|`(^\|[ \t]+\|[^\d\w_])ioev\((?=([^,]+?),([^)]+?)\))`|`\1jtioev(jt,`|
|`jsrc\ja.h`|#define istd1(x,y)                  jtistd1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c`|istd1|jtistd1|`(^\|[ \t]+\|[^\d\w_])istd1\((?=([^,]+?),([^)]+?)\))`|`\1jtistd1(jt,`|
|`jsrc\ja.h`|#define jdot2(x,y)                  jtjdot2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vrep.c`|jdot2|jtjdot2|`(^\|[ \t]+\|[^\d\w_])jdot2\((?=([^,]+?),([^)]+?)\))`|`\1jtjdot2(jt,`|
|`jsrc\ja.h`|#define jfwrite(x,y)                jtjfwrite(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define jico2(x,y)                  jtjico2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|jico2|jtjico2|`(^\|[ \t]+\|[^\d\w_])jico2\((?=([^,]+?),([^)]+?)\))`|`\1jtjico2(jt,`|
|`jsrc\ja.h`|#define jope(x,y)                   jtjope(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xf.c` `jsrc\xenos\xo.c`|jope|jtjope|`(^\|[ \t]+\|[^\d\w_])jope\((?=([^,]+?),([^)]+?)\))`|`\1jtjope(jt,`|
|`jsrc\ja.h`|#define jset(x,y)                   jtjset(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\io.c`|jset|jtjset|`(^\|[ \t]+\|[^\d\w_])jset\((?=([^,]+?),([^)]+?)\))`|`\1jtjset(jt,`|
|`jsrc\ja.h`|#define jsig(x,y)                   jtjsig(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\d.c`|jsig|jtjsig|`(^\|[ \t]+\|[^\d\w_])jsig\((?=([^,]+?),([^)]+?)\))`|`\1jtjsig(jt,`|
|`jsrc\ja.h`|#define lamin2(x,y)                 jtlamin2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cc.c` `jsrc\verbs\v2.c`|lamin2|jtlamin2|`(^\|[ \t]+\|[^\d\w_])lamin2\((?=([^,]+?),([^)]+?)\))`|`\1jtlamin2(jt,`|
|`jsrc\ja.h`|#define less(x,y)                   jtless(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\representations\rl.c` `jsrc\verbs\vcant.c` `jsrc\verbs\vcat.c` `jsrc\verbs\vfrom.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vi.c` `jsrc\verbs\vs.c` `jsrc\verbs\vx.c`|less|jtless|`(^\|[ \t]+\|[^\d\w_])less\((?=([^,]+?),([^)]+?)\))`|`\1jtless(jt,`|
|`jsrc\ja.h`|#define lev1(x,y)                   jtlev1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cl.c`|lev1|jtlev1|`(^\|[ \t]+\|[^\d\w_])lev1\((?=([^,]+?),([^)]+?)\))`|`\1jtlev1(jt,`|
|`jsrc\ja.h`|#define levs1(x,y)                  jtlevs1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cl.c`|levs1|jtlevs1|`(^\|[ \t]+\|[^\d\w_])levs1\((?=([^,]+?),([^)]+?)\))`|`\1jtlevs1(jt,`|
|`jsrc\ja.h`|#define locale(x,y)                 jtlocale(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\sl.c`|locale|jtlocale|`(^\|[ \t]+\|[^\d\w_])locale\((?=([^,]+?),([^)]+?)\))`|`\1jtlocale(jt,`|
|`jsrc\ja.h`|#define mapx(x,y)                   jtmapx(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vfrom.c`|mapx|jtmapx|`(^\|[ \t]+\|[^\d\w_])mapx\((?=([^,]+?),([^)]+?)\))`|`\1jtmapx(jt,`|
|`jsrc\ja.h`|#define match(x,y)                  jtmatch(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define matchs(x,y)                 jtmatchs(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v1.c`|matchs|jtmatchs|`(^\|[ \t]+\|[^\d\w_])matchs\((?=([^,]+?),([^)]+?)\))`|`\1jtmatchs(jt,`|
|`jsrc\ja.h`|#define matth1(x,y)                 jtmatth1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\format\f.c`|matth1|jtmatth1|`(^\|[ \t]+\|[^\d\w_])matth1\((?=([^,]+?),([^)]+?)\))`|`\1jtmatth1(jt,`|
|`jsrc\ja.h`|#define maxdenom(x,y)               jtmaxdenom(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|maxdenom|jtmaxdenom|`(^\|[ \t]+\|[^\d\w_])maxdenom\((?=([^,]+?),([^)]+?)\))`|`\1jtmaxdenom(jt,`|
|`jsrc\ja.h`|#define maxtypene(x,y)              jtmaxtype(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h` `jsrc\adverbs\ar.c` `jsrc\verbs\vo.c`|maxtypene|jtmaxtype|`(^\|[ \t]+\|[^\d\w_])maxtypene\((?=([^,]+?),([^)]+?)\))`|`\1jtmaxtype(jt,`|
|`jsrc\ja.h`|#define mdivsp(x,y)                 jtmdivsp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vd.c`|mdivsp|jtmdivsp|`(^\|[ \t]+\|[^\d\w_])mdivsp\((?=([^,]+?),([^)]+?)\))`|`\1jtmdivsp(jt,`|
|`jsrc\ja.h`|#define merge1(x,y)                 jtmerge1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am.c`|merge1|jtmerge1|`(^\|[ \t]+\|[^\d\w_])merge1\((?=([^,]+?),([^)]+?)\))`|`\1jtmerge1(jt,`|
|`jsrc\ja.h`|#define mnomx(x,y)                  jtmnomx(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|mnomx|jtmnomx|`(^\|[ \t]+\|[^\d\w_])mnomx\((?=([^,]+?),([^)]+?)\))`|`\1jtmnomx(jt,`|
|`jsrc\ja.h`|#define mt_init_by_array(x,y)       jtmt_init_by_array(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vrand.c`|mt_init_by_array|jtmt_init_by_array|`(^\|[ \t]+\|[^\d\w_])mt_init_by_array\((?=([^,]+?),([^)]+?)\))`|`\1jtmt_init_by_array(jt,`|
|`jsrc\ja.h`|#define multiple(x,y)               jtmultiple(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|multiple|jtmultiple|`(^\|[ \t]+\|[^\d\w_])multiple\((?=([^,]+?),([^)]+?)\))`|`\1jtmultiple(jt,`|
|`jsrc\ja.h`|#define nameref(x,y)                jtnameref(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\px.c` `jsrc\s.c` `jsrc\adverbs\a.c` `jsrc\adverbs\ar.c` `jsrc\parsing\pv.c`|nameref|jtnameref|`(^\|[ \t]+\|[^\d\w_])nameref\((?=([^,]+?),([^)]+?)\))`|`\1jtnameref(jt,`|
|`jsrc\ja.h`|#define namerefop(x,y)              jtnamerefop(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\sc.c` `jsrc\conjunctions\cx.c`|namerefop|jtnamerefop|`(^\|[ \t]+\|[^\d\w_])namerefop\((?=([^,]+?),([^)]+?)\))`|`\1jtnamerefop(jt,`|
|`jsrc\ja.h`|#define namerefacv(x,y)             jtnamerefacv(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\sc.c` `jsrc\parsing\p.c`|namerefacv|jtnamerefacv|`(^\|[ \t]+\|[^\d\w_])namerefacv\((?=([^,]+?),([^)]+?)\))`|`\1jtnamerefacv(jt,`|
|`jsrc\ja.h`|#define nfs(x,y)                    jtnfs(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\io.c` `jsrc\m.c` `jsrc\px.c` `jsrc\s.c` `jsrc\sl.c` `jsrc\sn.c` `jsrc\adverbs\a.c` `jsrc\adverbs\af.c` `jsrc\adverbs\ar.c` `jsrc\conjunctions\cx.c` `jsrc\words\w.c` `jsrc\words\wc.c`|nfs|jtnfs|`(^\|[ \t]+\|[^\d\w_])nfs\((?=([^,]+?),([^)]+?)\))`|`\1jtnfs(jt,`|
|`jsrc\ja.h`|#define numcase(x0,x1)              jtnumcase(jt,(x0),(x1))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\words\wn.c`|numcase|jtnumcase|`(^\|[ \t]+\|[^\d\w_])numcase\((?=([^,]+?),([^)]+?)\))`|`\1jtnumcase(jt,`|
|`jsrc\ja.h`|#define oblique(x,y)                jtoblique(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ao.c`|oblique|jtoblique|`(^\|[ \t]+\|[^\d\w_])oblique\((?=([^,]+?),([^)]+?)\))`|`\1jtoblique(jt,`|
|`jsrc\ja.h`|#define obqfslash(x,y)              jtobqfslash(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ao.c`|obqfslash|jtobqfslash|`(^\|[ \t]+\|[^\d\w_])obqfslash\((?=([^,]+?),([^)]+?)\))`|`\1jtobqfslash(jt,`|
|`jsrc\ja.h`|#define obverse(x,y)                jtobverse(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c`|obverse|jtobverse|`(^\|[ \t]+\|[^\d\w_])obverse\((?=([^,]+?),([^)]+?)\))`|`\1jtobverse(jt,`|
|`jsrc\ja.h`|#define omask(x,y)                  jtomask(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\as.c`|omask|jtomask|`(^\|[ \t]+\|[^\d\w_])omask\((?=([^,]+?),([^)]+?)\))`|`\1jtomask(jt,`|
|`jsrc\ja.h`|#define ordstat(x,y)                jtordstat(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vg.c`|ordstat|jtordstat|`(^\|[ \t]+\|[^\d\w_])ordstat\((?=([^,]+?),([^)]+?)\))`|`\1jtordstat(jt,`|
|`jsrc\ja.h`|#define ovs(x,y)                    jtovs(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vcat.c`|ovs|jtovs|`(^\|[ \t]+\|[^\d\w_])ovs\((?=([^,]+?),([^)]+?)\))`|`\1jtovs(jt,`|
|`jsrc\ja.h`|#define parsea(x,y)                 jtparsea(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cx.c` `jsrc\debugging\dsusp.c` `jsrc\parsing\p.c`|parsea|jtparsea|`(^\|[ \t]+\|[^\d\w_])parsea\((?=([^,]+?),([^)]+?)\))`|`\1jtparsea(jt,`|
|`jsrc\ja.h`|#define paxis(x,y)                  jtpaxis(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vf.c` `jsrc\verbs\vs.c` `jsrc\verbs\vt.c`|paxis|jtpaxis|`(^\|[ \t]+\|[^\d\w_])paxis\((?=([^,]+?),([^)]+?)\))`|`\1jtpaxis(jt,`|
|`jsrc\ja.h`|#define pcvt(x,y)                   jtpcvt(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\a.c` `jsrc\conjunctions\ca.c` `jsrc\verbs\v2.c` `jsrc\verbs\va2.c`|pcvt|jtpcvt|`(^\|[ \t]+\|[^\d\w_])pcvt\((?=([^,]+?),([^)]+?)\))`|`\1jtpcvt(jt,`|
|`jsrc\ja.h`|#define pdt(x,y)                    jtpdt(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v.c` `jsrc\verbs\v0.c` `jsrc\verbs\vd.c` `jsrc\verbs\ve.c` `jsrc\verbs\vfrom.c` `jsrc\verbs\vs.c`|pdt|jtpdt|`(^\|[ \t]+\|[^\d\w_])pdt\((?=([^,]+?),([^)]+?)\))`|`\1jtpdt(jt,`|
|`jsrc\ja.h`|#define pdtby(x,y)                  jtpdtby(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cip.c`|pdtby|jtpdtby|`(^\|[ \t]+\|[^\d\w_])pdtby\((?=([^,]+?),([^)]+?)\))`|`\1jtpdtby(jt,`|
|`jsrc\ja.h`|#define pdtsp(x,y)                  jtpdtsp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cip.c`|pdtsp|jtpdtsp|`(^\|[ \t]+\|[^\d\w_])pdtsp\((?=([^,]+?),([^)]+?)\))`|`\1jtpdtsp(jt,`|
|`jsrc\ja.h`|#define pdtspmv(x,y)                jtpdtspmv(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cpdtsp.c`|pdtspmv|jtpdtspmv|`(^\|[ \t]+\|[^\d\w_])pdtspmv\((?=([^,]+?),([^)]+?)\))`|`\1jtpdtspmv(jt,`|
|`jsrc\ja.h`|#define pdtspmm(x,y)                jtpdtspmm(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cpdtsp.c`|pdtspmm|jtpdtspmm|`(^\|[ \t]+\|[^\d\w_])pdtspmm\((?=([^,]+?),([^)]+?)\))`|`\1jtpdtspmm(jt,`|
|`jsrc\ja.h`|#define pdtspvm(x,y)                jtpdtspvm(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cpdtsp.c`|pdtspvm|jtpdtspvm|`(^\|[ \t]+\|[^\d\w_])pdtspvm\((?=([^,]+?),([^)]+?)\))`|`\1jtpdtspvm(jt,`|
|`jsrc\ja.h`|#define pdtspvv(x,y)                jtpdtspvv(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cpdtsp.c`|pdtspvv|jtpdtspvv|`(^\|[ \t]+\|[^\d\w_])pdtspvv\((?=([^,]+?),([^)]+?)\))`|`\1jtpdtspvv(jt,`|
|`jsrc\ja.h`|#define pfill(x,y)                  jtpfill(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vcant.c` `jsrc\verbs\vp.c`|pfill|jtpfill|`(^\|[ \t]+\|[^\d\w_])pfill\((?=([^,]+?),([^)]+?)\))`|`\1jtpfill(jt,`|
|`jsrc\ja.h`|#define piev(x,y)                   jtpiev(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vx.c`|piev|jtpiev|`(^\|[ \t]+\|[^\d\w_])piev\((?=([^,]+?),([^)]+?)\))`|`\1jtpiev(jt,`|
|`jsrc\ja.h`|#define pind(x,y)                   jtpind(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am.c` `jsrc\adverbs\am1.c` `jsrc\verbs\vfrom.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vp.c`|pind|jtpind|`(^\|[ \t]+\|[^\d\w_])pind\((?=([^,]+?),([^)]+?)\))`|`\1jtpind(jt,`|
|`jsrc\ja.h`|#define pmarea2(x,y)                jtpmarea2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xt.c`|pmarea2|jtpmarea2|`(^\|[ \t]+\|[^\d\w_])pmarea2\((?=([^,]+?),([^)]+?)\))`|`\1jtpmarea2(jt,`|
|`jsrc\ja.h`|#define poly2a(x,y)                 jtpoly2a(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|poly2a|jtpoly2a|`(^\|[ \t]+\|[^\d\w_])poly2a\((?=([^,]+?),([^)]+?)\))`|`\1jtpoly2a(jt,`|
|`jsrc\ja.h`|#define powseqlim(x,y)              jtpowseqlim(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cp.c`|powseqlim|jtpowseqlim|`(^\|[ \t]+\|[^\d\w_])powseqlim\((?=([^,]+?),([^)]+?)\))`|`\1jtpowseqlim(jt,`|
|`jsrc\ja.h`|#define prefix(x,y)                 jtprefix(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define probeis(x,y)                jtprobeis(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\s.c` `jsrc\conjunctions\cx.c`|probeis|jtprobeis|`(^\|[ \t]+\|[^\d\w_])probeis\((?=([^,]+?),([^)]+?)\))`|`\1jtprobeis(jt,`|
|`jsrc\ja.h`|#define probelocal(x,y)             jtprobelocal(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\s.c` `jsrc\sc.c` `jsrc\adverbs\af.c`|probelocal|jtprobelocal|`(^\|[ \t]+\|[^\d\w_])probelocal\((?=([^,]+?),([^)]+?)\))`|`\1jtprobelocal(jt,`|
|`jsrc\ja.h`|#define prod(x,y)                   jtprod(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\j.h` `jsrc\adverbs\am1.c` `jsrc\adverbs\ar.c` `jsrc\format\f.c` `jsrc\verbs\v.c` `jsrc\verbs\vf.c` `jsrc\verbs\vg.c` `jsrc\verbs\vgranking.c` `jsrc\verbs\vgsp.c` `jsrc\verbs\vi.c` `jsrc\verbs\visp.c` `jsrc\verbs\vo.c` `jsrc\verbs\vs.c` `jsrc\xenos\xfmt.c`|prod|jtprod|`(^\|[ \t]+\|[^\d\w_])prod\((?=([^,]+?),([^)]+?)\))`|`\1jtprod(jt,`|
|`jsrc\ja.h`|#define pscan(x,y)                  jtpscan(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define qco2(x,y)                   jtqco2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v2.c`|qco2|jtqco2|`(^\|[ \t]+\|[^\d\w_])qco2\((?=([^,]+?),([^)]+?)\))`|`\1jtqco2(jt,`|
|`jsrc\ja.h`|#define qco2x(x,y)                  jtqco2x(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v2.c`|qco2x|jtqco2x|`(^\|[ \t]+\|[^\d\w_])qco2x\((?=([^,]+?),([^)]+?)\))`|`\1jtqco2x(jt,`|
|`jsrc\ja.h`|#define reaxis(x,y)                 jtreaxis(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\amn.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\crs.c` `jsrc\verbs\v.c` `jsrc\verbs\v1.c` `jsrc\verbs\va2s.c` `jsrc\verbs\vcat.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vgsp.c` `jsrc\verbs\visp.c` `jsrc\verbs\vo.c` `jsrc\verbs\vs.c`|reaxis|jtreaxis|`(^\|[ \t]+\|[^\d\w_])reaxis\((?=([^,]+?),([^)]+?)\))`|`\1jtreaxis(jt,`|
|`jsrc\ja.h`|#define red0(x,y)                   jtred0(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ar.c`|red0|jtred0|`(^\|[ \t]+\|[^\d\w_])red0\((?=([^,]+?),([^)]+?)\))`|`\1jtred0(jt,`|
|`jsrc\ja.h`|#define redcat(x,y)                 jtredcat(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ao.c`|redcat|jtredcat|`(^\|[ \t]+\|[^\d\w_])redcat\((?=([^,]+?),([^)]+?)\))`|`\1jtredcat(jt,`|
|`jsrc\ja.h`|#define redef(x,y)                  jtredef(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\s.c` `jsrc\sl.c` `jsrc\sn.c`|redef|jtredef|`(^\|[ \t]+\|[^\d\w_])redef\((?=([^,]+?),([^)]+?)\))`|`\1jtredef(jt,`|
|`jsrc\ja.h`|#define reduce(x,y)                 jtreduce(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ar.c`|reduce|jtreduce|`(^\|[ \t]+\|[^\d\w_])reduce\((?=([^,]+?),([^)]+?)\))`|`\1jtreduce(jt,`|
|`jsrc\ja.h`|#define reducesp(x,y)               jtreducesp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ar.c`|reducesp|jtreducesp|`(^\|[ \t]+\|[^\d\w_])reducesp\((?=([^,]+?),([^)]+?)\))`|`\1jtreducesp(jt,`|
|`jsrc\ja.h`|#define reitem(x,y)                 jtreitem(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c` `jsrc\adverbs\ao.c` `jsrc\adverbs\ap.c` `jsrc\adverbs\ar.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cg.c` `jsrc\verbs\v.c` `jsrc\verbs\vf.c` `jsrc\verbs\vgranking.c` `jsrc\xenos\xfmt.c`|reitem|jtreitem|`(^\|[ \t]+\|[^\d\w_])reitem\((?=([^,]+?),([^)]+?)\))`|`\1jtreitem(jt,`|
|`jsrc\ja.h`|#define repeat(x,y)                 jtrepeat(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\adverbs\ao.c` `jsrc\adverbs\ar.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\cf.c` `jsrc\conjunctions\ch.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\crs.c` `jsrc\verbs\v.c` `jsrc\verbs\v1.c` `jsrc\verbs\v2.c` `jsrc\verbs\vf.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vi.c` `jsrc\verbs\visp.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c` `jsrc\verbs\vx.c` `jsrc\xenos\xt.c`|repeat|jtrepeat|`(^\|[ \t]+\|[^\d\w_])repeat\((?=([^,]+?),([^)]+?)\))`|`\1jtrepeat(jt,`|
|`jsrc\ja.h`|#define repeatr(x,y)                jtrepeatr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\amn.c` `jsrc\adverbs\ar.c`|repeatr|jtrepeatr|`(^\|[ \t]+\|[^\d\w_])repeatr\((?=([^,]+?),([^)]+?)\))`|`\1jtrepeatr(jt,`|
|`jsrc\ja.h`|#define reshape(x,y)                jtreshape(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\io.c` `jsrc\adverbs\a.c` `jsrc\adverbs\ai.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\adverbs\ao.c` `jsrc\adverbs\ap.c` `jsrc\adverbs\ar.c` `jsrc\adverbs\as.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cg.c` `jsrc\conjunctions\cip.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cr.c` `jsrc\conjunctions\crs.c` `jsrc\format\f2.c` `jsrc\representations\r.c` `jsrc\verbs\v.c` `jsrc\verbs\v0.c` `jsrc\verbs\v2.c` `jsrc\verbs\vb.c` `jsrc\verbs\vcat.c` `jsrc\verbs\vd.c` `jsrc\verbs\ve.c` `jsrc\verbs\vf.c` `jsrc\verbs\vfrom.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vg.c` `jsrc\verbs\vgranking.c` `jsrc\verbs\vgsp.c` `jsrc\verbs\vi.c` `jsrc\verbs\visp.c` `jsrc\verbs\vo.c` `jsrc\verbs\vrand.c` `jsrc\verbs\vs.c` `jsrc\verbs\vt.c` `jsrc\xenos\xd.c` `jsrc\xenos\xfmt.c`|reshape|jtreshape|`(^\|[ \t]+\|[^\d\w_])reshape\((?=([^,]+?),([^)]+?)\))`|`\1jtreshape(jt,`|
|`jsrc\ja.h`|#define rezero(x,y)                 jtrezero(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\visp.c` `jsrc\verbs\vs.c`|rezero|jtrezero|`(^\|[ \t]+\|[^\d\w_])rezero\((?=([^,]+?),([^)]+?)\))`|`\1jtrezero(jt,`|
|`jsrc\ja.h`|#define rfcz(x,y)                   jtrfcz(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v0.c`|rfcz|jtrfcz|`(^\|[ \t]+\|[^\d\w_])rfcz\((?=([^,]+?),([^)]+?)\))`|`\1jtrfcz(jt,`|
|`jsrc\ja.h`|#define rngga(x,y)                  jtrngga(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vrand.c`|rngga|jtrngga|`(^\|[ \t]+\|[^\d\w_])rngga\((?=([^,]+?),([^)]+?)\))`|`\1jtrngga(jt,`|
|`jsrc\ja.h`|#define rollksub(x,y)               jtrollksub(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vrand.c`|rollksub|jtrollksub|`(^\|[ \t]+\|[^\d\w_])rollksub\((?=([^,]+?),([^)]+?)\))`|`\1jtrollksub(jt,`|
|`jsrc\ja.h`|#define rollnot0(x,y)               jtrollnot0(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vrand.c`|rollnot0|jtrollnot0|`(^\|[ \t]+\|[^\d\w_])rollnot0\((?=([^,]+?),([^)]+?)\))`|`\1jtrollnot0(jt,`|
|`jsrc\ja.h`|#define root(x,y)                   jtroot(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define rotate(x,y)                 jtrotate(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vf.c`|rotate|jtrotate|`(^\|[ \t]+\|[^\d\w_])rotate\((?=([^,]+?),([^)]+?)\))`|`\1jtrotate(jt,`|
|`jsrc\ja.h`|#define rotsp(x,y)                  jtrotsp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vf.c`|rotsp|jtrotsp|`(^\|[ \t]+\|[^\d\w_])rotsp\((?=([^,]+?),([^)]+?)\))`|`\1jtrotsp(jt,`|
|`jsrc\ja.h`|#define roundID(x,y)                jtroundID(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xfmt.c`|roundID|jtroundID|`(^\|[ \t]+\|[^\d\w_])roundID\((?=([^,]+?),([^)]+?)\))`|`\1jtroundID(jt,`|
|`jsrc\ja.h`|#define sb2(x,y)                    jtsb2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\rl.c`|sb2|jtsb2|`(^\|[ \t]+\|[^\d\w_])sb2\((?=([^,]+?),([^)]+?)\))`|`\1jtsb2(jt,`|
|`jsrc\ja.h`|#define sblit(x,y)                  jtsblit(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vsb.c`|sblit|jtsblit|`(^\|[ \t]+\|[^\d\w_])sblit\((?=([^,]+?),([^)]+?)\))`|`\1jtsblit(jt,`|
|`jsrc\ja.h`|#define sbstr(x,y)                  jtsbstr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vsb.c`|sbstr|jtsbstr|`(^\|[ \t]+\|[^\d\w_])sbstr\((?=([^,]+?),([^)]+?)\))`|`\1jtsbstr(jt,`|
|`jsrc\ja.h`|#define sbunlit(x,y)                jtsbunlit(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vsb.c`|sbunlit|jtsbunlit|`(^\|[ \t]+\|[^\d\w_])sbunlit\((?=([^,]+?),([^)]+?)\))`|`\1jtsbunlit(jt,`|
|`jsrc\ja.h`|#define sbunstr(x,y)                jtsbunstr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vsb.c`|sbunstr|jtsbunstr|`(^\|[ \t]+\|[^\d\w_])sbunstr\((?=([^,]+?),([^)]+?)\))`|`\1jtsbunstr(jt,`|
|`jsrc\ja.h`|#define sc4(x,y)                    jtsc4(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\rl.c`|sc4|jtsc4|`(^\|[ \t]+\|[^\d\w_])sc4\((?=([^,]+?),([^)]+?)\))`|`\1jtsc4(jt,`|
|`jsrc\ja.h`|#define scfn(x,y)                   jtscfn(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cl.c`|scfn|jtscfn|`(^\|[ \t]+\|[^\d\w_])scfn\((?=([^,]+?),([^)]+?)\))`|`\1jtscfn(jt,`|
|`jsrc\ja.h`|#define scubb(x,y)                  jtscubb(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c`|scubb|jtscubb|`(^\|[ \t]+\|[^\d\w_])scubb\((?=([^,]+?),([^)]+?)\))`|`\1jtscubb(jt,`|
|`jsrc\ja.h`|#define seg(x,y)                    jtseg(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ap.c`|seg|jtseg|`(^\|[ \t]+\|[^\d\w_])seg\((?=([^,]+?),([^)]+?)\))`|`\1jtseg(jt,`|
|`jsrc\ja.h`|#define setfv(x,y)                  jtsetfv(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vcat.c` `jsrc\verbs\vf.c` `jsrc\verbs\vt.c`|setfv|jtsetfv|`(^\|[ \t]+\|[^\d\w_])setfv\((?=([^,]+?),([^)]+?)\))`|`\1jtsetfv(jt,`|
|`jsrc\ja.h`|#define sfn(x,y)                    jtsfn(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\s.c` `jsrc\sl.c` `jsrc\sn.c` `jsrc\adverbs\af.c` `jsrc\debugging\d.c` `jsrc\debugging\dc.c` `jsrc\debugging\dsusp.c` `jsrc\format\f.c` `jsrc\parsing\pv.c` `jsrc\representations\r.c` `jsrc\representations\rl.c` `jsrc\xenos\xt.c`|sfn|jtsfn|`(^\|[ \t]+\|[^\d\w_])sfn\((?=([^,]+?),([^)]+?)\))`|`\1jtsfn(jt,`|
|`jsrc\ja.h`|#define shift10(x,y)                jtshift10(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vx.c`|shift10|jtshift10|`(^\|[ \t]+\|[^\d\w_])shift10\((?=([^,]+?),([^)]+?)\))`|`\1jtshift10(jt,`|
|`jsrc\ja.h`|#define sprarg(x,y)                 jtsprarg(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\crs.c`|sprarg|jtsprarg|`(^\|[ \t]+\|[^\d\w_])sprarg\((?=([^,]+?),([^)]+?)\))`|`\1jtsprarg(jt,`|
|`jsrc\ja.h`|#define ssel(x,y)                   jtssel(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c`|ssel|jtssel|`(^\|[ \t]+\|[^\d\w_])ssel\((?=([^,]+?),([^)]+?)\))`|`\1jtssel(jt,`|
|`jsrc\ja.h`|#define ssnext(x,y)                 jtssnext(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\debugging\dss.c` `jsrc\debugging\dsusp.c`|ssnext|jtssnext|`(^\|[ \t]+\|[^\d\w_])ssnext\((?=([^,]+?),([^)]+?)\))`|`\1jtssnext(jt,`|
|`jsrc\ja.h`|#define stitch(x,y)                 jtstitch(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\adverbs\ao.c` `jsrc\conjunctions\crs.c` `jsrc\representations\rt.c` `jsrc\verbs\vd.c` `jsrc\verbs\vf.c` `jsrc\verbs\vi.c` `jsrc\verbs\visp.c` `jsrc\verbs\vrep.c` `jsrc\verbs\vs.c` `jsrc\xenos\xo.c`|stitch|jtstitch|`(^\|[ \t]+\|[^\d\w_])stitch\((?=([^,]+?),([^)]+?)\))`|`\1jtstitch(jt,`|
|`jsrc\ja.h`|#define stitchsp2(x,y)              jtstitchsp2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vcat.c`|stitchsp2|jtstitchsp2|`(^\|[ \t]+\|[^\d\w_])stitchsp2\((?=([^,]+?),([^)]+?)\))`|`\1jtstitchsp2(jt,`|
|`jsrc\ja.h`|#define str(x,y)                    jtstr(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\i.c` `jsrc\io.c` `jsrc\sn.c` `jsrc\u.c` `jsrc\conjunctions\cx.c` `jsrc\debugging\d.c` `jsrc\format\f.c` `jsrc\parsing\p.c` `jsrc\representations\rl.c` `jsrc\verbs\vsb.c` `jsrc\words\w.c` `jsrc\words\wc.c` `jsrc\words\wn.c` `jsrc\words\ws.c` `jsrc\xenos\xa.c` `jsrc\xenos\xfmt.c`|str|jtstr|`(^\|[ \t]+\|[^\d\w_])str\((?=([^,]+?),([^)]+?)\))`|`\1jtstr(jt,`|
|`jsrc\ja.h`|#define strq(x,y)                   jtstrq(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cx.c`|strq|jtstrq|`(^\|[ \t]+\|[^\d\w_])strq\((?=([^,]+?),([^)]+?)\))`|`\1jtstrq(jt,`|
|`jsrc\ja.h`|#define suffix(x,y)                 jtsuffix(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define sup(x,y)                    jtsup(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v2.c`|sup|jtsup|`(^\|[ \t]+\|[^\d\w_])sup\((?=([^,]+?),([^)]+?)\))`|`\1jtsup(jt,`|
|`jsrc\ja.h`|#define suq(x,y)                    jtsuq(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v2.c`|suq|jtsuq|`(^\|[ \t]+\|[^\d\w_])suq\((?=([^,]+?),([^)]+?)\))`|`\1jtsuq(jt,`|
|`jsrc\ja.h`|#define symnew(x,y)                 jtsymnew(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\s.c` `jsrc\sl.c` `jsrc\conjunctions\cx.c`|symnew|jtsymnew|`(^\|[ \t]+\|[^\d\w_])symnew\((?=([^,]+?),([^)]+?)\))`|`\1jtsymnew(jt,`|
|`jsrc\ja.h`|#define syrd(x,y)                   jtsyrd(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\s.c` `jsrc\sc.c` `jsrc\sn.c` `jsrc\conjunctions\cf.c` `jsrc\parsing\p.c`|syrd|jtsyrd|`(^\|[ \t]+\|[^\d\w_])syrd\((?=([^,]+?),([^)]+?)\))`|`\1jtsyrd(jt,`|
|`jsrc\ja.h`|#define take(x,y)                   jttake(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c` `jsrc\sl.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\ap.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\ch.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cx.c` `jsrc\format\f.c` `jsrc\representations\rl.c` `jsrc\verbs\v0.c` `jsrc\verbs\v2.c` `jsrc\verbs\vcat.c` `jsrc\verbs\vd.c` `jsrc\verbs\vf.c` `jsrc\verbs\vgcomp.c` `jsrc\verbs\vi.c` `jsrc\verbs\vo.c` `jsrc\verbs\vs.c` `jsrc\verbs\vt.c` `jsrc\xenos\xl.c`|take|jttake|`(^\|[ \t]+\|[^\d\w_])take\((?=([^,]+?),([^)]+?)\))`|`\1jttake(jt,`|
|`jsrc\ja.h`|#define taker(x,y)                  jttaker(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\am1.c` `jsrc\verbs\v2.c` `jsrc\verbs\vf.c` `jsrc\verbs\visp.c`|taker|jttaker|`(^\|[ \t]+\|[^\d\w_])taker\((?=([^,]+?),([^)]+?)\))`|`\1jttaker(jt,`|
|`jsrc\ja.h`|#define tclosure(x,y)               jttclosure(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cp.c`|tclosure|jttclosure|`(^\|[ \t]+\|[^\d\w_])tclosure\((?=([^,]+?),([^)]+?)\))`|`\1jttclosure(jt,`|
|`jsrc\ja.h`|#define teq(x,y)                    jtteq(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define tesa(x,y)                   jttesa(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\conjunctions\cc.c`|tesa|jttesa|`(^\|[ \t]+\|[^\d\w_])tesa\((?=([^,]+?),([^)]+?)\))`|`\1jttesa(jt,`|
|`jsrc\ja.h`|#define th2box(x,y)                 jtth2box(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\format\f2.c`|th2box|jtth2box|`(^\|[ \t]+\|[^\d\w_])th2box\((?=([^,]+?),([^)]+?)\))`|`\1jtth2box(jt,`|
|`jsrc\ja.h`|#define thbox(x,y)                  jtthbox(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\format\f.c`|thbox|jtthbox|`(^\|[ \t]+\|[^\d\w_])thbox\((?=([^,]+?),([^)]+?)\))`|`\1jtthbox(jt,`|
|`jsrc\ja.h`|#define thorn1main(x,y)             jtthorn1main(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\format\f.c`|thorn1main|jtthorn1main|`(^\|[ \t]+\|[^\d\w_])thorn1main\((?=([^,]+?),([^)]+?)\))`|`\1jtthorn1main(jt,`|
|`jsrc\ja.h`|#define thsb(x,y)                   jtthsb(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\format\f.c`|thsb|jtthsb|`(^\|[ \t]+\|[^\d\w_])thsb\((?=([^,]+?),([^)]+?)\))`|`\1jtthsb(jt,`|
|`jsrc\ja.h`|#define tk(x,y)                     jttk(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vt.c`|tk|jttk|`(^\|[ \t]+\|[^\d\w_])tk\((?=([^,]+?),([^)]+?)\))`|`\1jttk(jt,`|
|`jsrc\ja.h`|#define tks(x,y)                    jttks(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vt.c`|tks|jttks|`(^\|[ \t]+\|[^\d\w_])tks\((?=([^,]+?),([^)]+?)\))`|`\1jttks(jt,`|
|`jsrc\ja.h`|#define tlt(x,y)                    jttlt(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vcomp.h`|tlt|jttlt|`(^\|[ \t]+\|[^\d\w_])tlt\((?=([^,]+?),([^)]+?)\))`|`\1jttlt(jt,`|
|`jsrc\ja.h`|#define toc1(x,y)                   jttoc1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\vu.c` `jsrc\words\wn.c`|toc1|jttoc1|`(^\|[ \t]+\|[^\d\w_])toc1\((?=([^,]+?),([^)]+?)\))`|`\1jttoc1(jt,`|
|`jsrc\ja.h`|#define tokens(x,y)                 jttokens(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\px.c` `jsrc\conjunctions\ca.c` `jsrc\conjunctions\cx.c` `jsrc\parsing\pv.c`|tokens|jttokens|`(^\|[ \t]+\|[^\d\w_])tokens\((?=([^,]+?),([^)]+?)\))`|`\1jttokens(jt,`|
|`jsrc\ja.h`|#define toutf8a(x,y)                jttoutf8a(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xu.c`|toutf8a|jttoutf8a|`(^\|[ \t]+\|[^\d\w_])toutf8a\((?=([^,]+?),([^)]+?)\))`|`\1jttoutf8a(jt,`|
|`jsrc\ja.h`|#define traverse(x,y)               jttraverse(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\m.c`|traverse|jttraverse|`(^\|[ \t]+\|[^\d\w_])traverse\((?=([^,]+?),([^)]+?)\))`|`\1jttraverse(jt,`|
|`jsrc\ja.h`|#define troot(x,y)                  jttroot(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\rt.c`|troot|jttroot|`(^\|[ \t]+\|[^\d\w_])troot\((?=([^,]+?),([^)]+?)\))`|`\1jttroot(jt,`|
|`jsrc\ja.h`|#define tsit2(x,y)                  jttsit2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xt.c`|tsit2|jttsit2|`(^\|[ \t]+\|[^\d\w_])tsit2\((?=([^,]+?),([^)]+?)\))`|`\1jttsit2(jt,`|
|`jsrc\ja.h`|#define uco2(x,y)                   jtuco2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\rl.c` `jsrc\xenos\xfmt.c`|uco2|jtuco2|`(^\|[ \t]+\|[^\d\w_])uco2\((?=([^,]+?),([^)]+?)\))`|`\1jtuco2(jt,`|
|`jsrc\ja.h`|#define under(x,y)                  jtunder(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\adverbs\ai.c`|under|jtunder|`(^\|[ \t]+\|[^\d\w_])under\((?=([^,]+?),([^)]+?)\))`|`\1jtunder(jt,`|
|`jsrc\ja.h`|#define unlock2(x,y)                jtunlock2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\jdlllic.c` `jsrc\xenos\xs.c`|unlock2|jtunlock2|`(^\|[ \t]+\|[^\d\w_])unlock2\((?=([^,]+?),([^)]+?)\))`|`\1jtunlock2(jt,`|
|`jsrc\ja.h`|#define unparsem(x,y)               jtunparsem(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\r.c` `jsrc\representations\rl.c` `jsrc\representations\rt.c`|unparsem|jtunparsem|`(^\|[ \t]+\|[^\d\w_])unparsem\((?=([^,]+?),([^)]+?)\))`|`\1jtunparsem(jt,`|
|`jsrc\ja.h`|#define utype(x,y)                  jtutype(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vi.c`|utype|jtutype|`(^\|[ \t]+\|[^\d\w_])utype\((?=([^,]+?),([^)]+?)\))`|`\1jtutype(jt,`|
|`jsrc\ja.h`|#define v2(x,y)                     jtv2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\io.c` `jsrc\sl.c` `jsrc\adverbs\a.c` `jsrc\adverbs\ai.c` `jsrc\adverbs\am1.c` `jsrc\adverbs\amn.c` `jsrc\adverbs\ao.c` `jsrc\adverbs\ar.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cip.c` `jsrc\conjunctions\cpdtsp.c` `jsrc\conjunctions\crs.c` `jsrc\conjunctions\cu.c` `jsrc\debugging\dc.c` `jsrc\representations\r.c` `jsrc\representations\rt.c` `jsrc\verbs\v2.c` `jsrc\verbs\vd.c` `jsrc\verbs\vf.c` `jsrc\verbs\vfromsp.c` `jsrc\verbs\vgranking.c` `jsrc\verbs\vi.c` `jsrc\verbs\visp.c` `jsrc\verbs\vm.c` `jsrc\verbs\vs.c` `jsrc\verbs\vx.c` `jsrc\xenos\x.c` `jsrc\xenos\x15.c` `jsrc\xenos\xa.c` `jsrc\xenos\xd.c` `jsrc\xenos\xfmt.c` `jsrc\xenos\xt.c`|v2|jtv2|`(^\|[ \t]+\|[^\d\w_])v2\((?=([^,]+?),([^)]+?)\))`|`\1jtv2(jt,`|
|`jsrc\ja.h`|#define va1(x,y)                    jtva1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define vaxis(x,y)                  jtvaxis(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vs.c`|vaxis|jtvaxis|`(^\|[ \t]+\|[^\d\w_])vaxis\((?=([^,]+?),([^)]+?)\))`|`\1jtvaxis(jt,`|
|`jsrc\ja.h`|#define vc1(x,y)                    jtvc1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\xenos\xo.c`|vc1|jtvc1|`(^\|[ \t]+\|[^\d\w_])vc1\((?=([^,]+?),([^)]+?)\))`|`\1jtvc1(jt,`|
|`jsrc\ja.h`|#define vlocnl(x,y)                 jtvlocnl(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\sl.c`|vlocnl|jtvlocnl|`(^\|[ \t]+\|[^\d\w_])vlocnl\((?=([^,]+?),([^)]+?)\))`|`\1jtvlocnl(jt,`|
|`jsrc\ja.h`|#define vnm(x,y)                    jtvnm(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\io.c` `jsrc\m.c` `jsrc\sn.c` `jsrc\adverbs\a.c` `jsrc\adverbs\am.c` `jsrc\representations\rl.c` `jsrc\words\w.c` `jsrc\words\wc.c`|vnm|jtvnm|`(^\|[ \t]+\|[^\d\w_])vnm\((?=([^,]+?),([^)]+?)\))`|`\1jtvnm(jt,`|
|`jsrc\ja.h`|#define weight(x,y)                 jtweight(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\ve.c`|weight|jtweight|`(^\|[ \t]+\|[^\d\w_])weight\((?=([^,]+?),([^)]+?)\))`|`\1jtweight(jt,`|
|`jsrc\ja.h`|#define xbinp(x,y)                  jtxbinp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vx.c`|xbinp|jtxbinp|`(^\|[ \t]+\|[^\d\w_])xbinp\((?=([^,]+?),([^)]+?)\))`|`\1jtxbinp(jt,`|
|`jsrc\ja.h`|#define xd1(x,y)                      jtxd1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\k.c`|xd1|jtxd1|`(^\|[ \t]+\|[^\d\w_])xd1\((?=([^,]+?),([^)]+?)\))`|`\1jtxd1(jt,`|
|`jsrc\ja.h`|#define xev1(x,y)                   jtxev1(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vx.c`|xev1|jtxev1|`(^\|[ \t]+\|[^\d\w_])xev1\((?=([^,]+?),([^)]+?)\))`|`\1jtxev1(jt,`|
|`jsrc\ja.h`|#define xexp(x,y)                   jtxexp(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vx.c`|xexp|jtxexp|`(^\|[ \t]+\|[^\d\w_])xexp\((?=([^,]+?),([^)]+?)\))`|`\1jtxexp(jt,`|
|`jsrc\ja.h`|#define xlog2(x,y)                  jtxlog2(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define xlog2sub(x,y)               jtxlog2sub(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\ja.h`|#define xprimeq(x,y)                jtxprimeq(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\v2.c`|xprimeq|jtxprimeq|`(^\|[ \t]+\|[^\d\w_])xprimeq\((?=([^,]+?),([^)]+?)\))`|`\1jtxprimeq(jt,`|
|`jsrc\ja.h`|#define xrep(x,y)                   jtxrep(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\representations\r.c`|xrep|jtxrep|`(^\|[ \t]+\|[^\d\w_])xrep\((?=([^,]+?),([^)]+?)\))`|`\1jtxrep(jt,`|
|`jsrc\ja.h`|#define xroot(x,y)                  jtxroot(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vx.c`|xroot|jtxroot|`(^\|[ \t]+\|[^\d\w_])xroot\((?=([^,]+?),([^)]+?)\))`|`\1jtxroot(jt,`|
|`jsrc\ja.h`|#define zbin(x,y)                   jtzbin(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vbang.c`|zbin|jtzbin|`(^\|[ \t]+\|[^\d\w_])zbin\((?=([^,]+?),([^)]+?)\))`|`\1jtzbin(jt,`|
|`jsrc\ja.h`|#define zgauss(x,y)                 jtzgauss(jt,(x),(y))||`(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))`|``|
|`jsrc\verbs\vbang.c`|zgauss|jtzgauss|`(^\|[ \t]+\|[^\d\w_])zgauss\((?=([^,]+?),([^)]+?)\))`|`\1jtzgauss(jt,`|