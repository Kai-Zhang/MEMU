#include "exec/helper.h"
#include "cpu/reg.h"

make_helper(cld) {
	cpu.eflags.direction_flag = 0;
	print_asm("cld");
	return 1;
}

make_helper(std) {
	cpu.eflags.direction_flag = 1;
	print_asm("std");
	return 1;
}
