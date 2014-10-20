#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(call_rel_, SUFFIX)) {
	DATA_TYPE_S offset = instr_fetch(eip + 1, DATA_BYTE);
	reg_l(R_ESP) -= DATA_BYTE;
	MEM_W(reg_l(R_ESP), eip + DATA_BYTE);

	cpu.eip += offset;
#if DATA_BYTE == 2
	cpu.eip &= 0xffff;
#endif

	print_asm("call $0x%x", offset);
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
