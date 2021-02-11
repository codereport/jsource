
uthelper1 =: 0!:2 @ <
uthelper2 =: './test/',(],&'.ijs')
ut        =: uthelper1 @ uthelper2

slide =: <\
chunk =: 4 : '(-x) <\ y' NB. TODO figure out if there is a nicer way to write
