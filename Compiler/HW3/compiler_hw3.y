/*  Definition section */
%{
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "common.h" //Extern variables that communicate with lex
#define MAXI 1000
#define MAXS 100
//#define file stdout

extern int yylineno;
extern int yylex();

struct SYMBOL {
  char id[MAXS], type[MAXS];
  double f_num;
  int i_num;
} *sym[MAXI];

int num_line = 0, SIZE = 0, f_flag = 0, eq_flag = 0, ne_flag = 0, label = 0, exit_lb = 0;

FILE *file;

void yyerror(const char* error);

/* symbol table function */
void create_symbol();
void f_insert_symbol(char *, char const *);
void i_insert_symbol(char *, char const *);
int lookup_symbol(char *);
void dump_symbol();

%}

%union {
  RULE_TYPE rule_type;
  int intVal;
}

/* Token definition */
%token INC DEC
%token GE LE EQ NE
%token <rule_type> ADDASGN SUBASGN MULASGN DIVASGN MODASGN
%token AND OR NOT
%token IF ELSE FOR
%token VAR
%token QUOTA
%token NEWLINE

%token <rule_type> PRINT PRINTLN

%token <rule_type> I_CONST
%token <rule_type> F_CONST
%token <rule_type> VOID INT FLOAT STRING ID

%type <rule_type> initializer expr equality_expr relational_expr
%type <rule_type> additive_expr multiplicative_expr prefix_expr postfix_expr
%type <rule_type> primary_expr constant
%type <rule_type> type

%type <rule_type> print_func_op add_op mul_op relational_op assignment_op equality_op
%type <rule_type> '+' '-'
%start program

%right ')' ELSE

/* Grammar section */
%%

program
    : program stat
    |
;

stat
    : declaration
    | compound_stat
    | expression_stat
    | print_func
    | selection_stat
;

declaration
    : VAR ID type '=' initializer NEWLINE {
      if($3.type == FLOAT_t) f_insert_symbol($2.id, "float32");
      if($3.type == INT_t  ) i_insert_symbol($2.id, "int");

      fprintf(file, "fstore %d\n", lookup_symbol($2.id));

      f_flag = eq_flag = ne_flag = 0;
    }
    | VAR ID type NEWLINE {
      if($3.type == FLOAT_t) f_insert_symbol($2.id, "float32");
      if($3.type == INT_t  ) i_insert_symbol($2.id, "int");

      fprintf(file, "ldc 0.0\n");
      fprintf(file, "fstore %d\n", lookup_symbol($2.id));

      f_flag = eq_flag = ne_flag = 0;
    }
;

type
    : INT
    | FLOAT
    | VOID
;

initializer
    : equality_expr
;

compound_stat
    : '{' '}'
    | '{' NEWLINE block_item_list '}'
;

block_item_list
    : block_item
    | block_item_list block_item 
;

block_item
    : stat
;

selection_stat
    : IF '(' expr ')' if_stat ELSE {
      fprintf(file, "goto EXIT_%d\n", exit_lb);
      fprintf(file, "Label_%d:\n", label++);
    } stat {
      fprintf(file, "EXIT_%d:\n", exit_lb++);
    }
    | IF '(' expr ')' if_stat {
      fprintf(file, "Label_%d:\n", label++);
    }
;

if_stat
    : {
      fprintf(file, "f2i\n");
      if(eq_flag) fprintf(file, "ifne Label_%d\n", label);
      else if(ne_flag) fprintf(file, "ifeq Label_%d\n", label);
      else fprintf(file, "ifge Label_%d\n", label);
      eq_flag = ne_flag = 0;
    } stat
;

expression_stat
    : expr NEWLINE { f_flag = eq_flag = ne_flag = 0; }
    | NEWLINE
;

expr
    : equality_expr
    | ID '=' expr { fprintf(file, "fstore %d\n", lookup_symbol($1.id)); }
    | prefix_expr assignment_op { if($2.asgn == MODASGN_t) fprintf(file, "f2i\n"); } expr {
      if($2.asgn == ADDASGN_t) fprintf(file, "fadd\n");
      if($2.asgn == SUBASGN_t) fprintf(file, "fsub\n");
      if($2.asgn == MULASGN_t) fprintf(file, "fmul\n");
      if($2.asgn == DIVASGN_t) fprintf(file, "fdiv\n");
      if($2.asgn == MODASGN_t) {
        if(f_flag) yyerror("<ERROR> Modulation can't use with float number");
        else {
          fprintf(file, "f2i\n");
          fprintf(file, "irem\n");
          fprintf(file, "i2f\n");
        }
      }
      fprintf(file, "fstore %d\n", lookup_symbol($1.id));
    }
;

assignment_op
    : ADDASGN
    | SUBASGN
    | MULASGN
    | DIVASGN
    | MODASGN
;

equality_expr
    : relational_expr
    | equality_expr equality_op relational_expr {
      fprintf(file, "fsub\n");
      if($2.op == NE_t) ne_flag = 1;
      if($2.op == EQ_t) eq_flag = 1;
    }
;

equality_op
    : EQ
    | NE
