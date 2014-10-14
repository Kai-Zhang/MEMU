#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(cmp_a2i_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE rst = REG(R_EAX) - imm;
	
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

	print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return 1 + DATA_BYTE;
}

make_helper(concat(cmp_rm2i_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0, rst = 0;
	int len = 1;
	if(m.reg == 7) {
		if(m.mod == 3) {
			imm = instr_fetch(eip + 2, DATA_BYTE);
			rst = REG(m.R_M) - imm;

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

			print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		}
		else {
			swaddr_t addr;
			len = read_ModR_M(eip + 1, &addr);
			imm = instr_fetch(eip + 1 + len, DATA_BYTE);
			rst = MEM_R(addr) - imm;

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

			print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		}
	}
	else {
		printf("invalid `cmp'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}
	
	return 1 + len + DATA_BYTE;
}

make_helper(concat(cmp_rm2i8_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = 0, rst = 0;
	int len = 1;
	if(m.reg == 7) {
		if(m.mod == 3) {
			imm = instr_fetch(eip + 2, 1);
			rst = REG(m.R_M) - imm;

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

			print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		}
		else {
			swaddr_t addr;
			len = read_ModR_M(eip + 1, &addr);
			imm = instr_fetch(eip + 1 + len, 1);
			rst = MEM_R(addr) - imm;

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

			print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		}
	}
	else {
		printf("invalid `cmp'(eip = 0x%08x): unexpected reg/op %d\n", eip, m.reg);
		assert(0);
	}
	
	return 2 + len;
}

make_helper(concat(cmp_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	if(m.mod == 3) {
		DATA_TYPE rst = REG(m.R_M) - REG(m.reg);

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

		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE rst = MEM_R(addr) - REG(m.reg);

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

		print_asm("cmp" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	
	return 1 + len;
}

make_helper(concat(cmp_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 1;
	if(m.mod == 3) {
		DATA_TYPE rst = REG(m.reg) - REG(m.R_M);

		cpu.eflags.sign_flag = (rst < 0);
		cpu.eflags.zero_flag = (rst == 0);
		cpu.eflags.auxiliary_flag = ((rst >> 4) ^ REG(m.reg)) & 1;
		cpu.eflags.carry_flag = (rst < REG(m.reg));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.reg) < 0) == ((DATA_TYPE_S)REG(m.R_M) < 0))
			&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)REG(m.R_M) < 0));
		cpu.eflags.parity_flag = 0;
		while (rst) {
			cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
			rst = rst & (rst - 1);
		}

		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	}
	else {
		swaddr_t addr;
		len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE rst = REG(m.reg) - MEM_R(addr);

		cpu.eflags.sign_flag = (rst < 0);
		cpu.eflags.zero_flag = (rst == 0);
		cpu.eflags.auxiliary_flag = ((rst >> 4) ^ REG(m.reg)) & 1;
		cpu.eflags.carry_flag = (rst < REG(m.reg));
		cpu.eflags.overflow_flag = (((DATA_TYPE_S)REG(m.reg) < 0) == ((DATA_TYPE_S)MEM_R(addr) < 0))
			&& (((DATA_TYPE_S)rst < 0) != ((DATA_TYPE_S)MEM_R(addr) < 0));
		cpu.eflags.parity_flag = 0;
		while (rst) {
			cpu.eflags.parity_flag = !cpu.eflags.parity_flag;
			rst = rst & (rst - 1);
		}

		print_asm("cmp" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	}

	return 1 + len;
}

#include "exec/template-end.h"
