#include "exec/helper.h"

#define WRITE_BACK

#define DATA_BYTE 1
#define OP +
#define OP_NAME add
#define OP_SYMBOL '+'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP +
#define OP_NAME adc
#define OP_SYMBOL '+'
#define CARRY cpu.eflags.carry_flag
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP -
#define OP_NAME sub
#define OP_SYMBOL '-' 
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP -
#define OP_NAME sbb
#define OP_SYMBOL '-'
#define CARRY cpu.eflags.carry_flag
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#include "addsub-rm-i.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#define OP +
#define OP_NAME add
#define OP_SYMBOL '+'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP +
#define OP_NAME adc
#define OP_SYMBOL '+'
#define CARRY cpu.eflags.carry_flag
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP -
#define OP_NAME sub
#define OP_SYMBOL '-'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP -
#define OP_NAME sbb
#define OP_SYMBOL '-'
#define CARRY cpu.eflags.carry_flag
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#include "addsub-rm-i.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#define OP +
#define OP_NAME add
#define OP_SYMBOL '+'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP +
#define OP_NAME adc
#define OP_SYMBOL '+'
#define CARRY cpu.eflags.carry_flag
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP -
#define OP_NAME sub
#define OP_SYMBOL '-'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#define OP -
#define OP_NAME sbb
#define OP_SYMBOL '-'
#define CARRY cpu.eflags.carry_flag
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP

#include "addsub-rm-i.h"
#undef DATA_BYTE

#undef WRITE_BACK

#define DATA_BYTE 1
#define OP -
#define OP_NAME cmp
#define OP_SYMBOL '-'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP
#undef DATA_BYTE

#define DATA_BYTE 2
#define OP -
#define OP_NAME cmp
#define OP_SYMBOL '-'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP
#undef DATA_BYTE

#define DATA_BYTE 4
#define OP -
#define OP_NAME cmp
#define OP_SYMBOL '-'
#define CARRY 0
#include "add-sub-template.h"
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef OP
#undef DATA_BYTE

