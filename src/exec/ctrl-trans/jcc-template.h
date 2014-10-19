#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(concat(concat(j, CC), _), SUFFIX)) {
	DATA_TYPE_S offset = 0;
	if (COND) {
		offset = instr_fetch(eip + 1, DATA_BYTE);
		cpu.eip += (int32_t)offset;
	}

	print_asm("j" str(CC) " %x", eip + 1 + DATA_BYTE + offset);
	return 1 + DATA_BYTE;
}

#if DATA_BYTE == 4

extern char suffix;

make_helper(concat(concat(j, CC), _v)) {
	return (suffix == 'l' ? concat(concat(j, CC), _l)(eip) : concat(concat(j, CC), _w)(eip));
}

#endif

#include "exec/template-end.h"
