#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(concat(OP_NAME, _a2i_), SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE rst = REG(R_EAX) OP_SYMBOL (imm CARRY);
	REG(R_EAX) = rst;

	cpu.eflags.sign_flag = (rst < 0);
	cpu.eflags.zero_flag = (rst == 0);
	cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (REG(R_EAX))) & 1;
	cpu.eflags.carry_flag = (rst < REG(R_EAX));
	cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(R_EAX) < 0) == ((DATA_TYPE_S)imm < 0))
		&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)imm < 0));
	cpu.eflags.parity_flag = 0;
	while (rst) {
		cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
		rst = rst & (rst - 1);
	}

	print_asm(str(OPNAME) str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}

make_helper(concat(concat(OP_NAME, _rm2i_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0, rst = 0;
	int len = 1;
	if(m.reg == 0) {
		if(m.mod == 3) {
			imm = instr_fetch(eip + 2, DATA_BYTE);
			rst = REG(m.R_M) OP_SYMBOL (imm CARRY);
			REG(m.R_M) = rst;

			cpu.eflags.sign_flag = (rst < 0);
			cpu.eflags.zero_flag = (rst == 0);
			cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (REG(m.R_M))) & 1;
			cpu.eflags.carry_flag = (rst < REG(m.R_M));
			cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.R_M) < 0) == ((DATA_TYPE_S)imm < 0))
				&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)imm < 0));
			cpu.eflags.parity_flag = 0;
			while (rst) {
				cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
				rst = rst & (rst - 1);
			}

			print_asm(str(OP_NAME) str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		}
		else {
			swaddr_t addr;
			len = read_ModR_M(eip + 1, &addr);
			imm = instr_fetch(eip + 1 + len, DATA_BYTE);
			rst = MEM_R(addr) OP_SYMBOL (imm CARRY);
			MEM_W(addr, rst);

			cpu.eflags.sign_flag = (rst < 0);
			cpu.eflags.zero_flag = (rst == 0);
			cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (MEM_R(addr))) & 1;
			cpu.eflags.carry_flag = (rst < MEM_R(addr));
			cpu.eflags.overflow_flag = (((DATA_TYPE_S)MEM_R(addr) < 0) == ((DATA_TYPE_S)imm < 0))
				&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)imm < 0));
			cpu.eflags.parity_flag = 0;
			while (rst) {
				cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
				rst = rst & (rst - 1);
			}

			print_asm(str(OP_NAME) str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		}
	}
	else {
		printf("invalid `" str(OP_NAME) "'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}
	
	return 1 + len + DATA_BYTE;
}

#if SUFFIX != b

make_helper(concat(concat(OP_NAME, _rm2i8_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm = 0;
	DATA_TYPE rst = 0;
	int len = 1;
	if(m.reg == 0) {
		if(m.mod == 3) {
			imm = instr_fetch(eip + 2, 1);
			rst = REG(m.R_M) OP_SYMBOL ((DATA_TYPE_S)imm CARRY);
			REG(m.R_M) = rst;

			cpu.eflags.sign_flag = (rst < 0);
			cpu.eflags.zero_flag = (rst == 0);
			cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (REG(m.R_M))) & 1;
			cpu.eflags.carry_flag = (rst < REG(m.R_M));
			cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.R_M) < 0) == ((DATA_TYPE_S)imm < 0))
				&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)imm < 0));
			cpu.eflags.parity_flag = 0;
			while (rst) {
				cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
				rst = rst & (rst - 1);
			}

			print_asm(str(OP_NAME) str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		}
		else {
			swaddr_t addr;
			len = read_ModR_M(eip + 1, &addr);
			imm = instr_fetch(eip + 1 + len, 1);
			rst = MEM_R(addr) OP_SYMBOL ((DATA_TYPE_S)imm CARRY);
			MEM_W(addr, rst);

			cpu.eflags.sign_flag = (rst < 0);
			cpu.eflags.zero_flag = (rst == 0);
			cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (MEM_R(addr))) & 1;
			cpu.eflags.carry_flag = (rst < MEM_R(addr));
			cpu.eflags.overflow_flag = (((DATA_TYPE_S)MEM_R(addr) < 0) == ((DATA_TYPE_S)imm < 0))
				&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)imm < 0));
			cpu.eflags.parity_flag = 0;
			while (rst) {
				cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
				rst = rst & (rst - 1);
			}

			print_asm(str(OP_NAME) str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		}
	}
	else {
		printf("invalid `" str(OP_NAME) "'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}
	
	return 1 + len + 1;
}

#endif

make_helper(concat(concat(OP_NAME, _rm2r_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	if(m.mod == 3) {
		rst = REG(m.R_M) OP_SYMBOL (REG(m.reg) CARRY);
		REG(m.R_M) = rst;

		cpu.eflags.sign_flag = (rst < 0);
		cpu.eflags.zero_flag = (rst == 0);
		cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (REG(m.R_M))) & 1;
		cpu.eflags.carry_flag = (rst < REG(m.R_M));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.R_M) < 0) == ((DATA_TYPE_S)REG(m.reg) < 0))
			&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)REG(m.reg) < 0));
		cpu.eflags.parity_flag = 0;
		while (rst) {
			cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
			rst = rst & (rst - 1);
		}

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = MEM_R(addr) OP_SYMBOL (REG(m.reg) CARRY);
		MEM_W(addr, rst);

		cpu.eflags.sign_flag = (rst < 0);
		cpu.eflags.zero_flag = (rst == 0);
		cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (MEM_R(addr))) & 1;
		cpu.eflags.carry_flag = (rst < MEM_R(addr));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)MEM_R(addr) < 0) == ((DATA_TYPE_S)REG(m.reg) < 0))
			&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)REG(m.reg) < 0));
		cpu.eflags.parity_flag = 0;
		while (rst) {
			cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
			rst = rst & (rst - 1);
		}

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	
	return 1 + len;
}

