#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(xchg_a2r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	if(reg_code != R_EAX) {
		DATA_TYPE temp = REG(R_EAX);
		REG(R_EAX) = REG(reg_code);
		REG(reg_code) = temp;

		print_asm("xchg" str(SUFFIX) " %%%s, %%eax", REG_NAME(reg_code));
	} else {
		print_asm("nop");
	}

	return 1;
}

make_helper(concat(xchg_rm2r_ ,SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		DATA_TYPE temp = REG(m.reg);
		REG(m.reg) = REG(m.R_M);
		REG(m.R_M) = temp;

		print_asm("xchg" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE temp = MEM_R(addr);
		MEM_W(addr, REG(m.reg));
		REG(m.reg) = temp;

		print_asm("xchg" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return 1 + len;
	}
}

#include "exec/template-end.h"
