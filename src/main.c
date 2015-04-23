/*****************************************
  xjump version 2

  main.c   Initialization / X Intrinsics stuff
  
  (C) Oct 28, 1997  ROYALPANDA
*****************************************/

#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
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

#include "xjump.h"
#include "record.h"
#include "config.h"
#include "safestr.h"

#include"icon.xbm"
#include"icon_msk.xbm"

#define SPEED 25

typedef enum {
  TITLE=0,
  GAME=1,
  OVER=2,
  PAUSE=3
} GameState;

extern char *DefaultResources[];

static XtAppContext App;

static Widget
       Top,
       Score,
       Scr,
       Score_v,
       ScoreList,
       Gameover,
       Pause;


static Colormap Cmap;

static int IntervalState;
static XtIntervalId IntervalId;

static char HighscoreFilepath[PATH_MAX] = XJUMP_HIGHSCORE_FILEPATH;
static char SpriteFilepath[PATH_MAX] = XJUMP_SPRITES_FILEPATH;

static GameState GameMode; /* 0=Title; 1=Game; 2=GameOver; 3=Pause */

static int Sc_now;

static char Score_list[45*(XJUMP_HIGHSCORE_ENTRIES+2)+1]="";    /* High score text buffer */

/* Prototypes */

static void timi( XtPointer c, XtIntervalId id );


/* Globals */

Display *Disp;

GC Gc_nomask;
GC Gc_mask;

Drawable Scr_d;
Pixmap Char_p;
Pixmap Char_m;
Pixmap Floor_p;
Pixmap Back_p;

int Key[3] = {0,0,0};    /* Keyboard state */

hero_t Hero;             /* Hero state */

int Floor_L[ HEIGHT ];   /* Current floors */
int Floor_R[ HEIGHT ];

int Map_index;
int Floor_top;

record_t Record[ XJUMP_HIGHSCORE_ENTRIES ];
int Record_entry; /* Size of high-score list;  -1 means scores are not being recorded */

char *Myname;   /* Program name */


/* Show current score */
static void put_score( void )
{
  char buf[12];

  sprintf( buf,"%010d",Sc_now );
  XtVaSetValues( Score,XtNlabel,buf,NULL );
}

/* Show highscores table */
static void make_score( void )
{
  int i;
  char *p;

  if( Record_entry == -1 )
    return;

  p = Score_list;

  p += sprintf( p,"RANK    FLOOR               NAME\n\
----  ----------  -------------------------------\n");

  for( i = 0 ; i < Record_entry ; i++ ){
    size_t space_available = sizeof(Score_list) - (p-Score_list);
    int nprinted = snprintf(p, space_available,"%4d  %10d        %-20.20s\n",i+1,
                            Record[i].score, Record[i].name );
    if(nprinted < 0){
      break;
    }else if( (size_t) nprinted <= space_available){
      p += nprinted;
    }else{
      p += space_available;
      break;
    }
  }

  p--;
  *p = '\0';

  XtVaSetValues( ScoreList,
                XtNlabel,Score_list,
                NULL );
}


static void reset_timer( void )
{
  if( IntervalState ){
    XtRemoveTimeOut( IntervalId );
    IntervalState = 0;
  }
}

static void set_timer( void )
{
  if( IntervalState )
    reset_timer();
  IntervalId = XtAppAddTimeOut( App, SPEED ,(XtTimerCallbackProc)timi,NULL );
  IntervalState = 1;
}


static void gameover( void )
{
  XtMapWidget( Gameover );

  GameMode = OVER;

  save_record(HighscoreFilepath, Sc_now);
  make_score();

}

static void title( void )
{
  reset_timer();
  XtUnmapWidget( Gameover );
  XtUnmapWidget( Scr );
  GameMode = TITLE;
  if( Record_entry != -1 )
    XtMapWidget( Score_v );
}


static void timi( XtPointer c,XtIntervalId id )
{
  int floor;
  static int timer;

  IntervalState = 0;

  set_timer();

  switch( GameMode ){

  case GAME:
    floor = move();
    if( floor == DEAD ){
      gameover();
      timer = 0;
    }
    else if( floor > Sc_now ){
      Sc_now = floor;
      if( Sc_now > INT_MAX - 5)
        Sc_now = INT_MAX - 5;
      put_score();
    }
    break;
    
  case OVER:
    if( ++timer > 250 )
      title();
    break;

  default: break;
  }
}

