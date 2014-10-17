#include "exec/helper.h"

#define DATA_BYTE 2
#include "xchg-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "xchg-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(xchg_rm2r_b) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		uint8_t temp = reg_b(m.reg);
		reg_b(m.reg) = reg_b(m.R_M);
		reg_b(m.R_M) = temp;

		print_asm("xchgb %%%s,%%%s", regsb[m.reg], regsb[m.R_M]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		uint8_t temp = swaddr_read(addr, 1);
		swaddr_write(addr, 1, reg_b(m.reg));
		reg_b(m.reg) = temp;

		print_asm("xchgb %%%s,%s", regsb[m.reg], ModR_M_asm);
		return 1 + len;
	}
}

make_helper(xchg_a2r_v) {
	return (suffix == 'l' ? xchg_a2r_l(eip) : xchg_a2r_w(eip));
}

make_helper(xchg_rm2r_v) {
	return (suffix == 'l' ? xchg_rm2r_l(eip) : xchg_rm2r_w(eip));
}
