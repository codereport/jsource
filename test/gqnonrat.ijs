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

sqrt=: 4 : 0
 -:@(+(x:y)&%)^:x x:%:y
)

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


