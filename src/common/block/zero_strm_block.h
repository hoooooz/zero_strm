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

