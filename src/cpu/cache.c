#include "memu.h"
#include "lib/misc.h"

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);

#define BLOCK_SIZE_WIDTH 6
#define SET_WIDTH 12
#define NR_LINE 16
#define Cache L2Cache
#define cache L2cache
#define next_level_read dram_read
#define next_level_write dram_write
#define WRITE_BACK
#include "cache-template.h"
#undef WRITE_BACK

#define BLOCK_SIZE_WIDTH 6
#define SET_WIDTH 7
#define NR_LINE 8
#define Cache ICache
#define cache Icache
#define next_level_read L2cache_read
#define next_level_write L2cache_write
#define READ_ONLY
#include "cache-template.h"
#undef READ_ONLY

#define BLOCK_SIZE_WIDTH 6
#define SET_WIDTH 7
#define NR_LINE 8
#define Cache DCache
#define cache Dcache
#define next_level_read L2cache_read
#define next_level_write L2cache_write
#define SYNC Icache
#include "cache-template.h"
#undef SYNC

void init_cache() {
	init_Icache();
	init_Dcache();
	init_L2cache();
}
