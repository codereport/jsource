1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './gxco.ijs'
NB. x: ------------------------------------------------------------------

4 -: x: {: 3.4 4
4 -: x: {: 3j4 4

7r2     -: x: 3.5
3r5     -: x: 3r5
7r100   -: x: 0.07
271r100 -: x: 2.71

128 -: type x: -~2.5
128 -: type x: o. 0
128 -: type x: 1 2 3 4 5 6.5

128 -: type x: _
128 -: type x: __

(2^_53) = x: 2^_53

'domain error' -: x: etx 3j5
'domain error' -: x: etx '345'
'domain error' -: x: etx <1 2 3

NB. 'domain error' -: x: etx _.

'domain error' -: 'a' x: etx 1.5
'domain error' -: 3.4 x: etx 1.5
'domain error' -: 3j4 x: etx 1.5
'domain error' -: 3   x: etx 1.5
'domain error' -: _3  x: etx 1.5
'domain error' -: 0   x: etx 1.5

(= x:) ,1 _1 */ 1e43 1e_43
(= x:) ,1 _1 */ 2^137 _137

(": -: ":@(+&(-~1r2)))@x:"0 ] ,1 _1 */ 1e43 1e_43
(": -: ":@(+&(-~1r2)))@x:"0 ] ,1 _1 */ 2^137 _137

_ = 10^309
0 = 1.2 % 10^309

'ill-formed number' -: ex '1x ___'
