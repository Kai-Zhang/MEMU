#include "exec/helper.h"
#include "exec/template-start.h"
#include "exec/func-stack.h"
#include "cpu/modrm.h"


make_helper(concat(call_rel_, SUFFIX)) {
	DATA_TYPE_S offset = instr_fetch(eip + 1, DATA_BYTE);
	reg_l(R_ESP) -= DATA_BYTE;
	MEM_W(reg_l(R_ESP), eip + 1 + DATA_BYTE);

	func_stack.fstack[++func_stack.top] = eip + 1 + DATA_BYTE;

	cpu.eip += offset;
#if DATA_BYTE == 2
	cpu.eip &= 0xffff;
#endif

	print_asm("call %x", cpu.eip + 1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
