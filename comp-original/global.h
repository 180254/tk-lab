#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BSIZE 128
#define NONE -1
#define EOS '\0'
#define NUM 256
#define DIV 257
#define MOD 258
#define ID  259
#define DONE 260
extern int tokenval;
extern int lineno;
struct entry
{
  char *lexptr;
  int token;
};
extern struct entry symtable[];
int insert (char s[], int tok);
void error (char *m) ;
int lookup (char s[]) ;
void init () ;
void parse () ;
int lexan () ;
void expr () ;
void term () ;
void factor () ;
void match (int t) ;
void emit (int t, int tval) ;
