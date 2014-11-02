#include "exec/helper.h"
#include "movs.h"

extern char suffix;
extern int exec(swaddr_t);

make_helper(rep) {
	int len = 1;
	if(!reg_l(R_ECX)) {
		switch(swaddr_read(eip + 1, 1)) {
			case 0xa4:
				print_asm("rep movsb");				break;
			case 0xa5:
				print_asm("rep movs%c", suffix);	break;
			case 0xaa:
				print_asm("rep stosb");				break;
			case 0xab:
				print_asm("rep stos%c", suffix);	break;
			default:	
				printf("Undefined rep operator(eip = 0x%08x)\n", cpu.eip);
				assert(0);
		};
	}
	else {
		for (; reg_l(R_ECX) > 0; --reg_l(R_ECX)) {
			len = exec(eip + 1);
		}
		static char rep_prim[40];
		strncpy(rep_prim, assembly, 40);
		print_asm("rep %s", rep_prim);
	}
	return 1 + len;
}
