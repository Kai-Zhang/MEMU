#include "exec/helper.h"

#include "ui/ui.h"

#include "ui/breakpoint.h"

#include "nemu.h"

#include "escaped_instr.h"

make_helper(inv) {
	/* invalid opcode */

	uint8_t temp[8];
	((uint32_t *)temp)[0] = instr_fetch(eip, 4);
	((uint32_t *)temp)[1] = instr_fetch(eip + 4, 4);
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n", 
			eip, temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);

	assert(0);
}

make_helper(escape_code) {
	int instr_len = 0;
	switch(instr_fetch(eip + 1, 1)) {
		case 0x84:	instr_len = je_v(eip + 1);		break;
		case 0x90:	instr_len = seto(eip + 1);		break;
		case 0x91:	instr_len = setno(eip + 1);		break;
		case 0x92:	instr_len = setb(eip + 1);		break;
		case 0x93:	instr_len = setae(eip + 1);		break;
		case 0x94:	instr_len = sete(eip + 1);		break;
		case 0x95:	instr_len = setne(eip + 1);		break;
		case 0x96:	instr_len = setbe(eip + 1);		break;
		case 0x97:	instr_len = seta(eip + 1);		break;
		case 0x98:	instr_len = sets(eip + 1);		break;
		case 0x99:	instr_len = setns(eip + 1);		break;
		case 0x9a:	instr_len = setp(eip + 1);		break;
		case 0x9b:	instr_len = setnp(eip + 1);		break;
		case 0x9c:	instr_len = setl(eip + 1);		break;
		case 0x9d:	instr_len = setge(eip + 1);		break;
		case 0x9e:	instr_len = setle(eip + 1);		break;
		case 0x9f:	instr_len = setg(eip + 1);		break;
		case 0xaf:	instr_len = imul_r_rm_v(eip);	break;
		case 0xbe:	instr_len = movsx_v(eip + 1);	break;
		case 0xbf:	instr_len = movsx_w2l(eip + 1);	break;
		case 0xb6:	instr_len = movzx_v(eip + 1);	break;
		case 0xb7:	instr_len = movzx_w2l(eip + 1);	break;
		default:	instr_len = inv(eip);			break;
	}

	return 1 + instr_len;
}

make_helper(int3) {
	/* A breakpoint is hit! Do something here! */
	hit_bp = true;
	nemu_state = BREAK;
	printf("\nnemu: Hit Breakpoint %d at 0x%x\n", find_bp(cpu.eip), cpu.eip);

	++ (get_bp(find_bp(cpu.eip)))->hit_time;

	return 1;
}

make_helper(nemu_trap) {
	printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
	nemu_state = END;

	print_asm("nemu trap");
	return 1;
}
