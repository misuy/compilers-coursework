#include "ast.h"

Node * new_node(NodeType type, void *data) {
    Node *ret = malloc(sizeof(Node));
    *ret = (Node) { .type = type, .data = data };
    return ret;
}

ExprData * new_expr_data(ExprType type, void *data) {
    ExprData *ret = malloc(sizeof(ExprData));
    *ret = (ExprData) { .type = type, .data = data };
    return ret;
}

VarData * new_var_data(char *name) {
    VarData *ret = malloc(sizeof(VarData));
    *ret = (VarData) { .name = name };
    return ret;
}

U32ValueData * new_u32_value_data(uint32_t value) {
    U32ValueData *ret = malloc(sizeof(U32ValueData));
    *ret = (U32ValueData) { .value = value };
    return ret;
}

I32ValueData * new_i32_value_data(int32_t value) {
    I32ValueData *ret = malloc(sizeof(I32ValueData));
    *ret = (I32ValueData) { .value = value };
    return ret;
}

BoolValueData * new_bool_value_data(uint8_t value) {
    BoolValueData *ret = malloc(sizeof(BoolValueData));
    *ret = (BoolValueData) { .value = value };
    return ret;
}

UnaryOpData * new_unary_op_data(UnaryOpType type, Node *operand) {
    UnaryOpData *ret = malloc(sizeof(UnaryOpData));
    *ret = (UnaryOpData) { .type = type, .operand = operand };
    return ret;
}

BinaryOpData * new_binary_op_data(BinaryOpType type, Node *left_operand, Node *right_operand) {
    BinaryOpData *ret = malloc(sizeof(BinaryOpData));
    *ret = (BinaryOpData) { .type = type, .left_operand = left_operand, .right_operand = right_operand };
    return ret;
}

StmtData * new_stmt_data(StmtType type, void *data) {
    StmtData *ret = malloc(sizeof(StmtData));
    *ret = (StmtData) { .type = type, .data = data };
    return ret;
}

VarDeclData * new_var_decl_data(DataType type, char *name) {
    VarDeclData *ret = malloc(sizeof(VarDeclData));
    *ret = (VarDeclData) { .type = type, .name = name };
    return ret;
}

AssignData * new_assign_data(Node *var, Node *expr) {
    AssignData *ret = malloc(sizeof(AssignData));
    *ret = (AssignData) { .var = var, .expr = expr };
    return ret;
}

IfData * new_if_data(Node *cond, Node *body, IfData *next_if) {
    IfData *ret = malloc(sizeof(IfData));
    *ret = (IfData) { .cond = cond, .body = body, .next_if = next_if };
    return ret;
}

ForData * new_for_data(Node *init, Node *cond, Node *it, Node *body) {
    ForData *ret = malloc(sizeof(ForData));
    *ret = (ForData) { .init = init, .cond = cond, .it = it, .body = body };
    return ret;
}

BlkData * new_blk_data(Node *stmt, Node *next) {
    BlkData *ret = malloc(sizeof(BlkData));
    *ret = (BlkData) { .stmt = stmt, .next = next };
    return ret;
}
