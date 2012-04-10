%{
#include <stdio.h>
#include "compiler/node.h"
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
%expect 1

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
%left ASSIGN
%left DOT
%left IADD ISUB IMUL IDIV
%left LPAR LSQB
%left AND OR
%left NOT
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

%type <node> primary stmts cmpd_stmt stmt expr subscript
%type <node> if_stmt while_stmt try_stmt unop binop slotop args
%type <node> do_args hash_args list hash hash_element

%%

program       : stmts EOS { *root = $1; }

stmts         : cmpd_stmt opt_terms
              ;

cmpd_stmt     : { $$ = 0; }
              | stmt { $$ = $1; }
              | cmpd_stmt terms stmt { $$ = Node_new(BranchNode, $1, $3); }
              ;

opt_terms     :
              | terms
              ;

terms         : term
              | terms term
              ;

term          : NEWLINE
              | SEMICOLON
              ;

stmt          : expr       { $$ = $1; }
              | if_stmt    { $$ = $1; }
              | while_stmt { $$ = $1; }
              | try_stmt   { $$ = $1; }
              ;

expr          : LPAR expr RPAR { $$ = $2; }
              | primary        { $$ = $1; }
              | unop           { $$ = $1; }
              | binop          { $$ = $1; }
              | slotop         { $$ = $1; }
              | subscript      { $$ = $1; }
              | NAME ASSIGN expr { $$ = AssignNode_new($1, $3); }
              | expr LPAR args RPAR { $$ = Node_new(CallNode, $1, $3); }
              | DO do_args COLON stmts END { $$ = Node_new(DoNode, $2, $4); }
              | DO do_args LBRACE stmts RBRACE { $$ = Node_new(DoNode, $2, $4); }
              ;

slotop        : expr DOT NAME { $$ = GetSlotNode_new($1, $3); }
              | expr DOT NAME ASSIGN expr { $$ = SetSlotNode_new($1, $5, $3); }
              | expr DOT NAME LPAR args RPAR { $$ = SendNode_new($1, $5, $3); }
              ;

subscript     : expr LSQB expr RSQB { $$ = SubscriptNode_new($1, $3, 0); }
              | expr LSQB expr RSQB ASSIGN expr { $$ = SubscriptNode_new($1, $3, $6); }
              ;

args          : { $$ = Node_new(ArgsNode, 0, 0); }
              | expr { $$ = Node_new(ArgsNode, $1, 0); }
              | expr COMMA args { $$ = Node_new(ArgsNode, $1, $3); }
              ;

do_args       : { $$ = 0; }
              | NAME { $$ = Node_new(DoArgsNode, 0, 0); $$->data.sval = $1; }
              | NAME COMMA do_args { $$ = Node_new(DoArgsNode, 0, $3); $$->data.sval = $1; }
              ;

unop          : NOT expr { $$ = UnOpNode_new($2, "not"); }
              ;

binop         : expr ADD expr  { $$ = BinOpNode_new($1, $3, "+"); }
              | expr SUB expr  { $$ = BinOpNode_new($1, $3, "-"); }
              | expr MUL expr  { $$ = BinOpNode_new($1, $3, "*"); }
              | expr DIV expr  { $$ = BinOpNode_new($1, $3, "/"); }
              | expr EQ expr   { $$ = BinOpNode_new($1, $3, "=="); }
              | expr NE expr   { $$ = BinOpNode_new($1, $3, "!="); }
              | expr GT expr   { $$ = BinOpNode_new($1, $3, ">"); }
              | expr LT expr   { $$ = BinOpNode_new($1, $3, "<"); }
              | expr GE expr   { $$ = BinOpNode_new($1, $3, ">="); }
              | expr LE expr   { $$ = BinOpNode_new($1, $3, "<="); }
              | expr IADD expr { $$ = BinOpNode_new($1, $3, "+="); }
              | expr ISUB expr { $$ = BinOpNode_new($1, $3, "-="); }
              | expr IMUL expr { $$ = BinOpNode_new($1, $3, "*="); }
              | expr IDIV expr { $$ = BinOpNode_new($1, $3, "/="); }
              | expr AND expr  { $$ = BinOpNode_new($1, $3, "and"); }
              | expr OR expr   { $$ = BinOpNode_new($1, $3, "or"); }
              ;

primary       : INTEGER { $$ = Node_new(IntegerNode, 0, 0); $$->data.ival = $1; }
              | DECIMAL { $$ = Node_new(FloatNode, 0, 0); $$->data.dval = $1; }
              | STRING  { $$ = Node_new(StringNode, 0, 0); $$->data.sval = $1; }
              | NAME    { $$ = Node_new(NameNode, 0, 0); $$->data.sval = $1; }
              | COLON NAME { $$ = Node_new(SymbolNode, 0, 0); $$->data.sval = $2; }
              | list { $$ = $1; }
              | hash { $$ = $1; }
              ;

list          : LSQB args RSQB { $$ = Node_new(ListNode, $2, 0); }
              ;

hash          : LBRACE hash_args RBRACE { $$ = Node_new(HashNode, $2, 0); }
              ;

hash_args     : { $$ = Node_new(HashArgsNode, 0, 0); }
              | hash_element { $$ = Node_new(HashArgsNode, $1, 0); }
              | hash_element COMMA hash_args { $$ = Node_new(HashArgsNode, $1, $3); }
              ;

hash_element  : expr ARROW expr { $$ = Node_new(HashElementNode, $1, $3); }
              ;


if_stmt       : IF expr term stmts END { $$ = Node_new(IfNode, $2, $4); }
              | IF expr term stmts ELSE stmts END
                   { $$ = Node_new(IfNode, $2, Node_new(ElseNode, $4, $6)); }
              ;

while_stmt    : WHILE expr term stmts END { $$ = Node_new(WhileNode, $2, $4); }
              ;

try_stmt      : TRY term stmts CATCH stmts END { $$ = Node_new(TryNode, $3, $5); }
              ;

%%
