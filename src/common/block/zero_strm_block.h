#ifndef __ZERO_STRM_BLOCK_H__
#define __ZERO_STRM_BLOCK_H__

#include ".\app_cfg.h" 

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct mem_blk_t {
    struct mem_blk_t *ptNext;
    size_t tSizeInByte;
    uint8_t chMemory[];
} mem_blk_t;

typedef struct mem_blk_fifo_t {
    mem_blk_t *ptFreeList;
    struct {
        mem_blk_t *ptHead;
        mem_blk_t *ptTail;
    } FIFO;
} mem_blk_fifo_t;

extern 
    void zero_strm_block_fifo_init(mem_blk_fifo_t *ptThis) ;
extern 
    void zero_strm_block_free(mem_blk_fifo_t *ptThis,mem_blk_t *ptFreeBlock);
extern 
    mem_blk_t *zero_strm_block_new(mem_blk_fifo_t *ptThis) ;
extern 
    bool zero_strm_block_append(mem_blk_fifo_t *ptThis, mem_blk_t *ptNewNode) ;
extern 
    mem_blk_t *zero_strm_block_fetch(mem_blk_fifo_t *ptThis) ;

#endif

