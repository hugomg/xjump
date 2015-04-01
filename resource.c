/*****************************************
  xjump version 2

  resource.c   Fallback resources

  (C) Nov 8, 1997  ROYALPANDA
*****************************************/

#include<stdio.h>


char *DefaultResources[] = {

"XJump*font                     : -adobe-*-*-*-*-*-24-*-*-*-*-*-*-*",
"XJump*foreground               : white",
"XJump*background               : black",
"XJump*borderColor              : white",
"XJump*defaultDistance          : 16",
"XJump*borderWidth              : 0",

"XJump*title.label              : FALLING TOWER ver 2.7",
"XJump*title.horizDistance      : 64",
"XJump*title.foreground         : white",
"XJump*title.background         : blue",
"XJump*title.borderColor        : navy",
"XJump*title.borderWidth        : 4",

"XJump*scoreBoard.fromVert      : title",
"XJump*scoreBoard.horizDistance : 128",

"XJump*scLabel.label            : Floor",

"XJump*score.fromHoriz          : scLabel",

"XJump*gameover.label           : Game Over",
"XJump*gameover.fromVert        : scoreBoard",
"XJump*gameover.horizDistance   : 200",
"XJump*gameover.vertDistance    : 96",
"XJump*gameover.foreground      : white",
"XJump*gameover.background      : blue",
"XJump*gameover.borderColor     : navy",
"XJump*gameover.borderWidth     : 4",

"XJump*pause.label              : Pause",
"XJump*pause.fromVert           : scoreBoard",
"XJump*pause.horizDistance      : 224",
"XJump*pause.vertDistance       : 96",
"XJump*pause.foreground         : white",
"XJump*pause.background         : blue",
"XJump*pause.borderColor        : navy",
"XJump*pause.borderWidth        : 4",

"XJump*scr.fromVert             : scoreBoard",
"XJump*scr.borderWidth          : 0",

"XJump*record_v.fromVert        : scoreBoard",
"XJump*record_v.allowVert       : True",
"XJump*record_v.allowHoriz      : True",
"XJump*record_v*borderWidth     : 1",

"XJump*record.font		: -*-fixed-*-*-*-*-20-*-*-*-c-*-*-*",

"XJump*copyright.label          : (C) 1997 ROYALPANDA",
"XJump*copyright.foreground     : green",
"XJump*copyright.fromVert       : scr",
"XJump*copyright.horizDistance  : 64",

"XJump*game.translations        : #override \\n\
\
<Key>Left    : key_on(LEFT)          \\n\
<KeyUp>Left  : key_off(LEFT)         \\n\
<Key>j       : key_on(LEFT)          \\n\
<KeyUp>j     : key_off(LEFT)         \\n\
<Key>KP_4    : key_on(LEFT)          \\n\
<KeyUp>KP_4  : key_off(LEFT)         \\n\
<Btn2Down>   : key_on(LEFT)          \\n\
<Btn2Up>     : key_off(LEFT)         \\n\
\
<Key>Right   : key_on(RIGHT)         \\n\
<KeyUp>Right : key_off(RIGHT)        \\n\
<Key>l       : key_on(RIGHT)         \\n\
<KeyUp>l     : key_off(RIGHT)        \\n\
<Key>KP_6    : key_on(RIGHT)         \\n\
<KeyUp>KP_6  : key_off(RIGHT)        \\n\
<Btn3Down>   : key_on(RIGHT)         \\n\
<Btn3Up>     : key_off(RIGHT)        \\n\
\
<Key>Up      : key_on(UP)start()     \\n\
<KeyUp>Up    : key_off(UP)           \\n\
<Key>space   : key_on(UP)start()     \\n\
<KeyUp>space : key_off(UP)           \\n\
<Key>z       : key_on(UP)            \\n\
<KeyUp>z     : key_off(UP)start()    \\n\
<Key>KP_8    : key_on(UP)            \\n\
<KeyUp>KP_8  : key_off(UP)start()    \\n\
<Btn1Down>   : key_on(UP)            \\n\
<Btn1Up>     : key_off(UP)start()    \\n\
\
<Key>p       : pause()               \\n\
Shift<Key>q  : quit()                \\n\
",
NULL};
