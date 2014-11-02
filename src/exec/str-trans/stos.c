#include "exec/helper.h"

#define DATA_BYTE 1
#include "stos-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "stos-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "stos-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(stos_v) {
	return (suffix == 'l' ? stosl(eip) : stosw(eip));
}
