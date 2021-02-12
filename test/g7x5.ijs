1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './g7x5.ijs'
NB. 7!:5 ----------------------------------------------------------------

randuni''

bp=: (IF64{1 1 4 8 16 4 4 2 4,:1 1 8 8 16 8 8 2 4) {~ 1 2 4 8 16 32 65536 131072 262144 i. 3!:0
sp=: 7!:5
f =: 3 : '7!:5 <''y'''
fmapped =: 3 : '7!:5 <''y'''

g =: 3 : 0
 w=. IF64{4 8
 z=. w*0                              NB. 0 words for memory management
 z=. z + w*7                          NB. 7 words for non-shape header words
 z=. z + w*r+(-.IF64)*0=2|r=. #$y     NB. shape, pad to doubleword boundary if 32 bits
 z=. z + ((bp y)**/$y) + (w-1)*(3!:0 y)e. 1 2 131072 262144  NB. atoms & trailing 0 word (uses whole word of padding-1)
 >.&.(2&^.) z
)
gmapped =: 3 : 0  NB. contiguous header only
 w=. IF64{4 8
 z=. w*0                              NB. 0 words for memory management
 z=. z + w*7+64+(-.IF64)                     NB. fixed shape area
 z=. z + >.&.(%&w) ((bp y)**/$y) + (w)*(3!:0 y)e. 1 2 131072 262144  NB. atoms & trailing 0 word (uses whole word of padding-1)
)


(f -: g) 23
(f -: g) 2.3
(f -: g) 2j3
(f -: g) 2j3
(f -: g) 'a'
(f -: g) u:'a'
(f -: g) 10&u:'a'
(f -: g) {.s: ' a'

(f -: g)@($&    0 1 )"0 ]200+i.4 10
(f -: g)@($&    'x' )"0 ]200+i.4 10
(f -: g)@($&(u: 'x'))"0 ]100+i.4 10
(f -: g)@($&(10&u: 'x'))"0 ]100+i.4 10
(f -: g)@($&    0 1 )"0 ]160+i.4 10
(f -: g)@($&    'x' )"0 ]160+i.4 10
(f -: g)@($&(u: 'x'))"0 ] 80+i.4 10
(f -: g)@($&(10&u: 'x'))"0 ] 80+i.4 10

(f -: g)@($&    0 1 )"1 ]1,"0 ] 200+i.4 10
(f -: g)@($&    'x' )"1 ]1,"0 ] 200+i.4 10
(f -: g)@($&(u: 'x'))"1 ]1,"0 ] 100+i.4 10
(f -: g)@($&(10&u: 'x'))"1 ]1,"0 ] 100+i.4 10
(f -: g)@($&    0 1 )"1 ]1,"0 ] 160+i.4 10
(f -: g)@($&    'x' )"1 ]1,"0 ] 160+i.4 10
(f -: g)@($&(u: 'x'))"1 ]1,"0 ]  80+i.4 10
(f -: g)@($&(10&u: 'x'))"1 ]1,"0 ]  80+i.4 10

(f -: g) x=: (?1e4)$1 0
(f -: g) x=: (?1e4)$2 3
(f -: g) x=: (?1e4)$2.3
(f -: g) x=: (?1e4)$2j3
(f -: g) x=: (?1e4)$2j3
(f -: g) x=: (?1e4)$'ab'
(f -: g) x=: (?1e4)$u:'ab'
(f -: g) x=: (?1e4)$10&u:'ab'
(f -: g) x=: (?1e4)$s: ' a b'

(f -: g) x=: (1+?100 100)$1 0
(f -: g) x=: (1+?100 100)$2 3
(f -: g) x=: (1+?100 100)$2.3
(f -: g) x=: (1+?100 100)$2j3
(f -: g) x=: (1+?100 100)$2j3
(f -: g) x=: (1+?100 100)$'ab'
(f -: g) x=: (1+?100 100)$'ab'
(f -: g) x=: (1+?100 100)$10&u:'ab'
(f -: g) x=: (1+?100 100)$s: ' a b'

(f -: g) x=: (1+?100 10 50)$1 0
(f -: g) x=: (1+?100 10 50)$2 3
(f -: g) x=: (1+?100 10 50)$2.3
(f -: g) x=: (1+?100 10 50)$2j3
(f -: g) x=: (1+?100 10 50)$2j3
(f -: g) x=: (1+?100 10 50)$'ab'
(f -: g) x=: (1+?100 10 50)$u:'ab'
(f -: g) x=: (1+?100 10 50)$10&u:'ab'
(f -: g) x=: (1+?100 10 50)$s: ' a b'

(sp ;:'f g sp') -: (sp <'f'),(sp <'g'),sp <'sp'

(sp <'sp') -: sp <'sp_base_'
(sp <'f' ) -: sp <'f__x' [ x=: <'base'

0 <: f 5!:1 <'bp'

x=: 3 : 0
 if. y do. c=.1 else. c=.2 end.
 c
)

0 <: sp <'x'

x=: 4 : 0
 if. x<y do. c=.10 else. c=.20 end.
 s=. 0
 for_xyz. i.c do.
  s=. xyz+s
 end.
)

