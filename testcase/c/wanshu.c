#include "trap.h"

int ans[] = {6, 28, 496};

int main() {
	int n, sum, i, k = 0;
	for(n = 1; n < 1000; n ++) {
		sum = 0;
		for(i = 1; i < n; i ++) {
			if(n % i == 0) {
				sum += i;
			}
		}
		
		if(sum == n) {
			memu_assert(n == ans[k]);
			k ++;
		}
	}

	memu_assert(k == 3);

	HIT_GOOD_TRAP;

	return 0;
}
