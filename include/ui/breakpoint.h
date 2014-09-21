#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	struct breakpoint *next;

	/* TODO: Add more members if necessary */


} BP;

extern BP* new_bp(void);
extern void free_bp(BP* bp);

#endif