make_helper(concat(concat(OP_NAME, _r2rm_), SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE rst = 0;
	int len = 1;
	if(m.mod == 3) {
		rst = REG(m.reg) OP_SYMBOL (REG(m.R_M) CARRY);
		REG(m.reg) = rst;

		cpu.eflags.sign_flag = (rst < 0);
		cpu.eflags.zero_flag = (rst == 0);
		cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (REG(m.R_M))) & 1;
		cpu.eflags.carry_flag = (rst < REG(m.R_M));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.R_M) < 0) == ((DATA_TYPE_S)REG(m.reg) < 0))
			&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)REG(m.reg) < 0));
		cpu.eflags.parity_flag = 0;
		while (rst) {
			cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
			rst = rst & (rst - 1);
		}

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		rst = REG(m.reg) OP_SYMBOL (MEM_R(addr) CARRY);
		REG(m.reg) = rst;

		cpu.eflags.sign_flag = (rst < 0);
		cpu.eflags.zero_flag = (rst == 0);
		cpu.eflags.auxiliary_flag = ((rst >> 4) ^ (MEM_R(addr))) & 1;
		cpu.eflags.carry_flag = (rst < MEM_R(addr));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)MEM_R(addr) < 0) == ((DATA_TYPE_S)REG(m.reg) < 0))
			&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)(m.reg) < 0));
		cpu.eflags.parity_flag = 0;
		while (rst) {
			cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
			rst = rst & (rst - 1);
		}

		print_asm(str(OP_NAME) str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	
	return 1 + len;
}

#if DATA_BYTE == 4

extern char suffix;

make_helper(concat(OP_NAME, _a2i_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _a2i_l)(eip) : concat(OP_NAME, _a2i_w)(eip));
}

make_helper(concat(OP_NAME, _rm2i_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _a2i_l)(eip) : concat(OP_NAME, _a2i_w)(eip));
}

make_helper(concat(OP_NAME, _rm2i8_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _a2i_l)(eip) : concat(OP_NAME, _a2i_w)(eip));
}

make_helper(concat(OP_NAME, _rm2r_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _a2i_l)(eip) : concat(OP_NAME, _a2i_w)(eip));
}

make_helper(concat(OP_NAME, _r2rm_v)) {
	return (suffix == 'l' ? concat(OP_NAME, _a2i_l)(eip) : concat(OP_NAME, _a2i_w)(eip));
}

#endif

#include "exec/template-end.h"
