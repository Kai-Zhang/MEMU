#include "exec/helper.h"

make_helper(concat(set, COND)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	if(m.mod == 3) {
		reg_b(m.R_M) = FLAGS;

		print_asm("set" str(COND) " %%%s", regsb[m.R_M]);
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		swaddr_write(addr, 1, FLAGS);

		print_asm("set" str(COND) " %s", ModR_M_asm);
	}

	return 1 + len;
}

