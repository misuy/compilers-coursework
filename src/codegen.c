#include "codegen.h"

void print_spaces(FILE *out, uint32_t count);


RegMappingItem * reg_mapping_item_from_ir(IRRegMappingItem *item) {
    RegMappingItem *ret = malloc(sizeof(RegMappingItem));
    ret->ir_mapping = item;
    ret->phys_reg_number = 0;
    ret->phys_addr = 0;
    ret->neigh = vector_new(sizeof(RegMappingItem *), 10);
    return ret;
}

RegMapping * reg_mapping_from_ir(IRRegMapping *mapping) {
    RegMapping *ret = malloc(sizeof(RegMapping));
    ret->mem_tail = MEM_SIZE - 1;
    ret->items = vector_new(sizeof(RegMappingItem), mapping->items->items_count);
    for (uint32_t i=0; i<mapping->items->items_count; i++) {
        vector_push_back(ret->items, reg_mapping_item_from_ir(vector_get(mapping->items, i)));
    }
    return ret;
}

void reg_mapping_map_phys_regs(RegMapping *mapping) {
    for (uint32_t i=0; i<mapping->items->items_count; i++) {
        RegMappingItem *iitem = (RegMappingItem *) vector_get(mapping->items, i);
        for (uint32_t j=0; j<mapping->items->items_count; j++) {
            if (i == j)
                continue;
            RegMappingItem *jitem = (RegMappingItem *) vector_get(mapping->items, j);

            if (
                ((((int64_t) iitem->ir_mapping->last_used - (int64_t) jitem->ir_mapping->last_used) * ((int64_t) iitem->ir_mapping->last_used - (int64_t) jitem->ir_mapping->first_used)) <= 0) |
                ((((int64_t) iitem->ir_mapping->first_used - (int64_t) jitem->ir_mapping->last_used) * ((int64_t) iitem->ir_mapping->first_used - (int64_t) jitem->ir_mapping->first_used)) <= 0) |
                ((((int64_t) jitem->ir_mapping->last_used - (int64_t) iitem->ir_mapping->last_used) * ((int64_t) jitem->ir_mapping->last_used - (int64_t) iitem->ir_mapping->first_used)) <= 0) |
                ((((int64_t) jitem->ir_mapping->first_used - (int64_t) iitem->ir_mapping->last_used) * ((int64_t) jitem->ir_mapping->first_used - (int64_t) iitem->ir_mapping->first_used)) <= 0)
            )
                vector_push_back(iitem->neigh, &jitem);
        }
    }
    for (uint32_t i=0; i<mapping->items->items_count; i++) {
        RegMappingItem *item = (RegMappingItem *) vector_get(mapping->items, i);
        uint32_t *neigh_colour = (uint32_t *) malloc(sizeof(uint32_t) * FREE_REGS_COUNT);
        for (uint32_t j=0; j<FREE_REGS_COUNT; j++)
            neigh_colour[j] = 0;

        for (uint32_t j=0; j<item->neigh->items_count; j++) {
            RegMappingItem **neigh_item = (RegMappingItem **) vector_get(item->neigh, j);
            if ((*neigh_item)->phys_reg_number != 0) {
                neigh_colour[(*neigh_item)->phys_reg_number - FIRST_FREE_REG]++;
            }
        }

        uint32_t min_colour = 0;
        uint32_t min = UINT32_MAX;
        for (uint32_t j=0; j<FREE_REGS_COUNT; j++) {
            if (neigh_colour[j] < min) {
                min = neigh_colour[j];
                min_colour = j;
            }
        }

        item->phys_reg_number = min_colour + FIRST_FREE_REG;
        if ((neigh_colour[min_colour] != 0) & (!item->phys_addr)) {
            item->phys_addr = mapping->mem_tail;
            mapping->mem_tail--;
        }

        for (uint32_t j=0; j<item->neigh->items_count; j++) {
            RegMappingItem **neigh_item = (RegMappingItem **) vector_get(item->neigh, j);
            if (((*neigh_item)->phys_reg_number == item->phys_reg_number) & (!(*neigh_item)->phys_addr)) {
                (*neigh_item)->phys_addr = mapping->mem_tail;
                mapping->mem_tail--;
            }
        }
    }
}

