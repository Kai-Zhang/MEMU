#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define neg_flags(rst) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.auxiliary_flag = (rst) >> 4; \
	cpu.eflags.carry_flag = ((rst) != 0); \
	cpu.eflags.overflow_flag = ((DATA_TYPE_S)(-(rst)) >= 0) && ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

#define logical_flags(rst) \
	cpu.eflags.sign_flag = ((DATA_TYPE_S)(rst) < 0); \
	cpu.eflags.zero_flag = ((rst) == 0); \
	cpu.eflags.parity_flag = 0; \
	while (rst) { \
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag; \
		(rst) = (rst) & ((rst) - 1); \
	}

make_helper(concat(test_a_i_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE_S rst = REG(R_EAX) & imm;

	logical_flags(rst);

	print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}

make_helper(concat(test_rm_i_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0;
	DATA_TYPE_S rst = 0;
	int len = 1;
	switch(m.opcode == 0) {
		case 0:
			if(m.mod == 3) {
				imm = instr_fetch(eip + 2, DATA_BYTE);
				rst = REG(m.R_M) & imm;

				logical_flags(rst);

				print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				imm = instr_fetch(eip + 1 + len, DATA_BYTE);
				rst = REG(m.R_M) & imm;

				logical_flags(rst);

				print_asm("test" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
			}
			break;
		case 2:
			if(m.mod == 3) {
				DATA_TYPE rst = ~REG(m.R_M);
				REG(m.R_M) = rst;

				print_asm("not %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE rst = ~MEM_R(addr);
				MEM_W(addr, rst);

				print_asm("not" str(SUFFIX) " %s", ModR_M_asm);
			}

			return 1;
		case 3:
			if(m.mod == 3) {
				DATA_TYPE rst = -REG(m.R_M);
				REG(m.R_M) = rst;

				neg_flags(rst);
				
				print_asm("neg %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE rst = -MEM_R(addr);
				MEM_W(addr, rst);

				neg_flags(rst);

				print_asm("neg" str(SUFFIX) " %s", ModR_M_asm);
			}

			return 1;
		case 4:
			if(m.mod == 3) {
				uint64_t rst = (uint64_t)REG(m.R_M) * (uint64_t)REG(R_EAX);
#if DATA_BYTE == 1
				reg_w(R_AX) = rst;
				cpu.eflags.carry_flag = (reg_b(R_AH) != 0);
				cpu.eflags.overflow_flag = cpu.eflags.carry_flag;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rst >> (DATA_BYTE << 3);
				cpu.eflags.carry_flag = (REG(R_EDX) != 0);
				cpu.eflags.overflow_flag = (REG(R_EDX) != 0);
#endif
				
				print_asm("mul %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				uint64_t rst = (uint64_t)MEM_R(addr) * (uint64_t)REG(R_EAX);
#if DATA_BYTE == 1
				reg_w(R_AX) = rst;
				cpu.eflags.carry_flag = (reg_b(R_AH) != 0);
				cpu.eflags.overflow_flag = cpu.eflags.carry_flag;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rst >> (DATA_BYTE << 3);
				cpu.eflags.carry_flag = (REG(R_EDX) != 0);
				cpu.eflags.overflow_flag = (REG(R_EDX) != 0);
#endif

				print_asm("mul %s", ModR_M_asm);
			}

			return 1 + len;
		case 5:
			if(m.mod == 3) {
				int64_t rst = (int64_t)(DATA_TYPE_S)REG(m.R_M) * (int64_t)(DATA_TYPE_S)REG(R_EAX);
#if DATA_BYTE == 1
				reg_w(R_AX) = rst;
				cpu.eflags.carry_flag = ((int16_t)(int8_t)reg_b(R_AL) != (int16_t)reg_w(R_AX));
				cpu.eflags.overflow_flag = cpu.eflags.carry_flag;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rst >> (DATA_BYTE << 3);
				cpu.eflags.carry_flag = ((int64_t)(DATA_TYPE_S)REG(R_EAX) >> (DATA_BYTE << 3)) == ((int64_t)(DATA_TYPE_S)REG(R_EDX));
				cpu.eflags.overflow_flag = cpu.eflags.carry_flag;
#endif
				
				print_asm("imul %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				int64_t rst = (int64_t)MEM_R(addr) * (int64_t)REG(R_EAX);
#if DATA_BYTE == 1
				reg_w(R_AX) = rst;
				cpu.eflags.carry_flag = ((int16_t)(int8_t)reg_b(R_AL) != (int16_t)reg_w(R_AX));
				cpu.eflags.overflow_flag = cpu.eflags.carry_flag;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rst >> (DATA_BYTE << 3);
				cpu.eflags.carry_flag = ((int64_t)(DATA_TYPE_S)REG(R_EAX) >> (DATA_BYTE << 3)) == ((int64_t)(DATA_TYPE_S)REG(R_EDX));
				cpu.eflags.overflow_flag = cpu.eflags.carry_flag;
#endif

				print_asm("imul %s", ModR_M_asm);
			}

			return 1 + len;
		case 6:
			if(m.mod == 3) {
				DATA_TYPE rst = REG(R_EAX) / REG(m.R_M);
				DATA_TYPE rem = REG(R_EAX) % REG(m.R_M);
#if DATA_BYTE == 1
				reg_b(R_AL) = rst;
				reg_b(R_AH) = rem;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rem;
#endif
				
				print_asm("div %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE rst = REG(R_EAX) / MEM_R(addr);
				DATA_TYPE rem = REG(R_EAX) % MEM_R(addr);
#if DATA_BYTE == 1
				reg_b(R_AL) = rst;
				reg_b(R_AH) = rem;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rem;
#endif

				print_asm("div %s", ModR_M_asm);
			}

			return 1 + len;
		case 7:
			if(m.mod == 3) {
				DATA_TYPE_S rst = (DATA_TYPE_S)REG(R_EAX) / (DATA_TYPE_S)REG(m.R_M);
				DATA_TYPE_S rem = (DATA_TYPE_S)REG(R_EAX) % (DATA_TYPE_S)REG(m.R_M);
#if DATA_BYTE == 1
				reg_b(R_AL) = rst;
				reg_b(R_AH) = rem;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rem;
#endif
				
				print_asm("idiv %%%s", REG_NAME(m.R_M));
			}
			else {
				swaddr_t addr;
				len = read_ModR_M(eip + 1, &addr);
				DATA_TYPE_S rst = (DATA_TYPE_S)REG(R_EAX) / (DATA_TYPE_S)MEM_R(addr);
				DATA_TYPE_S rem = (DATA_TYPE_S)REG(R_EAX) % (DATA_TYPE_S)MEM_R(addr);
#if DATA_BYTE == 1
				reg_b(R_AL) = rst;
				reg_b(R_AH) = rem;
#else
				REG(R_EAX) = rst;
				REG(R_EDX) = rem;
#endif

				print_asm("idiv %s", ModR_M_asm);
			}

			return 1 + len;

		default:
			printf("invalid `test'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
			assert(0);
	}
	
	cpu.eflags.carry_flag = 0;
	cpu.eflags.overflow_flag = 0;

	return 1 + len + DATA_BYTE;
}

make_helper(concat(test_rm_r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S rst = 0;
	int len = 1;
	if(m.mod == 3) {
		rst = REG(m.R_M) & REG(m.reg);

		logical_flags(rst);

		print_asm("test" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = REG(m.R_M) & REG(m.reg);

		logical_flags(rst);

		print_asm("test" str(SUFFIX) " %%%s,%s", REG_NAME(m.R_M), ModR_M_asm);
	}
	
	cpu.eflags.carry_flag = 0;
	cpu.eflags.overflow_flag = 0;

	return 1 + len;
}

#include "exec/template-end.h"
