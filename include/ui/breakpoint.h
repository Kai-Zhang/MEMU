#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	struct breakpoint *next;
	bool watch;

	/* TODO: Add more members if necessary */
	union {
		struct {
			uint8_t replaced;
			swaddr_t address;
		};

		struct {
			char *watch_expr;
			uint32_t pre_rst;
		};
	};

	int hit_time;

} BP;

extern bool hit_bp;

extern BP* new_bp();
extern void free_bp(BP*);
extern BP* get_bp(int);
extern int find_bp(swaddr_t);
extern void show_bp();
extern void refresh_bp();
extern void instr_recover(swaddr_t);
extern bool check_watchpoint();

#endif
