/*****************************************
  xjump version 2

  record.h   �إå�( �쥳���ɴ�Ϣ )

  (C) July 16, 1997  ROYALPANDA
*****************************************/


typedef struct {

  int score;       /* ���� */
  int uid;         /* �桼����ID */
  char name[32];   /* ̾�� */
  int rank;        /* ��� */

} record_t;


extern record_t Record[];
extern int Record_entry;

extern char *Myname;

void init_record( void );
void save_record( int score );
