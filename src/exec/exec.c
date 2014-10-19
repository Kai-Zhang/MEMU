#include "exec/helper.h"

#include "all-instr.h"

typedef int (*helper_fun)(swaddr_t);


/* TODO: Add more instructions!!! */

helper_fun opcode_table [256] = {
/* 0x00 */	add_rm_r_b, add_rm_r_v, add_r_rm_b, add_r_rm_v, 
/* 0x04 */	add_a_i_b, add_a_i_v, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, escape_code, 
/* 0x10 */	adc_rm_r_b, adc_rm_r_v, adc_r_rm_b, adc_r_rm_v, 
/* 0x14 */	adc_a_i_b, adc_a_i_v, inv, inv, 
/* 0x18 */	sbb_rm_r_b, sbb_rm_r_v, sbb_r_rm_b, sbb_r_rm_v, 
/* 0x1c */	sbb_a_i_b, sbb_a_i_v, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	sub_rm_r_b, sub_rm_r_v, sub_r_rm_b, sub_r_rm_v, 
/* 0x2c */	sub_a_i_b, sub_a_i_v, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	cmp_rm_r_b, cmp_rm_r_v, cmp_r_rm_b, cmp_r_rm_v, 
/* 0x3c */	cmp_a_i_b, cmp_a_i_v, inv, inv, 
/* 0x40 */	inc_r_v, inc_r_v, inc_r_v, inc_r_v, 
/* 0x44 */	inc_r_v, inc_r_v, inc_r_v, inc_r_v,
/* 0x48 */	dec_r_v, dec_r_v, dec_r_v, dec_r_v, 
/* 0x4c */	dec_r_v, dec_r_v, dec_r_v, dec_r_v, 
/* 0x50 */	push_r_v, push_r_v, push_r_v, push_r_v, 
/* 0x54 */	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x58 */	pop_r_v, pop_r_v, pop_r_v, pop_r_v, 
/* 0x5c */	pop_r_v, pop_r_v, pop_r_v, pop_r_v, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, data_size, inv,
/* 0x68 */	push_i_v, imul_r_rm_i_v, push_i_b, imul_r_rm_i8_v, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	je_b, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	add_sub_rm_i_b, add_sub_rm_i_v, nemu_trap, add_sub_rm_i8_v, 
/* 0x84 */	test_rm_r_b, test_rm_r_v, xchg_rm2r_b, xchg_rm2r_v, 
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, inv, inv, pop_m_v, 
/* 0x90 */	xchg_a2r_v, xchg_a2r_v, xchg_a2r_v, xchg_a2r_v,
/* 0x94 */	xchg_a2r_v, xchg_a2r_v, xchg_a2r_v, xchg_a2r_v,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	test_a_i_b, test_a_i_v, inv, inv,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	int3, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, test_rm_i_b, test_rm_i_v,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	cld, std, inc_dec_rm_b, push_m_v
};

make_helper(exec) {
	return opcode_table[ instr_fetch(eip, 1) ](eip);
}
