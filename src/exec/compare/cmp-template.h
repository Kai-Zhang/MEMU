#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(cmp_a2i_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	uint32_t flags = 0;
	asm volatile ("cmp" str(SUFFIX) " %1, %2;"
			"pushfd; popl %0" : "=r"(flags)
			: "r"(REG(R_EAX)), "r"(imm));
	*(uint32_t *)(&cpu.eflags) = flags;

	print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
