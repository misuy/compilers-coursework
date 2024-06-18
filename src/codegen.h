#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ir.h"

#define ZERO_REG 0
#define TMP1_REG 1
#define TMP2_REG 2
#define FIRST_FREE_REG 3
#define LAST_FREE_REG 31
#define FREE_REGS_COUNT (LAST_FREE_REG - FIRST_FREE_REG + 1)
#define MEM_SIZE (2 << 16)



typedef struct RegMappingItem {
    uint32_t phys_reg_number;
    uint32_t phys_addr;
    IRRegMappingItem *ir_mapping;
    Vector *neigh;
} RegMappingItem;

typedef struct RegMapping {
    uint32_t mem_tail;
    Vector *items;
} RegMapping;



typedef enum InstType {
    INST_TYPE_LW = 1,
    INST_TYPE_SW,
    INST_TYPE_BEQ,
    INST_TYPE_LI,
    INST_TYPE_XORI,
    INST_TYPE_EBREAK,
    INST_TYPE_RD1_RS2,
} InstType;

typedef enum InstRd1Rs2Type {
    INST_RD1_RS2_TYPE_AND = 1,
    INST_RD1_RS2_TYPE_OR,
    INST_RD1_RS2_TYPE_XOR,
    INST_RD1_RS2_TYPE_ADD,
    INST_RD1_RS2_TYPE_SUB,
    INST_RD1_RS2_TYPE_MUL,
    INST_RD1_RS2_TYPE_DIV,
    INST_RD1_RS2_TYPE_SEQ,
    INST_RD1_RS2_TYPE_SNE,
    INST_RD1_RS2_TYPE_SLT,
} InstRd1Rs2Type;

typedef struct InstRd1Rs2 {
    InstRd1Rs2Type type;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
} InstRd1Rs2;

typedef struct InstLw {
    uint32_t rd;
    uint32_t rs1;
    int32_t imm;
} InstLw;

typedef struct InstSw {
    uint32_t rs1;
    uint32_t rs2;
    int32_t imm;
} InstSw;

typedef struct InstBeq {
    uint32_t rs1;
    uint32_t rs2;
    int32_t imm;
} InstBeq;

typedef struct InstLi {
    uint32_t rd;
    int32_t imm;
} InstLi;

typedef struct InstXori {
    uint32_t rd;
    uint32_t rs1;
    int32_t imm;
} InstXori;


typedef struct Inst {
    InstType type;
    void *data;
} Inst;

typedef struct Code {
    uint32_t regs[FREE_REGS_COUNT];
    Vector *addr_mapping;
    RegMapping *reg_mapping;
    Vector *instructions;
} Code;


void tmp_print(RegMapping *mapping);
Code * code_from_ir(IR *ir);
void code_print(FILE *out, Code *code);

#endif