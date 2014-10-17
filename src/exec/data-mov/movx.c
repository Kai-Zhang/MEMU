#include "exec/helper.h"
#include "cpu/modrm.h"

extern char suffix;

make_helper(movsx_b2l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_l(m.reg) = (int32_t)reg_b(m.R_M);
		print_asm("movsx %%%s,%%%s", regsb[m.R_M], regsl[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_l(m.reg) = (int32_t)swaddr_read(addr, 1);
		print_asm("movsx %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}

make_helper(movsx_b2w) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_w(m.reg) = (int16_t)reg_b(m.R_M);
		print_asm("movsx %%%s,%%%s", regsb[m.R_M], regsw[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_w(m.reg) = (int16_t)swaddr_read(addr, 1);
		print_asm("movsx %s,%%%s", ModR_M_asm, regsw[m.reg]);
		return 1 + len;
	}
}

make_helper(movsx_w2l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_l(m.reg) = (int32_t)reg_w(m.R_M);
		print_asm("movsx %%%s,%%%s", regsw[m.R_M], regsl[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_l(m.reg) = (int32_t)swaddr_read(addr, 2);
		print_asm("movsx %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}

make_helper(movsx_v) {
	return (suffix == 'l' ? movsx_b2l(eip) : movsx_b2w(eip));
}
