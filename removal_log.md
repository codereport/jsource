|Path|Macro Name|Function|Regex|Replace|
|---|---|---|---|---|
|`jsrc\k.c`|BfromI|jtBfromI|
|`jsrc\k.c`|BfromX|jtBfromX|
|`jsrc\k.c`|C1fromC2|jtC1fromC2|`(^\|[ \t]+\|[^\d\w_])C1fromC2\((?=([^,]+?),([^)]+?)\))`|`\1jtC1fromC2(jt,`|
|`jsrc\k.c`|C2fromC1|jtC2fromC1|`(^\|[ \t]+\|[^\d\w_])C2fromC1\((?=([^,]+?),([^)]+?)\))`|`\1jtC2fromC1(jt,`|
|`jsrc\k.c`|C1fromC4|jtC1fromC4|`(^\|[ \t]+\|[^\d\w_])C1fromC4\((?=([^,]+?),([^)]+?)\))`|`\1jtC1fromC4(jt,`|
|`jsrc\k.c`|C2fromC4|jtC2fromC4|`(^\|[ \t]+\|[^\d\w_])C2fromC4\((?=([^,]+?),([^)]+?)\))`|`\1jtC2fromC4(jt,`|
|`jsrc\k.c`|C4fromC1|jtC4fromC1|`(^\|[ \t]+\|[^\d\w_])C4fromC1\((?=([^,]+?),([^)]+?)\))`|`\1jtC4fromC1(jt,`|
|`jsrc\k.c`|C4fromC2|jtC4fromC2|`(^\|[ \t]+\|[^\d\w_])C4fromC2\((?=([^,]+?),([^)]+?)\))`|`\1jtC4fromC2(jt,`|
|`jsrc\k.c`|DfromQ|jtDfromQ|`(^\|[ \t]+\|[^\d\w_])DfromQ\((?=([^,]+?),([^)]+?)\))`|`\1jtDfromQ(jt,`|
|`jsrc\k.c`|DfromX|jtDfromX|`(^\|[ \t]+\|[^\d\w_])DfromX\((?=([^,]+?),([^)]+?)\))`|`\1jtDfromX(jt,`|
|`jsrc\k.c`|IfromX|jtIfromX|`(^\|[ \t]+\|[^\d\w_])IfromX\((?=([^,]+?),([^)]+?)\))`|`\1jtIfromX(jt,`|
|`jsrc\k.c`|QfromX|jtQfromX|`(^\|[ \t]+\|[^\d\w_])QfromX\((?=([^,]+?),([^)]+?)\))`|`\1jtQfromX(jt,`|
|`jsrc\k.c`|XfromB|jtXfromB|`(^\|[ \t]+\|[^\d\w_])XfromB\((?=([^,]+?),([^)]+?)\))`|`\1jtXfromB(jt,`|
|`jsrc\k.c`|XfromI|jtXfromI|`(^\|[ \t]+\|[^\d\w_])XfromI\((?=([^,]+?),([^)]+?)\))`|`\1jtXfromI(jt,`|
|`jsrc\k.c`|XfromQ|jtXfromQ|`(^\|[ \t]+\|[^\d\w_])XfromQ\((?=([^,]+?),([^)]+?)\))`|`\1jtXfromQ(jt,`|
|`jsrc\k.c`|ZfromD|jtZfromD|`(^\|[ \t]+\|[^\d\w_])ZfromD\((?=([^,]+?),([^)]+?)\))`|`\1jtZfromD(jt,`|
|`jsrc\conjunctions\cc.c` `jsrc\verbs\ve.c` `jsrc\verbs\vf.c` `jsrc\verbs\vp.c` `jsrc\verbs\vs.c`|abase2|jtabase2|`(^\|[ \t]+\|[^\d\w_])abase2\((?=([^,]+?),([^)]+?)\))`|`\1jtabase2(jt,`|
|`jsrc\verbs\vfrom.c`|afi|jtafi|`(^\|[ \t]+\|[^\d\w_])afi\((?=([^,]+?),([^)]+?)\))`|`\1jtafi(jt,`|
|`jsrc\verbs\vfrom.c`|afrom|jtafrom|`(^\|[ \t]+\|[^\d\w_])afrom\((?=([^,]+?),([^)]+?)\))`|`\1jtafrom(jt,`|
|`jsrc\xenos\xfmt.c`|afzrndID|jtafzrndID|`(^\|[ \t]+\|[^\d\w_])afzrndID\((?=([^,]+?),([^)]+?)\))`|`\1jtafzrndID(jt,`|
|`jsrc\adverbs\ai.c` `jsrc\conjunctions\cc.c` `jsrc\conjunctions\cp.c` `jsrc\conjunctions\cu.c` `jsrc\verbs\vb.c` `jsrc\verbs\vfrom.c` `jsrc\xenos\xfmt.c`|amp|jtamp|`(^\|[ \t]+\|[^\d\w_])amp\((?=([^,]+?),([^)]+?)\))`|`\1jtamp(jt,`|
|`jsrc\conjunctions\cu.c`|ampco|jtampco|`(^\|[ \t]+\|[^\d\w_])ampco\((?=([^,]+?),([^)]+?)\))`|`\1jtampco(jt,`|
|`jsrc\conjunctions\ch.c` `jsrc\conjunctions\cv.c`|ascan|jtascan|`(^\|[ \t]+\|[^\d\w_])ascan\((?=([^,]+?),([^)]+?)\))`|`\1jtascan(jt,`|
|`jsrc\conjunctions\ch.c` `jsrc\conjunctions\cv.c` `jsrc\verbs\v0.c` `jsrc\verbs\vb.c` `jsrc\verbs\ve.c` `jsrc\verbs\vrep.c`|aslash|jtaslash|`(^\|[ \t]+\|[^\d\w_])aslash\((?=([^,]+?),([^)]+?)\))`|`\1jtaslash(jt,`|