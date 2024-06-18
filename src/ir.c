#include "ir.h"


IR * ir_new();
void ir_change_lifetimes(IR *ir, uint32_t first_inst, uint32_t last_inst, uint32_t first_used, uint32_t last_used);
void ir_add_inst(IR *ir, IRInst *inst);
IRInst * ir_get_inst(IR *ir, uint32_t idx);
uint32_t ir_get_instructions_count(IR *ir);

IRInst * ir_inst_new(IRInstType type, IROp *op1, IROp *op2, IROp *op3);

IRRegMapping * ir_reg_mapping_new();
uint32_t ir_reg_mapping_get_items_count(IRRegMapping *mapping);
void ir_reg_mapping_add_mapping_item(IRRegMapping *mapping, IRRegMappingItem *item);
IRRegMappingItem * ir_reg_mapping_get_mapping_item_by_number(IRRegMapping *mapping, uint32_t number);
uint32_t ir_reg_mapping_find_mapping_item_number_by_var_name(IRRegMapping *mapping, char *name);

IRReg * ir_reg_new(uint32_t number);
IRImm * ir_imm_new(IRDataType type, uint32_t value);
IROp * ir_op_new(IROpDirection direction, IROpType type, void *data);
IRDataType ir_op_get_data_type(IRRegMapping *mapping, IROp *op);

IROp * ir_from_var(IR *ir, VarData *var);
IROp * ir_from_i32_value(IR *ir, I32ValueData *i32_value);
IROp * ir_from_bool_value(IR *ir, BoolValueData *bool_value);
IROp * ir_from_unary_op(IR *ir, UnaryOpData *unary_op);
IROp * ir_from_binary_op(IR *ir, BinaryOpData *binary_op);
IROp * ir_from_expr(IR *ir, ExprData *expr);

void ir_from_var_decl(IR *ir, VarDeclData *var_decl);
void ir_from_assign(IR *ir, AssignData *assign);
void ir_from_if_case(IR *ir, IfCase *if_case, uint32_t jmp_out);
void ir_from_if_stmt(IR* ir, IfData *if_stmt);
void ir_from_for_stmt(IR *ir, ForData *for_stmt);
void ir_from_stmt(IR* ir, StmtData *stmt);

void ir_from_blk(IR *ir, BlkData *blk);

void ir_from_node(IR *ir, Node *node);

void print_spaces(FILE *out, uint32_t count);
void ir_reg_mapping_item_print(FILE *out, IRRegMappingItem *item, uint32_t rec);
void ir_reg_print(FILE *out, IRReg *reg, uint32_t rec);
void ir_imm_print(FILE *out, IRImm *imm, uint32_t rec);
void ir_op_print(FILE *out, IROp *op, uint32_t rec);
void ir_inst_print(FILE *out, IRInst *inst, uint32_t rec);



IR * ir_new() {
    IR *ret = malloc(sizeof(IR));
    ret->instructions = vector_new(sizeof(IRInst), 10);
    ret->reg_mapping = ir_reg_mapping_new();
    return ret;
}

void ir_add_inst(IR *ir, IRInst *inst) {
    vector_push_back(ir->instructions, inst);

    if (inst->op1) {
        if (inst->op1->type == IR_OP_TYPE_REG) {
            IRRegMappingItem *item = ir_reg_mapping_get_mapping_item_by_number(ir->reg_mapping, ((IRReg *) inst->op1->data)->number);
            if (item->first_used == 0)
                item->first_used = ir_get_instructions_count(ir);
            if (item->last_used < ir_get_instructions_count(ir))
                item->last_used = ir_get_instructions_count(ir);
        }
    }
    if (inst->op2) {
        if (inst->op2->type == IR_OP_TYPE_REG) {
            IRRegMappingItem *item = ir_reg_mapping_get_mapping_item_by_number(ir->reg_mapping, ((IRReg *) inst->op2->data)->number);
            if (item->first_used == 0)
                item->first_used = ir_get_instructions_count(ir);
            if (item->last_used < ir_get_instructions_count(ir))
                item->last_used = ir_get_instructions_count(ir);
        }
    }
    if (inst->op3) {
        if (inst->op3->type == IR_OP_TYPE_REG) {
            IRRegMappingItem *item = ir_reg_mapping_get_mapping_item_by_number(ir->reg_mapping, ((IRReg *) inst->op3->data)->number);
            if (item->first_used == 0)
                item->first_used = ir_get_instructions_count(ir);
            if (item->last_used < ir_get_instructions_count(ir))
                item->last_used = ir_get_instructions_count(ir);
        }
    }
}

