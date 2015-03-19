/*****************************************
  xjump version 2

  misc.c   雑処理

  (C) July 16, 1997  ROYALPANDA
*****************************************/

#include<stdio.h>


/* 乱数 */

int rnd( int range )
{
  return ( random() & 0xffff ) * range / 0xffff ;
}


/* 乱数初期化 */

void srnd( void )
{
  srandom( time(NULL) );
}
