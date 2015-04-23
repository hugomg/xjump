/*****************************************
  xjump version 2

  record.c   Highscore recording

  (C) July 16, 1997  ROYALPANDA
*****************************************/

#define _GNU_SOURCE

#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/file.h>
#include<errno.h>
#include<ctype.h>
#include<pwd.h>

#include"record.h"


static int skipspace( FILE *fp )
{
  int c;

  for(;;){
    c = fgetc(fp);
    if( c == EOF )
      return -1;
    if( c == '\n' )
      return -1;
    if( !isspace(c) ){
      ungetc(c,fp);
      return 0;
    }
  }
}


static int read_num( FILE *fp )
{
  int c,i;
  int err;

  if( skipspace( fp ) )
    return -1;

  i = 0;
  err = 1;

  for(;;){
    c = fgetc( fp );
    if( isdigit( c ) ){
      i = i * 10 + c - '0';
      err = 0;
    }else
      break;
  }
  ungetc( c,fp );

  if( err )
    return -1;
  else
    return i;
}


static void read_name( char *buf, FILE *fp )
{
  int l,c;

  if( skipspace( fp ) ){
    *buf = '\0';
    return;
  }

  for( l = 0 ; l < 31 ; l++ ){
    c = fgetc( fp );
    if( c < 0 || iscntrl( c ) ){
      ungetc( c,fp );
      break;
    }
    *buf++ = c;
  }
  *buf = '\0';
}


static void void_line( FILE *fp )
{
  int c;

  while( ( c = fgetc(fp) ) != '\n' && c != EOF )
    ;
}


/* Get real name given UID */

static char *get_name( int uid, char *name )
{
  struct passwd *pw;
  int i;

  if( ( pw = getpwuid( uid ) ) == NULL )
     name[0] = '\0';

  else{

    i = 0;
    while( i < 31 && pw->pw_gecos[i] != '\0' && pw->pw_gecos[i] != ',' ){
      name[i] = pw->pw_gecos[i];
      i++;
    }

    if( i > 0 )
      name[i] = '\0';
    else
      /* Prevent any sort of overflow */
      sprintf (name, "%.31s", pw->pw_name);
  }

  return name;
}


/* Read one entry from record file */

static int read_entry( FILE *fp, record_t *rec )
{
  if( ( rec->score = read_num( fp ) ) < 0 )
    return -1;

  if( ( rec->uid = read_num( fp ) ) < 0 )
    return -1;

  read_name( rec->name,fp );
  if( rec->name[0] == '\0' )
    get_name( rec->uid,rec->name );

  return 0;
}


/* Read all entries from record file */  

static void read_record( FILE *fp )
{
  int i;

  i = 0;

  while( !feof( fp ) && i < XJUMP_HIGHSCORE_ENTRIES ){
    if( read_entry( fp,&Record[i] ) == 0 )
      i++;
    void_line( fp );
  }
  Record_entry = i;
}


static void error( void )
{
  perror( Myname );
  fprintf( stderr,"%s: do not record high-score.\n",Myname);

  Record_entry = -1;
}

void init_record( void )
{
  FILE *fp;

  Record_entry = -1;

  if( ( fp = fopen( XJUMP_HIGHSCORE_FILENAME,"r+" ) ) == NULL ){
    error();
    return;
  }
  flock( fileno(fp),LOCK_EX );
  read_record( fp );
  fclose( fp );
}

static int sort_cmp( record_t *r1, record_t *r2 )
{
  if( r1->score > r2->score )
    return -1;
  else if( r1->score < r2->score )
    return 1;

  else if( r1->rank > r2->rank )
    return 1;
  else
    return -1;
}


static void sort_record( void )
{
  int i;

  for( i = 0 ; i < Record_entry ; i++ )
    Record[i].rank = i;

  qsort( Record,Record_entry,sizeof(record_t),  (int (*)(const void*, const void*)) sort_cmp );
}


void save_record( int sc )
{
  FILE *fp;
  int i,r;
  int uid;

  if( Record_entry == -1 )
    return ;

  if( (fp = fopen(XJUMP_HIGHSCORE_FILENAME,"r+")) == NULL ){
    error();
    return;
  }

  flock( fileno(fp),LOCK_EX );
  read_record( fp );

  r = -1;
  uid = (int)getuid();

  for( i = 0 ; i < Record_entry ; i++ )
    if( Record[i].uid == uid ){
      r = i;
      break;
    }

  if( r == -1 ){
    if( Record_entry < XJUMP_HIGHSCORE_ENTRIES ){
      r = Record_entry++;
      Record[r].score = 0;
    }else
      r = XJUMP_HIGHSCORE_ENTRIES - 1;
  }

  if( sc > Record[r].score ){
    Record[r].score = sc;
    Record[r].uid = uid;
    get_name( uid,Record[r].name );

    sort_record();

    rewind( fp );
    ftruncate( fileno(fp) ,0 );
    for( i = 0 ; i < Record_entry ; i++ )
      fprintf( fp,"%d %d %s\n",
               Record[i].score,Record[i].uid,Record[i].name );

    fclose( fp );
    return;
  }
  fclose( fp );
}
