#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(imul_r_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	if(m.mod == 3) {
		int64_t rst = (int64_t)(DATA_TYPE_S)REG(m.reg) * (int64_t)(DATA_TYPE_S)REG(m.R_M);
		REG(m.reg) = rst;

		cpu.eflags.carry_flag = !(rst >> ((DATA_BYTE << 3) - 1)) || (rst >> ((DATA_BYTE << 3) - 1) != -1);
		cpu.eflags.overflow_flag = cpu.eflags.carry_flag;

		print_asm("imul %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		int64_t rst = (int64_t)(DATA_TYPE_S)REG(m.reg) * (int64_t)(DATA_TYPE_S)MEM_R(addr);
		REG(m.reg) = rst;

		cpu.eflags.carry_flag = !(rst >> ((DATA_BYTE << 3) - 1)) || (rst >> ((DATA_BYTE << 3) - 1) != -1);
		cpu.eflags.overflow_flag = cpu.eflags.carry_flag;

		print_asm("imul %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	}

	return 1 + len;
}

make_helper(concat(imul_r_rm_i_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S imm = 0;
	int len = 1;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 2, DATA_BYTE);
		int64_t rst = (int64_t)(DATA_TYPE_S)REG(m.R_M) * (int64_t)imm;
		REG(m.reg) = rst;

		cpu.eflags.carry_flag = !(rst >> ((DATA_BYTE << 3) - 1)) || (rst >> ((DATA_BYTE << 3) - 1) != -1);
		cpu.eflags.overflow_flag = cpu.eflags.carry_flag;

		if (m.reg == m.R_M) {
			print_asm("imul $0x%x,%%%s", imm, REG_NAME(m.reg));
		}
		else {
			print_asm("imul $0x%x,%%%s,%%%s", imm, REG_NAME(m.R_M), REG_NAME(m.reg));
		}
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		int64_t rst = (int64_t)(DATA_TYPE_S)MEM_R(addr) * (int64_t)imm;
		REG(m.reg) = rst;

		cpu.eflags.carry_flag = !(rst >> ((DATA_BYTE << 3) - 1)) || (rst >> ((DATA_BYTE << 3) - 1) != -1);
		cpu.eflags.overflow_flag = cpu.eflags.carry_flag;

		print_asm("imul $0x%x,%s,%%%s", imm, ModR_M_asm, REG_NAME(m.reg));
	}

	return 1 + len + DATA_BYTE;
}

make_helper(concat(imul_r_rm_i8_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm = 0;
	int len = 1;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 2, 1);
		int64_t rst = (int64_t)(DATA_TYPE_S)REG(m.R_M) * (int64_t)imm;
		REG(m.reg) = rst;

		cpu.eflags.carry_flag = !(rst >> ((DATA_BYTE << 3) - 1)) || (rst >> ((DATA_BYTE << 3) - 1) != -1);
		cpu.eflags.overflow_flag = cpu.eflags.carry_flag;

		if(m.reg == m.R_M) {
			print_asm("imul $0x%x,%%%s", imm, REG_NAME(m.reg));
		}
		else {
			print_asm("imul $0x%x,%%%s,%%%s", imm, REG_NAME(m.R_M), REG_NAME(m.reg));
		}
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		int64_t rst = (int64_t)(DATA_TYPE_S)MEM_R(addr) * (int64_t)imm;
		REG(m.reg) = rst;

		cpu.eflags.carry_flag = !(rst >> ((DATA_BYTE << 3) - 1)) || (rst >> ((DATA_BYTE << 3) - 1) != -1);
		cpu.eflags.overflow_flag = cpu.eflags.carry_flag;

		print_asm("imul $0x%x,%%%s,%%%s", imm, ModR_M_asm, REG_NAME(m.reg));
	}

	return 2 + len;
}

#include "exec/template-end.h"
