**2021-01-15**<br>
Pi Calculation:
```ijs
slidng =: <;._3
chunk  =: 3 : '(2*i.((#y) % 2)) { 2 sliding y'
pi     =: 3 : '+/ > 8% each */ each chunk 1 + 2 * i.y'
```
**2021-01-17**<br>
Pi Calculation (revised from above because we learned things):
```ijs
pi =: 3 : '+/ > 8 % each */ each _2 <\ 1 + 2 * i.y'
```
