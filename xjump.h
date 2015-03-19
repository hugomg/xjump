/*****************************************
  xjump version 2

  xjump.h   ヘッダ( レコード関連除く )

  (C) Oct 28, 1997  ROYALPANDA
*****************************************/

/* キーの定義 */

#define KEY_UP    0
#define KEY_LEFT  1
#define KEY_RIGHT 2


/* 生死判定用 */

#define DEAD -1


/* 画面サイズ */

#define WIDTH  32
#define HEIGHT 24


typedef struct{

  int x;         /* 座標 x */
  int y;         /* 座標 y */
  int vx;        /* 速度 x */
  int vy;        /* 速度 y */
  int jump;      /* ジャンプ力 */
  int pic;       /* グラフィック */
  int dir;       /* 向き */
  int g_count;   /* アニメーション用カウンタ */

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

int rnd( int range );
void srnd( void );

void query_keycode( void );
void keymap( void );
void wait_keyup( void );

/* マクロ */

#define floor_r(y)  Floor_R[ ( y+Map_index) % HEIGHT ]
#define floor_l(y)  Floor_L[ ( y+Map_index) % HEIGHT ]
