/*****************************************
  xjump version 2

  record.h   ヘッダ( レコード関連 )

  (C) July 16, 1997  ROYALPANDA
*****************************************/


typedef struct {

  int score;       /* 得点 */
  int uid;         /* ユーザーID */
  char name[32];   /* 名前 */
  int rank;        /* 順位 */

} record_t;


extern record_t Record[];
extern int Record_entry;

extern char *Myname;

void init_record( void );
void save_record( int score );
