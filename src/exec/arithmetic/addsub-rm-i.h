#include "exec/helper.h"
#include "exec/template-start.h"

#define flags_proc(rst); \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(add_sub_rm_i_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0, rst = 0;
	int len = 1;
	switch(m.opcode) {
		case 0: 
			if(m.mod == 3) {
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = lhs + imm;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("add" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = lhs + imm;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("add" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 1:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) | imm;
				REG(m.R_M) = rst;

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("or" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) | imm;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("or" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 2:
			if(m.mod == 3) {
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = lhs + imm + cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("adc" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = lhs + imm + cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("adc" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 3:
			if(m.mod == 3) {
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = lhs - imm - cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sbb" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = lhs - imm - cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sbb" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 4:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) & imm;
				REG(m.R_M) = rst;

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("and" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) & imm;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("and" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 5:
			if(m.mod == 3) {
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = lhs - imm;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sub" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = lhs - imm;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sub" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 6:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) ^ imm;
				REG(m.R_M) = rst;

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("xor" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) ^ imm;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("xor" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
		case 7:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) - imm;

				cpu.eflags.auxiliary_flag = ((rst) ^ REG(m.R_M)) >> 4;
				cpu.eflags.carry_flag = rst > REG(m.R_M);
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.R_M) < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)REG(m.R_M) < 0));
				flags_proc(rst);

				print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = MEM_R(addr) - imm;

				cpu.eflags.auxiliary_flag = ((rst) ^ MEM_R(addr)) >> 4;
				cpu.eflags.carry_flag = rst > MEM_R(addr);
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)MEM_R(addr) < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)MEM_R(addr) < 0));
				flags_proc(rst);

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
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, 1);
				rst = lhs + (DATA_TYPE_S)imm;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("add" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = lhs + (DATA_TYPE_S)imm;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("add" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 1:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, 1);
				rst = REG(m.R_M) | (DATA_TYPE_S)imm;
				REG(m.R_M) = rst;

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("or" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = MEM_R(addr) | (DATA_TYPE_S)imm;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("or" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 2:
			if(m.mod == 3) {
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, 1);
				rst = lhs + (DATA_TYPE_S)imm + cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("adc" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = lhs + (DATA_TYPE_S)imm + cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst < lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) == ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("adc" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 3:
			if(m.mod == 3) {
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, 1);
				rst = lhs - (DATA_TYPE_S)imm - cpu.eflags.carry_flag;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sbb" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = lhs - (DATA_TYPE_S)imm - cpu.eflags.carry_flag;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)(imm + cpu.eflags.carry_flag) < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sbb" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 4:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, 1);
				rst = REG(m.R_M) & (DATA_TYPE_S)imm;
				REG(m.R_M) = rst;

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("and" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = MEM_R(addr) & (DATA_TYPE_S)imm;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("and" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 5:
			if(m.mod == 3) {
				DATA_TYPE lhs = REG(m.R_M);
				imm = instr_fetch(eip + 2, 1);
				rst = lhs - (DATA_TYPE_S)imm;
				REG(m.R_M) = rst;

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sub" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE lhs = MEM_R(addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = lhs - (DATA_TYPE_S)imm;
				MEM_W(addr, rst);

				cpu.eflags.auxiliary_flag = ((rst) ^ (lhs)) >> 4;
				cpu.eflags.carry_flag = rst > lhs;
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)lhs < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)lhs < 0));
				flags_proc(rst);

				print_asm("sub" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 6:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, 1);
				rst = REG(m.R_M) ^ (DATA_TYPE_S)imm;
				REG(m.R_M) = rst;

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("xor" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = MEM_R(addr) ^ (DATA_TYPE_S)imm;
				MEM_W(addr, rst);

				cpu.eflags.carry_flag = 0;
				cpu.eflags.overflow_flag = 0;
				flags_proc(rst);

				print_asm("xor" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 7:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, 1);
				rst = REG(m.R_M) - (DATA_TYPE_S)imm;

				cpu.eflags.auxiliary_flag = ((rst) ^ REG(m.R_M)) >> 4;
				cpu.eflags.carry_flag = rst > REG(m.R_M);
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.R_M) < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)REG(m.R_M) < 0));
				flags_proc(rst);

				print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, 1);
				rst = MEM_R(addr) - (DATA_TYPE_S)imm;

				cpu.eflags.auxiliary_flag = ((rst) ^ REG(m.R_M)) >> 4;
				cpu.eflags.carry_flag = rst > MEM_R(addr);
				cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.R_M) < 0) != ((DATA_TYPE_S)imm < 0))
					&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)REG(m.R_M) < 0));
				flags_proc(rst);

				print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
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

#undef flags_proc

#include "exec/template-end.h"
