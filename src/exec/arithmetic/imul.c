#include "exec/helper.h"

#define DATA_BYTE 2
#include "imul-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "imul-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(imul_r_rm_v) {
	return (suffix == 'l' ? imul_r_rm_l(eip) : imul_r_rm_w(eip));
}

make_helper(imul_r_rm_i_v) {
	return (suffix == 'l' ? imul_r_rm_i_l(eip) : imul_r_rm_i_w(eip));
}

make_helper(imul_r_rm_i8_v) {
	return (suffix == 'l' ? imul_r_rm_i8_l(eip) : imul_r_rm_i8_w(eip));
}

