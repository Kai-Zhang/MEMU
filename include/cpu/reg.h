#ifndef __CPU_REG_H__
#define __CPU_REG_H__

#include "common.h"

/* TODO: Re-organize the 'CPU_state' structure to match
 * the GPR encoding scheme in i386 instruction format.
 * For example, if we access reg_w(R_BX) we will get the 'bx' register;
 * if we access reg_b(R_CH), we will get the 'ch' register.
 * Hint: Use 'union'.
 * For more details about the GPR encoding scheme, see i386 manual.
 */

typedef union {
	union {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. 
	 * They match the register encoding scheme used in i386 instruction format.
	 * See i386 manual for more details.
	 */

	struct {
		uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

		swaddr_t eip;
		union {
			struct {
				uint32_t					: 14;
				uint32_t virutal_mode		: 1;
				uint32_t resume_flag		: 1;
				uint32_t					: 1;
				uint32_t nested_task		: 1;
				uint32_t IO_privilege		: 2;
				uint32_t overflow_flag		: 1;
				uint32_t direction_flag		: 1;
				uint32_t interrupt_flag		: 1;
				uint32_t trap_flag			: 1;
				uint32_t sign_flag			: 1;
				uint32_t zero_flag			: 1;
				uint32_t					: 1;
				uint32_t auxiliary_flag		: 1;
				uint32_t					: 1;
				uint32_t parity_flag		: 1;
				uint32_t rsv_1				: 1;
				uint32_t carry_flag			: 1;
			};
			uint32_t value;
		} eflags;
	};
} CPU_state;

extern CPU_state cpu;

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

#define reg_l(index) (cpu.gpr[index]._32)
#define reg_w(index) (cpu.gpr[index]._16)
#define reg_b(index) (cpu.gpr[index & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
