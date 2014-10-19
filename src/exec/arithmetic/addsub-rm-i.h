#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(add_sub_rm_i_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0, rst = 0;
	int len = 1;
	switch(m.opcode) {
		case 0: 
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) + imm;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), imm);

				print_asm("add" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) + imm;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), imm);

				print_asm("add" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 2:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) + imm + cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), imm + cpu.eflags.carry_flag);

				print_asm("adc" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) + imm + cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), imm + cpu.eflags.carry_flag);

				print_asm("adc" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 3:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) - imm - cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), - imm - cpu.eflags.carry_flag);

				print_asm("sbb" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) - imm - cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), - imm - cpu.eflags.carry_flag);

				print_asm("sbb" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 5:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) - imm;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), - imm);

				print_asm("sub" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) - imm;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), - imm);

				print_asm("sub" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 7:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) - imm;

				arithmetic_flags(rst, REG(m.R_M), - imm);

				print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) - imm;

				arithmetic_flags(rst, MEM_R(addr), - imm);

				print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		default:
			printf("invalid arithmetic operator(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}
	
	return 1 + len + DATA_BYTE;
}

#if DATA_BYTE != 1

make_helper(concat(add_sub_rm_i8_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm = 0;
	DATA_TYPE rst = 0;
	int len = 1;
	switch(m.opcode) {
		case 0: 
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) + (int32_t)imm;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), (int32_t)imm);

				print_asm("add" str(SUFFIX) " $0x%x,%%%s", (int32_t)imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) + (int32_t)imm;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), (int32_t)imm);

				print_asm("add" str(SUFFIX) " $0x%x,%s", (int32_t)imm, ModR_M_asm);
			}
			break;
		case 2:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) + (int32_t)imm + cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), (int32_t)imm + cpu.eflags.carry_flag);

				print_asm("adc" str(SUFFIX) " $0x%x,%%%s", (int32_t)imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) + (int32_t)imm + cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), (int32_t)imm + cpu.eflags.carry_flag);

				print_asm("adc" str(SUFFIX) " $0x%x,%s", (int32_t)imm, ModR_M_asm);
			}
			break;
		case 3:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) - (int32_t)imm - cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), - (int32_t)imm - cpu.eflags.carry_flag);

				print_asm("sbb" str(SUFFIX) " $0x%x,%%%s", (int32_t)imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) - (int32_t)imm - cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), - (int32_t)imm - cpu.eflags.carry_flag);

				print_asm("sbb" str(SUFFIX) " $0x%x,%s", (int32_t)imm, ModR_M_asm);
			}
			break;
		case 5:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) - (int32_t)imm;
				REG(m.R_M) = rst;

				arithmetic_flags(rst, REG(m.R_M), - (int32_t)imm);

				print_asm("sub" str(SUFFIX) " $0x%x,%%%s", (int32_t)imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) - (int32_t)imm;
				MEM_W(addr, rst);

				arithmetic_flags(rst, MEM_R(addr), - (int32_t)imm);

				print_asm("sub" str(SUFFIX) " $0x%x,%s", (int32_t)imm, ModR_M_asm);
			}
			break;
		case 7:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) - (int32_t)imm;

				arithmetic_flags(rst, REG(m.R_M), - (int32_t)imm);

				print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", (int32_t)imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) - (int32_t)imm;

				arithmetic_flags(rst, MEM_R(addr), - (int32_t)imm);

				print_asm("cmp" str(SUFFIX) " $0x%x,%s", (int32_t)imm, ModR_M_asm);
			}
			break;
		default:
			printf("invalid arithmetic operator(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}
	
	return 2 + len;
}

#endif
#if DATA_BYTE == 4

make_helper(add_sub_rm_i_v) {
	return (suffix == 'l' ? add_sub_rm_i_l(eip) : add_sub_rm_i_w(eip));
}

make_helper(add_sub_rm_i8_v) {
	return (suffix == 'l' ? add_sub_rm_i8_l(eip) : add_sub_rm_i8_w(eip));
}

#endif

#include "exec/template-end.h"
