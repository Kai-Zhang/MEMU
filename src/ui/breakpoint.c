#include "ui/breakpoint.h"
#include "common.h"

#include "nemu.h"

#define NR_BP 32

extern uint32_t expr_calc(char *, bool *);

static BP bp_pool[NR_BP];
static BP *head, *free_;

bool hit_bp;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].NO = i;
	bp_pool[i].next = NULL;

	head = NULL;
	free_ = bp_pool;
	hit_bp = false;
}

/* TODO: Implement the function of breakpoint */
BP* new_bp() {
	if(!free_) return NULL;
	BP* newbp = free_; free_ = free_->next;

	newbp->next = NULL;
	if(!head) {
		head = newbp;
	} else {
		BP* tail = head;
		while(tail->next) {
			tail = tail->next;
		}
		tail->next = newbp;
	}
	return newbp;
}

void free_bp(BP* bp) {
	BP* trv = head;
	if(head != bp) {
		while(trv->next != bp) {
			trv = trv->next;
		}
		assert(trv->next == bp);
		trv->next = bp->next;
	} else {
		head = head->next;
	}
	bp->next = NULL;

	if(!free_) {
		free_ = bp;
		return;
	}
	trv = free_;
	while(trv->next) {
		trv = trv->next;
	}
	assert(trv->next == NULL);
	trv->next = bp;
}

BP* get_bp(int no) {
	BP* trv = head;
	while(trv) {
		if(trv->NO == no) {
			return trv;
		}
		trv = trv->next;
	}
	return NULL;
}

int find_bp(swaddr_t bp_addr) {
	BP *trv = head;
	while(trv) {
		if(!(trv->watch) && trv->address == bp_addr) {
			return trv->NO;
		}
		trv = trv->next;
	}
	return -1;
}

void show_bp() {
	if(!head) {
		puts("No breakpoints or watchpoints");
		return;
	}
	puts("Num\tType\t\tAddress\t\tWhat");
	BP* trv = head;
	while(trv) {
		if(trv->watch) {
			printf("%d\twatchpoint\t\t\t%s\n", trv->NO, trv->watch_expr);
		} else {
			printf("%d\tbreakpoint\t0x%08x\t\n", trv->NO, trv->address);
		}
		if(trv->hit_time == 1) {
			puts("\tbreakpoint already hit 1 time");
		} else if(trv->hit_time > 1) {
			printf("\tbreakpoint already hit %d times\n", trv->hit_time);
		}
		trv = trv->next;
	}
}

void refresh_bp() {
	BP *trv = head;
	while(trv) {
		if(!(trv->watch)) {
			swaddr_write(trv->address, 1, INT3_CODE);
		}
		trv->hit_time = 0;
		trv = trv->next;
	}
}

void instr_recover(swaddr_t eip) {
	BP *recover_bp = get_bp(find_bp(eip));
	assert(recover_bp != NULL);
	swaddr_write(eip, 1, recover_bp->replaced);
}

bool check_watchpoint() {
	BP *trv = head;
	uint32_t rst = 0;
	bool hit_wp = false;
	while(trv) {
		if(trv->watch) {
			if((rst = expr_calc(trv->watch_expr, NULL)) != trv->pre_rst) {
				printf("nemu: HIT watchpoint %d:%s\n\nOld value = %d\nNew value = %d\n",
						trv->NO, trv->watch_expr, trv->pre_rst, rst);
				trv->pre_rst = rst;
				++ trv->hit_time;
				hit_wp = true;
			}
		}
		trv = trv->next;
	}
	return hit_wp;
}
