#ifndef _IR_H
#define _IR_H

#include <inttypes.h>

#include "vector.h"
#include "ast.h"



typedef enum IRDataType {
    IR_DATA_TYPE_I32 = 1,
    IR_DATA_TYPE_BOOL,
} IRDataType;



typedef struct IRRegMappingItem {
    IRDataType type;
    char *var_name;
} IRRegMappingItem;

typedef struct IRRegMapping {
    Vector *items;
} IRRegMapping;



typedef struct IRReg {
    uint32_t number;
} IRReg;

typedef struct IRImm {
    IRDataType type;
    uint32_t value;
} IRImm;

typedef enum IROpDirection {
    IR_OP_DIRECTION_UNKNOWN = 1,
    IR_OP_DIRECTION_SRC,
    IR_OP_DIRECTION_DEST,
} IROpDirection;

typedef enum IROpType {
    IR_OP_TYPE_IMM = 1,
    IR_OP_TYPE_REG,
} IROpType;

typedef struct IROp {
    IROpDirection direction;
    IROpType type;
    void *data;
} IROp;



typedef enum IRInstType {
    IR_INST_TYPE_MOV = 1,
    IR_INST_TYPE_JMP,
    IR_INST_TYPE_JEQ,
    IR_INST_TYPE_JNE,
    IR_INST_TYPE_LOAD,
    IR_INST_TYPE_SAVE,
    IR_INST_TYPE_EQUAL,
    IR_INST_TYPE_NOT_EQUAL,
    IR_INST_TYPE_LESS,
    IR_INST_TYPE_GREATER,
    IR_INST_TYPE_NOT,
    IR_INST_TYPE_AND,
    IR_INST_TYPE_OR,
    IR_INST_TYPE_XOR,
    IR_INST_TYPE_ADD,
    IR_INST_TYPE_SUB,
    IR_INST_TYPE_MUL,
    IR_INST_TYPE_DIV,
} IRInstType;

typedef struct IRInst {
    IRInstType type;
    IROp *op1;
    IROp *op2;
    IROp *op3;
} IRInst;



typedef struct IR {
    IRRegMapping *reg_mapping;
    Vector *instructions;
} IR;



void ir_print(FILE *out, IR *ir);
IR *ir_build(Node *ast);


#endif