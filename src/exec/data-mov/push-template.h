#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

#define inc_dec_flags(rst, op) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.auxiliary_flag = ((rst) ^ (rst op 1)) >> 4; \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(push_m_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	switch(m.opcode) {
		case 0:
			if(m.mod == 3) {
				DATA_TYPE rst = ++ REG(m.R_M);

				cpu.eflags.carry_flag = ((DATA_TYPE)(rst) < (DATA_TYPE)(rst - 1));
				cpu.eflags.overflow_flag = ((DATA_TYPE_S)(rst) >= 0) && ((DATA_TYPE_S)(rst - 1) >= 0);
				inc_dec_flags(rst, -);

				print_asm("inc %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE rst = MEM_R(addr) + 1;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = ((DATA_TYPE)(rst) < (DATA_TYPE)(rst - 1));
				cpu.eflags.overflow_flag = ((DATA_TYPE_S)(rst) >= 0) && ((DATA_TYPE_S)(rst - 1) >= 0);
				inc_dec_flags(rst, +);

				print_asm("inc %s", ModR_M_asm);
			}
			break;
		case 1:
			if(m.mod == 3) {
				DATA_TYPE rst = -- REG(m.R_M);

				cpu.eflags.carry_flag = ((DATA_TYPE)(rst) < (DATA_TYPE)(rst - 1));
				cpu.eflags.overflow_flag = ((DATA_TYPE_S)(rst) < 0) && ((DATA_TYPE_S)(rst - 1) < 0);
				inc_dec_flags(rst, +);

				print_asm("dec %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE rst = MEM_R(addr) - 1;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = ((DATA_TYPE)(rst) < (DATA_TYPE)(rst - 1));
				cpu.eflags.overflow_flag = ((DATA_TYPE_S)(rst) < 0) && ((DATA_TYPE_S)(rst - 1) < 0);
				inc_dec_flags(rst, +);

				print_asm("dec %s", ModR_M_asm);
			}
			break;
#if DATA_BYTE != 1
		case 2:
			if(m.mod == 3) {
				reg_l(R_ESP) -= DATA_BYTE;
				MEM_W(reg_w(R_ESP), cpu.eip);
#if DATA_BYTE == 2
				cpu.eip = REG(m.R_M) & 0xffff;
#else
				cpu.eip = REG(m.R_M) - (1 + len);
#endif

				print_asm("call %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				reg_l(R_ESP) -= DATA_BYTE;
				MEM_W(reg_w(R_ESP), cpu.eip);
				cpu.eip += (int32_t)(DATA_TYPE_S)MEM_R(addr);
#if DATA_BYTE == 2
				cpu.eip = MEM_R(addr) & 0xffff;
#else
				cpu.eip = MEM_R(addr);
#endif

				print_asm("call %s", ModR_M_asm);
			}
			break;
		case 4:
			if(m.mod == 3) {
#if DATA_BYTE == 2
				cpu.eip = REG(m.R_M) & 0xffff;
#else
				cpu.eip = REG(m.R_M);
#endif

				print_asm("jmp %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				cpu.eip += (int32_t)(DATA_TYPE_S)MEM_R(addr);
#if DATA_BYTE == 2
				cpu.eip = MEM_R(addr) & 0xffff;
#else
				cpu.eip = MEM_R(addr);
#endif

				print_asm("jmp %s", ModR_M_asm);
			}
			break;
#endif
		case 6:
			assert(m.mod == 3);
			swaddr_t addr;
			len = read_ModR_M(eip + 1, &addr);
			if(cpu.eflags.virutal_mode) {
				reg_w(R_SP) -= DATA_BYTE;
				MEM_W(reg_w(R_SP), MEM_R(addr));
			}
			else {
				reg_l(R_ESP) -= DATA_BYTE;
				MEM_W(reg_l(R_ESP), MEM_R(addr));
			}
			break;
		default:
			printf("invalid operator(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}

	print_asm("push" str(SUFFIX) " %s", ModR_M_asm);
	return 1 + len;
}

make_helper(concat(push_r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	if(cpu.eflags.virutal_mode) {
		// 8086 `pushw %sp' pushes the latest value
		reg_w(R_SP) -= DATA_BYTE;
		MEM_W(reg_w(R_SP), REG(reg_code));
	}
	else {
		// 80386 `pushl %esp' pushes the former value
		MEM_W(reg_l(R_ESP) - DATA_BYTE, REG(reg_code));
		reg_l(R_ESP) -= DATA_BYTE;
	}

	print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(push_i_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	if(cpu.eflags.virutal_mode) {
		reg_w(R_SP) -= DATA_BYTE;
		MEM_W(reg_w(R_SP), imm);
	}
	else {
		reg_l(R_ESP) -= DATA_BYTE;
		MEM_W(reg_l(R_ESP), imm);
	}

	print_asm("push" str(SUFFIX) " $0x%x", imm);
	return 1;
}

#undef inc_dec_flags

#include "exec/template-end.h"
