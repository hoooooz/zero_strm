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
#ifndef __ZERO_STRM_WRITE__
#define __ZERO_STRM_WRITE__


#include "..\..\common\common.h"
#include "..\zero_strm_cfg.h"

typedef union {
    zero_strm_mem_blk_t tList;
    uint8_t buffer[sizeof(zero_strm_mem_blk_t) + WRITE_BUFF_CNT];
} write_fifo_t;


typedef struct zero_strm_write_t zero_strm_write_t;


declare_simple_fsm(enqueue);
declare_simple_fsm(stream_write_flush);

extern_fsm_implementation(enqueue,
    args(byte tByte
    )) ;

extern_fsm_implementation(stream_write_flush); 



extern_simple_fsm(stream_write_flush,
    def_params(
        zero_strm_write_t *ptStreamWrite;
        zero_strm_mem_blk_t **pptByteFifo;
        void (*fnDmaStart)(zero_strm_mem_blk_t *ptFifoSend); 
        stream_write_flush_fn  *fnFlush ;
    )
)

extern_simple_fsm(enqueue,
    def_params(
        fsm(stream_write_flush) fsmFlush;       
        zero_strm_write_t *ptStreamWrite;
        enqueue_fn *fnEnqueue;   
    )
)



struct zero_strm_write_t {
    bool bBusy ;
    zero_strm_mem_blk_t *ptFifoSend ;
    zero_strm_mem_blk_t *ptByteFifo ;
    zero_strm_mem_blk_t *ptByteFifoCpl ;
    zero_strm_mem_blk_fifo_t tMemBlockFifo ;
    zero_strm_dma_send_data_fn *fnDmaStart;
    zero_strm_byte_fifo_t tByteFifo ; 
    fsm(enqueue) fsmEnqueue ;
    fsm(stream_write_flush) fsmFlushAuto ;
    fsm(stream_write_flush) fsmFlushManual ;
} ;







extern
void zero_strm_write_init(zero_strm_write_t *ptThis,zero_strm_write_cfg_t *ptCfg) ;
extern
bool zero_strm_write(zero_strm_write_t *ptThis,byte tByte) ;
extern
void zero_strm_dma_send_data_cpl_event_handler(zero_strm_write_t *ptThis) ;

#endif

