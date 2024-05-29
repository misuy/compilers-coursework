#include "ast.h"

// constructors

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


IfCase * new_if_case(Node *cond, Node *body) {
    IfCase *ret = malloc(sizeof(IfCase));
    *ret = (IfCase) { .cond = cond, .body = body };
    return ret;
}

IfCaseList * new_if_case_list(IfCase *if_case, IfCaseList *next) {
    IfCaseList *ret = malloc(sizeof(IfCaseList));
    *ret = (IfCaseList) { .if_case = if_case, .next = next };
    return ret;
}

IfData * new_if_data(IfCaseList *if_case_list) {
    IfData *ret = malloc(sizeof(IfData));
    *ret = (IfData) { .if_case_list = if_case_list };
    return ret;
}


ForData * new_for_data(Node *init, Node *cond, Node *it, Node *body) {
    ForData *ret = malloc(sizeof(ForData));
    *ret = (ForData) { .init = init, .cond = cond, .it = it, .body = body };
    return ret;
}


StmtList * new_stmt_list(Node *stmt, StmtList *next) {
    StmtList *ret = malloc(sizeof(StmtList));
    *ret = (StmtList) { .stmt = stmt, .next = next };
    return ret;
}

BlkData * new_blk_data(StmtList *stmt_list) {
    BlkData *ret = malloc(sizeof(BlkData));
    *ret = (BlkData) { .stmt_list = stmt_list };
    return ret;
}


// print functions


void print_spaces(FILE *out, uint32_t count) {
    for (uint32_t i=0; i<count; i++) {
        fprintf(out, " ");
    }
}



