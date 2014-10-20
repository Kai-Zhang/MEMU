#include "exec/helper.h"

#define DATA_BYTE 2
#include "ari-misc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ari-misc-template.h"
#undef DATA_BYTE

#define inc_dec_flags(rst, op) \
	cpu.eflags.sign_flag = ((int8_t)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.auxiliary_flag = ((rst) ^ (rst op 1)) >> 4; \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(inc_dec_rm_b) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint8_t rst = 0;
	int len = 1;
	switch(m.opcode) {
		case 0:
			if(m.mod == 3) {
				rst = ++ reg_b(m.R_M);

				cpu.eflags.carry_flag = (uint8_t)rst < (uint8_t)(rst - 1);
				cpu.eflags.overflow_flag = ((int8_t)(rst) >= 0) && ((int8_t)(rst - 1) >= 0);
				inc_dec_flags(rst, -);

				print_asm("inc %%%s", regsb[m.R_M]);
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				rst = swaddr_read(addr, 1) + 1;
				swaddr_write(addr, 1, rst);

				cpu.eflags.carry_flag = (uint8_t)rst < (uint8_t)(rst - 1);
				cpu.eflags.overflow_flag = ((int8_t)(rst) >= 0) && ((int8_t)(rst - 1) >= 0);
				inc_dec_flags(rst, -);

				print_asm("inc %s", ModR_M_asm);
			}
			break;
		case 1:
			if(m.mod == 3) {
				rst = -- reg_b(m.R_M);

				cpu.eflags.carry_flag = (uint8_t)rst > (uint8_t)(rst + 1);
				cpu.eflags.overflow_flag = ((int8_t)(rst) < 0) && ((int8_t)(rst - 1) < 0);
				inc_dec_flags(rst, +);

				print_asm("dec %%%s", regsb[m.R_M]);
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				rst = swaddr_read(addr, 1) - 1;
				swaddr_write(addr, 1, rst);

				cpu.eflags.carry_flag = (uint8_t)rst > (uint8_t)(rst + 1);
				cpu.eflags.overflow_flag = ((int8_t)(rst) < 0) && ((int8_t)(rst - 1) < 0);
				inc_dec_flags(rst, +);

				print_asm("dec %s", ModR_M_asm);
			}
			break;
		default:
			printf("invalid arithmetic operator(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}

	return 1 + len;
}

extern char suffix;

make_helper(inc_r_v) {
	return (suffix == 'l' ? inc_r_l(eip) : inc_r_w(eip));
}
	
make_helper(dec_r_v) {
	return (suffix == 'l' ? dec_r_l(eip) : dec_r_w(eip));
}