void tmp_print(RegMapping *mapping) {
    for (uint32_t i=0; i<mapping->items->items_count; i++) {
        RegMappingItem *item = (RegMappingItem *) vector_get(mapping->items, i);
        if (item->ir_mapping->var_name)
            printf("%s\n", item->ir_mapping->var_name);
        for (uint32_t j=0; j<item->neigh->items_count; j++) {
            RegMappingItem **neigh_item = (RegMappingItem **) vector_get(item->neigh, j);
            if ((*neigh_item)->ir_mapping->var_name)
                printf("%s ", (*neigh_item)->ir_mapping->var_name);
        }
        printf("\n");
        printf("%"PRIu32", %"PRIu32"\n", item->phys_reg_number, item->phys_addr);
    }
}



InstRd1Rs2 * inst_rd1_rs2_new(InstRd1Rs2Type type, uint32_t rd, uint32_t rs1, uint32_t rs2) {
    InstRd1Rs2 *ret = malloc(sizeof(InstRd1Rs2));
    ret->type = type;
    ret->rd = rd;
    ret->rs1 = rs1;
    ret->rs2 = rs2;
    return ret;
}

InstLw * inst_lw_new(uint32_t rd, uint32_t rs1, int32_t imm) {
    InstLw *ret = malloc(sizeof(InstLw));
    ret->rd = rd;
    ret->rs1 = rs1;
    ret->imm = imm;
    return ret;
}

InstSw * inst_sw_new(uint32_t rs1, uint32_t rs2, int32_t imm) {
    InstSw *ret = malloc(sizeof(InstSw));
    ret->rs1 = rs1;
    ret->rs2 = rs2;
    ret->imm = imm;
    return ret;
}

InstBeq * inst_beq_new(uint32_t rs1, uint32_t rs2, int32_t imm) {
    InstBeq *ret = malloc(sizeof(InstBeq));
    ret->rs1 = rs1;
    ret->rs2 = rs2;
    ret->imm = imm;
    return ret;
}

InstLi * inst_li_new(uint32_t rd, int32_t imm) {
    InstLi *ret = malloc(sizeof(InstLi));
    ret->rd = rd;
    ret->imm = imm;
    return ret;
}

InstXori * inst_xori_new(uint32_t rd, uint32_t rs1, int32_t imm) {
    InstXori *ret = malloc(sizeof(InstXori));
    ret->rd = rd;
    ret->rs1 = rs1;
    ret->imm = imm;
    return ret;
}

Inst * inst_new(InstType type, void *data) {
    Inst *ret = malloc(sizeof(Inst));
    ret->type = type;
    ret->data = data;
    return ret;
}



Code * code_new(RegMapping *reg_mapping) {
    Code *ret = malloc(sizeof(Code));
    ret->reg_mapping = reg_mapping;
    ret->instructions = vector_new(sizeof(Inst), 10);
    ret->addr_mapping = vector_new(sizeof(uint32_t), 10);
    for (uint32_t i=0; i<FREE_REGS_COUNT; i++) {
        ret->regs[i] = 0;
    }
    return ret;
} 

void code_add_inst(Code *code, Inst *inst) {
    vector_push_back(code->instructions, inst);
}

