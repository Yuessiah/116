/* Definition section */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXI 1000
#define MAXS 100

extern int yylineno;
extern FILE *yyin;

struct idmap {
  char id[MAXS], type[MAXS];
  double f_num;
  int i_num;
} *sym[MAXI];

int num_line = 0, SIZE = 0;

int yylex();
void yyerror(char const *);
void create_symbol();
void f_insert_symbol(char *, char *, double);
void i_insert_symbol(char *, char *, int);
int lookup_symbol(char *);
void dump_symbol();
%}

/* Using union to define nonterminal and token type */
%union {
  int i_val;
  double f_val;
  char* string;
}

/* Token without return */
%token PRINT PRINTLN
%token IF ELSE FOR
%token VAR NEWLINE
%token INCRE DECRE
%token LEQ GEQ EQ NEQ
%token L_AND L_OR
%token END_OF_FILE
%token compound_stat

/* Token with return, which need to sepcify type */
%token <i_val> I_CONST
%token <f_val> F_CONST
%token <string> STRING ID
%token <string> INT FLOAT32 VOID

/* Nonterminal with return, which need to sepcify type */
%type <string> type
%type <i_val> i_expr b_expr
%type <f_val> f_expr

%left '+' '-'
%left '*' '/' '%'

/* Yacc will start at this nonterminal */
%start program

/* Grammar section */
%%

program
  : program stat NEWLINE
  | stat NEWLINE
;

stat
  : declaration 
  | print_func 
  | assignment 
  | compound_stat 
  | b_expr { puts($1? "true" : "false"); }
  | END_OF_FILE { YYACCEPT; }
;

declaration
  : VAR ID type '=' i_expr {
    if(lookup_symbol($2)) yyerror("Redefined variable!");
    else i_insert_symbol($2, $3, $5);
  }
  | VAR ID type '=' f_expr {
    if(lookup_symbol($2)) yyerror("Redefined variable!");
    else f_insert_symbol($2, $3, $5);
  }
  | VAR ID type {
    if(lookup_symbol($2)) yyerror("Redefined variable!");
    else i_insert_symbol($2, $3, 0);
  }
;

assignment
  : ID '='  i_expr { sym[lookup_symbol($1)]->i_num =  $3; }
  | ID '='  f_expr { sym[lookup_symbol($1)]->f_num =  $3; }
  | ID "*=" i_expr { sym[lookup_symbol($1)]->i_num *= $3; }
  | ID "*=" f_expr { sym[lookup_symbol($1)]->f_num *= $3; }
  | ID "/=" i_expr {
    if(!($3)) yyerror("<ERROR> The divisor can’t be 0");
    else sym[lookup_symbol($1)]->i_num /= $3;
  }
  | ID "/=" f_expr {
    if(!($3)) yyerror("<ERROR> The divisor can’t be 0");
    else sym[lookup_symbol($1)]->f_num /= $3;
  }
  | ID "+=" i_expr { sym[lookup_symbol($1)]->i_num += $3; }
  | ID "+=" f_expr { sym[lookup_symbol($1)]->f_num += $3; }
  | ID "-=" i_expr { sym[lookup_symbol($1)]->i_num -= $3; }
  | ID "-=" f_expr { sym[lookup_symbol($1)]->f_num -= $3; }
  | ID "%=" i_expr { sym[lookup_symbol($1)]->i_num %= $3; }
;

b_expr
  : i_expr
  | f_expr { $$ = (int)$1; }
  | b_expr '<' b_expr { $$ = $1 <  $3; }
  | b_expr LEQ b_expr { $$ = $1 <= $3; }
  | b_expr '>' b_expr { $$ = $1 >  $3; }
  | b_expr GEQ b_expr { $$ = $1 >= $3; }
  | b_expr EQ  b_expr { $$ = $1 == $3; }
  | b_expr NEQ b_expr { $$ = $1 != $3; }
  | b_expr L_AND b_expr { $$ = $1 && $3; }
  | b_expr L_OR  b_expr { $$ = $1 || $3; }
