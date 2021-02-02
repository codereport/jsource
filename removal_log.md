|Path|Macro Name|Function|Regex|Replace|
|---|---|---|---|---|
|`jsrc\k.c`|BfromI|jtBfromI|
|`jsrc\k.c`|BfromX|jtBfromX|
|`jsrc\k.c`|C1fromC2|jtC1fromC2|`(^\|[ \t]+\|[^\d\w_])C1fromC2\((?=([^,]+?),([^)]+?)\))`|`\1jtC1fromC2(jt,`|