#include "exec/helper.h"

#define DATA_BYTE 1
#include "shift-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "shift-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "shift-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(shift_rm_1_v) {
	return (suffix == 'l' ? shift_rm_1_l(eip) : shift_rm_1_w(eip));
}

make_helper(shift_rm_c_v) {
	return (suffix == 'l' ? shift_rm_c_l(eip) : shift_rm_c_w(eip));
}

make_helper(shift_rm_i8_v) {
	return (suffix == 'l' ? shift_rm_i8_l(eip) : shift_rm_i8_w(eip));
}
