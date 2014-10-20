#include "exec/helper.h"
#include "cpu/modrm.h"

extern char suffix;

make_helper(pop_m_l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	if(m.reg == 0) {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		if(cpu.eflags.virutal_mode) {
			swaddr_write(addr, 4, swaddr_read(reg_w(R_SP), 4));
			reg_w(R_SP) += 4;
		}
		else {
			swaddr_write(addr, 4, swaddr_read(reg_l(R_ESP), 4));
			reg_l(R_ESP) += 4;
		}
	}
	else {
		printf("invalid `pop'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}

	print_asm("popl %s", ModR_M_asm);
	return 1 + len;
}

make_helper(pop_m_w) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	if(m.reg == 0) {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		if(cpu.eflags.virutal_mode) {
			swaddr_write(addr, 2, swaddr_read(reg_w(R_SP), 2));
			reg_w(R_SP) += 2;
		}
		else {
			swaddr_write(addr, 2, swaddr_read(reg_l(R_ESP), 2));
			reg_l(R_ESP) += 2;
		}
	}
	else {
		printf("invalid `pop'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}

	print_asm("popw %s", ModR_M_asm);
	return 1 + len;
}

make_helper(pop_r_l) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	if(cpu.eflags.virutal_mode) {
		reg_l(reg_code) = swaddr_read(reg_w(R_SP), 4);
		reg_w(R_SP) += 4;
	}
	else {
		reg_l(reg_code) = swaddr_read(reg_l(R_ESP), 4);
		reg_l(R_ESP) += 4;
	}

	print_asm("popl %%%s", regsl[reg_code]);
	return 1;
}

make_helper(pop_r_w) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	if(cpu.eflags.virutal_mode) {
		reg_l(reg_code) = swaddr_read(reg_w(R_SP), 2);
		reg_w(R_SP) += 2;
	}
	else {
		reg_l(reg_code) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 2;
	}

	print_asm("popw %%%s", regsw[reg_code]);
	return 1;
}

make_helper(pop_m_v) {
	return (suffix == 'l' ? pop_m_l(eip) : pop_m_w(eip));
}

make_helper(pop_r_v) {
	return (suffix == 'l' ? pop_r_l(eip) : pop_r_w(eip));
}
