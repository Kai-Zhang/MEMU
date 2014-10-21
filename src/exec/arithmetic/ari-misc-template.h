#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define inc_dec_flags(rst, op); \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.auxiliary_flag = ((rst) ^ (rst op 1)) >> 4; \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(inc_r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	DATA_TYPE rst = ++ REG(reg_code);

	cpu.eflags.carry_flag = (DATA_TYPE)(rst) < (DATA_TYPE)(rst - 1);
	cpu.eflags.overflow_flag = ((DATA_TYPE_S)(rst) < 0) && ((DATA_TYPE_S)(rst - 1) >= 0);
	inc_dec_flags(rst, -);

	print_asm("inc %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(dec_r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	DATA_TYPE rst = -- REG(reg_code);

	cpu.eflags.carry_flag = (DATA_TYPE)(rst) > (DATA_TYPE)(rst + 1);
	cpu.eflags.overflow_flag = ((DATA_TYPE_S)(rst) >= 0) && ((DATA_TYPE_S)(rst - 1) < 0);
	inc_dec_flags(rst, +);

	print_asm("dec %%%s", REG_NAME(reg_code));
	return 1;
}

#undef inc_dec_flags

#include "exec/template-end.h"
