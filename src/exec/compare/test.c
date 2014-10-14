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

make_helper(test_a2i_v) {
	return (suffix == 'l' ? test_a2i_l(eip) : test_a2i_w(eip));
}

make_helper(test_rm2i_v) {
	return (suffix == 'l' ? test_rm2i_l(eip) : test_rm2i_w(eip));
}

make_helper(test_rm2r_v) {
	return (suffix == 'l' ? test_rm2r_l(eip) : test_rm2r_w(eip));
}