uint32_t code_get_reg(Code *code, IROp *op, uint32_t reg_for_imm) {
    switch (op->type) {
        case IR_OP_TYPE_IMM:
            Inst *li_inst = inst_new(INST_TYPE_LI, inst_li_new(reg_for_imm, (int32_t) ((IRImm *) op->data)->value));
            code_add_inst(code, li_inst);
            return reg_for_imm;

        case IR_OP_TYPE_REG:
            RegMappingItem *item = (RegMappingItem *) vector_get(code->reg_mapping->items, ((IRReg *) op->data)->number - 1);
            if (code->regs[item->phys_reg_number - FIRST_FREE_REG] != (((IRReg *) op->data)->number - 1)) {
                if (code->regs[item->phys_reg_number - FIRST_FREE_REG] != 0) {
                    uint8_t flag = 0;
                    RegMappingItem *neigh = vector_get(code->reg_mapping->items, code->regs[item->phys_reg_number - FIRST_FREE_REG]);
                    for (uint32_t i=0; i<item->neigh->items_count; i++) {
                        if (neigh == *((RegMappingItem **) vector_get(item->neigh, i))) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag) {
                        Inst *li_inst = inst_new(INST_TYPE_LI, inst_li_new(TMP1_REG, (int32_t) neigh->phys_addr));
                        code_add_inst(code, li_inst);
                        Inst *sw_inst = inst_new(INST_TYPE_SW, inst_sw_new(TMP1_REG, item->phys_reg_number, 0));
                        code_add_inst(code, sw_inst);
                    }
                }
                if (item->phys_addr) {
                    Inst *li_inst = inst_new(INST_TYPE_LI, inst_li_new(TMP1_REG, (int32_t) item->phys_addr));
                    code_add_inst(code, li_inst);
                    Inst *lw_inst = inst_new(INST_TYPE_LW, inst_lw_new(item->phys_reg_number, TMP1_REG, 0));
                    code_add_inst(code, lw_inst);
                }
                code->regs[item->phys_reg_number - FIRST_FREE_REG] = (((IRReg *) op->data)->number - 1);
            }
            return item->phys_reg_number;

        default:
            return 0;
    }
}

int32_t code_get_imm(Code *code, IROp *op) {
    switch (op->type) {
        case IR_OP_TYPE_IMM:
            return (int32_t) (((IRImm *) op->data)->value);
        default:
            return 0;
    }
}

void code_add_ir_inst(Code *code, IRInst *ir_inst) {
    uint32_t curr_inst = code->instructions->items_count;
    vector_push_back(code->addr_mapping, &curr_inst);

    Inst *inst;
    switch (ir_inst->type) {
        case IR_INST_TYPE_MOV:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_ADD,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    ZERO_REG
                )
            );
            break;
        case IR_INST_TYPE_EQUAL:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_SEQ,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_NOT_EQUAL:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_SNE,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_LESS:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_SLT,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_GREATER:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_SLT,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op3, TMP1_REG),
                    code_get_reg(code, ir_inst->op2, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_NOT:
            inst = inst_new(
                INST_TYPE_XORI, 
                inst_xori_new(
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    (int32_t) 0xFFF
                )
            );
            break;
        case IR_INST_TYPE_AND:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_AND,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_OR:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_OR,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_XOR:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_XOR,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_ADD:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_ADD,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_SUB:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_SUB,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_MUL:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_MUL,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_DIV:
            inst = inst_new(
                INST_TYPE_RD1_RS2, 
                inst_rd1_rs2_new(
                    INST_RD1_RS2_TYPE_DIV,
                    code_get_reg(code, ir_inst->op1, ZERO_REG),
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    code_get_reg(code, ir_inst->op3, TMP2_REG)
                )
            );
            break;
        case IR_INST_TYPE_JMP:
            inst = inst_new(
                INST_TYPE_BEQ,
                inst_beq_new(
                    ZERO_REG,
                    ZERO_REG,
                    code_get_imm(code, ir_inst->op1)
                )
            );
            break;
        case IR_INST_TYPE_JMP_IF0:
            inst = inst_new(
                INST_TYPE_BEQ,
                inst_beq_new(
                    code_get_reg(code, ir_inst->op2, TMP1_REG),
                    ZERO_REG,
                    code_get_imm(code, ir_inst->op1)
                )
            );
            break;
        default:
            break;
    }
    code_add_inst(code, inst);
}

void code_fix_addr(Code *code) {
    for (uint32_t i=0; i<code->instructions->items_count; i++) {
        Inst *inst = vector_get(code->instructions, i);
        printf("hi1\n");
        if (inst->type == INST_TYPE_BEQ) {
            int32_t old_addr = ((InstBeq *) inst->data)->imm;
            printf("fix_addr: %"PRIi32"\n", old_addr);
            int32_t new_addr = (int32_t) *((uint32_t *) vector_get(code->addr_mapping, old_addr - 1)) - i - 1;
            ((InstBeq *) inst->data)->imm = new_addr;
        }
    }
}

