%{
#include <stdio.h>
#include "node.h"
#include "parser.h"
int yyerror(void* yyloc, void* scanner, Node** root, const char* msg);
int yylex(void* yylval_param, void* loc, void* scanner);
%}

%locations
%error-verbose
%pure_parser
%parse-param {void* scanner}
%parse-param {Node** root}
%lex-param {yyscan_t* scanner}
%defines

%code provides {
    int yyparse(void* scanner, Node** root);
}

%union {
    int ival;
    double dval;
    const char* sval;
    Node* node;
};

%token EOS 0 "end of stream"
%token NEWLINE "new line"
%token UNDEF "unknown token"

// operators
%left ASSIGN IADD ISUB IMUL IDIV
%left LPAR
%left NOT
%left AND OR
%left EQ NE GT LT LE GE
%left ADD SUB
%left MUL DIV

// primitive data types
%token <ival> INTEGER "integer"
%token <dval> DECIMAL "decimal"
%token <sval> STRING "string"
%token <sval> NAME "name"

// keywords
%token DO "do"
%token IF "if"
%token ELSE "else"
%token WHILE "while"
%token END "end"
%token TRY "try"
%token CATCH "catch"

// braces
%token LPAR "("
%token RPAR ")"
%token LSQB "["
%token RSQB "]"
%token LBRACE "{"
%token RBRACE "}"

// other symbols
%token COMMA ","
%token DOT "."
%token COLON ":"
%token SEMICOLON ";"
%token ARROW "=>"

%right DOT

%type <node> primary block statements statement expression
%type <node> if_stmt while_stmt unop binop args func_args

%%

program:        statements EOS { *root = $1; }

block:          LBRACE block RBRACE { $$ = $2; }
              | statements { $$ = $1; }
              ;

statements:     { $$ = 0; }
              | NEWLINE statements { $$ = $2; }
              | statement statements { $$ = Node_new(BranchNode, $1, $2); }
              ;

statement:      expression { $$ = $1; }
              | if_stmt    { $$ = $1; }
              | while_stmt { $$ = $1; }
              ;

expression:     LPAR expression RPAR { $$ = $2; }
              | unop                 { $$ = $1; }
              | binop                { $$ = $1; }
              | primary              { $$ = $1; }
              | expression ASSIGN expression { $$ = AssignNode_new($1, $3); }
              | expression DOT NAME { $$ = GetSlotNode_new($1, $3); }
              | expression DOT NAME ASSIGN expression { $$ = SetSlotNode_new($1, $5, $3); }
              | expression DOT NAME LPAR args RPAR { $$ = SendNode_new($1, $5, $3); }
              | expression LPAR args RPAR { $$ = Node_new(CallNode, $1, $3); }
              | DO LPAR func_args RPAR block END { $$ = Node_new(DoNode, $3, $5); }
              ;

args:           { $$ = Node_new(ArgsNode, 0, 0); }
              | expression { $$ = Node_new(ArgsNode, $1, 0); }
              | expression COMMA args { $$ = Node_new(ArgsNode, $1, $3); }
              ;

func_args:      { $$ = 0; }
              | NAME { $$ = Node_new(FuncArgsNode, 0, 0); $$->data.sval = $1; }
              | NAME COMMA func_args { $$ = Node_new(FuncArgsNode, 0, $3); $$->data.sval = $1; }
              ;

unop:           NOT expression       { $$ = Node_new(UnOpNode, 0, $2);
                                       $$->data.sval = "not"; }
              ;

binop:          expression ADD expression  { $$ = BinOpNode_new($1, $3, "+"); }
              | expression SUB expression  { $$ = BinOpNode_new($1, $3, "-"); }
              | expression MUL expression  { $$ = BinOpNode_new($1, $3, "*"); }
              | expression DIV expression  { $$ = BinOpNode_new($1, $3, "/"); }
              | expression EQ expression   { $$ = BinOpNode_new($1, $3, "=="); }
              | expression NE expression   { $$ = BinOpNode_new($1, $3, "!="); }
              | expression GT expression   { $$ = BinOpNode_new($1, $3, ">"); }
              | expression LT expression   { $$ = BinOpNode_new($1, $3, "<"); }
              | expression GE expression   { $$ = BinOpNode_new($1, $3, ">="); }
              | expression LE expression   { $$ = BinOpNode_new($1, $3, "<="); }
              | expression IADD expression { $$ = BinOpNode_new($1, $3, "+="); }
              | expression ISUB expression { $$ = BinOpNode_new($1, $3, "-="); }
              | expression IMUL expression { $$ = BinOpNode_new($1, $3, "*="); }
              | expression IDIV expression { $$ = BinOpNode_new($1, $3, "/="); }
              ;

primary:        INTEGER { $$ = Node_new(IntegerNode, 0, 0);
                          $$->data.ival = $1; }

              | DECIMAL { $$ = Node_new(FloatNode, 0, 0);
                          $$->data.dval = $1; }

              | STRING  { $$ = Node_new(StringNode, 0, 0);
                          $$->data.sval = $1; }

              | NAME    { $$ = Node_new(NameNode, 0, 0);
                          $$->data.sval = $1; }

              | COLON NAME { $$ = Node_new(SymbolNode, 0, 0);
                             $$->data.sval = $2; }
              ;

if_stmt:        IF expression block END { $$ = Node_new(IfNode, $2, $3); }
              | IF expression block ELSE block END { $$ = Node_new(IfNode, $2,
                                                      Node_new(ElseNode, $3, $5)); }
              ;

while_stmt:     WHILE expression block END { $$ = Node_new(WhileNode, $2, $3); }
              ;

%%
