/****************************************************************************
*  Copyright 2025 hoooooz  (https://github.com/hoooooz)                     *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/
#ifndef __ZERO_STRM_BLOCK_H__
#define __ZERO_STRM_BLOCK_H__

#include ".\app_cfg.h" 

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct zero_strm_mem_blk_t {
    struct zero_strm_mem_blk_t *ptNext;
    size_t tSizeInByte;
    uint8_t chMemory[];
} zero_strm_mem_blk_t;

typedef struct zero_strm_mem_blk_fifo_t {
    zero_strm_mem_blk_t *ptFreeList;
    struct {
        zero_strm_mem_blk_t *ptHead;
        zero_strm_mem_blk_t *ptTail;
    } FIFO;
} zero_strm_mem_blk_fifo_t;



#define BLOCK_FIFO_INIT(__PTOBJ)              zero_strm_block_fifo_init(__PTOBJ)
#define BLOCK_FREE(__PTOBJ,__PTFREEBLOCK)     zero_strm_block_free(__PTOBJ,__PTFREEBLOCK)
#define BLOCK_NEW(__PTOBJ)                    zero_strm_block_new(__PTOBJ)
#define BLOCK_APPEND(__PTOBJ,__PTNEWNODE)     zero_strm_block_append(__PTOBJ,__PTNEWNODE)
#define BLOCK_FETCH(__PTOBJ)                  zero_strm_block_fetch(__PTOBJ)

    void zero_strm_block_fifo_init(zero_strm_mem_blk_fifo_t *ptThis) ;
extern 
    void zero_strm_block_free(zero_strm_mem_blk_fifo_t *ptThis,zero_strm_mem_blk_t *ptFreeBlock);
extern 
    zero_strm_mem_blk_t *zero_strm_block_new(zero_strm_mem_blk_fifo_t *ptThis) ;
extern 
    bool zero_strm_block_append(zero_strm_mem_blk_fifo_t *ptThis, zero_strm_mem_blk_t *ptNewNode) ;
extern 
    zero_strm_mem_blk_t *zero_strm_block_fetch(zero_strm_mem_blk_fifo_t *ptThis) ;

#endif