/* Expose-screen event handler */
static void expose( Widget w,XtPointer p,XEvent *e )
{
  recover_scr( e->xexpose.x,e->xexpose.y,
              e->xexpose.width,e->xexpose.height );
  draw_hero();
}

/* Action handler for quit-game shortcut */
static void quit_game( Widget w,XEvent *e,String *s,Cardinal *num )
{
  exit(0);
}


/* End-of-program handler if signal is received */

static void sig_handler( int i )
{
  exit(0);
}


/* Start game action handler */

static void start_game( Widget w,XEvent *e,String *s,Cardinal *num )
{
  switch( GameMode ){
  case TITLE:

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

  default: break;
  }
}


/* Pause game action handler */

static void try_pause()
{
  if( GameMode == GAME ){
    reset_timer();
    XtMapWidget( Pause );
    GameMode = PAUSE;
  }
}

static void try_unpause()
{
  if( GameMode == PAUSE ){
    GameMode = GAME;
    XtUnmapWidget( Pause );
    set_timer();
  }
}

static void pause_game( Widget w,XEvent *e,String *s,Cardinal *num )
{
  if( GameMode == GAME ){
    try_pause();
  }else if( GameMode == PAUSE ){
    try_unpause();
  }
}


static void focus_in( void )
{
  /* do nothing special */
}


static void focus_out( void )
{
  try_pause();
}


/* Focus change event handler */
static void focus( Widget w,XtPointer p,XEvent *e )
{
  if( e->type == FocusIn )
    focus_in();
  else
    focus_out();
}

/* Keydown action handler */