0 <: sp <'x'

x=: 2 : 0
  :
 select. x 
  case.  1 do. u y
  case.  2 do. u 2*y
  fcase. 3 do. 3*y
  case.  4 do. u 4*y
 end.
)

0 <: sp <'x'
0 < 7!:5 <'x' [ x=: 9e4 $ <2e4$0
0 < 7!:5 <'x' [ x=: 9e4 $ <4e4$0

'value error'     -: 7!:5 etx <'nonexistent'

'domain error'    -: 7!:5 etx <i.4
'domain error'    -: 7!:5 etx <1 2.3 4
'domain error'    -: 7!:5 etx <1 2j3 4
'domain error'    -: 7!:5 etx <10&u:'abc'
'domain error'    -: 7!:5 etx <s: ' a b c'
'domain error'    -: 7!:5 etx <<'abc'
'domain error'    -: 7!:5 etx i.4
'domain error'    -: 7!:5 etx 1 2.3 4
'domain error'    -: 7!:5 etx 1 2j3 4
'domain error'    -: 7!:5 etx u:'abc'
'domain error'    -: 7!:5 etx 10&u:'abc'
'domain error'    -: 7!:5 etx s: ' a b c'

'rank error'      -: 7!:5 etx <,:'abc'

'ill-formed name' -: 7!:5 etx <'bad name'
'ill-formed name' -: 7!:5 etx <''


NB. 7!:5 on mapped arrays -----------------------------------------------
NB. jmf is broken without extended
NB. load'jmf'
NB. 3 : 0 ''
NB. if. _1=nc<'MAPNAME_jmf_' do.
NB.  'MAPNAME_jmf_ MAPFN_jmf_ MAPSN_jmf_ MAPFH_jmf_ MAPMH_jmf_ MAPADDRESS_jmf_ MAPHEADER_jmf_ MAPFSIZE_jmf_ MAPMSIZE_jmf_ MAPREFS_jmf_'=: i.10
NB. end. 
NB. 1
NB. ) 
NB. 18!:4 <'base'
NB. 1 [ unmap_jmf_ 'q'
NB. f=: <jpath '~temp/q.jmf'
NB. 1 [ createjmf_jmf_ f,<3e5      NB. 3e5 bytes for data
NB. map_jmf_ (<'q'),f,'';0   NB. map q to jmf file
NB. '' -: q
NB. 
NB. (fmapped -: gmapped) q [ q=:x=:     (?1e4)?@$2
NB. (fmapped -: gmapped) q [ q=:x=: a.{~(?1e4)?@$#a.
NB. (fmapped -: gmapped) q [ q=:x=: adot1{~(?1e4)?@$#adot1
NB. (fmapped -: gmapped) q [ q=:x=: adot2{~(?1e4)?@$#adot2
NB. (fmapped -: gmapped) q [ q=:x=: (?100 100)?@$1e6
NB. (fmapped -: gmapped) q [ q=:x=: o.(?30 30 30)?@$1e6
NB. (fmapped -: gmapped) q [ q=:x=: j./(2,?100 100)?@$1e6
NB. (fmapped -: gmapped) q [ q=:x=: s: <"0 a.{~(?1e4)?@$#a.
NB. (fmapped -: gmapped) q [ q=:x=: s: <"0 adot1{~(?1e4)?@$#adot1
NB. (fmapped -: gmapped) q [ q=:x=: s: <"0 adot2{~(?1e4)?@$#adot2
NB. 
NB. 'domain error' -: 3!:9 etx q
NB. 'domain error' -: (1) 3!:9 etx q
NB. 'rank error' -: (,.0) 3!:9 etx q
NB. 1 -: (0) 3!:9 q
NB. (,1) -: (,0) 3!:9 q
NB. 0 -: (0) 3!:9 a:
NB. (,0) -: (,0) 3!:9 a:
NB. 
NB. 
NB. 1 [ unmap_jmf_ 'q'
NB. 
NB. NB. Test usecount on mapped arrays
NB. NB. create clean mapped noun a
NB. f=: jpath'~temp/t.jmf'
NB. 1 [ createjmf_jmf_ f;1000
NB. 1 [ 1 unmap_jmf_'a' NB. 1 forces unmap - even with dangling refs
NB. 1 [ map_jmf_ 'a';f
NB. a=: i.5
NB. 
NB. NB. create foo and goo to create the problem
NB. foo=: 4 : 0
NB. if. x do. goo'' return. end.
NB. )
NB. 
NB. goo=: 3 : 0
NB. 0 foo <a NB. ".&.><'a'
NB. )
NB. 
NB. NB. run foo calling goo calling foo (note perhaps nasty goo calling foo!)
NB. 1 [ 1 foo '' NB. a NB. ".&.> <'a' [ !a
NB. (<,2) -: (<1,MAPREFS_jmf_) { showmap_jmf_''
NB. 1 [ unmap_jmf_ 'a'
NB. 
NB. 18!:55 <'jmf'
NB.

4!:55 ;:'adot1 adot2 fmapped gmapped sdot0 bp f g q sp x a foo goo'
randfini''

