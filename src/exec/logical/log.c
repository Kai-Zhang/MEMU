#include "exec/helper.h"

#define DATA_BYTE 1
#define OP_NAME and
#define OP_SYMBOL &
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME or
#define OP_SYMBOL |
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME xor
#define OP_SYMBOL ^
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME
#undef DATA_BYTE

#define DATA_BYTE 2
#define OP_NAME and
#define OP_SYMBOL &
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME or
#define OP_SYMBOL |
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME xor
#define OP_SYMBOL ^
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME
#undef DATA_BYTE

#define DATA_BYTE 4
#define OP_NAME and
#define OP_SYMBOL &
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME or
#define OP_SYMBOL |
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME

#define OP_NAME xor
#define OP_SYMBOL ^
#include "log-template.h"
#undef OP_SYMBOL
#undef OP_NAME
#undef DATA_BYTE

