/*****************************************
  xjump version 2

  xjump.h   �إå�( �쥳���ɴ�Ϣ���� )

  (C) Oct 28, 1997  ROYALPANDA
*****************************************/

/* ��������� */

#define KEY_UP    0
#define KEY_LEFT  1
#define KEY_RIGHT 2


/* ����Ƚ���� */

#define DEAD -1


/* ���̥����� */

#define WIDTH  32
#define HEIGHT 24


typedef struct{

  int x;         /* ��ɸ x */
  int y;         /* ��ɸ y */
  int vx;        /* ®�� x */
  int vy;        /* ®�� y */
  int jump;      /* �������� */
  int pic;       /* ����ե��å� */
  int dir;       /* ���� */
  int g_count;   /* ���˥᡼������ѥ����� */

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

/* �ޥ��� */

#define floor_r(y)  Floor_R[ ( y+Map_index) % HEIGHT ]
#define floor_l(y)  Floor_L[ ( y+Map_index) % HEIGHT ]
