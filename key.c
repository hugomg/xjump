/*****************************************
  xjump version 2

  key.c   キー処理

  (C) Oct 28, 1997  ROYALPANDA
*****************************************/

#include<stdio.h>
#include<X11/Xlib.h>
#include<X11/keysym.h>

#include"xjump.h"

static int Keycode_raw[3];
static int Keycode_col[3];

void query_keycode( void )
{
  int i;
  KeyCode k;
  static KeySym sym[3] = {XK_space,XK_Left,XK_Right};

  for( i = 0 ; i < 3 ; i++ ){
    k = XKeysymToKeycode( Disp,sym[i] );
    Keycode_raw[i] = k >> 3;
    Keycode_col[i] = 1 << (k & 7);
  }
}


void keymap( void )
{
  char key[32];
  int i;

  XQueryKeymap( Disp,key );

  for( i = 0 ; i < 3 ; i++ )
    if( key[ Keycode_raw[i] ] & Keycode_col[i] )
      Key[i] = 1;
    else
      Key[i] = 0;
}


void wait_keyup( void )
{
  do{
    keymap();
  }while( Key[0] || Key[1] || Key[2] );
}

