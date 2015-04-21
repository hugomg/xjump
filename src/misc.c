/*****************************************
  xjump version 2

  misc.c   Miscelaneous stuff

  (C) July 16, 1997  ROYALPANDA
*****************************************/

#define  _GNU_SOURCE

#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int rnd( int range )
{
  return ( random() & 0xffff ) * range / 0xffff ;
}

void srnd( void )
{
  srandom( time(NULL) );
}