void ir_change_lifetimes(IR *ir, uint32_t first_inst, uint32_t last_inst, uint32_t first_used, uint32_t last_used) {
    for (uint32_t i=first_inst; i<(last_inst + 1); i++) {
        IRInst *inst = ir_get_inst(ir, i);

        if (inst->op1) {
            if (inst->op1->type == IR_OP_TYPE_REG) {
                IRRegMappingItem *item = ir_reg_mapping_get_mapping_item_by_number(ir->reg_mapping, ((IRReg *) inst->op1->data)->number);
                if (item->var_name) {
                    if (first_used < item->first_used)
                        item->first_used = first_used;
                    if (item->last_used < last_used)
                        item->last_used = last_used;
                }
            }
        }
        if (inst->op2) {
            if (inst->op2->type == IR_OP_TYPE_REG) {
                IRRegMappingItem *item = ir_reg_mapping_get_mapping_item_by_number(ir->reg_mapping, ((IRReg *) inst->op2->data)->number);
                if (item->var_name) {
                    if (first_used < item->first_used)
                        item->first_used = first_used;
                    if (item->last_used < last_used)
                        item->last_used = last_used;
                }
            }
        }
        if (inst->op3) {
            if (inst->op3->type == IR_OP_TYPE_REG) {
                IRRegMappingItem *item = ir_reg_mapping_get_mapping_item_by_number(ir->reg_mapping, ((IRReg *) inst->op3->data)->number);
                if (item->var_name) {
                    if (first_used < item->first_used)
                        item->first_used = first_used;
                    if (item->last_used < last_used)
                        item->last_used = last_used;
                }
            }
        }
    }
}

IRInst * ir_get_inst(IR *ir, uint32_t idx) {
    if (idx == 0)
        return NULL;
    return vector_get(ir->instructions, idx - 1);
}

uint32_t ir_get_instructions_count(IR *ir) {
    return ir->instructions->items_count;
}



IRInst * ir_inst_new(IRInstType type, IROp *op1, IROp *op2, IROp *op3) {
    IRInst *ret = (IRInst *) malloc(sizeof(IRInst));
    ret->type = type;
    ret->op1 = op1;
    ret->op2 = op2;
    ret->op3 = op3;
    return ret;
}



IRRegMapping * ir_reg_mapping_new() {
    IRRegMapping *ret = malloc(sizeof(IRRegMapping));
    ret->items = vector_new(sizeof(IRRegMappingItem), 10);
    return ret;
}

uint32_t ir_reg_mapping_get_items_count(IRRegMapping *mapping) {
    return mapping->items->items_count;
}

void ir_reg_mapping_add_mapping_item(IRRegMapping *mapping, IRRegMappingItem *item) {
    item->first_used = 0;
    item->last_used = 0;
    vector_push_back(mapping->items, item);
}

IRRegMappingItem * ir_reg_mapping_get_mapping_item_by_number(IRRegMapping *mapping, uint32_t number) {
    return vector_get(mapping->items, number - 1);
}

uint32_t ir_reg_mapping_find_mapping_item_number_by_var_name(IRRegMapping *mapping, char *name) {
    for (uint32_t i=0; i<mapping->items->items_count; i++) {
        IRRegMappingItem *item = (IRRegMappingItem *) vector_get(mapping->items, i);
        if (item->var_name) {
            if (strcmp(name, item->var_name) == 0)
                return i+1;
        }
    }
    return 0;
}



IRReg * ir_reg_new(uint32_t number) {
    IRReg *ret = (IRReg *) malloc(sizeof(IRReg));
    ret->number = number;
    return ret;
}