void print_var_data(FILE *out, VarData *var_data, uint32_t rec) {
    if (!var_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "var_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "name: %s\n", var_data->name);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_u32_value_data(FILE *out, U32ValueData *u32_value_data, uint32_t rec) {
    if (!u32_value_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "u32_value_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "value: %" PRIu32 "\n", u32_value_data->value);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_i32_value_data(FILE *out, I32ValueData *i32_value_data, uint32_t rec) {
    if (!i32_value_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "i32_value_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "value: %" PRIi32 "\n", i32_value_data->value);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_bool_value_data(FILE *out, BoolValueData *bool_value_data, uint32_t rec) {
    if (!bool_value_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "bool_value_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "value: %" PRIu8 "\n", bool_value_data->value);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_unary_op_data(FILE *out, UnaryOpData *unary_op_data, uint32_t rec) {
    if (!unary_op_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "unary_op_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "type: ");
    switch (unary_op_data->type) {
        case UNARY_OP_TYPE_NOT:
            fprintf(out, "NOT(!)\n");
            break;
        default:
            fprintf(out, "UNKNOWN(?)\n");
            break;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "operand:\n");
    print_node(out, unary_op_data->operand, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_binary_op_data(FILE *out, BinaryOpData *binary_op_data, uint32_t rec) {
    if (!binary_op_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "binary_op_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "type: ");
    switch (binary_op_data->type) {
        case BINARY_OP_TYPE_EQUAL:
            fprintf(out, "EQUAL(==)\n");
            break;
        case BINARY_OP_TYPE_NOT_EQUAL:
            fprintf(out, "NOT_EQUAL(!=)\n");
            break;
        case BINARY_OP_TYPE_LESS:
            fprintf(out, "LESS(<)\n");
            break;
        case BINARY_OP_TYPE_GREATER:
            fprintf(out, "GREATER(>)\n");
            break;
        case BINARY_OP_TYPE_AND:
            fprintf(out, "AND(&)\n");
            break;
        case BINARY_OP_TYPE_OR:
            fprintf(out, "OR(|)\n");
            break;
        case BINARY_OP_TYPE_XOR:
            fprintf(out, "XOR(^)\n");
            break;
        case BINARY_OP_TYPE_ADD:
            fprintf(out, "ADD(+)\n");
            break;
        case BINARY_OP_TYPE_SUB:
            fprintf(out, "SUB(-)\n");
            break;
        case BINARY_OP_TYPE_MUL:
            fprintf(out, "MUL(*)\n");
            break;
        case BINARY_OP_TYPE_DIV:
            fprintf(out, "DIV(/)\n");
            break;
        default:
            fprintf(out, "UNKNOWN(?)\n");
            break;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "left_operand:\n");
    print_node(out, binary_op_data->left_operand, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "right_operand:\n");
    print_node(out, binary_op_data->right_operand, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_expr_type(FILE *out, ExprType expr_type) {
    switch (expr_type)
    {
        case EXPR_TYPE_VAR:
            printf("VAR");
            break;

        case EXPR_TYPE_U32_VALUE:
            printf("U32_VALUE");
            break;

        case EXPR_TYPE_I32_VALUE:
            printf("I32_VALUE");
            break;

        case EXPR_TYPE_BOOL_VALUE:
            printf("BOOL_VALUE");
            break;

        case EXPR_TYPE_UNARY_OP:
            printf("UNARY_OP");
            break;

        case EXPR_TYPE_BINARY_OP:
            printf("BINARY_OP");
            break;
        
        default:
            printf("UNKNOWN");
            break;
    }
}

void print_expr_data(FILE *out, ExprData *expr_data, uint32_t rec) {
    if (!expr_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "expr_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "type: ");
    print_expr_type(out, expr_data->type);
    fprintf(out, "\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "data:\n");
    switch (expr_data->type)
    {
        case EXPR_TYPE_VAR:
            print_var_data(out, (VarData *) expr_data->data, rec + 1);
            break;

        case EXPR_TYPE_U32_VALUE:
            print_u32_value_data(out, (U32ValueData *) expr_data->data, rec + 1);
            break;

        case EXPR_TYPE_I32_VALUE:
            print_i32_value_data(out, (I32ValueData *) expr_data->data, rec + 1);
            break;

        case EXPR_TYPE_BOOL_VALUE:
            print_bool_value_data(out, (BoolValueData *) expr_data->data, rec + 1);
            break;

        case EXPR_TYPE_UNARY_OP:
            print_unary_op_data(out, (UnaryOpData *) expr_data->data, rec + 1);
            break;

        case EXPR_TYPE_BINARY_OP:
            print_binary_op_data(out, (BinaryOpData *) expr_data->data, rec + 1);
            break;
        
        default:
            break;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}



void print_data_type(FILE *out, DataType data_type) {
    switch (data_type) {
        case DATA_TYPE_BOOL:
            fprintf(out, "BOOL");
            break;
        case DATA_TYPE_U32:
            fprintf(out, "U32");
            break;
        case DATA_TYPE_I32:
            fprintf(out, "I32");
            break;
        default:
            fprintf(out, "UNKNOWN");
            break;
    }
}

void print_var_decl_data(FILE *out, VarDeclData *var_decl_data, uint32_t rec) {
    if (!var_decl_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "var_decl_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "type: ");
    print_data_type(out, var_decl_data->type);
    fprintf(out, "\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "name: %s\n", var_decl_data->name);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_assign_data(FILE *out, AssignData *assign_data, uint32_t rec) {
    if (!assign_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "assign_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "var:\n");
    print_node(out, assign_data->var, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "expr:\n");
    print_node(out, assign_data->expr, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_if_case(FILE *out, IfCase *if_case, uint32_t rec) {
    if (!if_case) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "if_case {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "cond:\n");
    print_node(out, if_case->cond, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "body:\n");
    print_node(out, if_case->body, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_if_case_list(FILE *out, IfCaseList *if_case_list, uint32_t rec) {
    if (!if_case_list) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "if_case_list {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "if_case:\n");
    print_if_case(out, if_case_list->if_case, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "next:\n");
    print_if_case_list(out, if_case_list->next, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_if_data(FILE *out, IfData *if_data, uint32_t rec) {
    if (!if_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "if_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "if_case_list:\n");
    print_if_case_list(out, if_data->if_case_list, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_for_data(FILE *out, ForData *for_data, uint32_t rec) {
    if (!for_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "for_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "init:\n");
    print_node(out, for_data->init, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "cond:\n");
    print_node(out, for_data->cond, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "it:\n");
    print_node(out, for_data->it, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "body:\n");
    print_node(out, for_data->body, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_stmt_type(FILE *out, StmtType stmt_type) {
    switch (stmt_type) {
        case STMT_TYPE_VAR_DECL:
            fprintf(out, "VAR_DECL");
            break;
        case STMT_TYPE_ASSIGN:
            fprintf(out, "ASSIGN");
            break;
        case STMT_TYPE_IF:
            fprintf(out, "IF");
            break;
        case STMT_TYPE_FOR:
            fprintf(out, "FOR");
            break;
        default:
            fprintf(out, "UNKNOWN");
            break;
    }
}

void print_stmt_data(FILE *out, StmtData *stmt_data, uint32_t rec) {
    if (!stmt_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "stmt_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "type: ");
    print_stmt_type(out, stmt_data->type);
    fprintf(out, "\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "data:\n");
    switch (stmt_data->type) {
        case STMT_TYPE_VAR_DECL:
            print_var_decl_data(out, (VarDeclData *) stmt_data->data, rec + 1);
            break;
        case STMT_TYPE_ASSIGN:
            print_assign_data(out, (AssignData *) stmt_data->data, rec + 1);
            break;
        case STMT_TYPE_IF:
            print_if_data(out, (IfData *) stmt_data->data, rec + 1);
            break;
        case STMT_TYPE_FOR:
            print_for_data(out, (ForData *) stmt_data->data, rec + 1);
            break;
        default:
            break; 
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_stmt_list(FILE *out, StmtList *stmt_list, uint32_t rec) {
    if (!stmt_list) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "stmt_list {\n");
    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "stmt:\n");
    print_node(out, stmt_list->stmt, rec + 1);
    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "next:\n");
    print_stmt_list(out, stmt_list->next, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_blk_data(FILE *out, BlkData *blk_data, uint32_t rec) {
    if (!blk_data) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "blk_data {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "stmt_list:\n");
    print_stmt_list(out, blk_data->stmt_list, rec + 1);

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}

void print_node_type(FILE *out, NodeType node_type, uint32_t rec) {
    switch (node_type) {
        case NODE_TYPE_BLK:
            fprintf(out, "BLK");
            break;
        case NODE_TYPE_EXPR:
            fprintf(out, "EXPR");
            break;
        case NODE_TYPE_STMT:
            fprintf(out, "STMT");
            break;
        default:
            fprintf(out, "UNKNOWN");
            break;
    }
}

void print_node(FILE *out, Node *node, uint32_t rec) {
    if (!node) {
        print_spaces(out, rec * REC_SHIFT);
        fprintf(out, "NULL\n");
        return;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "node {\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "type: ");
    print_node_type(out, node->type, rec + 1);
    fprintf(out, "\n");

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "data:\n");

    switch (node->type)
    {
        case NODE_TYPE_BLK:
            print_blk_data(out, (BlkData *) node->data, rec + 1);
            break;

        case NODE_TYPE_EXPR:
            print_expr_data(out, (ExprData *) node->data, rec + 1);
            break;
        
        case NODE_TYPE_STMT:
            print_stmt_data(out, (StmtData *) node->data, rec + 1);
            break;
    
        default:
            break;
    }

    print_spaces(out, rec * REC_SHIFT);
    fprintf(out, "}\n");
}