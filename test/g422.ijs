1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './g422.ijs'
NB. /:y -----------------------------------------------------------------

randuni''

NB. Boolean
a =: 1=?10 5$2
i =: /:a
k =: ?10$5
i =&# a
(/:~i)  -: i. #a
(/:k#a) -: ;i{(i.&.>k)+&.>+/\}:0,k
(,0)    -: /:0
''      -: /:(0,?(?5)$10)$0
(i.#a)  -: /:a =: ((?10),0)$0
(/:a)   -: /:,.~a=:?400$2
(/:a)   -: /:,.~a=:?  1$2
((+/a=/0 1)#0 1) -: /:~a=:?1000$2
(,1) -: /:~ 1
(,0) -: /:~ 0
(,1) -: /:~ ,1
(,0) -: /:~ ,0


f=: 3 : 0 " 0
 a=: (1000,y) ?@$ 2
 k=: (#a) ?@$ 5
 i=: ,/:a
 assert. i -: /:a{'01'
 assert. i -: /: #.a
 assert. (/:k#a) -: ; i { (i.&.>k)+&.>+/\}:0,k
 1
)

f >: i.16

NB. literal
a =: a.{~32+?10 5$95
i =: /:a
k =: ?10$5
i =&# a
(/:~i)  -: i.#a
(/:k#a) -: ;i{(i.&.>k)+&.>+/\_1}.0,k
(,0)    -: /:'g'
''      -: /:(0,?(?5)$10)$0
(i.#a)  -: /:a =: ((?10),0)$0
(/:a)   -: /:,.~ a=:a.{~?400$256
(/:a)   -: /:,.~ a=:a.{~?1  $256
((+/x=/  a.)#  a.) -: /:~x=:a.{~?1000$#a.
(,' ') -: /:~ ' '
(,' ') -: /:~ ,' '

f=: 3 : 0 " 0
 a=: a.{~(1000,y) ?@$ #a.
 k=: (#a) ?@$ 5
 assert. (/:k#a) -: ; (/:a) { (i.&.>k)+&.>+/\}:0,k
 1
)

f >:i.16

NB. literal2
a =: adot1{~32+?10 5$95
i =: /:a
k =: ?10$5
i =&# a
(/:~i)  -: i.#a
(/:k#a) -: ;i{(i.&.>k)+&.>+/\_1}.0,k
(,0)    -: /:'g'
''      -: /:(0,?(?5)$10)$0
(i.#a)  -: /:a =: ((?10),0)$0
(/:a)   -: /:,.~ a=:adot1{~?400$(#adot1)
(/:a)   -: /:,.~ a=:adot1{~?1  $(#adot1)
((+/x=/  adot1)#  adot1) -: /:~x=:adot1{~?1000$#adot1
(,u:' ') -: /:~ u:' '
(,u:' ') -: /:~ ,u:' '

f=: 3 : 0 " 0
 a=: adot1{~(1000,y) ?@$ #adot1
 k=: (#a) ?@$ 5
 assert. (/:k#a) -: ; (/:a) { (i.&.>k)+&.>+/\}:0,k
 1
)

f >:i.16

NB. literal4
a =: adot2{~32+?10 5$95
i =: /:a
k =: ?10$5
i =&# a
(/:~i)  -: i.#a
(/:k#a) -: ;i{(i.&.>k)+&.>+/\_1}.0,k
(,0)    -: /:'g'
''      -: /:(0,?(?5)$10)$0
(i.#a)  -: /:a =: ((?10),0)$0
(/:a)   -: /:,.~ a=:adot2{~?400$(#adot2)
(/:a)   -: /:,.~ a=:adot2{~?1  $(#adot2)
((+/x=/  adot2)#  adot2) -: /:~x=:adot2{~?1000$#adot2
(,10&u:' ') -: /:~ 10&u:' '
(,10&u:' ') -: /:~ ,10&u:' '

f=: 3 : 0 " 0
 a=: adot2{~(1000,y) ?@$ #adot2
 k=: (#a) ?@$ 5
 assert. (/:k#a) -: ; (/:a) { (i.&.>k)+&.>+/\}:0,k
 1
)

f >:i.16

NB. symbol
a =: sdot0{~32+?10 5$95
i =: /:a
k =: ?10$5
i =&# a
(/:~i)  -: i.#a
(/:k#a) -: ;i{(i.&.>k)+&.>+/\_1}.0,k
(,0)    -: /:s:@<"0 'g'
''      -: /:(0,?(?5)$10)$0
(i.#a)  -: /:a =: ((?10),0)$0
(/:a)   -: /:,.~ a=:adot1{~?400$(#adot1)
(/:a)   -: /:,.~ a=:adot1{~?1  $(#adot1)
((+/x=/  sdot0)#  sdot0) -: /:~x=:sdot0{~?1000$#sdot0
(,s:@<"0 ' ') -: /:~ s:@<"0 ' '
(,s:@<"0 ' ') -: /:~ ,s:@<"0 ' '

f=: 3 : 0 " 0
 a=: sdot0{~(1000,y) ?@$ #sdot0
 k=: (#a) ?@$ 5
 assert. (/:k#a) -: ; (/:a) { (i.&.>k)+&.>+/\}:0,k
 1
)

f >:i.16

NB. integer
a =: ?10 5$100
i =: /:a
k =: ?10$5
i=&#a
(/:~i)  -: i.#a
(/:k#a) -: ;i{(i.&.>k)+&.>+/\_1}.0,k
(,0)    -: /:5
''      -: /:(0,?(?5)$10)$5
(i.#a)  -: /:a =: ((?10),0)$5
*./a>:}:0, a=:/:~?1000$1e2
*./a>:}:0, a=:/:~?1000$1e3
*./a>:}:0, a=:/:~?1000$1e4
*./a>:}:0, a=:/:~?1000$1e9
(/: 100#.a) -: /: a=: ?1000 2$100
(/: 100#.a) -: /: a=: ?1000 2$  5

a=: (--:m) + 1000 ?@$ m=:IF64{2e9 9e18
y=: (/:a) { a
*./ (}:y) <: }.y

m=:<._1+2^31
1 0 -: /: m,0
0 1 -: /: 0,m
m=:<.-2^31
0 1 -: /: m,0
1 0 -: /: 0,m

((i.!#a) A. i.#a) (/:@[ -: /:@:{)"1 2 a=: 88#"0 i.1
((i.!#a) A. i.#a) (/:@[ -: /:@:{)"1 2 a=: 88#"0 i.2
((i.!#a) A. i.#a) (/:@[ -: /:@:{)"1 2 a=: 88#"0 i.3
((i.!#a) A. i.#a) (/:@[ -: /:@:{)"1 2 a=: 88#"0 i.4
((i.!#a) A. i.#a) (/:@[ -: /:@:{)"1 2 a=: 88#"0 i.5
((i.!#a) A. i.#a) (/:@[ -: /:@:{)"1 2 a=: 88#"0 i.6

((i.!#a) A. i.#a) (] -: /:~@:{)"1 2 a=: /:~ (88$8),"1?1 8$5e6
((i.!#a) A. i.#a) (] -: /:~@:{)"1 2 a=: /:~ (88$8),"1?2 8$5e6
((i.!#a) A. i.#a) (] -: /:~@:{)"1 2 a=: /:~ (88$8),"1?3 8$5e6
((i.!#a) A. i.#a) (] -: /:~@:{)"1 2 a=: /:~ (88$8),"1?4 8$5e6
((i.!#a) A. i.#a) (] -: /:~@:{)"1 2 a=: /:~ (88$8),"1?5 8$5e6
((i.!#a) A. i.#a) (] -: /:~@:{)"1 2 a=: /:~ (88$8),"1?6 8$5e6

(,2) -: /:~ 2
(,3) -: /:~ 3
(,4) -: /:~ ,4
(,5) -: /:~ ,5

f=: 3 : 0 " 0
 a=: (100,y) ?@$ 200
 k=: (#a) ?@$ 5
 assert. (/:k#a) -: ; (/:a) { (i.&.>k)+&.>+/\}:0,k
 1
)

f >: i.16

NB. literal
(/:v) -: /: a.{~        v=:         ?10000$#a.
(/:v) -: /: a.{~        v=:         ?66000$#a.

(/:v) -: /: a.{~ 2000 + v=: _2000 + ?10000$#a.
(/:v) -: /: a.{~ 2000 + v=: _2000 + ?66000$#a.

(/:v) -: /: a.{~  128 + v=: _128  + ?10000$#a.
(/:v) -: /: a.{~  128 + v=: _128  + ?66000$#a.

(/:"1 v) -: /:"1 a.{~        v=:         ?2 3 10000$#a.
(/:"1 v) -: /:"1 a.{~        v=:         ?2 3 66000$#a.

(/:"1 v) -: /:"1 a.{~ 2000 + v=: _2000 + ?2 3 10000$#a.
(/:"1 v) -: /:"1 a.{~ 2000 + v=: _2000 + ?2 3 66000$#a.

(/:"1 v) -: /:"1 a.{~  128 + v=:  _128 + ?2 3 10000$#a.
(/:"1 v) -: /:"1 a.{~  128 + v=:  _128 + ?2 3 66000$#a.

NB. literal2
(/:v) -: /: adot1{~        v=:         ?10000$#adot1
(/:v) -: /: adot1{~        v=:         ?66000$#adot1

(/:v) -: /: adot1{~ 2000 + v=: _2000 + ?10000$#adot1
(/:v) -: /: adot1{~ 2000 + v=: _2000 + ?66000$#adot1

(/:v) -: /: adot1{~  (-:@#adot1) + v=: (-@-:@#adot1)  + ?10000$#adot1
(/:v) -: /: adot1{~  (-:@#adot1) + v=: (-@-:@#adot1)  + ?66000$#adot1

(/:"1 v) -: /:"1 adot1{~        v=:         ?2 3 10000$#adot1
(/:"1 v) -: /:"1 adot1{~        v=:         ?2 3 66000$#adot1

(/:"1 v) -: /:"1 adot1{~ 2000 + v=: _2000 + ?2 3 10000$#adot1
(/:"1 v) -: /:"1 adot1{~ 2000 + v=: _2000 + ?2 3 66000$#adot1

(/:"1 v) -: /:"1 adot1{~  (-:@#adot1) + v=:  (-@-:@#adot1) + ?2 3 10000$#adot1
(/:"1 v) -: /:"1 adot1{~  (-:@#adot1) + v=:  (-@-:@#adot1) + ?2 3 66000$#adot1

NB. literal4
(/: UNSGN32 v) -: /: adot2{~        v=:         ?10000$#adot2
(/: UNSGN32 v) -: /: adot2{~        v=:         ?66000$#adot2

(/:"1 UNSGN32 v) -: /:"1 adot2{~        v=:         ?2 3 10000$#adot2
(/:"1 UNSGN32 v) -: /:"1 adot2{~        v=:         ?2 3 66000$#adot2

NB. symbol
(/:v) -: /: sdot0{~        v=:         ?10000$#sdot0
(/:v) -: /: sdot0{~        v=:         ?66000$#sdot0

(/:v) -: /: sdot0{~ 2000 + v=: _2000 + ?10000$#sdot0
(/:v) -: /: sdot0{~ 2000 + v=: _2000 + ?66000$#sdot0

(/:v) -: /: sdot0{~  (-:@#sdot0) + v=: (-@-:@#sdot0)  + ?10000$#sdot0
(/:v) -: /: sdot0{~  (-:@#sdot0) + v=: (-@-:@#sdot0)  + ?66000$#sdot0

(/:"1 v) -: /:"1 sdot0{~        v=:         ?2 3 10000$#sdot0
(/:"1 v) -: /:"1 sdot0{~        v=:         ?2 3 66000$#sdot0

(/:"1 v) -: /:"1 sdot0{~ 2000 + v=: _2000 + ?2 3 10000$#sdot0
(/:"1 v) -: /:"1 sdot0{~ 2000 + v=: _2000 + ?2 3 66000$#sdot0

(/:"1 v) -: /:"1 sdot0{~  (-:@#sdot0) + v=:  (-@-:@#sdot0) + ?2 3 10000$#sdot0
(/:"1 v) -: /:"1 sdot0{~  (-:@#sdot0) + v=:  (-@-:@#sdot0) + ?2 3 66000$#sdot0

(/:v) -: /: o. v=: (?~20000){(?10000$65536),32768*?10000$65536
(/:v) -: /: (-#v){.0 (3!:3) v

NB. floating point
a =: o._40+?10 5$100
i =: /:a
k =: ?10$5
i =&# a
(/:~i)  -: i.#a
(/:k#a) -: ;i{(i.&.>k)+&.>+/\_1}.0,k
(,0)    -: /:2.718
''      -: /:(0,?(?5)$10)$3.14
(i.#x)  -: /:x=: ((?10),0)$3.14
*./x>:}:0, x=:/:~o.?1000$1e2

v=:?100$25
(/:v) -: /:v-25
(/:v) -: /:v+100
(/:v) -: /:o.v
(/:v) -: /:,.~v

(/:v) -: /: 0.01*v=:        ?10000$50000
(/:v) -: /: 0.01*v=:        ?66000$50000

(/:v) -: /: 0.01*v=: - 1+   ?10000$50000
(/:v) -: /: 0.01*v=: - 1+   ?66000$50000

(/:v) -: /: 0.01*v=: _25000+?10000$50000
(/:v) -: /: 0.01*v=: _25000+?66000$50000

(/:"1 v) -: /:"1 o. v=:        ?2 3 10000$50000
(/:"1 v) -: /:"1 o. v=:        ?2 3 66000$50000

(/:"1 v) -: /:"1 o. v=: - 1+   ?2 3 10000$50000
(/:"1 v) -: /:"1 o. v=: - 1+   ?2 3 66000$50000

(/:"1 v) -: /:"1 o. v=: _25000+?2 3 10000$50000
(/:"1 v) -: /:"1 o. v=: _25000+?2 3 66000$50000
(,1.1) -: /:~ 1.1
(,1.2) -: /:~ 1.2
(,1.3) -: /:~ ,1.3
(,1.4) -: /:~ ,1.4

NB. complex
a =: r._40+?10 5$100
i =: /:a
k =: ?10$5
i =&# a
i       -: /: +.a
(/:~i)  -: i. #a
(/:k#a) -: ; i { (i.&.>k) +&.> +/\ }:0,k
(,0)    -: /: 3j4
''      -: /: (0,?(?5)$10)$3j4
(i.#x)  -: /: x=: ((?10),0)$3j4
(i.12)  -: /: r.12 5$100
(,1j1) -: /:~ 1j1
(,1j2) -: /:~ 1j2
(,1j3) -: /:~ ,1j3
(,1j4) -: /:~ ,1j4

NB. boxed
(/:a) -: /: <"0  a=: ?20 7$1000
(/:a) -: /: <"1  a
(/:a) -: /: <&.> a
(/:a) -: /: <&.> a
(/:a) -: /: > a=:     _50+&.>(?10$20)$&.>100
(/:a) -: /: > a=:o.&.>_50+&.>(?10$20)$&.>100
(/:a) -: /: > a=:r.&.>_50+&.>(?10$20)$&.>100
(/:a) -: /: > a=:(<"0 ?5$2),(<"0 ?5$100),(<"0 o.?5$100),<"0 r.?5$100
(/:a) -: /: > a=:(?~#a){a
(/:a) -: /: i.&.>a=:?~30
(,<1) -: /:~ <1
(,<'a') -: /:~ <'a'
(,<2) -: /:~ ,<2
(,<1j4) -: /:~ ,<1j4

NB. boxed symbol
(/:b) -: /: a=: s: (b=: 4 2 3 1 0){5 5$25{.65}.a.
(/:b) -: /: 7&s: a
(/:b) -: /: <"0 a
(/:;a) -: /:a=: <"0@s:'`onomatopoeia`paraclete`parousia`paronomasiac`parousia`paraclete`onomatopoeia`paraclete'
(/:;a) -: /: 7&s:&> a

f=: 3 : 0
 assert. (/:   a) -: /:   <"0 a=.?(100,y)$3
 assert. (/:"1 a) -: /:"1 <"0 a
 assert. (/:"2 a) -: /:"2 <"0 a
 assert. (/:   a) -: /:   <"1 a
 assert. (/:"2 a) -: /:"1 <"1 a
 assert. (/:"3 a) -: /:"2 <"1 a
 1
)

*./@f"0 >:       i.3 10
*./@f"1 >:  5 6#:i.3 10
*./@f"1 >:5 3 2#:i.3 10

g=: 3 : '0 1 -: /: ,:~ t=. y$t,<t=.1 2 3;''abc'' '
g"0 i.3 10

(/:"1 a) -: /:"1 <"0 a=:?  20 17$1000
(/:"2 a) -: /:"2 <"0 a=:?4 20 17$1000

le=: 0: = {.@/:
x=: o.&.>_50+&.>(?10$20)$&.>100
y=: ;:'Cogito, ergo sum. Sui generis. Sine qua non.'
z=: <"0 >5!:1 <'le'
2 le\ /:~a=: (?~#x,y,z) { x,y,z

0 1 2 -: /: x=:3j4;     'Ex ungue leonem'; <<5 6 7
2 1 0 -: /: |. x
0 1 2 -: /: x=:(i.0);   'Ex ungue leonem'; <<5 6 7
2 1 0 -: /: |. x
0 1 2 -: /: x=:'';      'Ex ungue leonem'; <<5 6 7
2 1 0 -: /: |. x
0 1 2 -: /: x=:(o.i.0); 'Ex ungue leonem'; <<5 6 7
2 1 0 -: /: |. x
0 1 2 -: /: x=:(j.i.0); 'Ex ungue leonem'; <<5 6 7
2 1 0 -: /: |. x
0 1 2 -: /: x=:(0$a:);  'Ex ungue leonem'; <<5 6 7
2 1 0 -: /: |. x
0 1 2 -: /: x=:'' ; (i.0) ; <0$a: 
0 1 2 -: /: |. x

a=: < 2 3 $ 1 2 3 4 5 6
b=: < 3 2 $ 1 2 5 6 3 4
c=: < 1 3 $ 1 2 3
d=: < 1 2 $ 1 2
e=: < 2 2 $ 1 2 5 6

(i.5) -: /: d,e,b,c,a
(/:"1 p) -: /:"1 (d,e,b,c,a){~p=: (i.!5) A. i.5

NB. rationals
(/: -: /:       @:x:) a=: -:_500+?100  $1000
(/: -: /:@:(<"0)@:x:) a
(/: -: /:       @:x:) a=: -:_500+?100 4$1000
(/: -: /:@:(<"1)@:x:) a
(,1r2) -: /:~ 1r2
(,1r3) -: /:~ 1r3
(,1r4) -: /:~ ,1r4
(,1r5) -: /:~ ,1r5

'index error'  -: 3 4 5 /: etx 1 2 3 4


NB. /:y on integer lists ------------------------------------------------

v=:?100$25
(/:v) -: /:v-25
(/:v) -: /:v+100
(/:v) -: /:o.v
(/:v) -: /:,.~v

1 2 3 4 5 0 -: /: v=: 2e9 _2e9 1 2 3 4
_2e9 1 2 3 4  2e9 -: /:~v

x=: <._1+2^31
y=: _1-x
v=:x,y,1 2 3 4
(/:v) -: 1 2 3 4 5 0
(/:~v) -: y,1 2 3 4,x

NB. Verify radix & smallrange sort on vectors, <64K and greater

(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  40000 ?@$ 40000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  40000 ?@$ 40000
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  1000 * 40000 ?@$ 40000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  1000 * 40000 ?@$ 40000
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  80000 ?@$ 80000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  80000 ?@$ 80000
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  1000 * 80000 ?@$ 80000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  1000 * 80000 ?@$ 80000

NB. Include negative numbers

(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  _2000 + 40000 ?@$ 40000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  _2000 + 40000 ?@$ 40000
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  _2000 + 1000 * 40000 ?@$ 40000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  _2000 + 1000 * 40000 ?@$ 40000
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  _2000 + 80000 ?@$ 80000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  _2000 + 80000 ?@$ 80000
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  _2000 + 1000 * 80000 ?@$ 80000
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  _2000 + 1000 * 80000 ?@$ 80000




NB. Floats must include -0

(i.@# -: /:)  n0vec =: (-~ 0.5) * 1 _1 {~ 40000 ?@$ 2
(i.@# -: \:)  n0vec
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  n0vec , _0.5 + 2000 ?@$ 0
(((*./@:(2 <:/\ ])@:({~)) *. *./@:~:@])   /:)  n0vec ,~ _0.5 + 2000 ?@$ 0
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  n0vec , _0.5 + 2000 ?@$ 0
(((*./@:(2 >:/\ ])@:({~)) *. *./@:~:@])   \:)  n0vec ,~ _0.5 + 2000 ?@$ 0

NB. /:"r ----------------------------------------------------------------

g =: 3 : ('/:y'; ':'; 'x/:y')

(/:~ -: /:~&.:>:) y=:?254$2
(/:~ -: /:~&.:>:)"1 y
(/:~"1 -: /:~&.:>:"1) y
(/:~ -: /:~&.:>:) y=:?257$2
(/:~ -: /:~&.:>:)"1 y
(/:~"1 -: /:~&.:>:"1) y
(/:~ -: /:~&.:>:) y=:?512$2
(/:~ -: /:~&.:>:)"1 y
(/:~"1 -: /:~&.:>:"1) y
(/:~ -: /:~&.:>:) y=:?2 3 4 17$2
(/:~ -: /:~&.:>:)"1 y
(/:~"1 -: /:~&.:>:"1) y

(/:"0 -: g"0) y=:?2 3 4 17$2
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

(/:"0 -: g"0) y=:?2 3 4 24$2
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

(/:"0 -: g"0) y=:a.{~?2 3 4 17$#a.
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y
(/:"0 -: g"0) y=:a.{~?2 3 4 18$#a.
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

(/:"0 -: g"0) y=:adot1{~?2 3 4 17$#adot1
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y
(/:"0 -: g"0) y=:adot1{~?2 3 4 18$#adot1
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

(/:"0 -: g"0) y=:adot2{~?2 3 4 17$#adot2
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y
(/:"0 -: g"0) y=:adot2{~?2 3 4 18$#adot2
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

(/:"0 -: g"0) y=:sdot0{~?2 3 4 17$#sdot0
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y
(/:"0 -: g"0) y=:sdot0{~?2 3 4 18$#sdot0
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

(/:"0 -: g"0) y=:?2 3 4 17$34
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y
(/:"0 -: g"0) y=:?2 3 4 17$+:*/2 3 4 17
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y
(/:"0 -: g"0) y=:_50+?2 3 4 17$100
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

(/:"0 -: g"0) y=:o.?2 3 4 17$34
(/:"1 -: g"1) y
(/:"2 -: g"2) y
(/:"3 -: g"3) y
(/:"4 -: g"4) y

x=:?20 3$100
x (/:"2 2 -: g"2 2) y=:?2 20 3$100
x (/:"2 3 -: g"2 3) y=:?20 2 3$100
x (/:"1 1 -: g"1 1) y=:o.?3$100
x (/:"1 0 -: g"1 0) y=:?1000
x (/:"1 0 -: g"1 0) y=:a.{~?20$256

(/:~"1 -: g~"1) y=:?4 17$2
(/:~"1 -: g~"1) y=:a.{~32+?17 4$95
(/:~"1 -: g~"1) y=:?2 7 16$100
(/:~"1 -: g~"1) y=:o.?4 17$1232

(2 3 4 1$0) -: /:"0 i.2 3 4
(0 0 0 1$0) -: /:"0 i.0 0 0

'index error' -: (i.4) /: etx i.5

4!:55 ;:'a adot1 adot2 sdot0 b c d e f g ge i k le m n0vec p v x y z'
randfini''

