#include "exec/helper.h"

#define DATA_BYTE 1
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "test-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(test_a_i_v) {
	return (suffix == 'l' ? test_a_i_l(eip) : test_a_i_w(eip));
}

make_helper(test_rm_i_v) {
	return (suffix == 'l' ? test_rm_i_l(eip) : test_rm_i_w(eip));
}

make_helper(test_rm_r_v) {
	return (suffix == 'l' ? test_rm_r_l(eip) : test_rm_r_w(eip));
}

