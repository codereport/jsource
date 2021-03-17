1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './gpdd.ijs'
NB. p.. y ---------------------------------------------------------------

2 6 12  -: p.. 1 2 3 4

(,0)    -: p.. 3j4
(,0)    -: p.. i.0
(,0)    -: p.. ''

(,!6  ) -: p..^:6  ]7 $1
(,!88) -: p..^:88 ]89$1

'domain error' -: p.. etx 'abc'
'domain error' -: p.. etx u:'abc'
'domain error' -: p.. etx 10&u:'abc'


NB. x p.. y -------------------------------------------------------------

(1 2 1.5,4%3) -: 1 p.. 2 3 4
(1 2 3r2 4r3) -: 1 p.. 2 3 4

x -: ({.x) p.. p.. x=: ?10$100

(%!i.10) -: p../10$1


'domain error' -: 3    p.. etx 'abc'
'domain error' -: 3    p.. etx u:'abc'
'domain error' -: 3    p.. etx 10&u:'abc'
'domain error' -: 'x'  p.. etx 3 4 
'domain error' -: (<4) p.. etx 3 4 


4!:55 ;:'x'


