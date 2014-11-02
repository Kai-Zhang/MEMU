#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(stos, SUFFIX)) {
	MEM_W(reg_l(R_EDI), REG(R_EAX));
	if(cpu.eflags.direction_flag) {
		reg_l(R_EDI) -= DATA_BYTE;
	}
	else {
		reg_l(R_EDI) += DATA_BYTE;
	}

	print_asm("stos" str(SUFFIX));
	return 1;
}

#include "exec/template-end.h"
