|Path|Macro Name|Function|Regex|Replace|
|---|---|---|---|---|
|`jsrc\k.c`|BfromI|jtBfromI|
|`jsrc\k.c`|BfromX|jtBfromX|
|`jsrc\k.c`|C1fromC2|jtC1fromC2|`(^\|[ \t]+\|[^\d\w_])C1fromC2\((?=([^,]+?),([^)]+?)\))`|`\1jtC1fromC2(jt,`|
|`jsrc\k.c`|C2fromC1|jtC2fromC1|`(^\|[ \t]+\|[^\d\w_])C2fromC1\((?=([^,]+?),([^)]+?)\))`|`\1jtC2fromC1(jt,`|
|`jsrc\k.c`|C1fromC4|jtC1fromC4|`(^\|[ \t]+\|[^\d\w_])C1fromC4\((?=([^,]+?),([^)]+?)\))`|`\1jtC1fromC4(jt,`|