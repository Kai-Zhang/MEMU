#include "memory.h"
#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

#define BLOCK_WIDTH 6		// 64 Bytes
#define CACHE_WIDTH 16		// 64 kB
#define SET_ASSOCIATIVE
#define NR_WAY 8
#define RANDOM_REPLACE
#define WRITE_THROUGH
#define NON_WRITE_ALLOCATE
#define CACHE_NAME cache_l1
#define __read_from_next_level dram_read
#define __write_to_next_level dram_write
#include "cache-template.h"
#undef __write_to_next_level
#undef __read_from_next_level
#undef NON_WRITE_ALLOCATE
#undef WRITE_THROUGH
#undef RANDOM_REPLACE
#undef NR_WAY
#undef SET_ASSOCIATIVE
#undef CACHE_WIDTH
#undef BLOCK_WIDTH

void init_cache() {
	init_cache_l1();
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	return cache_l1_read(addr, len);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_l1_write(addr, len, data);
}
