/*****************************************
  xjump version 2

  main.c   ����� �� X Intrinsics�ط�

  (C) Oct 28, 1997  ROYALPANDA
*****************************************/

#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<sys/types.h>
#include<signal.h>
#include<X11/Intrinsic.h>
#include<X11/StringDefs.h>
#include<X11/Xaw/Form.h>
#include<X11/Xaw/Viewport.h>
#include<X11/Xaw/Label.h>
#include<X11/Shell.h>
#include<X11/xpm.h>

#include"xjump.h"
#include"record.h"

#include"picture.xpm"
#include"icon.xbm"
#include"icon_msk.xbm"

#define TITLE 0
#define GAME  1
#define OVER  2

#define SPEED 25

extern char *DefaultResources[];

static XtAppContext App;   /* ���ץꥱ������󥳥�ƥ����� */

static Widget
       Top,              /* �Ǿ��                        */
       Score,            /* ������(��٥�)                */
       Scr,              /* �ᥤ����̡ʥ���)             */
       Score_v,          /* �ϥ�������(�ӥ塼�ݡ���)      */
       ScoreList,        /* �ϥ�������(��٥�)            */
       Gameover,         /* �����४���С�(��٥�)        */
       Pause;            /* �ݡ���(��٥�)                */


static Colormap Cmap;    /* ���顼�ޥå� */

static int IntervalState;       /* �����޽�����? */
static XtIntervalId IntervalId; /* �����ޡ������Σɣ� */

static char *GraphFile = NULL;     /* �桼��������ե��å��ե����� */

static int GameMode; /* �⡼�� (0�����ȥ�  1������  2�����४���С� 3�ݡ���) */

static unsigned int Sc_now;

static char Score_list[43*(RECORD_ENTRY+2)+1]="";    /* �ϥ��������ƥ����� */

static XKeyboardState Keyboard;  /* �����ܡ��ɥ��ơ����� */
static int Repeat_mode = 1;      /* ������ԡ��Ȥξ���(1:default 0:off) */

static int Use_keymap = 0;       /* �����ޥåפ�ľ���ɤफ�� */

/* �ץ�ȥ�������� */

static void timi( XtPointer c, XtIntervalId id );


/* �����Х��ѿ� */

Display *Disp;           /* �ǥ����ץ쥤��¤�� */

GC Gc_nomask;            /* GC (�ޥ���̵��) */
GC Gc_mask;              /* GC (�ޥ�������) */

Drawable Scr_d;          /* �ᥤ����� */
Pixmap Char_p;           /* ����饯�� */
Pixmap Char_m;           /* ����饯��(�ޥ���) */
Pixmap Floor_p;          /* �� */
Pixmap Back_p;           /* �ط� */

int Key[3] = {0,0,0};    /* �����ξ��� */

hero_t Hero;             /* ��͸��ξ��� */

int Floor_L[ HEIGHT ];   /* ���ξ��� */
int Floor_R[ HEIGHT ];

int Map_index;
int Floor_top;

record_t Record[ RECORD_ENTRY ];  /* �ϥ��������ꥹ��                    */
int Record_entry;                 /* �ϥ��������ꥹ�Ȥο�  -1:��Ͽ���ʤ� */

char *Myname;   /* �ץ����̾ */


/* ������ԡ��Ȥ�ߤ�� */

static void repeat_off( void )
{
#if 0
  if( Repeat_mode ){
    XGetKeyboardControl( Disp,&Keyboard );
    XAutoRepeatOff( Disp );
    Repeat_mode = 0;
  }
#endif
}


/* ������ԡ��Ȥ򸵤��᤹ */

static void repeat_on( void )
{
  if( !Repeat_mode ){
    if( Keyboard.global_auto_repeat )
      XAutoRepeatOn( Disp );
    else
      XAutoRepeatOff( Disp );

    Repeat_mode = 1;
  }
}


/* ������ɽ�� */

static void put_score( void )
{
  char buf[12];

  sprintf( buf,"%010d",Sc_now );
  XtVaSetValues( Score,XtNlabel,buf,NULL );
}


/* �ϥ����������̺��� */