;

f_expr
  : F_CONST
  | ID {
    int idx = lookup_symbol($1);
    if(!strcmp(sym[idx]->type, "float32")) $$ = sym[idx]->f_num;
    if(!strcmp(sym[idx]->type, "int"    )) $$ = (double)sym[idx]->i_num;
  }
  | '(' f_expr ')' { $$ = $2; }
  | f_expr '*' f_expr { $$ = $1 * $3; }
  | f_expr '/' f_expr {
    if(!($3)) yyerror("<ERROR> The divisor can’t be 0");
    else $$ = $1 / $3;
  }
  | f_expr '+' f_expr { $$ = $1 + $3; }
  | f_expr '-' f_expr { $$ = $1 - $3; }
;

i_expr
  : I_CONST
  | ID {
    int idx = lookup_symbol($1);
    if(!strcmp(sym[idx]->type, "float32")) $$ = (int)sym[idx]->f_num;
    if(!strcmp(sym[idx]->type, "int"    )) $$ = sym[idx]->i_num;
  }
  | '(' i_expr ')' { $$ = $2; }
  | i_expr '*' i_expr { $$ = $1 * $3; }
  | i_expr '/' i_expr {
    if(!($3)) yyerror("<ERROR> The divisor can’t be 0");
    else $$ = $1 / $3;
  }
  | i_expr '%' i_expr { $$ = $1 % $3; }
  | i_expr '+' i_expr { $$ = $1 + $3; }
  | i_expr '-' i_expr { $$ = $1 - $3; }
;


print_func
  : PRINT '(' b_expr ')' { printf("%d", $3); }
  | PRINT '(' i_expr ')' { printf("%d", $3); }
  | PRINT '(' f_expr ')' { printf("%f", $3); }
  | PRINTLN '(' b_expr ')' { printf("%d\n", $3); }
  | PRINTLN '(' i_expr ')' { printf("%d\n", $3); }
  | PRINTLN '(' f_expr ')' { printf("%f\n", $3); }
;

type
  : INT | FLOAT32 | VOID
;

%%

/* C code section */
void create_symbol() {
  printf("Create a symbol table\n");
  int i;
  for(i = 0; i < MAXI; i++)
    sym[i] = (struct idmap*)malloc(sizeof(struct idmap));
}

void f_insert_symbol(char *a, char *b, double c) {
  printf("Insert a symbol: %s\n", a);
  SIZE++;
  strcpy(sym[SIZE]->id, a);
  strcpy(sym[SIZE]->type, b);
  sym[SIZE]->f_num = c;
}

void i_insert_symbol(char *a, char *b, int c) {
  printf("Insert a symbol: %s\n", a);
  SIZE++;
  strcpy(sym[SIZE]->id, a);
  strcpy(sym[SIZE]->type, b);
  sym[SIZE]->i_num = c;
}

int lookup_symbol(char *a) {
  int i;
  for(i = 1; i <= SIZE; i++)
    if(!strcmp(sym[i]->id, a)) return i;
  return 0;
}

void dump_symbol() {
  int i;
  for(i = 1; i <= SIZE; i++) {
    printf("%-10s %-10s ", sym[i]->id, sym[i]->type);
    if(!strcmp("float32", sym[i]->type)) printf("%-10f\n", sym[i]->f_num);
    if(!strcmp("int"    , sym[i]->type)) printf("%-10d\n", sym[i]->i_num);
  }
}

void yyerror(char const *s) {
  fprintf(stderr, "%s (line %d)\n", s, num_line+1);
}

int main(int argc,char *argv[]) {
  create_symbol();

  yyin = fopen(argv[1],"r");
  yyparse();

  dump_symbol();

  printf("Number of line(s): %d\n", num_line);

  return 0;
}
