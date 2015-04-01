/*****************************************
  xjump version 2

  record.h

  (C) July 16, 1997  ROYALPANDA
*****************************************/


typedef struct {

  int score;
  int uid;
  char name[32];
  int rank;

} record_t;


extern record_t Record[];
extern int Record_entry;

extern char *Myname;

void init_record( void );
void save_record( int score );
