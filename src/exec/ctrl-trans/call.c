#include "exec/helper.h"

#define DATA_BYTE 2
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "call-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(call_rel_v) {
	return (suffix == 'l' ? call_rel_l(eip) : call_rel_w(eip));
}

