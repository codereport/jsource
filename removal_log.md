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