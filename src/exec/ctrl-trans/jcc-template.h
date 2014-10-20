#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(concat(concat(j, CC), _), SUFFIX)) {
	DATA_TYPE_S offset = instr_fetch(eip + 1, DATA_BYTE);
	printf(str(COND));
	if (COND) {
		cpu.eip += (int32_t)offset;
#if DATA_BYTE == 2
		cpu.eip &= 0xffff;
#endif
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
