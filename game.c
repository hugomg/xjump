/*****************************************
  xjump version 2

  game.c   Movement and drawing routines

  (C) July 28, 1997  ROYALPANDA
*****************************************/

#include<stdio.h>
#include<X11/Xlib.h>
#include <time.h>
#include <stdlib.h>

#include"xjump.h"


/* Macros */

#define max( a,b ) ( (a) > (b) ? (a) : (b) )
#define min( a,b ) ( (a) < (b) ? (a) : (b) )

/* Variables */

static int T_speed; /* Current scrolling speed */
static int T_count; /* Total scroll displacement */
 

/* Generate a new floor and draw it on the screen */

static void make_floor( int y )
{
  int f,x1,x2,fvec;
  static int fpos = WIDTH/2;

  f = Floor_top - y;

  if( f % 250 == 0 ){

    floor_l(y) = 16;
    floor_r(y) = WIDTH*16-17;

    XCopyArea( Disp,Floor_p,Scr_d,Gc_nomask,0,0,WIDTH*16,16,0,y*16 );

    return;
  }

  if( f % 5 == 0 ){

    fvec = (rnd(2)*2-1) * ( rnd( (WIDTH-11)/4 ) + (WIDTH-11)/4 );

    fpos = ( fpos + fvec +WIDTH-11 ) % (WIDTH-11);
    
    x1 = ( fpos+5 - rnd( 3 ) - 2)*16;
    x2 = ( fpos+5 + rnd( 3 ) + 2)*16+15;

    floor_l(y) = x1;
    floor_r(y) = x2;

    XCopyArea( Disp,Floor_p,Scr_d,Gc_nomask,x1,0,x2-x1+1,16,x1,y*16 );

  }else{

    floor_l(y) = WIDTH*16;
    floor_r(y) = 0;
  }
}


/* Draw the player sprite */

static void draw_hero( void )
{
  int pic;
  int sx,sy;

  pic = Hero.pic * 2 + Hero.dir;
  sx = ( pic & 3 ) * 32;
  sy = ( pic >> 2 ) * 32;

  XSetClipOrigin( Disp,Gc_mask,Hero.x-sx,Hero.y-sy );
  int hh = min(32, HEIGHT*16 - Hero.y);
  XCopyArea( Disp,Char_p,Scr_d,Gc_mask,sx,sy,32,hh,Hero.x,Hero.y );

}


/* Undraw the player sprite */

static void del_hero( void )
{
  recover_scr( Hero.x, Hero.y, 32, 32 );
}


/* Scroll up by one line */

static void scroll_up( void )
{
  int x,width,y;

  for( y = Floor_top % 5 ; y < HEIGHT ; y += 5 ){
    x = floor_l(y);
    width = floor_r(y)-x+1;

    XCopyArea( Disp,Back_p,Scr_d,Gc_nomask,x,0,width,16,x,y*16 );

    if( y < HEIGHT-1 )
      XCopyArea( Disp,Floor_p,Scr_d,Gc_nomask,x,0,width,16,x,y*16+16 );
  }

  Floor_top++;

  if( --Map_index < 0 )
    Map_index = HEIGHT - 1;

  make_floor( 0 );

  Hero.y += 16;
}


/* Determine if the hero is currently standing on a platform */

static int stand( void )
{
  int y;

  if( Hero.vy < 0 )
    return 0;

  y = ( ( Hero.y + 32 ) / 16 );
  if( y >= HEIGHT )
    return 0;

  if( floor_l(y) < Hero.x+24 && Hero.x+8 < floor_r(y) )
    return 1;
  else
    return 0;
}


/* Move the player sprite */

int move( void )
{
  int acc,st,floor;

  del_hero();

  if( T_speed < 5000 )
    T_speed++;

  T_count += T_speed;
  if( T_count > 20000 ){
    T_count -= 20000;
    scroll_up();
  }

  Hero.x += Hero.vx / 2;
  Hero.y += Hero.vy;

  if( Hero.x < 16 ){
    Hero.x = 16;
    Hero.vx = -Hero.vx/2;
  }else if( Hero.x > WIDTH*16-48 ){
    Hero.x = WIDTH*16-48;
    Hero.vx = -Hero.vx/2;
  }

  if( Hero.y < 80 ){

    scroll_up();
    T_count = 0;

  } else if( Hero.y >= HEIGHT*16 )
    return DEAD;

  if( ( st = stand() ) ){
    acc = 3;
    floor = (Floor_top - Hero.y / 16)/5 + 1;

    Hero.pic &= 1;

    Hero.y = (Hero.y / 16 ) * 16;
    Hero.vy = 0;

    if( ++Hero.g_count == 5 ){
      Hero.g_count = 0;
      Hero.pic ^= 1;
    }

    if( Key[ KEY_UP ] ){
      Hero.jump = abs(Hero.vx)/4+7;
      Hero.vy = -Hero.jump/2-12;
      Hero.pic = 2;
      st = 0;
    }

  }else{
    acc = 2;
    floor = 0;
  }

  if( Key[ KEY_LEFT ] ){
    Hero.vx -= acc;
    if( Hero.vx < -32 )
      Hero.vx = -32;
    Hero.dir = 0;
  }else if( Key[ KEY_RIGHT ] ){
    Hero.vx += acc;
    if( Hero.vx > 32 )
      Hero.vx = 32;
    Hero.dir = 1;
  }else if( st ){
    if( Hero.vx < -2 )
      Hero.vx+=3;
    else if( Hero.vx > 2 )
      Hero.vx-=3;
    else
      Hero.vx = 0;
  }

  if( !st ){
    if( Hero.jump ){
      Hero.vy = -Hero.jump/2-12;
      if( Key[ KEY_UP ] )
	Hero.jump--;
      else
	Hero.jump = 0;
    }else{
      Hero.vy+=2;
      if( Hero.vy > 0 )
	Hero.pic = 3;

      if( Hero.vy > 16 )
	Hero.vy = 16;
    }
  }

  draw_hero();

  return floor;
}


/* Initialize the game */

void init_game( void )
{
  int y;

  Floor_top = HEIGHT - 4;

  for ( y = 0 ; y < HEIGHT ; y++ ){
    XCopyArea( Disp,Back_p,Scr_d,Gc_nomask,0,0,WIDTH*16,16,0,y*16 );
    make_floor(y);
  }

  Hero.x = WIDTH * 8 - 16;
  Hero.y = (HEIGHT - 4)*16-32;
  Hero.vx = Hero.vy = 0;
  Hero.pic = 0;
  Hero.dir = 0;
  Hero.g_count = 0;

  T_speed = 200;
  T_count = 0;

  Key[ KEY_UP ] = 0;
  draw_hero();
}


/* Repaint the background in a portion of the screen */

void recover_scr( int x, int y, int width, int height )
{
  int xl,xr,y2;

  y2 = min(HEIGHT-1, (y+height-1) / 16);
  y /= 16;

  for( ; y <= y2 ; y++ ){
    XCopyArea( Disp,Back_p,Scr_d,Gc_nomask,x,0,width,16,x,y*16 );

    xl = max( x,floor_l(y) );
    xr = min( x+width-1,floor_r(y) );

    if( xl < xr )
      XCopyArea( Disp,Floor_p,Scr_d,Gc_nomask,xl,0,xr-xl+1,16,xl,y*16 );
  }
}