;

relational_expr
    : additive_expr
    | relational_expr relational_op additive_expr {
      if($2.op == GT_t || $2.op == GE_t) fprintf(file, "swap\n");
      fprintf(file, "fsub\n");
      if($2.op == GE_t || $2.op == LE_t) {
        fprintf(file, "ldc 1.0\n");
        fprintf(file, "fsub\n");
      }
    }
;

relational_op
    : '<'
    | '>'
    | LE
    | GE
;

additive_expr
    : multiplicative_expr
    | additive_expr add_op multiplicative_expr {
      if($2.op == ADD_t) fprintf(file, "fadd\n");
      if($2.op == SUB_t) fprintf(file, "fsub\n");
    }
;

add_op
    : '+'
    | '-'

;

multiplicative_expr
    : prefix_expr
    | multiplicative_expr mul_op { if($2.asgn == MODASGN_t) fprintf(file, "f2i\n"); } prefix_expr {
      if($2.op == MUL_t) fprintf(file, "fmul\n");
      if($2.op == DIV_t) fprintf(file, "fdiv\n");
      if($2.op == MOD_t)  {
        if(f_flag) yyerror("<ERROR> Modulation can't use with float number");
        else {
          fprintf(file, "f2i\n");
          fprintf(file, "irem\n");
          fprintf(file, "i2f\n");
        }
      }
    }
;

mul_op
    : '*'
    | '/'
    | '%'
;

prefix_expr
    : postfix_expr
    | INC prefix_expr {
      fprintf(file, "ldc 1.0\n");
      fprintf(file, "fadd\n");
      fprintf(file, "fstore %d\n", lookup_symbol($2.id));
    }
    | DEC prefix_expr {
      fprintf(file, "ldc 1.0\n");
      fprintf(file, "fsub\n");
      fprintf(file, "fstore %d\n", lookup_symbol($2.id));
    }
;

postfix_expr
    : primary_expr
    | postfix_expr INC {
      fprintf(file, "ldc 1.0\n");
      fprintf(file, "fadd\n");
      fprintf(file, "fstore %d\n", lookup_symbol($1.id));
    }
    | postfix_expr DEC {
      fprintf(file, "ldc 1.0\n");
      fprintf(file, "fsub\n");
      fprintf(file, "fstore %d\n", lookup_symbol($1.id));
    }
;

primary_expr
    : ID {
      int idx = lookup_symbol($1.id);
      if(!strcmp("float32", sym[idx]->type)) f_flag = 1;
      fprintf(file, "fload %d\n", idx);
    }
    | constant
    | '(' expr ')'
;

constant
    : I_CONST { fprintf(file, "ldc %f\n", $1.f_val); }
    | F_CONST {
      fprintf(file, "ldc %f\n", $1.f_val);
      f_flag = 1;
    }
;

print_func
    : print_func_op '(' equality_expr ')' NEWLINE {
      if(!f_flag) fprintf(file, "f2i\n");

      fprintf(file, "getstatic java/lang/System/out Ljava/io/PrintStream;\nswap\n");
      if($1.func == PRINT_t) fprintf(file, "invokevirtual java/io/PrintStream/print(");
      if($1.func == PRINTLN_t) fprintf(file, "invokevirtual java/io/PrintStream/println("); 
      if(f_flag) fprintf(file, "F)V\n");
      else fprintf(file, "I)V\n");

      f_flag = eq_flag = ne_flag = 0;
    }
    | print_func_op '(' QUOTA STRING QUOTA ')' NEWLINE {
      fprintf(file, "ldc \"%s\"\n", $4.string);

      fprintf(file, "getstatic java/lang/System/out Ljava/io/PrintStream;\nswap\n");
      if($1.func == PRINT_t) fprintf(file, "invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
      if($1.func == PRINTLN_t) fprintf(file, "invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n"); 
    }
;

print_func_op
    : PRINT
    | PRINTLN
;

%%

/* C code section */

void create_symbol() {
  printf("Create a symbol table\n");
  int i;
  for(i = 0; i < MAXI; i++)
    sym[i] = (struct SYMBOL*)malloc(sizeof(struct SYMBOL));
}

void f_insert_symbol(char *a, char const *b) {
  printf("Insert a symbol: %s\n", a);
  SIZE++;
  strcpy(sym[SIZE]->id, a);
  strcpy(sym[SIZE]->type, b);
  //sym[SIZE]->f_num = c;
}

void i_insert_symbol(char *a, char const *b) {
  printf("Insert a symbol: %s\n", a);
  SIZE++;
  strcpy(sym[SIZE]->id, a);
  strcpy(sym[SIZE]->type, b);
  //sym[SIZE]->i_num = c;
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

int main(int argc, char** argv)
{
  file = fopen("Computer.j", "w");

  fprintf(file, ".class public main\n.super java/lang/Object\n.method public static main([Ljava/lang/String;)V\n.limit stack 10\n.limit locals 10\n");

  yylineno = 0;

  create_symbol();
  yyparse();

  fprintf(file, "\nreturn\n.end method\n");

  fclose(file);

  return 0;
}
