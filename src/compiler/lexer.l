%{
#include <stdlib.h>
#include "compiler/node.h"
#include "parser.h"
#define YY_USER_ACTION yylloc->last_column += yyleng;
%}

%option noyywrap
%option nounput
%option reentrant bison-bridge
%option batch
%option yylineno
%option bison-locations
%option stack
%option never-interactive

%x COMMENT

%%

"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }

"=="    { return EQ; }
"!="    { return NE; }
">"     { return GT; }
"<"     { return LT; }
">="    { return GE; }
"<="    { return LE; }

"="     { return ASSIGN; }
"+="    { return IADD; }
"-="    { return ISUB; }
"*="    { return IMUL; }
"/="    { return IDIV; }

"and"   { return AND; }
"or"    { return OR; }
"not"   { return NOT; }

"do"    { return DO; }
"if"    { return IF; }
"else"  { return ELSE; }
"while" { return WHILE; }
"end"   { return END; }
"try"   { return TRY; }
"catch" { return CATCH; }

"("     { return LPAR; }
")"     { return RPAR; }
"["     { return LSQB; }
"]"     { return RSQB; }
"{"     { return LBRACE; }
"}"     { return RBRACE; }

","     { return COMMA; }
"."     { return DOT; }
":"     { return COLON; }
";"     { return SEMICOLON; }
"=>"    { return ARROW; }

[0-9]+\.[0-9]+         { yylval->dval = atof(yytext); return DECIMAL; }
[0-9]+                 { yylval->ival = atoi(yytext); return INTEGER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval->sval = strdup(yytext); return NAME; }
\"([^"\\]|\\.)*\"      { yylval->sval = strndup(yytext+1, strlen(yytext)-2); return STRING; }

"/*"              { BEGIN(COMMENT); }
<COMMENT>\n       { yylloc->last_line += yyleng;
                    yylloc->last_column = 1; return NEWLINE; }
<COMMENT>"*"+"/"  { BEGIN(INITIAL); }
<COMMENT>.*       { }

"#"(.*)(\n)? { yylloc->last_line += 1; return NEWLINE; }

\n      { yylloc->last_line += yyleng;
          yylloc->last_column = 1; return NEWLINE; }

[ \t]+  { /* ignore spaces */ }
.       { return UNDEF; }

%%

int yyerror(YYLTYPE* loc, void* scanner, Node** root, const char* p) {
    fprintf(stderr, "Error at %i.%i:\n  %s\n", loc->last_line,
            loc->last_column, p);

    if (*root) Node_delete(*root);
    return 0;
}
