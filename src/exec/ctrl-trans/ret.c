#include "exec/helper.h"

extern char suffix;

make_helper(ret) {
	if(suffix == 'l') {
		cpu.eip = swaddr_read(reg_l(R_ESP), 4) - 1;
		reg_l(R_ESP) += 4;
	}
	else {
		cpu.eip = (swaddr_read(reg_l(R_ESP), 2) & 0xffff) - 1;
		reg_l(R_ESP) += 2;
	}

	print_asm("ret");
	return 1;
}
