#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(jmp_rel_, SUFFIX)) {
	DATA_TYPE_S offset = instr_fetch(eip + 1, DATA_BYTE);
	cpu.eip += offset;
#if DATA_BYTE == 2
	cpu.eip &= 0xffff;
#endif

	print_asm("jmp %x", cpu.eip + 1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
