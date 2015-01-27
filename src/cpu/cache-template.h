#include <stdlib.h>
#define TAG_WIDTH (27-BLOCK_SIZE_WIDTH-SET_WIDTH)

typedef union {
	struct {
		uint32_t offset : BLOCK_SIZE_WIDTH;
		uint32_t set	: SET_WIDTH;
		uint32_t tag	: TAG_WIDTH;
	};
	uint32_t addr;
} concat(Cache, _addr);

#define BLOCK_SIZE (1<<BLOCK_SIZE_WIDTH)
#define NR_SET (1<<SET_WIDTH)

typedef struct {
	uint8_t block[BLOCK_SIZE];
	int32_t tag;
	bool valid;
#ifdef WRITE_BACK
	bool dirty;
#endif
} Cache[NR_SET][NR_LINE];

Cache cache;
void concat(init_, cache)() {
	int i, j;
	for (i = 0; i < NR_SET; i++)
		for (j = 0; j < NR_LINE; j++)
			cache[i][j].valid = false;
}

static void concat(cache, _replace)(uint32_t set, int i, uint32_t tag) {
	int k;
	concat(Cache, _addr) temp;
#ifdef WRITE_BACK
	if (cache[set][i].valid&&cache[set][i].dirty) {
		temp.addr = 0;
		temp.tag = cache[set][i].tag;
		temp.set = set;
		for (k = 0; k < BLOCK_SIZE; k++)
			next_level_write(temp.addr+k, 1, cache[set][i].block[k]);
	}
#endif
	temp.addr = 0;
	temp.tag = tag;
	temp.set = set;
	for (k = 0; k < BLOCK_SIZE; k++)
		cache[set][i].block[k] = next_level_read(temp.addr+k, 1);
	cache[set][i].tag = tag;
	cache[set][i].valid = true;
#ifdef WRITE_BACK
	cache[set][i].dirty = false;
#endif
}

#define DATA_LEN 4
#define DATA_MASK (DATA_LEN - 1)

static void concat(cache, __read)(hwaddr_t addr, void* data) {
	test(addr < HW_MEM_SIZE, "addr = %x\n", addr);
	concat(Cache, _addr) temp;
	temp.addr = addr&(~DATA_MASK);
	uint32_t offset = temp.offset;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;

	int i, j = NR_LINE;
	for (i = 0; i < NR_LINE; i++) {
		if (cache[set][i].valid&&(cache[set][i].tag == tag))
			break;
		if (!cache[set][i].valid) j = i;
	}
	if (i == NR_LINE) {
		i = (j == NR_LINE)?rand()%NR_LINE:j;
		concat(cache, _replace)(set, i, tag);
	}
	memcpy(data, cache[set][i].block+offset, DATA_LEN);
}

static void concat(cache, __write)(hwaddr_t addr, void* data, uint8_t *mask) {
	test(addr < HW_MEM_SIZE, "addr = %x\n", addr);
	concat(Cache, _addr) temp;
	temp.addr = addr&(~DATA_MASK);
	uint32_t offset = temp.offset;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;

	int i;
	for (i = 0; i < NR_LINE; i++)
		if (cache[set][i].valid&&(cache[set][i].tag == tag))
			break;
	if (i < NR_LINE) memcpy_with_mask(cache[set][i].block+offset, data, DATA_LEN, mask);
#ifdef WRITE_BACK
	if (i == NR_LINE) {
		int j;
		for (j = 0; j < NR_LINE; j++)
			if (!cache[set][j].valid) break;
		i = (j == NR_LINE)?rand()%NR_LINE:j;
		concat(cache, _replace)(set, i, tag);
		memcpy_with_mask(cache[set][i].block+offset, data, DATA_LEN, mask);
	}
	cache[set][i].dirty = true;
#endif

}

uint32_t concat(cache, _read)(hwaddr_t addr, size_t len) {
	assert((len == 1)||(len == 2)||(len == 4));
	uint32_t offset = addr&DATA_MASK;
	uint8_t temp[2*DATA_LEN];

	concat(cache, __read)(addr, temp);
	if ((addr^(addr+len-1))&(~DATA_MASK))
		concat(cache, __read)(addr+DATA_LEN, temp+DATA_LEN);

	return *(uint32_t*)(temp+offset)&(~0u>>((4-len)<<3));
}

#ifndef READ_ONLY
void concat(cache, _write)(hwaddr_t addr, size_t len, uint32_t data) {
	assert((len == 1)||(len == 2)||(len == 4));
	uint32_t offset = addr&DATA_MASK;
	uint8_t temp[2*DATA_LEN];
	uint8_t mask[2*DATA_LEN];
	memset(mask, 0, 2*DATA_LEN);

	*(uint32_t*)(temp+offset) = data;
	memset(mask+offset, 1, len);
	concat(cache, __write)(addr, temp, mask);
	if ((addr^(addr+len-1))&(~DATA_MASK))
		concat(cache, __write)(addr+DATA_LEN, temp+DATA_LEN, mask+DATA_LEN);

#ifdef SYNC
	concat(SYNC, __write)(addr, temp, mask);
	if ((addr^(addr+len-1))&(~DATA_MASK))
		concat(SYNC, __write)(addr+DATA_LEN, temp+DATA_LEN, mask+DATA_LEN);
#endif

#ifndef WRITE_BACK
	next_level_write(addr, len, data);
#endif
}
#endif

#undef BLOCK_SIZE_WIDTH
#undef SET_WIDTH
#undef TAG_WIDTH
#undef NR_SET
#undef NR_LINE
#undef Cache
#undef cache
#undef next_level_read
#undef next_level_write
