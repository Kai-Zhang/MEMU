#ifndef __CACHE_H__
#define __CACHE_H__
#include "common.h"

void init_cache();
uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

#endif