static void key_on( Widget w,XEvent *e,String *s,Cardinal *num )
{
  if(GameMode == PAUSE){
    try_unpause();
    return;
  }
  
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

/* Keyup action handler */

static void key_off( Widget w,XEvent *e,String *s,Cardinal *num )
{
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


static void set_icon( void )
{
  Pixmap icon,mask;

  /* casting to char* is fine according to http://stackoverflow.com/a/12786292 */

  icon = XCreateBitmapFromData( Disp,DefaultRootWindow(Disp), (char*) icon_bits,
                                icon_width, icon_height );

  mask = XCreateBitmapFromData( Disp,DefaultRootWindow(Disp), (char*) icon_msk_bits,
                                icon_msk_width,icon_msk_height );

  XtVaSetValues( Top,XtNiconPixmap,icon,XtNiconMask,mask,NULL );
}


static void help()
{
  fprintf( stderr,"Usage: %s [options]\n",Myname );
  fprintf( stderr,"\t-help\t\t\tshow this messages.\n" );
  fprintf( stderr,"\t-graphic \"file\"\t\tuse your xpm graphic \"file\"\n" );
  fprintf( stderr,"\t-toolkitoption ...\n" );
}


/* Process command-line args */

static void read_command_line_options( int argc, char **argv )
{
  int i;

  for( i = 1 ; i < argc ; i++ ){

    if( strcmp( argv[i],"-help" ) == 0 ){
      help();
      exit(0);
    }

    if( strcmp( argv[i],"-graphic" ) == 0 ){
      try_strncpy(SpriteFilepath, argv[++i], PATH_MAX);
      continue;
    }

    if( strcmp( argv[i], "-highscoreFile" ) == 0 ){
      try_strncpy(HighscoreFilepath, argv[++i], PATH_MAX);
      continue;
    }

    fprintf(stderr,"%s: Unknown command line option \"%s\".\n",Myname,argv[i]);
    help();
    exit(1);
  }
}  


/* Remove whitespace from the start and end of the string */
static void trim_string(char *s)
{
  char *in = s;
  char *out = s;

  /* Skip leading whitespace */
  while(*in && isspace(*in)){ ++in; }
  while(*in){ *out++ = *in++; }

  /* Erase trailing whitespace */
  while(s < out && isspace(*(out-1))){ --out; }

  *out = '\0';

}

/* Proccess a configuration file and update the correspondig global variables*/

static void read_configuration_file(const char * filename)
{
  FILE * file = fopen(filename, "r");
  if (!file) return;

  size_t size_line = 0;
  char *line_buf = NULL;

  while(0 < getline(&line_buf, &size_line, file)){

    { /* Comments */
      if(line_buf[0] == '#') continue; /* This read is safe due to the loop condition */
    }

    { /* Blank lines */
      int allspace = 1;
      for(char *c=line_buf; *c; c++){ allspace = allspace && isspace(*c); }
      if(allspace) continue;
    }

    /* Key-Value pairs*/

    char *sep = strchr(line_buf, '=');
    if(!sep){
      fprintf(stderr, "Syntax error in %s\n", filename);
      goto cleanup;
    }

    *sep = '\0';
    char *key = line_buf;
    char *value = sep + 1;
    trim_string(key);
    trim_string(value);

    if      (0 == strcmp(key, "theme")){
      try_snprintf(SpriteFilepath, PATH_MAX, "%s/%s.xpm", XJUMP_THEMES_DIR, value);

    }else if(0 == strcmp(key, "themeFile")){
      try_strncpy(SpriteFilepath, value, PATH_MAX);

    }else if(0 == strcmp(key, "highscoreFile")){
      try_strncpy(HighscoreFilepath, value, PATH_MAX);
    }
  }

 cleanup:
  fclose(file);
  free(line_buf);
}


/* Read configuration files from lowest to highest priority */
static void read_configuration_files(void)
{
  const char *xdg_config_home = getenv("XDG_CONFIG_HOME");
  const char *homedir = getenv("HOME");
  if(!homedir){
    fprintf(stderr, "$HOME doesn't exist\n");
    exit(1);
  }

  char filename[PATH_MAX];

  try_snprintf(filename, PATH_MAX, "%s/%s", XJUMP_GLOBAL_SETTINGS_DIR, XJUMP_CONFIG_FILENAME);
  read_configuration_file(filename);

  /* Location of user config files, per the Freedesktop spec. */
  if(xdg_config_home && *xdg_config_home){
    try_snprintf(filename, PATH_MAX, "%s/%s", xdg_config_home, XJUMP_CONFIG_FILENAME);
  }else{
    try_snprintf(filename, PATH_MAX, "%s/.config/%s", homedir, XJUMP_CONFIG_FILENAME);
  }
  read_configuration_file(filename);
}

/* Initialize graphics and textures */

static void make_graphic( void )
{
  XpmAttributes attr;
  attr.valuemask = XpmColormap;
  attr.colormap = Cmap;

  int err = XpmReadFileToPixmap( Disp, DefaultRootWindow(Disp), SpriteFilepath,
                                 &Char_p,&Char_m,&attr );
  if( err ){
    fprintf( stderr,"%s: %s\n",Myname,XpmGetErrorString(err) );
    exit(1);
  }

  Gc_mask = XCreateGC( Disp,DefaultRootWindow(Disp),0,NULL );
  XSetClipMask( Disp,Gc_mask,Char_m );
}



/* Xlib initialization */

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
    Myname = PACKAGE_NAME;

  uid = geteuid();
  seteuid(getuid());

  XtSetLanguageProc( NULL,NULL,NULL );

  Top = XtVaAppInitialize( &App,"XJump",NULL,0,
                          &argc,argv,DefaultResources,NULL );

  Disp = XtDisplay( Top );


  Cmap = DefaultColormap( Disp,DefaultScreen(Disp) );

  read_configuration_files();
  read_command_line_options( argc,argv );

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

  if( signal( SIGINT,SIG_IGN ) != SIG_IGN )
    signal( SIGINT,sig_handler );

  if( signal( SIGTERM,SIG_IGN ) != SIG_IGN )
    signal( SIGTERM,sig_handler );

  XtRealizeWidget( Top );
  init_graph();
  set_icon();
  Map_index = 0;
  Scr_d = XtWindow( Scr );

  init_record(HighscoreFilepath);
  if( Record_entry != -1 )
    make_score();

  srnd();

  GameMode = TITLE;

  XtAppMainLoop( App );

  return 0;
}
