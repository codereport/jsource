1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './gq201.ijs'
NB. ^. on integers and rational numbers ------------------------

NB. test failed on small ct (comparison tolerance)
ct   =: 9!:18''
9!:19[2^_40

test=: 4 : 0
 assert. y -: x ^. x ^ y
 1
)

2  test y=: 2+10 10?@$300
3  test y
10 test y

12345    test 17
12345678 test 23

31 -: 2    ^. x=:*/31$2
31 -: 2 <.@^. x
31 -: 2 >.@^. x
30 -: 2 <.@^.<:x
31 -: 2 >.@^.<:x
31 -: 2 <.@^.>:x
32 -: 2 >.@^.>:x

0 -: 314159 <.@^. 1
0 -: 314159 >.@^. 1

y1=: (1-1e4)+10 11 ?@$ 2e4-1
y2=: (1-1e8)+10 11 ?@$ 2e8-1
y3=: (1-1e9)+10 11 ?@$ 2e9-1

1 = 16x <.@^. {. 123 0.5

0  = 0x ^. _5 5
__ = 1 5 6 7 ^. 0x 
__ 0 _ _ = 1 ^. 0 1 2 3

(^. 2.5) -: ^. 5r2

(3.7 ^. 2.5) -: 3.7   ^. 5r2
(3.7 ^. 2.5) -: 37r10 ^. 2.5
(3.7 ^. 2.5) -: 37r10 ^. 5r2

phi=: -:>:%:5
((^.phi)%~300*^.100%3 ) -: phi^.100r3^300

9!:19 ct

4!:55 ;:'ct phi t test x xx y y1 y2 y3 yy'


