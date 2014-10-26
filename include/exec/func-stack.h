#ifndef __FUNC_STACK_H__
#define __FUNC_STACK_H__
#include "common.h"

struct frame_stack {
	swaddr_t fstack[8 * 1024];
	int top;
};

extern struct frame_stack func_stack;

swaddr_t current_func(swaddr_t);
char *func_name(swaddr_t);

#endif
