#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stddef.h>

typedef struct MemBlock MemBlock;

typedef struct MemPool {
    MemBlock* cur;
    MemBlock* first;
} MemPool;

MemPool new_mem_pool(void);
void free_mem_pool(MemPool*);
void* mem_pool_alloc(MemPool*, size_t size, size_t align);

#endif
