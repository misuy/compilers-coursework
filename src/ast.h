#ifndef _AST_H
#define _AST_H

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>


#define REC_SHIFT 3


typedef enum NodeType {
    NODE_TYPE_BLK,
    NODE_TYPE_EXPR,
    NODE_TYPE_STMT,
} NodeType;



typedef enum ExprType {
    EXPR_TYPE_VAR,
    EXPR_TYPE_U32_VALUE,
    EXPR_TYPE_I32_VALUE,
    EXPR_TYPE_BOOL_VALUE,
    EXPR_TYPE_UNARY_OP,
    EXPR_TYPE_BINARY_OP,
} ExprType;

typedef enum UnaryOpType {
    UNARY_OP_TYPE_NOT,
} UnaryOpType;

typedef enum BinaryOpType {
    BINARY_OP_TYPE_EQUAL,
    BINARY_OP_TYPE_NOT_EQUAL,
    BINARY_OP_TYPE_LESS,
    BINARY_OP_TYPE_GREATER,
    BINARY_OP_TYPE_AND,
    BINARY_OP_TYPE_OR,
    BINARY_OP_TYPE_XOR,
    BINARY_OP_TYPE_ADD,
    BINARY_OP_TYPE_SUB,
    BINARY_OP_TYPE_MUL,
    BINARY_OP_TYPE_DIV,
} BinaryOpType;



typedef enum StmtType {
    STMT_TYPE_VAR_DECL,
    STMT_TYPE_ASSIGN,
    STMT_TYPE_IF,
    STMT_TYPE_FOR,
} StmtType;

typedef enum DataType {
    DATA_TYPE_U32,
    DATA_TYPE_I32,
    DATA_TYPE_BOOL,
} DataType;




typedef struct Node {
    NodeType type;
    void *data;
} Node;



typedef struct ExprData {
    ExprType type;
    void *data;
} ExprData;

typedef struct VarData {
    char *name;
} VarData;

typedef struct U32ValueData {
    uint32_t value;
} U32ValueData;

typedef struct I32ValueData {
    int32_t value;
} I32ValueData;

typedef struct BoolValueData {
    uint8_t value;
} BoolValueData;

typedef struct UnaryOpData {
    UnaryOpType type;
    Node *operand; // NODE_TYPE_EXPR
} UnaryOpData;

typedef struct BinaryOpData {
    BinaryOpType type;
    Node *left_operand; // NODE_TYPE_EXPR
    Node *right_operand; // NODE_TYPE_EXPR
} BinaryOpData;



typedef struct StmtData {
    StmtType type;
    void *data;
} StmtData;

typedef struct VarDeclData {
    DataType type;
    char *name;
} VarDeclData;

typedef struct AssignData {
    Node *var; // NODE_TYPE_STMT(STMT_TYPE_VAR_DECL) | NODE_TYPE_EXPR(EXPR_TYPE_VAR)
    Node *expr; // NODE_TYPE_EXPR
} AssignData;

typedef struct IfCase {
    Node *cond; // NODE_TYPE_EXPR
    Node *body; // NODE_TYPE_BLK
} IfCase;

typedef struct IfCaseList {
    IfCase *if_case;
    struct IfCaseList *next;
} IfCaseList;

typedef struct IfData {
    IfCaseList *if_case_list;
} IfData;

typedef struct ForData {
    Node *init; // NODE_TYPE_BLK
    Node *cond; // NODE_TYPE_EXPR
    Node *it; // NODE_TYPE_BLK
    Node *body; // NODE_TYPE_BLK
} ForData;



typedef struct StmtList {
    Node *stmt; // NODE_TYPE_STMT
    struct StmtList *next;
} StmtList;

typedef struct BlkData {
    StmtList *stmt_list;
} BlkData;




Node * new_node(NodeType type, void *data);
ExprData * new_expr_data(ExprType type, void *data);
VarData * new_var_data(char *name);
U32ValueData * new_u32_value_data(uint32_t value);
I32ValueData * new_i32_value_data(int32_t value);
BoolValueData * new_bool_value_data(uint8_t value);
UnaryOpData * new_unary_op_data(UnaryOpType type, Node *operand);
BinaryOpData * new_binary_op_data(BinaryOpType type, Node *left_operand, Node *right_operand);
StmtData * new_stmt_data(StmtType type, void *data);
VarDeclData * new_var_decl_data(DataType type, char *name);
AssignData * new_assign_data(Node *var, Node *expr);

IfCase * new_if_case(Node *cond, Node *body);
IfCaseList * new_if_case_list(IfCase *if_case, IfCaseList *next);
IfData * new_if_data(IfCaseList *if_case_list);

ForData * new_for_data(Node *init, Node *cond, Node *it, Node *body);

StmtList * new_stmt_list(Node *stmt, StmtList *next);
BlkData * new_blk_data(StmtList *stmt_list);


void print_node(FILE *out, Node *node, uint32_t rec);


#endif