IRImm * ir_imm_new(IRDataType type, uint32_t value) {
    IRImm *ret = (IRImm *) malloc(sizeof(IRImm));
    ret->type = type;
    ret->value = value;
    return ret;
}

IROp * ir_op_new(IROpDirection direction, IROpType type, void *data) {
    IROp *ret = (IROp *) malloc(sizeof(IROp));
    ret->direction = direction;
    ret->type = type;
    ret->data = data;
    return ret;
}

IRDataType ir_op_get_data_type(IRRegMapping *mapping, IROp *op) {
    switch (op->type) {
        case IR_OP_TYPE_REG:
            IRRegMappingItem *item = ir_reg_mapping_get_mapping_item_by_number(mapping, ((IRReg *) op->data)->number);
            if (!item)
                return 0;
            return item->type;
        
        case IR_OP_TYPE_IMM:
            return ((IRImm *) op->data)->type;

        default:
            return 0;
    }
}



IROp * ir_from_var(IR *ir, VarData *var) {
    uint32_t number = ir_reg_mapping_find_mapping_item_number_by_var_name(ir->reg_mapping, var->name);
    if (number == 0)
        return NULL;
    
    return ir_op_new(IR_OP_DIRECTION_UNKNOWN, IR_OP_TYPE_REG, ir_reg_new(number));
}

IROp * ir_from_i32_value(IR *ir, I32ValueData *i32_value) {
    return ir_op_new(IR_OP_DIRECTION_UNKNOWN, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_I32, i32_value->value));
}

IROp * ir_from_bool_value(IR *ir, BoolValueData *bool_value) {
    return ir_op_new(IR_OP_DIRECTION_UNKNOWN, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_BOOL, bool_value->value));
}

IROp * ir_from_unary_op(IR *ir, UnaryOpData *unary_op) {
    if (unary_op->operand->type != NODE_TYPE_EXPR)
        return NULL;
    
    IROp *src_op = ir_from_expr(ir, unary_op->operand->data);
    if (!src_op)
        return NULL;

    src_op->direction = IR_OP_DIRECTION_SRC;

    IRRegMappingItem dest_item;
    dest_item.var_name = NULL;
    dest_item.type = ir_op_get_data_type(ir->reg_mapping, src_op);

    IRInstType inst_type;
    switch (unary_op->type) {
        case UNARY_OP_TYPE_NOT:
            inst_type = IR_INST_TYPE_NOT;
        default:
            return NULL;
    }

    ir_reg_mapping_add_mapping_item(ir->reg_mapping, &dest_item);

    IROp *dest_op = ir_op_new(IR_OP_DIRECTION_DEST, IR_OP_TYPE_REG, ir_reg_new(ir_reg_mapping_get_items_count(ir->reg_mapping)));

    IRInst *inst = ir_inst_new(inst_type, dest_op, src_op, NULL);
    ir_add_inst(ir, inst);

    return ir_op_new(IR_OP_DIRECTION_UNKNOWN, IR_OP_TYPE_REG, ir_reg_new(ir_reg_mapping_get_items_count(ir->reg_mapping)));
}

