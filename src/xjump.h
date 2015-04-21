/*****************************************
  xjump version 2

  xjump.h   Header for non-highscore stuff

  (C) Oct 28, 1997  ROYALPANDA
*****************************************/

/* Control keys */

#define KEY_UP    0
#define KEY_LEFT  1
#define KEY_RIGHT 2


/* For life and death decisions */

#define DEAD -1


/* Size of visible gameplay area */

#define WIDTH  32
#define HEIGHT 24


typedef struct{

  int x;
  int y;
  int vx;
  int vy;
  int jump;
  int pic;
  int dir;
  int g_count;   /* Animation counter */

} hero_t;


extern int Floor_L[];
extern int Floor_R[];
extern int Map_index;

extern hero_t Hero;

extern int Key[];

extern int Floor_top;

extern Display *Disp;
extern Drawable Scr_d;
extern Pixmap Floor_p;
extern Pixmap Back_p;
extern Pixmap Char_p;
extern Pixmap Char_m;

extern GC Gc_nomask;
extern GC Gc_mask;

void init_game( void );
int move( void );
void recover_scr( int x, int y, int x2, int y2 );
void draw_hero( void );

int rnd( int range );
void srnd( void );

/* Macros */

#define floor_r(y)  Floor_R[ ( y+Map_index) % HEIGHT ]
#define floor_l(y)  Floor_L[ ( y+Map_index) % HEIGHT ]
