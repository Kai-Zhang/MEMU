#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define logical_flags(rst) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(shift_rm_1_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	switch(m.opcode) {
		case 4:
			if(m.mod == 3) {
				cpu.eflags.carry_flag = MSB(REG(m.R_M));
				rst = REG(m.R_M) << 1;
				REG(m.R_M) = rst;
				cpu.eflags.overflow_flag = MSB(REG(m.R_M)) != cpu.eflags.carry_flag;

				logical_flags(rst);

				print_asm("shl" str(SUFFIX) " $0x1,%%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				cpu.eflags.carry_flag = MSB(MEM_R(addr));
				rst = MEM_R(addr) << 1;
				MEM_W(addr, rst);
				cpu.eflags.overflow_flag = MSB(MEM_R(addr)) != cpu.eflags.carry_flag;

				logical_flags(rst);

				print_asm("shl" str(SUFFIX) " $0x1,%s", ModR_M_asm);
			}
			break;
		case 5:
			if(m.mod == 3) {
				cpu.eflags.overflow_flag = MSB(REG(m.R_M));
				cpu.eflags.carry_flag = REG(m.R_M) & 1;
				rst = (REG(m.R_M) >> 1) & ~(1 << ((DATA_BYTE << 3) - 1));
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("shr" str(SUFFIX) " $0x1,%%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				cpu.eflags.overflow_flag = MSB(MEM_R(addr));
				cpu.eflags.carry_flag = MEM_R(addr) & 1;
				rst = (MEM_R(addr) >> 1) & ~(1 << ((DATA_BYTE << 3) - 1));
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("shr" str(SUFFIX) " $0x1,%s", ModR_M_asm);
			}
			break;
		case 7:
			if(m.mod == 3) {
				cpu.eflags.overflow_flag = MSB(REG(m.R_M));
				cpu.eflags.carry_flag = REG(m.R_M);
				rst = REG(m.R_M) >> 1;
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("sar" str(SUFFIX) " $0x1,%%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				cpu.eflags.overflow_flag = MSB(MEM_R(addr));
				cpu.eflags.carry_flag = MEM_R(addr);
				rst = MEM_R(addr) >> 1;
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("sar" str(SUFFIX) " $0x1,%s", ModR_M_asm);
			}
			break;
		default:
			printf("invalid shift operator(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}

	return 1 + len;
}

make_helper(concat(shift_rm_c_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	switch(m.opcode) {
		case 4:
			if(m.mod == 3) {
				if(reg_b(R_CL)) {
					cpu.eflags.carry_flag = REG(m.R_M) >> ((DATA_BYTE << 3) - reg_b(R_CL));
				}
				else {
					cpu.eflags.carry_flag = MSB(REG(m.R_M));
				}
				rst = REG(m.R_M) << reg_b(R_CL);
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("shl" str(SUFFIX) " %%cl,%%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				if(reg_b(R_CL)) {
					cpu.eflags.carry_flag = MEM_R(addr) >> ((DATA_BYTE << 3) - reg_b(R_CL));
				}
				else {
					cpu.eflags.carry_flag = MSB(MEM_R(addr));
				}
				rst = MEM_R(addr) << reg_b(R_CL);
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("shl" str(SUFFIX) " %%cl,%s", ModR_M_asm);
			}
			break;
		case 5:
			if(m.mod == 3) {
				if(reg_b(R_CL)) {
					cpu.eflags.carry_flag = REG(m.R_M) >> (reg_b(R_CL) - 1);
					rst = (REG(m.R_M) >> reg_b(R_CL)) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (reg_b(R_CL) - 1));
				}
				else {
					cpu.eflags.carry_flag = REG(m.R_M);
					rst = REG(m.R_M);
				}
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("shr" str(SUFFIX) " %%cl,%%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				if(reg_b(R_CL)) {
					cpu.eflags.carry_flag = MEM_R(addr) >> (reg_b(R_CL) - 1);
					rst = (MEM_R(addr) >> reg_b(R_CL)) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (reg_b(R_CL) - 1));
				}
				else {
					cpu.eflags.carry_flag = MEM_R(addr);
					rst = MEM_R(addr);
				}
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("shr" str(SUFFIX) " %%cl,%s", ModR_M_asm);
			}
			break;
		case 7:
			if(m.mod == 3) {
				if(REG(m.R_M)) {
					cpu.eflags.carry_flag = REG(m.R_M) >> (reg_b(R_CL) - 1);
				}
				else {
					cpu.eflags.carry_flag = REG(m.R_M);
				}
				rst = REG(m.R_M) >> reg_b(R_CL);
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("sar" str(SUFFIX) " %%cl,%%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				if(MEM_R(addr)) {
					cpu.eflags.carry_flag = MEM_R(addr) >> (reg_b(R_CL) - 1);
				}
				else {
					cpu.eflags.carry_flag = MEM_R(addr);
				}
				rst = MEM_R(addr) >> reg_b(R_CL);
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("sar" str(SUFFIX) " %%cl,%s", ModR_M_asm);
			}
			break;
		default:
			printf("invalid shift operator(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}

	return 1 + len;
}

make_helper(concat(shift_rm_i8_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	uint8_t imm = 0;
	int len = 1;
	switch(m.opcode) {
		case 4:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, 1) & 0x1f;
				if(imm) {
					cpu.eflags.carry_flag = REG(m.R_M) >> ((DATA_BYTE << 3) - imm);
				}
				else {
					cpu.eflags.carry_flag = MSB(REG(m.R_M));
				}
				rst = REG(m.R_M) << imm;
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("shl" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, 1) & 0x1f;
				if(imm) {
					cpu.eflags.carry_flag = MEM_R(addr) >> ((DATA_BYTE << 3) - imm);
				}
				else {
					cpu.eflags.carry_flag = MSB(MEM_R(addr));
				}
				rst = MEM_R(addr) << imm;
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("shl" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 5:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, 1) & 0x1f;
				if(imm) {
					cpu.eflags.carry_flag = REG(m.R_M) >> (imm - 1);
					rst = (REG(m.R_M) >> imm) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (imm - 1));
				}
				else {
					cpu.eflags.carry_flag = REG(m.R_M);
					rst = REG(m.R_M);
				}
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("shr" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, 1) & 0x1f;
				if(imm) {
					cpu.eflags.carry_flag = MEM_R(addr) >> (imm - 1);
					rst = (MEM_R(addr) >> imm) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (imm - 1));
				}
				else {
					cpu.eflags.carry_flag = MEM_R(addr);
					rst = MEM_R(addr);
				}
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("shr" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 7:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, 1) & 0x1f;
				if(REG(m.R_M)) {
					cpu.eflags.carry_flag = REG(m.R_M) >> (imm - 1);
				}
				else {
					cpu.eflags.carry_flag = REG(m.R_M);
				}
				rst = REG(m.R_M) >> imm;
				REG(m.R_M) = rst;

				logical_flags(rst);

				print_asm("sar" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, 1) & 0x1f;
				if(MEM_R(addr)) {
					cpu.eflags.carry_flag = MEM_R(addr) >> (imm - 1);
				}
				else {
					cpu.eflags.carry_flag = MEM_R(addr);
				}
				rst = MEM_R(addr) >> imm;
				MEM_W(addr, rst);

				logical_flags(rst);

				print_asm("sar" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		default:
			printf("invalid shift operator(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}

	return 2 + len;
}

#undef logical_shift
#include "exec/template-end.h"
