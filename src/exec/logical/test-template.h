#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define neg_flags(rst) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.auxiliary_flag = (rst) >> 4; \
	cpu.eflags.carry_flag = ((rst) != 0); \
	cpu.eflags.overflow_flag = (((DATA_TYPE_S)(-(rst)) < 0) == 0) \
		&& (((DATA_TYPE_S)(rst) < 0) != 0); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

#define logical_flags(rst) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(test_a2i_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE_S rst = REG(R_EAX) & imm;

	logical_flags(rst);

	print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}

make_helper(concat(test_rm2i_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0;
	DATA_TYPE_S rst = 0;
	int len = 1;
	switch(m.opcode == 0) {
		case 0:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) & imm;

				logical_flags(rst);

				print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = REG(m.R_M) & imm;

				logical_flags(rst);

				print_asm("test" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 3:
			if(m.mod == 3) {
				DATA_TYPE rst = -REG(m.R_M);
				REG(m.R_M) = rst;

				neg_flags(rst);
				
				print_asm("neg %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE rst = -MEM_R(addr);
				MEM_W(addr, rst);

				neg_flags(rst);

				print_asm("neg %s", ModR_M_asm);
			}

			return 1;
		default:
			printf("invalid `test'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}
	
	cpu.eflags.carry_flag = 0;
	cpu.eflags.overflow_flag = 0;

	return 1 + len + DATA_BYTE;
}

make_helper(concat(test_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S rst = 0;
	int len = 1;
	if(m.mod == 3) {
		rst = REG(m.R_M) & REG(m.reg);

		logical_flags(rst);

		print_asm("test" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = REG(m.R_M) & REG(m.reg);

		logical_flags(rst);

		print_asm("test" str(SUFFIX) " %%%s,%s", REG_NAME(m.R_M), ModR_M_asm);
	}
	
	cpu.eflags.carry_flag = 0;
	cpu.eflags.overflow_flag = 0;

	return 1 + len;
}

#include "exec/template-end.h"
