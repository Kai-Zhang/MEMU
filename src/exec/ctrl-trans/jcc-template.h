#include "exec/helper.h"
#include "exec/template-start.h"

#include "nemu.h"

make_helper(concat(je_, SUFFIX)) {
	DATA_TYPE_S offset = 0;
	if (cpu.eflags.zero_flag) {
		offset = instr_fetch(eip + 1, DATA_BYTE);
		cpu.eip += offset;
	}

	print_asm("je $0x%x", eip + 1 + DATA_BYTE + offset);
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