IROp * ir_from_binary_op(IR *ir, BinaryOpData *binary_op) {
    if (binary_op->right_operand->type != NODE_TYPE_EXPR)
        return NULL;

    IROp *right_src_op = ir_from_expr(ir, binary_op->right_operand->data);
    if (!right_src_op)
        return NULL;
    right_src_op->direction = IR_OP_DIRECTION_SRC;
    IRDataType right_src_type = ir_op_get_data_type(ir->reg_mapping, right_src_op);

    if (binary_op->left_operand->type != NODE_TYPE_EXPR)
        return NULL;

    IROp *left_src_op = ir_from_expr(ir, binary_op->left_operand->data);
    if (!left_src_op)
        return NULL;
    left_src_op->direction = IR_OP_DIRECTION_SRC;
    IRDataType left_src_type = ir_op_get_data_type(ir->reg_mapping, left_src_op);

    IRRegMappingItem dest_item;
    dest_item.var_name = NULL;

    if ((left_src_op->type == IR_DATA_TYPE_BOOL) & (right_src_op->type == IR_DATA_TYPE_BOOL))
        dest_item.type = IR_DATA_TYPE_BOOL;
    else
        dest_item.type = IR_DATA_TYPE_I32;

    IRInstType inst_type;
    switch (binary_op->type) {
        case BINARY_OP_TYPE_EQUAL:
            inst_type = IR_INST_TYPE_EQUAL;
            dest_item.type = IR_DATA_TYPE_BOOL;
            break;
        case BINARY_OP_TYPE_NOT_EQUAL:
            inst_type = IR_INST_TYPE_NOT_EQUAL;
            dest_item.type = IR_DATA_TYPE_BOOL;
            break;
        case BINARY_OP_TYPE_LESS:
            inst_type = IR_INST_TYPE_LESS;
            dest_item.type = IR_DATA_TYPE_BOOL;
            break;
        case BINARY_OP_TYPE_GREATER:
            inst_type = IR_INST_TYPE_GREATER;
            dest_item.type = IR_DATA_TYPE_BOOL;
            break;
        case BINARY_OP_TYPE_AND:
            inst_type = IR_INST_TYPE_AND;
            break;
        case BINARY_OP_TYPE_OR:
            inst_type = IR_INST_TYPE_OR;
            break;
        case BINARY_OP_TYPE_XOR:
            inst_type = IR_INST_TYPE_XOR;
            break;
        case BINARY_OP_TYPE_ADD:
            inst_type = IR_INST_TYPE_ADD;
            dest_item.type = IR_DATA_TYPE_I32;
            break;
        case BINARY_OP_TYPE_SUB:
            inst_type = IR_INST_TYPE_SUB;
            dest_item.type = IR_DATA_TYPE_I32;
            break;
        case BINARY_OP_TYPE_MUL:
            inst_type = IR_INST_TYPE_MUL;
            dest_item.type = IR_DATA_TYPE_I32;
            break;
        case BINARY_OP_TYPE_DIV:
            inst_type = IR_INST_TYPE_DIV;
            dest_item.type = IR_DATA_TYPE_I32;
            break;
        default:
            return NULL;
    }

    ir_reg_mapping_add_mapping_item(ir->reg_mapping, &dest_item);
    
    IROp *dest_op = ir_op_new(IR_OP_DIRECTION_DEST, IR_OP_TYPE_REG, ir_reg_new(ir_reg_mapping_get_items_count(ir->reg_mapping)));

    IRInst *inst = ir_inst_new(inst_type, dest_op, left_src_op, right_src_op);
    ir_add_inst(ir, inst);

    return ir_op_new(IR_OP_DIRECTION_UNKNOWN, IR_OP_TYPE_REG, ir_reg_new(ir_reg_mapping_get_items_count(ir->reg_mapping)));
}

IROp * ir_from_expr(IR *ir, ExprData *expr) {    
    if (!expr)
        return NULL;

    switch (expr->type) {
        case EXPR_TYPE_VAR:
            return ir_from_var(ir, expr->data);
        case EXPR_TYPE_I32_VALUE:
            return ir_from_i32_value(ir, expr->data);
        case EXPR_TYPE_BOOL_VALUE:
            return ir_from_bool_value(ir, expr->data);
        case EXPR_TYPE_UNARY_OP:
            return ir_from_unary_op(ir, expr->data);
        case EXPR_TYPE_BINARY_OP:
            return ir_from_binary_op(ir, expr->data);
        default:
            return NULL;
    }
}



void ir_from_var_decl(IR *ir, VarDeclData *var_decl) {
    if (ir_reg_mapping_find_mapping_item_number_by_var_name(ir->reg_mapping, var_decl->name) != 0)
        return;
    IRRegMappingItem item;
    item.var_name = var_decl->name;
    switch (var_decl->type) {
        case DATA_TYPE_BOOL:
            item.type = IR_DATA_TYPE_BOOL;
            break;
        case DATA_TYPE_I32:
            item.type = IR_DATA_TYPE_I32;
            break;
        default:
            return;
    }

    ir_reg_mapping_add_mapping_item(ir->reg_mapping, &item);
}

