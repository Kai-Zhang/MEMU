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
	if(m.reg == 7) {
		DATA_TYPE imm = instr_fetch(eip + 2, DATA_BYTE);
		uint32_t flags = 0;
		asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(REG(R_EAX)), "r"(imm));
		cpu.eflags.value = flags;
	} else {
		printf("invalid cmp(eip = 0x%08x)\n", eip);
		assert(0);
	}

	print_asm("cmp" str(SUFFIX) " %%%s,%s", REG_NAME(R_EAX), ModR_M_asm);
	return 2 + DATA_BYTE;
}
	
make_helper(concat(cmp_rm2i8_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.reg == 7) {
		DATA_TYPE imm = instr_fetch(eip + 2, 1);
		uint32_t flags = 0;
		asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(REG(R_EAX)), "r"(imm));
		cpu.eflags.value = flags;
	} else {
		printf("invalid cmp(eip = 0x%08x)\n", eip);
		assert(0);
	}

	print_asm("cmp" str(SUFFIX) " %%%s,%s", REG_NAME(R_EAX), ModR_M_asm);
	return 3;
}

#include "exec/template-end.h"
