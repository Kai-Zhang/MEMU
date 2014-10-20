#include "exec/helper.h"

#define WRITE_BACK

#define DATA_BYTE 1
#define OP_NAME add
#define OP_SYMBOL +
#define CARRY 0
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME adc
#define OP_SYMBOL +
#define CARRY cpu.eflags.carry_flag
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME sub
#define OP_SYMBOL -
#define CARRY 0
#define CARRY_COND >
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME sbb
#define OP_SYMBOL -
#define CARRY cpu.eflags.carry_flag
#define CARRY_COND >
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#include "addsub-rm-i.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#define OP_NAME add
#define OP_SYMBOL +
#define CARRY 0
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME adc
#define OP_SYMBOL +
#define CARRY cpu.eflags.carry_flag
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME sub
#define OP_SYMBOL -
#define CARRY 0
#define CARRY_COND >
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME sbb
#define OP_SYMBOL -
#define CARRY cpu.eflags.carry_flag
#define CARRY_COND >
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#include "addsub-rm-i.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#define OP_NAME add
#define OP_SYMBOL +
#define CARRY 0
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME adc
#define OP_SYMBOL +
#define CARRY cpu.eflags.carry_flag
#define CARRY_COND >
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME sub
#define OP_SYMBOL -
#define CARRY 0
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME sbb
#define OP_SYMBOL -
#define CARRY cpu.eflags.carry_flag
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME

#include "addsub-rm-i.h"
#undef DATA_BYTE

#undef WRITE_BACK

#define DATA_BYTE 1
#define OP_NAME cmp
#define OP_SYMBOL -
#define CARRY 0
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef DATA_BYTE

#define DATA_BYTE 2
#define OP_NAME cmp
#define OP_SYMBOL -
#define CARRY 0
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef DATA_BYTE

#define DATA_BYTE 4
#define OP_NAME cmp
#define OP_SYMBOL -
#define CARRY 0
#define CARRY_COND <
#include "add-sub-template.h"
#undef CARRY_COND
#undef CARRY
#undef OP_SYMBOL
#undef OP_NAME
#undef DATA_BYTE

