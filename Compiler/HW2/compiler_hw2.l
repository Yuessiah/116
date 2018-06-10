%{
#include "y.tab.h"

#define MAXI 1000
#define MAXS 100

extern YYSTYPE yylval;
extern struct idmap {
	char id[100], type[100];
	int i_num;
	double f_num;
} *sym[1000];

extern int num_line;

extern void yyerror(char const *);
extern int lookup_symbol(char *);
%}

letter     [a-zA-Z]
digit      [0-9]
id         {letter}+({letter}|{digit})*
type       int|float32
integer    {digit}+
float      {integer}\.{integer}
string     \"[^"\n]*\"
relational [<>]={0,1}|==|!=
assignment [-+*/%]{0,1}=
blank      [ \r\t\f\v]
cppcomment \/\/[^\n]*

%option yylineno
%x CCOMMENT

%%

"/*" { BEGIN(CCOMMENT); }
<CCOMMENT>[^*\n]*|\*+[^/\n]* {;}
<CCOMMENT>\n { num_line++; return NEWLINE; }
<CCOMMENT>\*+\/ { BEGIN(INITIAL); }
{cppcomment} { num_line++; }

"print"   { return PRINTLN; }
"println" { return PRINT; }

"if"      { return IF; }
"else"    { return ELSE; }
"for"     { return FOR; }

"var"     { return VAR; }
"void"    { yylval.string = strdup(yytext); return VOID; }
"int"     { yylval.string = strdup(yytext); return INT; }
"float32" { yylval.string = strdup(yytext); return FLOAT32; }

{float}   { yylval.f_val = atof(yytext); return F_CONST; }
{integer} { yylval.i_val = atoi(yytext); return I_CONST; }

{id}     { yylval.string = strdup(yytext); return ID; }
{string} { yylval.string = strdup(yytext); return STRING; }

"++" { return INCRE; }
"--" { return DECRE; }
[-+*/%();]   { return yytext[0]; }

"<=" { return LEQ; }
">=" { return GEQ; }
"==" { return EQ; }
"!=" { return NEQ; }
[<>] { return yytext[0]; }

"&&" { return L_AND; }
"||" { return L_OR; }

{assignment} { return *yytext; }

"\n"       { num_line++; return NEWLINE; }
{blank}    {;}
<<EOF>>    { return END_OF_FILE; }
.          { ECHO; yyerror("unexpected character"); }

%%

int yywrap(void) { return 1; }