static void make_score( void )
{
  int i;
  char *p;

  if( Record_entry == -1 )
    return;

  p = Score_list;

  p += sprintf( p,"RANK    FLOOR               NAME\n\
----  ----------  -------------------------------\n");

  for( i = 0 ; i < Record_entry ; i++ )
    p += sprintf( p,"%4d  %10d        %-20.20s\n",i+1,
	    Record[i].score,Record[i].name );

  p--;
  *p = '\0';

  XtVaSetValues( ScoreList,
		XtNlabel,Score_list,
		NULL );
}


/* �����ޡ����� */

static void reset_timer( void )
{
  if( IntervalState ){
    XtRemoveTimeOut( IntervalId );
    IntervalState = 0;
  }
}


/* �����ޡ��������� */

static void set_timer( void )
{
  if( IntervalState )
    reset_timer();
  IntervalId = XtAppAddTimeOut( App, SPEED ,(XtTimerCallbackProc)timi,NULL );
  IntervalState = 1;
}


/* �����४���С� */

static void gameover( void )
{
  XtMapWidget( Gameover );

  if( Use_keymap )
    wait_keyup();

  GameMode = OVER;

  save_record( Sc_now );
  make_score();

}


/* �����ȥ� */

static void title( void )
{
  reset_timer();
  XtUnmapWidget( Gameover );
  XtUnmapWidget( Scr );
  GameMode = TITLE;
  if( Record_entry != -1 )
    XtMapWidget( Score_v );
}



/* �����ޡ������롼���� */

static void timi( XtPointer c,XtIntervalId id )
{
  unsigned int floor;
  static int timer;

  IntervalState = 0;

  set_timer();

  switch( GameMode ){

  case GAME:
    if( Use_keymap )
      keymap();
    floor = move();
    if( floor == DEAD ){
      gameover();
      timer = 0;
    }
    else if( floor > Sc_now ){
      Sc_now = floor;
      if( Sc_now > UINT_MAX - 5)
	Sc_now = UINT_MAX - 5;
      put_score();
    }
    break;
    
  case OVER:
    if( ++timer > 250 )
      title();
    break;
  }
}


/* �ե��������ѹ�(������ԡ��Ȥ����ؤ�)(���٥�ȥϥ�ɥ�) */

static void focus( Widget w,XtPointer p,XEvent *e )
{
  if( e->type == FocusIn )
    repeat_off();
  else
    repeat_on();
}


/* �������ݡ������� (���٥�ȥϥ�ɥ�) */

static void expose( Widget w,XtPointer p,XEvent *e )
{
  recover_scr( e->xexpose.x,e->xexpose.y,
	      e->xexpose.width,e->xexpose.height );
}


/* �ץ���ཪλ (���������ϥ�ɥ�) */

static void quit_game( Widget w,XEvent *e,String *s,Cardinal *num )
{
  repeat_on();
  XFlush( Disp );
  exit(0);
}


/* �ץ���ཪλ(�����ʥ�ϥ�ɥ�) */

static void sig_handler( int i )
{
  repeat_on();
  XFlush( Disp );
  exit(0);
}


/* �������� (���������ϥ�ɥ�) */

static void start_game( Widget w,XEvent *e,String *s,Cardinal *num )
{
  switch( GameMode ){
  case TITLE:

    if( Use_keymap )
      wait_keyup();

    XtUnmapWidget( Score_v );
    XtUnmapWidget( Gameover );
    reset_timer();
    XtMapWidget( Scr );
    init_game();
    Sc_now = 0;
    GameMode = GAME;

    set_timer();
    break;

  case OVER:
    title();
    break;
  }
}


/* �ݡ��� (���������ϥ�ɥ�) */

static void pause_game( Widget w,XEvent *e,String *s,Cardinal *num )
{
  if( GameMode == 1 ){
    reset_timer();
    XtMapWidget( Pause );
    GameMode = 3;
  }else if( GameMode == 3 ){
    GameMode = 1;
    XtUnmapWidget( Pause );
    set_timer();
  }
}



/* ���������줿(���������ϥ�ɥ�) */

