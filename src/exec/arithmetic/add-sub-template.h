#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define arithmetic_flags(rst, lhs, rhs) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4; \
	cpu.eflags.carry_flag = ((rst) CARRY_COND (lhs)); \
	cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) == ((DATA_TYPE_S)(rhs) < 0)) \
		&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0)); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(concat(OP_NAME, _a_i_), SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE rst = REG(R_EAX) OP_SYMBOL (imm + CARRY);
#ifdef WRITE_BACK
	REG(R_EAX) = rst;
#endif

	arithmetic_flags(rst, REG(R_EAX), imm);

	print_asm(str(OP_NAME) str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}


make_helper(concat(concat(OP_NAME, _rm_r_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	if(m.mod == 3) {
		rst = REG(m.R_M) OP_SYMBOL (REG(m.reg) + CARRY);
#ifdef WRITE_BACK
		REG(m.R_M) = rst;
#endif

		arithmetic_flags(rst, REG(m.R_M), (REG(m.reg) + CARRY));

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = MEM_R(addr) OP_SYMBOL (REG(m.reg) + CARRY);
#ifdef WRITE_BACK
		MEM_W(addr, rst);
#endif

		arithmetic_flags(rst, MEM_R(addr), (REG(m.reg) + CARRY));

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
		rst = REG(m.reg) OP_SYMBOL (REG(m.R_M) + CARRY);
#ifdef WRITE_BACK
		REG(m.reg) = rst;
#endif

		arithmetic_flags(rst, REG(m.reg), (REG(m.R_M) + CARRY));

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = REG(m.reg) OP_SYMBOL (MEM_R(addr) + CARRY);
#ifdef WRITE_BACK
		REG(m.reg) = rst;
#endif

		arithmetic_flags(rst, REG(m.reg), (MEM_R(addr) + CARRY));

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
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

#undef arithmetic_flags
#include "exec/template-end.h"
