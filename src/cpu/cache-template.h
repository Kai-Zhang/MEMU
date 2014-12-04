/**
 * This file provides the cache template
 * Use macro to configure the cache
 * Options:
 * +  BLOCK_WIDTH: size of a block in cache(in 2^n)
 * +  CACHE_WIDTH: size of the cache(in 2^n)
 * +  mapping: DIRCT_MAPPED, SET_ASSOCIATIVE, FULLY_ASSOCIATIVE
 * +  replace: RANDOM_REPLACE, LRU_REPLACE
 * +  write policy: WRITE_BACK, WRITE_THROUGH
 * +  write miss: WRITE_ALLOCATE, NON_WRITE_ALLOCATE
 * TODO: Only support set associative for now..
 */
#include <stdlib.h>
#include "common.h"
#include "macro.h"
#include "lib/misc.h"

/* check the configuration */
#ifndef BLOCK_WIDTH
#error unknown BLOCK_BIT
#endif
#ifndef CACHE_WIDTH
#error unknown CACHE_WIDTH
#endif
#ifdef SET_ASSOCIATIVE
#ifndef NR_WAY
#error unknown NR_WAY
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

#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_SIZE (1 << CACHE_WIDTH)
#define NR_SET (1 << (CACHE_WIDTH - BLOCK_WIDTH))

#define DATA_LEN 4
#define DATA_MASK (DATA_LEN - 1)

typedef union {
	struct {
		uint32_t offset		: BLOCK_WIDTH;
		uint32_t set		: CACHE_WIDTH - BLOCK_WIDTH;
		uint32_t tag		: 27 - CACHE_WIDTH - BLOCK_WIDTH;
	};
	uint32_t addr;
} concat(CACHE_NAME, _addr);

#define cache_addr concat(CACHE_NAME, _addr)

struct {
	bool valid;
#ifdef WRITE_BACK
	bool dirty;
#endif
#ifdef LRU_replace
	uint32_t LRUtag;
#endif
	uint32_t tag;
	uint8_t blocks[NR_BLOCK];
} CACHE_NAME[NR_SET][NR_WAY];

static void concat(init_, CACHE_NAME)() {
	int i, j;
	for ( i = 0; i < NR_SET; ++ i ) {
		for ( j = 0; j < NR_WAY; ++ j ) {
			CACHE_NAME[i][j].valid = false;
#ifdef WRITE_BACK
			CACHE_NAME[i][j].dirty = false;
#endif
#ifdef LRU_REPLACE
			CACHE_NAME[i][j].LRUtag = 0;
#endif
		}
	}
}

static void concat(CACHE_NAME, _random_replace)(uint32_t set, int way, uint32_t tag) {
	cache_addr temp;
	int i;
#ifdef WRITE_BACK
	if (CACHE_NAME[set][way].valid && CACHE_NAME[set][way].dirty) {
		temp.addr = 0;
		temp.tag = CACHE_NAME[set][way].tag;
		temp.set = set;
		for ( i = 0; i < NR_BLOCK; ++ i ) {
			__write_to_next_level(temp.addr + i, 1, CACHE[set][way].block[i]);
		}
	}
#endif
	temp.addr = 0;
	temp.tag = tag;
	temp.set = set;
	for ( i = 0; i < NR_BLOCK; ++ i ) {
		CACHE_NAME[set][way].blocks[i] = __read_from_next_level(temp.addr + i, 1);
	}
	CACHE_NAME[set][way].tag = tag;
	CACHE_NAME[set][way].valid = true;
#ifdef WRITE_BACK
	CACHE_NAME[set][way].dirty = false;
#endif
}
#define __random_replace concat(CACHE_NAME, _random_replace)

static void concat(concat(__, CACHE_NAME), _read)(hwaddr_t addr, void *data) {
	cache_addr temp;
	temp.addr = addr & ~DATA_MASK;
	uint32_t offset = temp.offset;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;

	int i = 0, replaced = -1;
	for ( ; i < NR_WAY; ++ i ) {
		if (CACHE_NAME[set][i].valid && CACHE_NAME[set][i].tag == tag) {
			break;
		}
		if (!CACHE_NAME[set][i].valid) {
			replaced = i;
		}
	}
	if (i == NR_WAY) {
#ifdef RANDOM_REPLACE
		i = (replaced == -1) ? (rand() % NR_WAY) : replaced;
		__random_replace(set, i, tag);
#endif
	}
	memcpy(data, CACHE_NAME[set][i].blocks + offset, DATA_LEN);
}

static void concat(concat(__, CACHE_NAME), _write)(hwaddr_t addr, void *data, uint8_t *mask) {
	cache_addr temp;
	temp.addr = addr & ~DATA_MASK;
	uint32_t offset = temp.offset;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;

	int i = 0;
#ifdef WRITE_ALLOCATE
	int replaced = -1;
#endif
	for ( ; i < NR_WAY; ++ i ) {
		if (CACHE_NAME[set][i].valid && CACHE_NAME[set][i].tag == tag) {
			break;
		}
#ifdef WRITE_ALLOCATE
		if (!CACHE_NAME[set][i].valid) {
			replaced = i;
		}
#endif
	}
	if (i < NR_WAY) {
		memcpy_with_mask(CACHE_NAME[set][i].blocks + offset, data, DATA_LEN, mask);
	}
#ifdef WRITE_ALLOCATE
	else {
#ifdef RANDOM_REPLACE
		i = (replaced == -1) ? (rand() % NR_WAY) : replaced;
		__random_replace(set, i, tag);
#endif
		memcpy_with_mask(CACHE_NAME[set][i].block + offset, data, DATA_LEN, mask);
#ifdef WRITE_BACK
		CACHE_NAME[set][i].dirty = true;
#endif
	}
#endif
}

#define __cache_read concat(concat(__, CACHE_NAME), _read)
#define __cache_write concat(concat(__, CACHE_NAME), _write)

static uint32_t concat(CACHE_NAME, _read)(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & DATA_MASK;
	uint8_t temp[2 * DATA_LEN];

	__cache_read(addr, temp);
	
	if ( (addr ^ (addr + len - 1)) & ~(DATA_MASK) ) {
		__cache_read(addr + DATA_LEN, temp + DATA_LEN);
	}

	return *(uint32_t *)(temp + offset) & (~0u >> ((4 - len) << 3));
}

static void concat(CACHE_NAME, _write)(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & DATA_MASK;
	uint8_t temp[2 * DATA_LEN];
	uint8_t mask[2 * DATA_LEN];
	memset(mask, 0, 2 * DATA_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	__cache_write(addr, temp, mask);

	if ( (addr ^ (addr + len - 1)) & ~(DATA_MASK) ) {
		__cache_write(addr + DATA_LEN, temp + DATA_LEN, mask + DATA_LEN);
	}

#ifdef WRITE_THROUGH
	__write_to_next_level(addr, len, data);
#endif
}

#undef __random_replace
#undef __cache_read
#undef __cache_write
#undef cache_addr
#undef DATA_MASK
#undef DATA_LEN
#undef NR_SET
#undef NR_SIZE
#undef NR_BLOCK
