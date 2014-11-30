/**
 * This file provides a simple L1 cache
 * Use macro to configure the cache
 * Options:
 * +  CACHE_BLOCK_BIT: size of a block in cache(in 2^n)
 * +  CACHE_SIZE_BIT: size of the cache(in 2^n)
 * +  mapping: DIRCT_MAPPED, SET_ASSOCIATIVE, FULLY_ASSOCIATIVE
 * +  replace: RANDOM_REPLACE, LRU_REPLACE
 * +  write policy: WRITE_BACK, WRITE_THROUGH
 * +  write miss: WRITE_ALLOCATE, NON_WRITE_ALLOCATE
 */
#include <stdlib.h>
#include "common.h"

/**
 * Configuration
 */
#define CACHE_BLOCK_BIT 6	// 64 Byte
#define CACHE_SIZE_BIT 16	// 64 KB
#define SET_ASSOCIATIVE
#define SET_NUM 4
#define RANDOM_REPLACE
#define WRITE_THROUGH
#define WRITE_ALLOCATE


/* check the configuration */
#ifndef CACHE_BLOCK_BIT
#error unknown CACHE_BLOCK_BIT
#endif
#ifndef CACHE_SIZE_BIT
#error unknown CACHE_SIZE_BIT
#endif
#ifdef SET_ASSOCIATIVE
#ifndef SET_NUM
#error unknown SET_NUM
#endif
#else
#ifndef DIRECT_MAPPED
#ifndef FULLY_ASSOCIATIVE
#error unknown MAPPING
#endif
#endif
#endif
#ifndef RANDOM_REPLACE
#ifndef LRU_REPLACE
#error unknown REPLACE
#endif
#endif
#ifndef WRITE_THROUGH
#ifndef WRITE_BACK
#error unknown WRITE_POLICY
#endif
#endif
#ifndef WRITE_ALLOCATE
#ifndef NON_WRITE_ALLOCATE
#error unknown WRITE_REPLACE
#endif
#endif


extern uint32_t dram_read(hwaddr_t, size_t);

#define CACHE_BLOCK (1 << CACHE_BLOCK_BIT)
#define CACHE_SIZE (1 << CACHE_SIZE_BIT)
#define CACHE_MASK (CACHE_BLOCK - 1)

struct cache_block {
	bool valid;
#ifdef WRITE_BACK
	bool dirty;
#endif
#ifdef LRU_REPLACE
	uint32_t LRUtag;
#endif
	uint32_t tag;
	uint8_t blocks[CACHE_BLOCK];
};

static struct cache_block cache[1 << (CACHE_SIZE_BIT - CACHE_BLOCK_BIT)][SET_NUM];

static void block_replace(hwaddr_t addr, uint32_t set_index, uint32_t block_index) {
	int i = 0;
	hwaddr_t block_addr = addr & CACHE_MASK;
	for ( ; i < CACHE_BLOCK; ++ i ) {
		cache[set_index][block_index].blocks[i] = dram_read(block_addr + i, 1);
	}
}

#ifdef LRU_REPLACE
static void LRU_process(uint32_t set_index, uint32_t block_index) {
	cache[set_index][block_index].LRUtag = 0;
	int block = 0;
	for ( ; block < SET_NUM; ++ set ) {
		if (cache[set_index][block].valid) {
			if (block != block_index) {
				++ cache[set_index][block].LRUtag;
			}
			else break;
		}
	}
}

static uint32_t LRU_replace(hwaddr_t addr, uint32_t set_index, uint32_t tag) {
	int least_used = 0;
	uint32_t least_used_times = cache[set_index][0].LRUtag ++;
	int block = 1;
	for ( ; block < SET_NUM; ++ block) {
		if (cache[set_index][block].LRUtag > least_used_times) {
			least_used_times = cache[set_index][block].LRUtag ++;
			least_used = block;
		}
		else {
			++ cache[set_index][block].LRUtag;
		}
	}
	cache[set_index][least_used].tag = tag;
	cache[set_index][least_used].LRUtag = 0;
	block_replace(addr, set_index, least_used);
	return cache[set_index][least_used].blocks[addr & CACHE_MASK];
}
#else
static uint32_t random_replace(addr, set_index, tag) {
	int randindex = rand() % SET_NUM;
	cache[set_index][randindex].tag = tag;
	block_replace(addr, set_index, randindex);
	return cache[set_index][randindex].blocks[addr & CACHE_MASK];
}
#endif

uint32_t cache_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t block_index = addr & CACHE_MASK;

	if (block_index + len > CACHE_BLOCK) {
		size_t rest = CACHE_BLOCK - block_index;
		uint32_t highbit = cache_read((addr | CACHE_MASK) + 1, len - rest);
		uint32_t lowbit = cache_read(addr, rest);
		return (highbit << (1 << rest)) | lowbit;
	}

	uint32_t cache_addr = addr >> (CACHE_BLOCK_BIT + CACHE_SIZE_BIT);
	uint32_t set_index = cache_addr % ((1 << (CACHE_SIZE_BIT - CACHE_BLOCK_BIT)) / SET_NUM);
	uint32_t tag = cache_addr / ((1 << (CACHE_SIZE_BIT - CACHE_BLOCK_BIT)) / SET_NUM);

	int i = 0;
	for ( ; i < SET_NUM; ++ i ) {
		if (cache[set_index][i].tag == tag) {
#ifdef LRU_REPLACE
			LRU_process(set_index, i);
#endif
			return *(uint32_t*)(&cache[set_index][i].blocks[block_index]) & (~0u >> ((4 - len) << 3));
		}
		else {
			cache[set_index][i].valid = true;
			cache[set_index][i].tag = tag;
			block_replace(addr, set_index, i);
#ifdef LRU_REPLACE
			LRU_process(set_index, i);
#endif
			return cache[set_index][i].blocks[block_index] & (~0u >> ((4 - len) << 3));
		}
	}

#ifdef LRU_REPLACE
	return LRU_replace(set_index, tag) & (~0u >> ((4 - len) << 3));
#else
	return random_replace(set_index, tag) & (~0u >> ((4 - len) << 3));
#endif

}

