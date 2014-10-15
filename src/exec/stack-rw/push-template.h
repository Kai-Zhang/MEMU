#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(push_m_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	if(m.reg == 6) {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		if(cpu.eflags.virutal_mode) {
			reg_w(R_SP) -= DATA_BYTE;
			MEM_W(reg_w(R_SP), MEM_R(addr));
		}
		else {
			reg_l(R_ESP) -= DATA_BYTE;
			MEM_W(reg_l(R_ESP), MEM_R(addr));
		}
	}
	else {
		printf("invalid `push'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}

	print_asm("push %s", ModR_M_asm);
	return 1 + len;
}

make_helper(concat(push_r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	if(cpu.eflags.virutal_mode) {
		// 8086 `pushw %sp' pushes the latest value
		reg_w(R_SP) -= DATA_BYTE;
		MEM_W(reg_w(R_SP), REG(reg_code));
	}
	else {
		// 80386 `pushl %esp' pushes the former value
		MEM_W(reg_l(R_ESP) - DATA_BYTE, REG(reg_code));
		reg_l(R_ESP) -= DATA_BYTE;
	}

	print_asm("push %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(push_i_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	if(cpu.eflags.virutal_mode) {
		reg_w(R_SP) -= DATA_BYTE;
		MEM_W(reg_w(R_SP), imm);
	}
	else {
		reg_l(R_ESP) -= DATA_BYTE;
		MEM_W(reg_l(R_ESP), imm);
	}

	print_asm("push $0x%x", imm);
	return 1;
}

#include "exec/template-end.h"
