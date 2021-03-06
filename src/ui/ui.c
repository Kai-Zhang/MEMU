#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "exec/func-stack.h"

#include "memu.h"

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int memu_state = END;

struct frame_stack func_stack;

void cpu_exec(uint32_t);
void restart();
uint32_t expr_calc(char *, bool *);
uint32_t dram_read(hwaddr_t, size_t);

/* We use the readline library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(memu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

/* This function will be called when you press <C-c>. And it will return to 
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */
static void control_C(int signum) {
	if(memu_state == RUNNING) {
		memu_state = INT;
	}
}

void init_signal() {
	/* Register a signal handler. */
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}

static void cmd_c() {
	if(memu_state == END) {
		puts("The Program does not start. Use 'r' command to start the program.");
		return;
	}

	memu_state = RUNNING;
	cpu_exec(-1);
	if(memu_state != END) { memu_state = STOP; }
}

static void cmd_r() {
	if(memu_state != END) { 
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			fflush(stdout);
			scanf(" %c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
			}
		}
	}

restart_:
	restart();
	memu_state = STOP;
	cmd_c();
}

static void cmd_si(int step) {
	if(memu_state == END) {
		restart();
	}

	memu_state = RUNNING;
	cpu_exec(step);
	if(memu_state != END) {
		memu_state = STOP;
	}
}

static void cmd_i(char* arg) {
	if(!arg) {
		puts("\"info\" must be followed by the name of an info command.");
		puts("List of info subcommands:\n");
		puts("info registers		-- List of integer registers and their contents");
		puts("info breakpoints		-- List of the information of breakpoints");
	} else if(strcmp(arg, "r") == 0) {
		printf("eax\t0x%08x\t%d\n", reg_l(R_EAX), reg_l(R_EAX));
		printf("ecx\t0x%08x\t%d\n", reg_l(R_ECX), reg_l(R_ECX));
		printf("edx\t0x%08x\t%d\n", reg_l(R_EDX), reg_l(R_EDX));
		printf("ebx\t0x%08x\t%d\n", reg_l(R_EBX), reg_l(R_EBX));
		printf("esp\t0x%08x\t0x%08x\n", reg_l(R_ESP), reg_l(R_ESP));
		printf("ebp\t0x%08x\t0x%08x\n", reg_l(R_EBP), reg_l(R_EBP));
		printf("esi\t0x%08x\t%d\n", reg_l(R_ESI), reg_l(R_ESI));
		printf("edi\t0x%08x\t%d\n", reg_l(R_EDI), reg_l(R_EDI));
		printf("eip\t0x%08x\t0x%08x\n", cpu.eip, cpu.eip);
		printf("eflags\t0x%x\t\t[", cpu.eflags.value);
		int i = B_CF;
		for( ; i <= B_VM; ++i) {
			if((i != 1) && ((cpu.eflags.value >> i) & 1)) {
				printf(" %s", regf[i]);
			}
		}
		puts(" ]");
	} else if(strcmp(arg, "b") == 0) {
		show_bp();
	} else {
		printf("No explicit command for \"%s\", try \"info\" for more details.\n", arg);
	}
}

static void cmd_x(char* amount_str, char* address_str) {
	if(!amount_str) {
		puts("Argument required (size of memory and starting display address).");
		return;
	}
	int amount = strtol(amount_str, NULL, 0);
	if(!address_str) {
		puts("Syntax error: unrecongized address.");
		return;
	}
	bool valid_addr = true;
	int address = expr_calc(address_str, &valid_addr);
	if(!valid_addr) {
		puts("Invalid address expression");
		return;
	}

	int blank_count = address & 0xf;
	int column = 0;
	while(amount > 0) {
		printf("%08x:  ", address & ~0xf);
		while(blank_count-- > 0) { printf("   "); ++ column; }
		for(; column < 0x10 && amount > 0; ++ column, -- amount) {
			printf("%02x ", dram_read(address ++, 1));
		}
		putchar('\n');
		column = 0;
	}
}