void ir_from_assign(IR *ir, AssignData *assign) {
    if (assign->var->type != NODE_TYPE_EXPR)
        return;
    IROp *dest_op = ir_from_expr(ir, assign->var->data);
    if (!dest_op)
        return;
    if (dest_op->type != IR_OP_TYPE_REG)
        return;
    dest_op->direction = IR_OP_DIRECTION_DEST;
    
    if (assign->expr->type != NODE_TYPE_EXPR)
        return;
    IROp *src_op = ir_from_expr(ir, assign->expr->data);
    if (!src_op)
        return;
    src_op->direction = IR_OP_DIRECTION_SRC;
    
    if ((ir_op_get_data_type(ir->reg_mapping, dest_op) == IR_DATA_TYPE_BOOL) & (ir_op_get_data_type(ir->reg_mapping, src_op) == IR_DATA_TYPE_I32))
        return;
    
    IRInst *inst = ir_inst_new(IR_INST_TYPE_MOV, dest_op, src_op, NULL);
    ir_add_inst(ir, inst);
}

void ir_from_if_case(IR *ir, IfCase *if_case, uint32_t jmp_out) {
    uint32_t saved_inst_n = 0;
    if (if_case->cond) {
        if (if_case->cond->type != NODE_TYPE_EXPR)
            return;
        
        IROp *op = ir_from_expr(ir, if_case->cond->data);
        if (!op)
            return;
        op->direction = IR_OP_DIRECTION_SRC;

        IRInst *jmp_inst = ir_inst_new(IR_INST_TYPE_JMP_IF0, NULL, op, NULL);
        ir_add_inst(ir, jmp_inst);
        saved_inst_n = ir_get_instructions_count(ir); 
    }

    ir_from_node(ir, if_case->body);

    IRInst *jmp_out_inst = ir_inst_new(
        IR_INST_TYPE_JMP, 
        ir_op_new(IR_OP_DIRECTION_SRC, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_I32, jmp_out)), 
        NULL, 
        NULL
    );
    ir_add_inst(ir, jmp_out_inst);


    if (saved_inst_n != 0) {
        IRInst *jmp_inst = ir_get_inst(ir, saved_inst_n);
        jmp_inst->op1 = ir_op_new(IR_OP_DIRECTION_SRC, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_I32, ir_get_instructions_count(ir) + 1));
    }
}

void ir_from_if_stmt(IR* ir, IfData *if_stmt) {
    IRInst *skip_jmp_inst = ir_inst_new(
        IR_INST_TYPE_JMP,
        ir_op_new(IR_OP_DIRECTION_SRC, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_I32, ir_get_instructions_count(ir) + 3)),
        NULL,
        NULL
    );
    ir_add_inst(ir, skip_jmp_inst);

    IRInst *skip_if_inst = ir_inst_new(
        IR_INST_TYPE_JMP,
        NULL,
        NULL,
        NULL
    );
    ir_add_inst(ir, skip_if_inst);
    uint32_t skip_if_inst_n = ir_get_instructions_count(ir);

    IfCaseList *if_list = if_stmt->if_case_list;
    while (if_list) {
        ir_from_if_case(ir, if_list->if_case, skip_if_inst_n);
        if_list = if_list->next;
    }

    skip_if_inst = ir_get_inst(ir, skip_if_inst_n);
    skip_if_inst->op1 = ir_op_new(IR_OP_DIRECTION_SRC, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_I32, ir_get_instructions_count(ir) + 1));
}