static void key_on( Widget w,XEvent *e,String *s,Cardinal *num )
{
  if( Use_keymap )
    return;

  switch( **s ){
  case 'U':
    Key[ KEY_UP ] = 1;
    break;

  case 'R':
    Key[ KEY_RIGHT ] = 1;
    break;

  case 'L':
    Key[ KEY_LEFT ] = 1;
    break;
  }
}

/* ����Υ���줿 (���������ϥ�ɥ�) */

static void key_off( Widget w,XEvent *e,String *s,Cardinal *num )
{
  if( Use_keymap )
    return;

  switch( **s ){
  case 'U':
    Key[ KEY_UP ] = 0;
    break;

  case 'R':
    Key[ KEY_RIGHT ] = 0;
    break;

  case 'L':
    Key[ KEY_LEFT ] = 0;
    break;
  }
}


/* ������������� */

static void set_icon( void )
{
  Pixmap icon,mask;

  icon = XCreateBitmapFromData( Disp,DefaultRootWindow(Disp),icon_bits,
			       icon_width,icon_height );

  mask = XCreateBitmapFromData( Disp,DefaultRootWindow(Disp),icon_msk_bits,
			       icon_msk_width,icon_msk_height );

  XtVaSetValues( Top,XtNiconPixmap,icon,XtNiconMask,mask,NULL );
}


/* �إ�� */

static void help()
{
  fprintf( stderr,"Usage: %s [options]\n",Myname );
  fprintf( stderr,"\t-toolkitoption ...\n" );
  fprintf( stderr,"\t-private\t\tuse private colormap.\n" );
  fprintf( stderr,"\t-graphic \"file\"\t\tuse your xpm graphic \"file\"\n" );
  fprintf( stderr,"\t-keymap\t\t\tread keymap directly.\n" );
  fprintf( stderr,"\t-help\t\t\tshow this messages.\n" );
}


/* ���ޥ�ɥ饤�󥪥ץ������� */

static void option( int argc, char **argv )
{
  int i;

  for( i = 1 ; i < argc ; i++ ){

    if( strcmp( argv[i],"-private" ) == 0 ){
      Cmap =
	XCreateColormap( Disp,DefaultRootWindow(Disp),
			DefaultVisual(Disp,DefaultScreen(Disp)),AllocNone );
      XtVaSetValues( Top,XtNcolormap,Cmap,NULL );
      continue;
    }

    if( strcmp( argv[i],"-graphic" ) == 0 ){
      GraphFile = argv[++i];
      continue;
    }

    if( strcmp( argv[i],"-keymap" ) == 0 ){
      Use_keymap = 1;
      continue;
    }

    if( strcmp( argv[i],"-help" ) == 0 ){
      help();
      exit(0);
    }
    fprintf(stderr,"%s: Unknown command line option \"%s\".\n",Myname,argv[i]);
    help();
    exit(1);
  }
}  


/* ����ե��å��κ������ɤ߹��� */

static void make_graphic( void )
{
  int i;
  XpmAttributes attr;

  attr.valuemask = XpmColormap;
  attr.colormap = Cmap;

  if( GraphFile != NULL )
    i = XpmReadFileToPixmap( Disp,DefaultRootWindow(Disp),GraphFile,
			    &Char_p,&Char_m,&attr );
    else
      i = XpmCreatePixmapFromData( Disp,DefaultRootWindow(Disp),
				  picture_xpm,&Char_p,&Char_m,&attr );
  if( i ){
    fprintf( stderr,"%s: %s\n",Myname,XpmGetErrorString(i) );
    exit(1);
  }

  Gc_mask = XCreateGC( Disp,DefaultRootWindow(Disp),0,NULL );
  XSetClipMask( Disp,Gc_mask,Char_m );
}



/* Xlib�ط��������� */

