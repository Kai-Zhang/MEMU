#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define logical_flags(rst) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(concat(OP_NAME, _a_i_), SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE rst = REG(R_EAX) OP_SYMBOL imm;
	REG(R_EAX) = rst;

	logical_flags(rst);

	print_asm(str(OP_NAME) str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}

make_helper(concat(concat(OP_NAME, _rm_r_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	if(m.mod == 3) {
		rst = REG(m.R_M) OP_SYMBOL REG(m.reg);
		REG(m.R_M) = rst;

		logical_flags(rst);

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = MEM_R(addr) OP_SYMBOL REG(m.reg);
		MEM_W(addr, rst);

		logical_flags(rst);

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	
	return 1 + len;
}

make_helper(concat(concat(OP_NAME, _r_rm_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	if(m.mod == 3) {
		rst = REG(m.reg) OP_SYMBOL REG(m.R_M);
		REG(m.reg) = rst;

		logical_flags(rst);

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = REG(m.reg) OP_SYMBOL MEM_R(addr);
		REG(m.reg) = rst;

		logical_flags(rst);

		print_asm(str(OP_NAME) str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	}
	
	return 1 + len;
}

#if DATA_BYTE == 4

extern char suffix;

make_helper(concat(OP_NAME, _a_i_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _a_i_l)(eip) : concat(OP_NAME, _a_i_w)(eip));
}

make_helper(concat(OP_NAME, _rm_r_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _rm_r_l)(eip) : concat(OP_NAME, _rm_r_w)(eip));
}

make_helper(concat(OP_NAME, _r_rm_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _r_rm_l)(eip) : concat(OP_NAME, _r_rm_w)(eip));
}

#endif

#include "exec/template-end.h"
