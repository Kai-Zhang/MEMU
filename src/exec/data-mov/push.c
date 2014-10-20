#include "exec/helper.h"
#include "cpu/modrm.h"

#include "nemu.h"

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(push_i_b) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	if(cpu.eflags.virutal_mode) {
		reg_w(R_SP) -= 2;
		swaddr_write(reg_w(R_SP), 2, imm);
	}
	else {
		reg_l(R_ESP) -= 2;
		swaddr_write(reg_l(R_ESP), 2, imm);
	}

	print_asm("push $0x%x", imm);
	return 1;
}

make_helper(leave) {
	reg_l(R_ESP) = reg_l(R_EBP);
	if(suffix == 'l') {
		reg_l(R_EBP) = swaddr_read(reg_l(R_ESP), 4);
		reg_l(R_ESP) += 4;
	}
	else {
		reg_w(R_BP) = swaddr_read(reg_l(R_ESP), 2);
		reg_l(R_ESP) += 4;
	}

	print_asm("leave");
	return 1;
}

make_helper(push_m_v) {
	return (suffix == 'l' ? push_m_l(eip) : push_m_w(eip));
}

make_helper(push_r_v) {
	return (suffix == 'l' ? push_r_l(eip) : push_r_w(eip));
}

make_helper(push_i_v) {
	return (suffix == 'l' ? push_i_l(eip) : push_i_w(eip));
}