static void init_graph( void )
{
  int x;

  make_graphic();

  Gc_nomask = XCreateGC( Disp,DefaultRootWindow(Disp),0,NULL );

  Floor_p = XCreatePixmap( Disp,DefaultRootWindow(Disp),
			  WIDTH*16,16,
			  DefaultDepth(Disp,DefaultScreen(Disp)) );
  XCopyArea( Disp,Char_p,Floor_p,Gc_nomask,128,16,16,16,0,0 );
  XCopyArea( Disp,Char_p,Floor_p,Gc_nomask,128,32,16,16,WIDTH*16-16,0 );
  for( x = 16 ; x < WIDTH*16-16 ; x+=16 )
    XCopyArea( Disp,Char_p,Floor_p,Gc_nomask,128,48,16,16,x,0 );

  Back_p = XCreatePixmap( Disp,DefaultRootWindow(Disp),
			 WIDTH*16,16,
			 DefaultDepth(Disp,DefaultScreen(Disp)) );
  XCopyArea( Disp,Char_p,Back_p,Gc_nomask,128,16,16,16,0,0 );
  XCopyArea( Disp,Char_p,Back_p,Gc_nomask,128,32,16,16,WIDTH*16-16,0 );
  for( x = 16 ; x < WIDTH*16-16 ; x+=16 )
    XCopyArea( Disp,Char_p,Back_p,Gc_nomask,128,0,16,16,x,0 );

}


/* �ᥤ�� */

int main( int argc,char **argv )
{
  static XtActionsRec a_table[] = {
    {"start",  (XtActionProc)start_game},
    {"pause",  (XtActionProc)pause_game},
    {"quit",   (XtActionProc)quit_game},
    {"key_on", (XtActionProc)key_on},
    {"key_off",(XtActionProc)key_off},
  };

  Widget game,w;

  uid_t uid;

  if( (Myname = argv[0]) == NULL )
    Myname = "xjump";

  uid = geteuid();
  seteuid(getuid());

  XtSetLanguageProc( NULL,NULL,NULL );

  Top = XtVaAppInitialize( &App,"XJump",NULL,0,
			  &argc,argv,DefaultResources,NULL );

  Disp = XtDisplay( Top );


  Cmap = DefaultColormap( Disp,DefaultScreen(Disp) );

  option( argc,argv );

  seteuid( uid );

  game = XtVaCreateManagedWidget( "game",formWidgetClass,Top,NULL );

  XtVaCreateManagedWidget( "title",labelWidgetClass,game,NULL );

  w = XtVaCreateManagedWidget( "scoreBoard",formWidgetClass,game,
			      NULL );

  XtVaCreateManagedWidget( "scLabel",labelWidgetClass,w,
			  NULL );

  Score = XtVaCreateManagedWidget( "score",labelWidgetClass,w,
				  XtNlabel,"0000000000",
				  NULL );

  Gameover = XtVaCreateManagedWidget( "gameover",labelWidgetClass,game,
				     XtNmappedWhenManaged,FALSE,
				     NULL );

  Pause = XtVaCreateManagedWidget( "pause",labelWidgetClass,game,
				  XtNmappedWhenManaged,FALSE,
				  NULL );

  Scr = XtVaCreateManagedWidget( "scr",widgetClass,game,
				XtNwidth,WIDTH*16,
				XtNheight,HEIGHT*16,
				XtNmappedWhenManaged,FALSE,
				NULL );

  Score_v = XtVaCreateManagedWidget( "record_v",viewportWidgetClass,game,
				    XtNwidth,WIDTH*16,
				    XtNheight,HEIGHT*16,
				    NULL );

  ScoreList = XtVaCreateManagedWidget( "record",labelWidgetClass,Score_v,
				      XtNlabel,"",
				      NULL );

  XtVaCreateManagedWidget( "copyright",labelWidgetClass,game,NULL );

  XtAddEventHandler( Top,FocusChangeMask,FALSE,(XtEventHandler)focus,NULL );
  XtAddEventHandler( Scr,ExposureMask,FALSE,(XtEventHandler)expose,NULL );

  XtAppAddActions( App,a_table,XtNumber(a_table) );

  if( Use_keymap )
    query_keycode();

  if( signal( SIGINT,SIG_IGN ) != SIG_IGN )
    signal( SIGINT,sig_handler );

  if( signal( SIGTERM,SIG_IGN ) != SIG_IGN )
    signal( SIGTERM,sig_handler );

  XtRealizeWidget( Top );
  init_graph();
  set_icon();
  Map_index = 0;
  Scr_d = XtWindow( Scr );

  init_record();
  if( Record_entry != -1 )
    make_score();

  srnd();

  GameMode = TITLE;

  XtAppMainLoop( App );

  return 0;
}