static void cmd_b(char *expr) {
	if(!expr) {
		puts("Argument required (breakpoint address).");
		return;
	}

	bool valid_expr = true;
	swaddr_t bp_addr = expr_calc(expr, &valid_expr);
	if(!valid_expr) {
		puts("Invalid expression.");
		return;
	}
	BP* newbp = new_bp();
	newbp->watch = false;
	newbp->hit_time = 0;
	newbp->address = bp_addr;
	newbp->replaced = swaddr_read(bp_addr, 1);
	swaddr_write(bp_addr, 1, INT3_CODE);
}

static void cmd_d(char *bp_no) {
	if(!bp_no) {
		puts("Argument required (breakpoint number).");
		return;
	}
	char *rest_str = NULL;
	int no = strtol(bp_no, &rest_str, 0);
	if(strcmp(rest_str, "")) {
		printf("warning: bad breakpoint number at or near '%s'\n", bp_no);
		return;
	}
	BP* freebp = get_bp(no);
	if(!freebp) {
		printf("No breakpoint number %d.\n", no);
		return;
	}
	free_bp(freebp);
}

static void cmd_p(char *expr) {
	if(!expr) {
		puts("An expression is expected.");
		return;
	}
	bool success = true;
	uint32_t rst = expr_calc(expr, &success);
	if(!success) {
		puts("A syntax error in expression.");
		return;
	}
	printf("Result = %d\n", rst);
}

static void cmd_w(char *expr) {
	if(!expr) {
		puts("Argument required (expression to compute).");
		return;
	}

	bool success = true;
	uint32_t rst = expr_calc(expr, &success);
	if(!success) {
		puts("Invalid expression.");
		return;
	}

	char *watch = (char *)malloc(strlen(expr) + 1);
	strcpy(watch, expr);

	BP* newbp = new_bp();
	newbp->watch = true;
	newbp->hit_time = 0;
	newbp->pre_rst = rst;
	newbp->watch_expr = watch;
}

static void cmd_bt() {
	if(!cpu.eip) {
		puts("No stack.");
		return;
	}
	printf("#0\tfunc:%s\n", func_name(cpu.eip));
	int t = func_stack.top;
	for(; t >= 0; --t) {
		printf("#%d\t0x%08x in func:%s\n",
				func_stack.top - t + 1, func_stack.fstack[t], func_name(current_func(func_stack.fstack[t])));
	}
}

void main_loop() {
	char *cmd;
	while(1) {
		cmd = rl_gets();
		char *p = strtok(cmd, " ");

		if(p == NULL) { continue; }

		if(strcmp(p, "c") == 0) { cmd_c(); }
		else if(strcmp(p, "r") == 0) { cmd_r(); }
		else if(strcmp(p, "q") == 0) { return; }


		/* TODO: Add more commands */

		else if(strcmp(p, "si") == 0) {
			char* sstep = strtok(NULL, " ");
			if(!sstep) {
				cmd_si(1);
			} else {
				int step = atoi(sstep);
				cmd_si(step ? step : 1);
			}
		}
		else if(strcmp(p, "info") == 0 || strcmp(p, "i") == 0) { cmd_i(strtok(NULL, " ")); }
		else if(strcmp(p, "x") == 0) { 
			char* amount = strtok(NULL, " ");
			char* address = strtok(NULL, "\0");
			cmd_x(amount, address);
		}
		else if(strcmp(p, "b") == 0) {
			char* expr = strtok(NULL, "\0");
			cmd_b(expr);
		}
		else if(strcmp(p, "d") == 0) {
			char* bp_no = strtok(NULL, " ");
			cmd_d(bp_no);
		}
		else if(strcmp(p, "p") == 0) {
			char *expr = strtok(NULL, "\0");
			cmd_p(expr);
		}
		else if(strcmp(p, "w") == 0) {
			char *expr = strtok(NULL, "\0");
			cmd_w(expr);
		}
		else if(strcmp(p, "bt") == 0) {
			cmd_bt();
		}


		else { printf("Unknown command '%s'\n", p); }
	}
}
