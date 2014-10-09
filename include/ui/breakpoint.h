#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	struct breakpoint *next;

	/* TODO: Add more members if necessary */
	uint8_t replaced;
	swaddr_t address;
	int hit_time;

} BP;

extern bool hit_bp;

extern BP* new_bp();
extern void free_bp(BP*);
extern BP* find_bp(int);
extern void show_bp();

#endif
