#include "exec/helper.h"

#define FLAG(x) cpu.eflags.x

#define DATA_BYTE 1
#define CC a
#define COND !(FLAG(carry_flag) || FLAG(zero_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ae
#define COND !FLAG(carry_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC b
#define COND FLAG(carry_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC be
#define COND FLAG(carry_flag) || FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC cxz
#define COND !reg_w(R_CX)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ecxz
#define COND !reg_l(R_ECX)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC e
#define COND FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC g
#define COND !FLAG(zero_flag) && (FLAG(sign_flag) == FLAG(overflow_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ge
#define COND FLAG(sign_flag) == FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC l
#define COND FLAG(sign_flag) != FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC le
#define COND FLAG(zero_flag) || (FLAG(sign_flag) != FLAG(overflow_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ne
#define COND !FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC no
#define COND !FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC np
#define COND !FLAG(parity_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ns
#define COND !FLAG(sign_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC o
#define COND FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC p
#define COND FLAG(parity_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC s
#define COND FLAG(sign_flag)
#include "jcc-template.h"
#undef COND
#undef CC
#undef DATA_BYTE

#define DATA_BYTE 2
#define CC a
#define COND !(FLAG(carry_flag) || FLAG(zero_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ae
#define COND !FLAG(carry_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC b
#define COND FLAG(carry_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC be
#define COND FLAG(carry_flag) || FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC e
#define COND FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC g
#define COND !FLAG(zero_flag) && (FLAG(sign_flag) == FLAG(overflow_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ge
#define COND FLAG(sign_flag) == FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC l
#define COND FLAG(sign_flag) != FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC le
#define COND FLAG(zero_flag) && (FLAG(sign_flag) != FLAG(overflow_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ne
#define COND !FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC no
#define COND !FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC np
#define COND !FLAG(parity_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ns
#define COND !FLAG(sign_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC o
#define COND FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC p
#define COND FLAG(parity_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC s
#define COND FLAG(sign_flag)
#include "jcc-template.h"
#undef COND
#undef CC
#undef DATA_BYTE

#define DATA_BYTE 4
#define CC a
#define COND !(FLAG(carry_flag) || FLAG(zero_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ae
#define COND !FLAG(carry_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC b
#define COND FLAG(carry_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC be
#define COND FLAG(carry_flag) || FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC e
#define COND FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC g
#define COND !FLAG(zero_flag) && (FLAG(sign_flag) == FLAG(overflow_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ge
#define COND FLAG(sign_flag) == FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC l
#define COND FLAG(sign_flag) != FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC le
#define COND FLAG(zero_flag) && (FLAG(sign_flag) != FLAG(overflow_flag))
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ne
#define COND !FLAG(zero_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC no
#define COND !FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC np
#define COND !FLAG(parity_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC ns
#define COND !FLAG(sign_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC o
#define COND FLAG(overflow_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC p
#define COND FLAG(parity_flag)
#include "jcc-template.h"
#undef COND
#undef CC

#define CC s
#define COND FLAG(sign_flag)
#include "jcc-template.h"
#undef COND
#undef CC
#undef DATA_BYTE

make_helper(jcxz_v) {
	return (suffix == 'l' ? jcxz_b(eip) : jecxz_b(eip));
}
