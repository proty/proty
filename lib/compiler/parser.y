%{
#include "ast.h"
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
%left MUL DIV
%left ADD SUB
%left EQ NE GT LT LE GE
%left AND OR
%left NOT

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
%token LBRACE "}"
%token RBRACE "{"

// other symbols
%token COMMA ","
%token DOT "."
%token COLON ":"
%token SEMICOLON ";"
%token ARROW "=>"

%type <node> primary block statements unop binop expression
%type <node> if_stmt while_stmt

%%

program: statements EOS { *root = $1; }

block:          LBRACE block RBRACE { $$ = $2; }
              | statements END { $$ = $1; }
              ;

statements:     expression           { $$ = $1; }
              | expression NEWLINE statements { $$ = Node_new(BranchNode, $1, $3); }
              | expression SEMICOLON statements { $$ = Node_new(BranchNode, $1, $3); }
              | if_stmt              { $$ = $1; }
              | while_stmt           { $$ = $1; }
              | NEWLINE statements   { $$ = $2; }
              | SEMICOLON statements { $$ = $2; }
              ;

expression:     LPAR expression RPAR { $$ = $2; }
              | unop                 { $$ = $1; }
              | binop                { $$ = $1; }
              | primary              { $$ = $1; }
              ;

unop:           NOT expression       { $$ = Node_new(UnOpNode, 0, $2);
                                       $$->data.sval = "not"; }
              ;

binop:          expression ADD expression { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "+"; }

              | expression SUB expression { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "-"; }

              | expression MUL expression { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "*"; }

              | expression DIV expression { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "/"; }

              | expression EQ expression  { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "=="; }

              | expression NE expression  { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "!="; }

              | expression GT expression  { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = ">"; }

              | expression LT expression  { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "<"; }

              | expression GE expression  { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = ">="; }

              | expression LE expression  { $$ = Node_new(BinOpNode, $1, $3);
                                            $$->data.sval = "<="; }
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

if_stmt:        IF expression block { $$ = Node_new(IfNode, $2, $3); }
              | IF expression block ELSE block { Node* elseNode = Node_new(ElseNode, $3, $5);
                                                 $$ = Node_new(IfNode, $2, elseNode); }
              ;

while_stmt:     WHILE expression block
              ;

%%
