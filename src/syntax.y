%{
    #include "ast.h"
%}

%union {
    Node *node;
    ExprData *expr_data;
    VarData *var_data;
    U32ValueData *u32_value_data;
    I32ValueData *i32_value_data;
    BoolValueData *bool_value_data;
    UnaryOpData *unary_op_data;
    BinaryOpData *binary_op_data;
    StmtData *stmt_data;
    VarDeclData *var_decl_data;
    AssignData *assign_data;
    IfData *if_data;
    ForData *for_data;
    BlkData *blk_data;
}

%type <var_data> expr_var
%type <i32_value_data> expr_i32
%type <bool_value_data> expr_bool
%type <unary_op_data> expr_unary_op
%type <if_data> stmt_if
%type <for_data> stmt_for

%token TOK_EQUAL
%token TOK_VAR_NAME
%token TOK_DATA_TYPE
%token TOK_INTEGER
%token TOK_BOOL
%token TOK_IF_KEYWORD
%token TOK_ELIF_KEYWORD
%token TOK_ELSE_KEYWORD
%token TOK_FOR_KEYWORD

%%

blk:
|   blk stmt
|   '{' blk '}'
;

expr_var:
    TOK_VAR_NAME { $$ = new_var_data($1); }
;

expr_i32:
    TOK_INTEGER { $$ = new_i32_value_data($1); }
;

expr_bool:
    TOK_BOOL { $$ = new_bool_value_data($1); }
;

expr_unary_op:
    '!' expr
    {
        
    }
;

expr_binary_op:
    expr TOK_EQUAL expr

expr: ;

stmt_if: 
    TOK_IF_KEYWORD '(' expr ')' blk
    { 
        $$ = new_if_data($2, $4, NULL);
    }
|   stmt_if TOK_ELIF_KEYWORD '(' expr ')' blk 
    {
        $$ = new_if_data($4, $6, NULL);
        $1->next_if = $$;
    }
|   stmt_if TOK_ELSE_KEYWORD blk
    {
        $$ = new_if_data(NULL, $3, NULL);
        $1->next_if = $$;
    }
;

stmt_for: TOK_FOR_KEYWORD '(' blk ',' expr ',' blk ')' blk { $$ = new_for_data(); }

stmt: TOK_IF_KEYWORD '(' blk ',' expr ',' blk ')' '{' blk '}'

%%