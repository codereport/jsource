
ut =: verb define
  load './test/tsu.ijs'
  testname =. y
  erase <'y'
  RUN1 testname  NB. RUN1 defined in tsu.ijs
)

slide =: <\
chunk =: 4 : '(-x) <\ y' NB. TODO figure out if there is a nicer way to write
