#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(movs, SUFFIX)) {
	MEM_W(reg_l(R_EDI), MEM_R(reg_l(R_ESI)));
	if(cpu.eflags.direction_flag) {
		reg_l(R_EDI) -= DATA_BYTE;
		reg_l(R_ESI) -= DATA_BYTE;
	}
	else {
		reg_l(R_EDI) += DATA_BYTE;
		reg_l(R_ESI) += DATA_BYTE;
	}

	print_asm("movs" str(SUFFIX));
	return 1;
}

#include "exec/template-end.h"
