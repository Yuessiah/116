%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#define MAXI 1000
	#define MAXS 100

	void create_symbol();
	void insert_symbol();
	int lookup_symbol();
	void dump_symbol();

	struct idmap {
		char id[MAXS], type[MAXS];
	} *sym[MAXI];

	int num_line = 0, num_comment = 0, SIZE = 0;
%}

letter     [a-zA-Z]
digit      [0-9]
id         {letter}+({letter}|{digit})*
type       int|float32
integer    {digit}+
float      {integer}\.{integer}
string     \"[^"\n]*\"
arithmetic [+-*/%]|++|--
relation   [<>]={0,1}|==|!=
assignment [+-*/%]{0,1}=
logical    &&|\|\||!
blank      [\r\t\f\v ]
cppcomment \/\/[^\n]*

%x CCOMMENT
%x DECLARE

%%

"/*" { BEGIN(CCOMMENT); ECHO; }
<CCOMMENT>[^*\n]*|\*+[^/\n]* { ECHO; }
<CCOMMENT>\n { num_line++; num_comment++; ECHO; }
<CCOMMENT>\*+\/ { num_comment++; ECHO; putchar('\n'); BEGIN(INITIAL); }
{cppcomment} { num_comment++; ECHO; putchar('\n'); }


"var " { BEGIN(DECLARE); }
<DECLARE>{id}{blank}{type} {
	char id[MAXS], type[MAXS];
	sscanf(yytext, "%s %s", id, type);

	if(lookup_symbol(id)) fprintf(stderr, "%-10s Redefined variable!\n", id);
	else {
		printf("%-10s %s TYPE VAR\n", id, type);
		insert_symbol(id);
		strcpy(sym[SIZE]->type, strdup(type));
	}
	BEGIN(INITIAL);
}

"("          { printf("%-10s LB \n", yytext); }
")"          { printf("%-10s RB \n", yytext); }
"{"          { printf("%-10s LCB \n", yytext); }
"}"          { printf("%-10s RCB \n", yytext); }
"\""         { printf("%-10s Quotation \n", yytext); }
"\n"         { num_line++; }
{blank}      {;}
"+"          { printf("%-10s ADD \n", yytext); }
"-"          { printf("%-10s SUB \n", yytext); }
"*"          { printf("%-10s MUL \n", yytext); }
"/"          { printf("%-10s DIV \n", yytext); }
"%"          { printf("%-10s MOD \n", yytext); }
"++"         { printf("%-10s Increment \n", yytext); }
"--"         { printf("%-10s Decrement \n", yytext); }
"<"          { printf("%-10s Less than \n", yytext); }
">"          { printf("%-10s Greater than \n", yytext); }
"<="         { printf("%-10s Less than or Equal to \n", yytext); }
">="         { printf("%-10s Greater than or Equal to \n", yytext); }
"=="         { printf("%-10s Equal \n", yytext); }
"!="         { printf("%-10s Not Equel \n", yytext); }
"="          { printf("%-10s Assignment \n", yytext); }
"+="         { printf("%-10s Addition Assignment \n", yytext); }
"-="         { printf("%-10s Subtraction Assignment \n", yytext); }
"*="         { printf("%-10s Multiplication Assignment \n", yytext); }
"/="         { printf("%-10s Division Assignment \n", yytext); }
"%="         { printf("%-10s modulo Assignment \n", yytext); }
"&&"         { printf("%-10s Conjunction \n", yytext); }
"||"         { printf("%-10s Disjunction \n", yytext); }
"!"          { printf("%-10s NOT \n", yytext); }
"print"      { printf("%-10s PRINT \n", yytext); }
"println"    { printf("%-10s PRINTLN \n", yytext); }
"if"         { printf("%-10s IF \n", yytext); }
"else"       { printf("%-10s ELSE \n", yytext); }
"for"        { printf("%-10s FOR \n", yytext); }
{string}     { printf("%-10s String \n", yytext); }
{integer}    { printf("%-10s Integer \n", yytext); }
{float}      { printf("%-10s Float \n", yytext); }

{id} {
	if(lookup_symbol(yytext)) printf("%-10s ID \n", yytext);
	else fprintf(stderr, "%-10s Undeclared variable!\n", yytext);
}

%%

int yywrap(void) {
	return 1;
}

void create_symbol() {
	printf("Create a symbol table\n");
	int i; for(i = 0; i < MAXI; i++)
		sym[i] = (struct idmap*)malloc(sizeof(struct idmap));
}

void insert_symbol(char *a) {
	printf("Insert a symbol: %s\n", a);
	SIZE++;
	strcpy(sym[SIZE]->id, a);
	strcpy(sym[SIZE]->type, "void");
}

int lookup_symbol(char *a) {
	int i; for(i = 1; i <= SIZE; i++)
		if(!strcmp(sym[i]->id, a)) return i;
	return 0;
}
	
void dump_symbol() {
	int i; for(i = 1; i <= SIZE; i++)
		printf("%-10d %-10s %-10s\n", i, sym[i]->id, sym[i]->type);
}

int main(int argc,char *argv[])
{
	create_symbol();

	yyin = fopen(argv[1],"r");
	yylex();

	dump_symbol();

	printf("Number of line(s): %d\n", num_line);
	printf("Number of comment(s): %d\n", num_comment);

	return 0;
}