Code * code_from_ir(IR *ir) {
    RegMapping *reg_mapping = reg_mapping_from_ir(ir->reg_mapping);
    reg_mapping_map_phys_regs(reg_mapping);
    tmp_print(reg_mapping);
    Code *code = code_new(reg_mapping);
    for (uint32_t i=0; i<ir->instructions->items_count; i++) {
        IRInst *inst = vector_get(ir->instructions, i);
        code_add_ir_inst(code, inst);
    }
    uint32_t curr_inst = code->instructions->items_count;
    vector_push_back(code->addr_mapping, &curr_inst);
    code_add_inst(code, inst_new(INST_TYPE_EBREAK, NULL));
    code_fix_addr(code);
    return code;
}



void inst_rd1_rs2_print(FILE *out, InstRd1Rs2 *inst, uint32_t rec) {
    print_spaces(out, rec);
    switch (inst->type) {
        case INST_RD1_RS2_TYPE_AND:
            fprintf(out, "and");
            break;
        case INST_RD1_RS2_TYPE_OR:
            fprintf(out, "or");
            break;
        case INST_RD1_RS2_TYPE_XOR:
            fprintf(out, "xor");
            break;
        case INST_RD1_RS2_TYPE_ADD:
            fprintf(out, "add");
            break;
        case INST_RD1_RS2_TYPE_SUB:
            fprintf(out, "sub");
            break;
        case INST_RD1_RS2_TYPE_MUL:
            fprintf(out, "mul");
            break;
        case INST_RD1_RS2_TYPE_DIV:
            fprintf(out, "div");
            break;
        case INST_RD1_RS2_TYPE_SEQ:
            fprintf(out, "seq");
            break;
        case INST_RD1_RS2_TYPE_SNE:
            fprintf(out, "sne");
            break;
        case INST_RD1_RS2_TYPE_SLT:
            fprintf(out, "slt");
            break;
        default:
            fprintf(out, "unknown");
            break;
    }

    fprintf(out, " x%"PRIu32", x%"PRIu32", x%"PRIu32"\n", inst->rd, inst->rs1, inst->rs2);
}

void inst_lw_print(FILE *out, InstLw *inst, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "lw x%"PRIu32", x%"PRIu32", %"PRIi32"\n", inst->rd, inst->rs1, inst->imm);
}

void inst_sw_print(FILE *out, InstSw *inst, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "sw x%"PRIu32", %"PRIi32", x%"PRIu32"\n", inst->rs1, inst->imm, inst->rs2);
}

void inst_beq_print(FILE *out, InstBeq *inst, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "beq x%"PRIu32", x%"PRIu32", %"PRIi32"\n", inst->rs1, inst->rs2, inst->imm);
}

void inst_li_print(FILE *out, InstLi *inst, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "li x%"PRIu32", %"PRIi32"\n", inst->rd, inst->imm);
}

void inst_xori_print(FILE *out, InstXori *inst, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "xori x%"PRIu32", x%"PRIu32", %"PRIi32"\n", inst->rd, inst->rs1, inst->imm);
}

void inst_ebreak_print(FILE *out, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "ebreak\n");
}

void inst_print(FILE *out, Inst *inst, uint32_t rec) {
    switch (inst->type) {
        case INST_TYPE_RD1_RS2:
            inst_rd1_rs2_print(out, inst->data, rec);
            break;
        
        case INST_TYPE_SW:
            inst_sw_print(out, inst->data, rec);
            break;
        
        case INST_TYPE_LW:
            inst_lw_print(out, inst->data, rec);
            break;

        case INST_TYPE_LI:
            inst_li_print(out, inst->data, rec);
            break;
        
        case INST_TYPE_BEQ:
            inst_beq_print(out, inst->data, rec);
            break;
        
        case INST_TYPE_XORI:
            inst_xori_print(out, inst->data, rec);
            break;
        
        case INST_TYPE_EBREAK:
            inst_ebreak_print(out, rec);
            break;
        
        default:
            print_spaces(out, rec);
            fprintf(out, "unknown inst\n");
            break;
    }
}

void code_print(FILE *out, Code *code) {
    fprintf(out, "code_print\n");
    for (uint32_t i=0; i<code->instructions->items_count; i++) {
        inst_print(out, vector_get(code->instructions, i), 0);
    }
}