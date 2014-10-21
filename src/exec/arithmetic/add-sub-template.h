#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define arithmetic_flags(rst, lhs); \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4; \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(concat(OP_NAME, _a_i_), SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE lhs = REG(R_EAX);
	DATA_TYPE rst = lhs OP (imm + CARRY);
#ifdef WRITE_BACK
	REG(R_EAX) = rst;
#endif

#if OP_SYMBOL == '+'
	cpu.eflags.carry_flag = ((rst) < (lhs));
	cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) == ((DATA_TYPE_S)(imm + CARRY) < 0))
		&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#else
	cpu.eflags.carry_flag = ((rst) > (lhs));
	cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) != ((DATA_TYPE_S)(imm + CARRY) < 0))
		&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#endif
	arithmetic_flags(rst, lhs);

	print_asm(str(OP_NAME) str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}


make_helper(concat(concat(OP_NAME, _rm_r_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	if(m.mod == 3) {
		DATA_TYPE lhs = REG(m.R_M);
		rst = lhs OP (REG(m.reg) + CARRY);
#ifdef WRITE_BACK
		REG(m.R_M) = rst;
#endif

#if OP_SYMBOL == '+'
		cpu.eflags.carry_flag = ((rst) < (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) == ((DATA_TYPE_S)(REG(m.reg) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#else
		cpu.eflags.carry_flag = ((rst) > (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) != ((DATA_TYPE_S)(REG(m.reg) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#endif
		arithmetic_flags(rst, lhs);

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE lhs = MEM_R(addr);
		rst = lhs OP (REG(m.reg) + CARRY);
#ifdef WRITE_BACK
		MEM_W(addr, rst);
#endif

#if OP_SYMBOL == '+'
		cpu.eflags.carry_flag = ((rst) < (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) == ((DATA_TYPE_S)(REG(m.reg) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#else
		cpu.eflags.carry_flag = ((rst) > (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) != ((DATA_TYPE_S)(REG(m.reg) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#endif
		arithmetic_flags(rst, lhs);

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	
	return 1 + len;
}

make_helper(concat(concat(OP_NAME, _r_rm_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0, lhs = REG(m.reg);
	int len = 1;
	if(m.mod == 3) {
		rst = lhs OP (REG(m.R_M) + CARRY);
#ifdef WRITE_BACK
		REG(m.reg) = rst;
#endif

#if OP_SYMBOL == '+'
		cpu.eflags.carry_flag = ((rst) < (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) == ((DATA_TYPE_S)(REG(m.R_M) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#else
		cpu.eflags.carry_flag = ((rst) > (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) != ((DATA_TYPE_S)(REG(m.R_M) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#endif
		arithmetic_flags(rst, lhs);

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = lhs OP (MEM_R(addr) + CARRY);
#ifdef WRITE_BACK
		REG(m.reg) = rst;
#endif

#if OP_SYMBOL == '+'
		cpu.eflags.carry_flag = ((rst) < (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) == ((DATA_TYPE_S)(MEM_R(addr) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#else
		cpu.eflags.carry_flag = ((rst) > (lhs));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)(lhs) < 0) != ((DATA_TYPE_S)(MEM_R(addr) + CARRY) < 0))
			&& (((DATA_TYPE_S)(rst) < 0) != ((DATA_TYPE_S)(lhs) < 0));
#endif
		arithmetic_flags(rst, lhs);

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

#undef arithmetic_flags
#include "exec/template-end.h"
