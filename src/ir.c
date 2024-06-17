#include "ir.h"



void ir_add_inst(IR *ir, IRInst *inst) {
    vector_push_back(ir->instructions, inst);
}



IRInst * ir_inst_new(IRInstType type, IROp *op1, IROp *op2, IROp *op3) {
    IRInst *ret = (IRInst *) malloc(sizeof(IRInst));
    ret->type = type;
    ret->op1 = op1;
    ret->op2 = op2;
    ret->op3 = op3;
    return ret;
}



uint32_t ir_reg_mapping_get_items_count(IRRegMapping *mapping) {
    return mapping->items->items_count;
}

void ir_reg_mapping_add_mapping_item(IRRegMapping *mapping, IRRegMappingItem *item) {
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
    uint32_t number = ir_reg_mapping_find_reg_by_var_name(ir->reg_mapping, var->name);
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
    
    IROp *src_op = ir_from_expr(ir, unary_op->operand);
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

IROp * ir_from_binary_exp(IR *ir, BinaryOpData *binary_op) {
    if (binary_op->right_operand->type != NODE_TYPE_EXPR)
        return NULL;

    IROp *right_src_op = ir_from_expr(ir, binary_op->right_operand);
    if (!right_src_op)
        return NULL;
    right_src_op->direction = IR_OP_DIRECTION_SRC;

    if (binary_op->left_operand->type != NODE_TYPE_EXPR)
        return NULL;

    IROp *left_src_op = ir_from_expr(ir, binary_op->left_operand);
    if (!left_src_op)
        return NULL;
    left_src_op->direction = IR_OP_DIRECTION_SRC;

    
    
}

IROp * ir_from_expr(IR *ir, ExprData *expr) {    
    switch (expr->type) {
        case EXPR_TYPE_VAR:

    }
}