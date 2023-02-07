#include "mem_pool.h"

#include <stdalign.h>
#include <stdlib.h>

#define MIN_BLOCK_SIZE 4096

struct MemBlock {
    size_t size, cap;
    MemBlock* next;
    alignas(max_align_t) char data[];
};

static inline MemBlock* alloc_block(size_t size) {
    MemBlock* block = malloc(sizeof(MemBlock) + size);
    block->size = 0;
    block->cap = size;
    block->next = NULL;
    return block;
}

MemPool new_mem_pool(void) {
    MemBlock* block = alloc_block(MIN_BLOCK_SIZE);
    return (MemPool) {
        .first = block,
        .cur = block
    };
}

void free_mem_pool(MemPool* mem_pool) {
    for (MemBlock* cur = mem_pool->first; cur;) {
        MemBlock* next = cur->next;
        free(cur);
        cur = next;
    }
}

static inline MemBlock* find_block(MemPool* mem_pool, size_t size) {
    for (MemBlock* cur = mem_pool->cur; cur; cur = cur->next) {
        mem_pool->cur = cur;
        if (cur->cap - cur->size >= size)
            return cur;
    }
    size = size < MIN_BLOCK_SIZE ? MIN_BLOCK_SIZE : size;
    MemBlock* block = alloc_block(size);
    mem_pool->cur->next = block;
    mem_pool->cur = block;
    return block;
}

static inline size_t round_up(size_t num, size_t denom) {
    return (num + (denom - 1) / denom) * denom;
}

void* mem_pool_alloc(MemPool* mem_pool, size_t size) {
    size = round_up(size, sizeof(max_align_t));
    MemBlock* block = find_block(mem_pool, size);
    void* ptr = block->data + block->size;
    block->size += size;
    return ptr;
}
