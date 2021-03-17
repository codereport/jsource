1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './gq101.ijs'
NB. rational numbers +. -------------------------------------------------

gcd=: 4 : 0 " 0
 x=. |x
 y=. |y
 while. r=. y|x do.
  x=. y
  y=. r
 end.
)

x=: %/ 0 1 + ?2 100$2e9
y=: %/ 0 1 + ?2 100$2e9

0 (+. -: gcd) y
x (+. -: gcd) 0
x (+. -: gcd) x

4!:55 ;:'gcd i j x y'
