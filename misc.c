/*****************************************
  xjump version 2

  misc.c   ������

  (C) July 16, 1997  ROYALPANDA
*****************************************/

#include<stdio.h>


/* ��� */

int rnd( int range )
{
  return ( random() & 0xffff ) * range / 0xffff ;
}


/* �������� */

void srnd( void )
{
  srandom( time(NULL) );
}