void ir_from_for_stmt(IR *ir, ForData *for_stmt) {
    if (for_stmt->cond->type != NODE_TYPE_EXPR)
        return;

    ir_from_node(ir, for_stmt->init);

    uint32_t jump_back = ir_get_instructions_count(ir) + 1;
    uint32_t saved_jump_inst_n = 0;

    if (for_stmt->cond->data) {
        IROp *op = ir_from_expr(ir, for_stmt->cond->data);
        if (!op)
            return;
        op->direction = IR_OP_DIRECTION_SRC;

        IRInst *jmp_inst = ir_inst_new(
            IR_INST_TYPE_JMP_IF0,
            NULL,
            op,
            NULL
        );
        ir_add_inst(ir, jmp_inst);
        saved_jump_inst_n = ir_get_instructions_count(ir);
    }

    ir_from_node(ir, for_stmt->body);
    ir_from_node(ir, for_stmt->it);

    IRInst *jmp_back_inst = ir_inst_new(
        IR_INST_TYPE_JMP,
        ir_op_new(IR_OP_DIRECTION_SRC, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_I32, jump_back)),
        NULL,
        NULL
    );
    ir_add_inst(ir, jmp_back_inst);

    if (saved_jump_inst_n != 0) {
        IRInst *jmp_inst = ir_get_inst(ir, saved_jump_inst_n);
        jmp_inst->op1 = ir_op_new(IR_OP_DIRECTION_SRC, IR_OP_TYPE_IMM, ir_imm_new(IR_DATA_TYPE_I32, ir_get_instructions_count(ir) + 1));
    }

    ir_change_lifetimes(ir, jump_back, ir_get_instructions_count(ir), jump_back, ir_get_instructions_count(ir));
}

void ir_from_stmt(IR* ir, StmtData *stmt) {
    if (!stmt)
        return;
    
    switch (stmt->type) {
        case STMT_TYPE_VAR_DECL:
            ir_from_var_decl(ir, stmt->data);
            break;
        case STMT_TYPE_ASSIGN:
            ir_from_assign(ir, stmt->data);
            break;
        case STMT_TYPE_IF:
            ir_from_if_stmt(ir, stmt->data);
            break;
        case STMT_TYPE_FOR:
            ir_from_for_stmt(ir, stmt->data);
        default:
            break;
    }
}



void ir_from_blk(IR *ir, BlkData *blk) {
    if (!blk)
        return;

    StmtList *list = blk->stmt_list;
    while (list) {
        ir_from_node(ir, list->stmt);
        list = list->next;
    }
}



void ir_from_node(IR *ir, Node *node) {
    if (!node)
        return;
    
    switch (node->type) {
        case NODE_TYPE_STMT:
            ir_from_stmt(ir, node->data);
            break;
        case NODE_TYPE_EXPR:
            ir_from_expr(ir, node->data);
            break;
        case NODE_TYPE_BLK:
            ir_from_blk(ir, node->data);
            break;
        default:
            break;
    }
}



void ir_reg_mapping_item_print(FILE *out, IRRegMappingItem *item, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "var_name: ");
    if (item->var_name)
        fprintf(out, "%s\n", item->var_name);
    else
        fprintf(out, "NULL\n");
    
    print_spaces(out, rec);
    fprintf(out, "type: ");
    switch (item->type) {
        case IR_DATA_TYPE_BOOL:
            fprintf(out, "BOOL\n");
            break;
        case IR_DATA_TYPE_I32:
            fprintf(out, "I32\n");
            break;
        default:
            break;
    }

    print_spaces(out, rec);
    fprintf(out, "first_used: %"PRIu32"\n", item->first_used);

    print_spaces(out, rec);
    fprintf(out, "last_used: %"PRIu32"\n", item->last_used);
}

void ir_reg_print(FILE *out, IRReg *reg, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "number: %"PRIu32"\n", reg->number);
}

void ir_imm_print(FILE *out, IRImm *imm, uint32_t rec) {
    print_spaces(out, rec);
    fprintf(out, "type: ");
    switch (imm->type) {
        case IR_DATA_TYPE_BOOL:
            fprintf(out, "BOOL");
            break;
        case IR_DATA_TYPE_I32:
            fprintf(out, "I32");
            break;
        default:
            break;
    }
    fprintf(out, "\n");

    print_spaces(out, rec);
    fprintf(out, "value: %"PRIu32"\n", imm->value);
}

