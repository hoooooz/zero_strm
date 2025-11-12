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
#include "zero_strm_block.h"

#include "perf_counter.h"
#undef  this
#define this    (*ptThis)


void zero_strm_block_fifo_init(zero_strm_mem_blk_fifo_t *ptThis)  
{   
    if ( NULL == ptThis ) {
        return;
    }
    
    memset(&this,0,sizeof(zero_strm_mem_blk_fifo_t));  
}

void zero_strm_block_free(zero_strm_mem_blk_fifo_t *ptThis,zero_strm_mem_blk_t *ptFreeBlock) 
{   
    if ( NULL == ptThis ) {
        return ;
    } 
    if ( NULL == ptFreeBlock ) {
        return ;
    }

    ptFreeBlock->ptNext = this.ptFreeList; 
    this.ptFreeList = ptFreeBlock; 
   
}

zero_strm_mem_blk_t *zero_strm_block_new(zero_strm_mem_blk_fifo_t *ptThis) 
{     
    if ( NULL == ptThis ) {
        return NULL;
    }
    zero_strm_mem_blk_t *ptTemp = this.ptFreeList;  
    if ( NULL == ptTemp ) {
        return NULL;
    }

    this.ptFreeList = this.ptFreeList->ptNext ; 
   
    return ptTemp;
}

bool zero_strm_block_append(zero_strm_mem_blk_fifo_t *ptThis,zero_strm_mem_blk_t *ptNewNode) 
{
    if ( NULL == ptNewNode ) {
        return false;
    }
    if ( NULL == ptThis ) {
        return false;
    }

    ptNewNode->ptNext = NULL;
    /*   fifo empty   */
    if ( NULL == this.FIFO.ptTail ) {        
        this.FIFO.ptTail = ptNewNode;
        this.FIFO.ptHead = this.FIFO.ptTail;
    } else {  
        /* update  link */
        this.FIFO.ptTail->ptNext = ptNewNode;
        /* update  node */
        this.FIFO.ptTail = ptNewNode;  
    } 
  
    return true;
}

zero_strm_mem_blk_t *zero_strm_block_fetch(zero_strm_mem_blk_fifo_t *ptThis) 
{
    if ( NULL == ptThis ) {
        return NULL;
    }
    zero_strm_mem_blk_t *ptTemp = this.FIFO.ptHead;
    if ( NULL == ptTemp ) {
        return NULL;
    }
 
    /* the last one block */
    if ( this.FIFO.ptHead == this.FIFO.ptTail ) {
        this.FIFO.ptTail = NULL;
    }

    this.FIFO.ptHead = this.FIFO.ptHead->ptNext;  
    /* user get target from ptTemp,
    and can know module inside information by ptTemp->ptNext
    so we would best to set ptNext 0 for safety */
    ptTemp->ptNext = NULL;

    return ptTemp;
}