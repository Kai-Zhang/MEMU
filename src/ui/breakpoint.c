#include "ui/breakpoint.h"

#include "nemu.h"

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].next = NULL;

	head = NULL;
	free_ = bp_pool;
}

/* TODO: Implement the function of breakpoint */
 BP* new_bp() {
	if(!free_) return NULL;
	BP* newbp = free_; free_ = free_->next;

	newbp->next = NULL;
	BP* tail = head;
	while(!(tail->next)) {
		tail = tail->next;
	}
	tail->next = newbp;

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
	while(!(trv->next)) {
		trv = trv->next;
	}
	assert(trv->next == NULL);
	trv->next = bp;
}
