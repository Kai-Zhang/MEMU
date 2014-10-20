#include "exec/helper.h"
#include "cpu/modrm.h"

#define FLAG(x) cpu.eflags.x

#define COND a
#define FLAGS !(FLAG(carry_flag) || FLAG(zero_flag))
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND ae
#define FLAGS !FLAG(carry_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND b
#define FLAGS FLAG(carry_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND be
#define FLAGS FLAG(carry_flag) || FLAG(zero_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND e
#define FLAGS FLAG(zero_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND g
#define FLAGS !FLAG(zero_flag) || (FLAG(sign_flag) == FLAG(overflow_flag))
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND ge
#define FLAGS FLAG(sign_flag) == FLAG(overflow_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND l
#define FLAGS FLAG(sign_flag) != FLAG(overflow_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND le
#define FLAGS FLAG(zero_flag) || (FLAG(sign_flag) != FLAG(overflow_flag))
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND ne
#define FLAGS !FLAG(zero_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND no
#define FLAGS !FLAG(overflow_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND np
#define FLAGS !FLAG(parity_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND ns
#define FLAGS !FLAG(sign_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND o
#define FLAGS FLAG(overflow_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND p
#define FLAGS FLAG(parity_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

#define COND s
#define FLAGS FLAG(sign_flag)
#include "setcc-template.h"
#undef FLAGS
#undef COND

