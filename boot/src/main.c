#include "trap.h"

extern void loader();

void init() {
	/* perform the remaining initializations */

	/* load the program, and jump to it */
	loader();

	/* should never reach here */
	memu_assert(0);
}