void ir_op_print(FILE *out, IROp *op, uint32_t rec) {
    print_spaces(out, rec);
    if (!op) {
        fprintf(out, "NULL\n");
        return;
    }
    
    fprintf(out, "direction: ");
    switch (op->direction) {
        case IR_OP_DIRECTION_UNKNOWN:
            fprintf(out, "UNKNOWN");
            break;
        case IR_OP_DIRECTION_SRC:
            fprintf(out, "SRC");
            break;
        case IR_OP_DIRECTION_DEST:
            fprintf(out, "DEST");
            break;
        default:
            break;
    }
    fprintf(out, "\n");

    print_spaces(out, rec);
    fprintf(out, "type: ");
    switch (op->type) {
        case IR_OP_TYPE_IMM:
            fprintf(out, "IMM\n");
            ir_imm_print(out, op->data, rec + REC_SHIFT);
            break;
        case IR_OP_TYPE_REG:
            fprintf(out, "REG\n");
            ir_reg_print(out, op->data, rec + REC_SHIFT);
            break;
        default:
            break;
    }
}

void ir_inst_print(FILE *out, IRInst *inst, uint32_t rec) {
    print_spaces(out, rec);
    if (!inst) {
        fprintf(out, "NULL\n");
        return;
    }

    fprintf(out, "type: ");
    switch (inst->type) {
        case IR_INST_TYPE_MOV:
            fprintf(out, "MOV");
            break;
        case IR_INST_TYPE_EQUAL:
            fprintf(out, "EQUAL");
            break;
        case IR_INST_TYPE_NOT_EQUAL:
            fprintf(out, "NOT_EQUAL");
            break;
        case IR_INST_TYPE_LESS:
            fprintf(out, "LESS");
            break;
        case IR_INST_TYPE_GREATER:
            fprintf(out, "GREATER");
            break;
        case IR_INST_TYPE_JMP:
            fprintf(out, "JMP");
            break;
        case IR_INST_TYPE_JMP_IF0:
            fprintf(out, "JMP_IF0");
            break;
        case IR_INST_TYPE_JMP_IF1:
            fprintf(out, "JMP_IF1");
            break;
        case IR_INST_TYPE_LOAD:
            fprintf(out, "LOAD");
            break;
        case IR_INST_TYPE_SAVE:
            fprintf(out, "SAVE");
            break;
        case IR_INST_TYPE_NOT:
            fprintf(out, "NOT");
            break;
        case IR_INST_TYPE_AND:
            fprintf(out, "AND");
            break;
        case IR_INST_TYPE_OR:
            fprintf(out, "OR");
            break;
        case IR_INST_TYPE_XOR:
            fprintf(out, "XOR");
            break;
        case IR_INST_TYPE_ADD:
            fprintf(out, "ADD");
            break;
        case IR_INST_TYPE_SUB:
            fprintf(out, "SUB");
            break;
        case IR_INST_TYPE_MUL:
            fprintf(out, "MUL");
            break;
        case IR_INST_TYPE_DIV:
            fprintf(out, "DIV");
            break;
        default:
            fprintf(out, "UNKNOWN_INST");
            break;
    }
    fprintf(out, "\n");

    print_spaces(out, rec);
    fprintf(out, "op1:\n");
    ir_op_print(out, inst->op1, rec + REC_SHIFT);

    print_spaces(out, rec);
    fprintf(out, "op2:\n");
    ir_op_print(out, inst->op2, rec + REC_SHIFT);

    print_spaces(out, rec);
    fprintf(out, "op3:\n");
    ir_op_print(out, inst->op3, rec + REC_SHIFT);
}

void ir_print(FILE *out, IR *ir) {
    fprintf(out, "\n\nREG MAPPING\n");
    for (uint32_t i=0; i<ir->reg_mapping->items->items_count; i++) {
        fprintf(out, "reg mapping item %"PRIu32":\n", i + 1);
        ir_reg_mapping_item_print(out, (IRRegMappingItem *) vector_get(ir->reg_mapping->items, i), REC_SHIFT);
        fprintf(out, "\n");
    }

    fprintf(out, "\n\nINSTRUCTIONS\n");
    for (uint32_t i=0; i<ir->instructions->items_count; i++) {
        fprintf(out, "inst %"PRIu32":\n", i + 1);
        ir_inst_print(out, (IRInst *) vector_get(ir->instructions, i), REC_SHIFT);
        fprintf(out, "\n");
    }
}



IR *ir_build(Node *ast) {
    IR *ir = ir_new();
    ir_from_node(ir, ast);
    return ir;
}