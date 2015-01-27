#ifndef __CACHE_H__
#define __CACHE_H__
#include "common.h"

void init_cache();
uint32_t Dcache_read(hwaddr_t, size_t);
uint32_t Icache_read(hwaddr_t, size_t);
void Dcache_write(hwaddr_t, size_t, uint32_t);

#endif
