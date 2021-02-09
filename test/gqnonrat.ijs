1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './gqnonrat.ijs'
NB. rational approximations of non-rational functions -------------------

NB. e f  x -- compute f x within error e
NB. n f0 x -- n terms of series for f x
NB. e nf x -- number of terms required for error e and f x

exp0 =: ] ([: +/ ^ % !@])       i.@[
ln0  =: 4 : '+: +/ i %~ (%/y+_1 1)^i=. 1+2*i.x'  NB. AS 4.1.27
sin0 =: ] ([: -/ ^ % !@]) >:@+:@i.@[  
cos0 =: ] ([: -/ ^ % !@])    +:@i.@[  

nexp=: 4 : '2x + (^.x) (> i. 1:) (i*^.y)-+/\^.i=.1+i.200'
nsin=: 4 : '1x + >. !@>:@+:^:_1 %x'
ncos=: 4 : '1x + >. !@   +:^:_1 %x'

nln =: 4 : 0
 r=. |%/(x:^:_1 y)+_1 1
 k=. 1+2*i.1+>.-:r^.-:x
 1x + (-:x) (> i. 1:) k%~r^k
)

sin=: nsin sin0 ]
cos=: ncos cos0 ]
exp=: nexp exp0 ]

NB. (ln x*y) = (ln x)+(ln y)
NB. (ln x^e) = e * ln x

ln=: 4 : 0
 assert. 0<y
 e=. <.2^.x:^:_1 y
 r=. (x: y) % 2x^e
 a=.b=.0
 if. 1~:r do. a=.   (x nln r) ln0 r  end.
 if. 0~:e do. b=. e*(x nln 2) ln0 2x end.
 a+b   
)

sqrt=: 4 : 0
 -:@(+(x:y)&%)^:x x:%:y
)

'0.6931471805599453094172321214581765680755' -: 0j40 ": 1e_40 ln 2
'1.0986122886681096913952452369225257046475' -: 0j40 ": 1e_40 ln 3
'2.3025850929940456840179914546843642076011' -: 0j40 ": 1e_40 ln 10


NB. notes ---------------------------------------------------------------

sin0 =: ] ([: -/ ^ % !@]) >:@+:@i.@[  
nsin=: 4 : '1x + >. !@>:@+:^:_1 %x'
sin=: nsin sin0 ]

1: 0 : 0
 Identities to reduce sin(x) for any x to
 sin(y) or cos(y) where (0<:y)*.(y<:0.25p1)
 sin(x+2*n*1p1) = sin(x)
 sin(x)         = sin(1p1-x)
 sin(-x)        = -sin(x)
 sin(0.5p1+x)   = cos(x)
 sin(0.5p1-x)   = cos(x)
 
)


4!:55 ;:'cos cos0 exp exp0 ln ln0 ncos nexp nln nsin sin sin0 sqrt'


