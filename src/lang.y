%{
    extern int yylex();
    extern void yyerror();
%}

%code requires { 
    #include "ast.h"
}
%parse-param { Node **result }


%union {
    Node *node;

    VarData *var_data;
    U32ValueData *u32_value_data;
    I32ValueData *i32_value_data;
    BoolValueData *bool_value_data;
    UnaryOpData *unary_op_data;
    BinaryOpData *binary_op_data;

    VarDeclData *var_decl_data;
    AssignData *assign_data;
    ForData *for_data;

    IfCase *if_case;
    IfCaseList *if_case_list;
    IfData *if_data;

    DataType data_type;

    StmtList *stmt_list;

    int32_t i32_value;
    uint8_t bool_value;
    char *string;
}


%type <node> expr stmt blk

%type <stmt_list> stmt_list

%type <var_data> expr_var
%type <i32_value_data> expr_i32
%type <bool_value_data> expr_bool
%type <unary_op_data> expr_unary_op
%type <binary_op_data> expr_binary_op

%type <var_decl_data> stmt_var_decl
%type <assign_data> stmt_assign
%type <for_data> stmt_for

%type <if_case> if_case
%type <if_case_list> if_case_list
%type <if_data> stmt_if

%left TOK_EQUAL TOK_NOT_EQUAL
%left '<' '>'
%left '+' '-'
%left '*' '/'
%left '&' '|' '^'
%left '!'

%token <string> TOK_VAR_NAME

%token <i32_value> TOK_BOOL_VALUE
%token <bool_value> TOK_I32_VALUE

%token TOK_IF_KEYWORD
%token TOK_ELIF_KEYWORD
%token TOK_ELSE_KEYWORD
%token TOK_FOR_KEYWORD

%token <data_type> TOK_DATA_TYPE


%%



blk:
    stmt { $$ = new_node(NODE_TYPE_BLK, new_blk_data(new_stmt_list($1, NULL))); *result = $$; }
|   '{' stmt_list '}' { $$ = new_node(NODE_TYPE_BLK, new_blk_data($2)); *result = $$; }
|   '{' '}' { $$ = new_node(NODE_TYPE_BLK, NULL); *result = $$; }
;

stmt_list:
    stmt { $$ = new_stmt_list($1, NULL); }
|   stmt stmt_list { $$ = new_stmt_list($1, $2); }
;


expr_var:
    TOK_VAR_NAME { $$ = new_var_data($1); }
;

expr_i32:
    TOK_I32_VALUE { $$ = new_i32_value_data($1); }
;

expr_bool:
    TOK_BOOL_VALUE { $$ = new_bool_value_data($1); }
;

expr_unary_op:
    '!' expr { $$ = new_unary_op_data(UNARY_OP_TYPE_NOT, $2); }
;

expr_binary_op:
    expr TOK_EQUAL expr { $$ = new_binary_op_data(BINARY_OP_TYPE_EQUAL, $1, $3); }
|   expr TOK_NOT_EQUAL expr { $$ = new_binary_op_data(BINARY_OP_TYPE_NOT_EQUAL, $1, $3); }
|   expr '<' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_LESS, $1, $3); }
|   expr '>' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_GREATER, $1, $3); }
|   expr '&' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_AND, $1, $3); }
|   expr '|' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_OR, $1, $3); }
|   expr '^' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_XOR, $1, $3); }
|   expr '+' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_ADD, $1, $3); }
|   expr '-' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_SUB, $1, $3); }
|   expr '*' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_MUL, $1, $3); }
|   expr '/' expr { $$ = new_binary_op_data(BINARY_OP_TYPE_DIV, $1, $3); }
;

expr: 
    expr_var { $$ = new_node(NODE_TYPE_EXPR, new_expr_data(EXPR_TYPE_VAR, $1)); }
|   expr_i32 { $$ = new_node(NODE_TYPE_EXPR, new_expr_data(EXPR_TYPE_I32_VALUE, $1)); }
|   expr_bool { $$ = new_node(NODE_TYPE_EXPR, new_expr_data(EXPR_TYPE_BOOL_VALUE, $1)); }
|   expr_unary_op { $$ = new_node(NODE_TYPE_EXPR, new_expr_data(EXPR_TYPE_UNARY_OP, $1)); }
|   expr_binary_op { $$ = new_node(NODE_TYPE_EXPR, new_expr_data(EXPR_TYPE_BINARY_OP, $1)); }
;



stmt_var_decl:
    TOK_DATA_TYPE TOK_VAR_NAME { $$ = new_var_decl_data($1, $2); }
;

stmt_assign:
    expr '=' expr { $$ = new_assign_data($1, $3); }
;


if_case:
    TOK_IF_KEYWORD '(' expr ')' blk { $$ = new_if_case($3, $5); }
|   TOK_ELIF_KEYWORD '(' expr ')' blk { $$ = new_if_case($3, $5); }
|   TOK_ELSE_KEYWORD blk { $$ = new_if_case(NULL, $2); }
;

if_case_list:
    if_case { $$ = new_if_case_list($1, NULL); }
|   if_case if_case_list { $$ = new_if_case_list($1, $2); }
;

stmt_if: 
    if_case_list { $$ = new_if_data($1); }
;


stmt_for: 
    TOK_FOR_KEYWORD '(' blk ',' expr ',' blk ')' blk 
    { 
        $$ = new_for_data($3, $5, $7, $9);
    }
;

stmt: 
    stmt_var_decl ';' { $$ = new_node(NODE_TYPE_STMT, new_stmt_data(STMT_TYPE_VAR_DECL, $1)); }
|   stmt_assign ';' { $$ = new_node(NODE_TYPE_STMT, new_stmt_data(STMT_TYPE_ASSIGN, $1)); }
|   stmt_if { $$ = new_node(NODE_TYPE_STMT, new_stmt_data(STMT_TYPE_IF, $1)); }
|   stmt_for { $$ = new_node(NODE_TYPE_STMT, new_stmt_data(STMT_TYPE_FOR, $1)); }
;



%%