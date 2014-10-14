#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(cmp_a2i_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	uint32_t flags = 0;
	asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(REG(R_EAX)), "r"(imm));
	cpu.eflags.value = flags;

	print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}

make_helper(concat(cmp_rm2i_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0;
	int len = 1;
	uint32_t flags = 0x2;
	if(m.reg == 7) {
		if(m.mod == 3) {
			imm = instr_fetch(eip + 2, DATA_BYTE);
			asm volatile ("cmp" str(SUFFIX) " %1, %2;"
				"pushfd; popl %0" : "=r"(flags)
				: "r"(REG(m.R_M)), "r"(imm));

			print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
		}
		else {
			swaddr_t addr;
			len = read_ModR_M(eip + 1, &addr);
			imm = instr_fetch(eip + 1 + len, DATA_BYTE);
			asm volatile ("cmp" str(SUFFIX) " %1, %2;"
				"pushfd; popl %0" : "=r"(flags)
				: "r"(MEM_R(addr)), "r"(imm));
			print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		}
	}
	else {
		printf("invalid cmp(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}
	
	cpu.eflags.value = flags;

	return 1 + len + DATA_BYTE;
}

make_helper(concat(cmp_rm2i8_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0;
	int len = 1;
	uint32_t flags = 0x2;
	if(m.reg == 7) {
		if(m.mod == 3) {
			imm = instr_fetch(eip + 2, 1);
			asm volatile ("cmp" str(SUFFIX) " %1, %2;"
				"pushfd; popl %0" : "=r"(flags)
				: "r"(REG(m.R_M)), "r"(imm));
			print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
		}
		else {
			swaddr_t addr;
			len = read_ModR_M(eip + 1, &addr);
			imm = instr_fetch(eip + 1 + len, 1);
			asm volatile ("cmp" str(SUFFIX) " %1, %2;"
				"pushfd; popl %0" : "=r"(flags)
				: "r"(MEM_R(addr)), "r"(imm));
			print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		}
	}
	else {
		printf("invalid cmp(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}
	
	cpu.eflags.value = flags;

	return 2 + len;
}

make_helper(concat(cmp_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	uint32_t flags = 0x2;
	if(m.mod == 3) {
		asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(REG(m.R_M)), "r"(REG(m.reg)));
		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(MEM_R(addr)), "r"(REG(m.reg)));
		print_asm("cmp" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	
	cpu.eflags.value = flags;

	return 1 + len;
}

make_helper(concat(cmp_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	uint32_t flags = 0x2;
	if(m.mod == 3) {
		asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(REG(m.reg)), "r"(REG(m.R_M)));
		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(REG(m.reg)), "r"(MEM_R(addr)));
		print_asm("cmp" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	}
	
	cpu.eflags.value = flags;

	return 1 + len;
}

#include "exec/template-end.h"
