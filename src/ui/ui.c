#include "ui/ui.h"

#include "nemu.h"

#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int nemu_state = END;

void cpu_exec(uint32_t);
void restart();

/* We use the readline library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

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
	if(nemu_state == RUNNING) {
		nemu_state = INT;
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
	if(nemu_state == END) {
		puts("The Program does not start. Use 'r' command to start the program.");
		return;
	}

	nemu_state = RUNNING;
	cpu_exec(-1);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_r() {
	if(nemu_state != END) { 
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			scanf("%c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
			}
		}
	}

restart_:
	restart();
	nemu_state = STOP;
	cmd_c();
}

static void cmd_si(int step) {
	if(nemu_state == END) {
		restart();
	}

	nemu_state = RUNNING;
	cpu_exec(step);
	if(nemu_state != END) {
		nemu_state = STOP;
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
		else if(strcmp(p, "si") == 0) {
			char* step = strtok(NULL, " ");
			if(!step) {
				cmd_si(1);
			} else {
				int offset = atoi(step);
				cmd_si(offset);
			}
		}
		else if(strcmp(p, "info") == 0 || strcmp(p, "i") == 0) {
			char* arg = strtok(NULL, " ");
			if (!arg) {
				puts("\"info\" must be followed by the name of an info command.");
				puts("List of info subcommands:\n");
				puts("info registers		-- List of integer registers and their contents");
			}
			if(strcmp(arg, "registers") == 0 || strcmp(arg, "r") == 0) {
				printf("eax\t%8x\t%d", reg_l(R_EAX), reg_l(R_EAX));
				printf("ecx\t%8x\t%d", reg_l(R_ECX), reg_l(R_ECX));
				printf("edx\t%8x\t%d", reg_l(R_EDX), reg_l(R_EDX));
				printf("ebx\t%8x\t%d", reg_l(R_EBX), reg_l(R_EBX));
				printf("esp\t%8x\t%d", reg_l(R_ESP), reg_l(R_ESP));
				printf("ebp\t%8x\t%d", reg_l(R_EBP), reg_l(R_EBP));
				printf("esi\t%8x\t%d", reg_l(R_ESI), reg_l(R_ESI));
				printf("edi\t%8x\t%d", reg_l(R_EDI), reg_l(R_EDI));
				printf("eip\t%8x\t%d", cpu.eip, cpu.eip);
			}
		}

		/* TODO: Add more commands */

		else { printf("Unknown command '%s'\n", p); }
	}
}
