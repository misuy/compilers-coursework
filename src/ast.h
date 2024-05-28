#include <inttypes.h>
#include <stdlib.h>

typedef enum NodeType {
    NODE_TYPE_BLK,
    NODE_TYPE_EXPR,
    NODE_TYPE_STMT,
} NodeType;



typedef enum ExprType {
    EXPT_TYPE_VAR,
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

typedef struct IfData {
    Node *cond; // NODE_TYPE_EXPR
    Node *body; // NODE_TYPE_BLK
    struct IfData *next_if; // STMT_TYPE_IF
} IfData;

typedef struct ForData {
    Node *init; // NODE_TYPE_BLK
    Node *cond; // NODE_TYPE_EXPR
    Node *it; // NODE_TYPE_BLK
    Node *body; // NODE_TYPE_BLK
} ForData;



typedef struct BlkData {
    Node *stmt; // NODE_TYPE_STMT
    Node *next; // NODE_TYPE_BLK
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
IfData * new_if_data(Node *cond, Node *body, Node *next_if);
ForData * new_for_data(Node *init, Node *cond, Node *it, Node *body);
BlkData * new_blk_data(Node *stmt, Node *next);
