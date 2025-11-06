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
#ifndef __ZERO_STRM_READ__
#define __ZERO_STRM_READ__


#include "..\..\common\common.h"
#include "..\zero_strm_cfg.h"

typedef struct zero_strm_read_t zero_strm_read_t;

typedef union {
    zero_strm_mem_blk_t tList;
    uint8_t buffer[sizeof(zero_strm_mem_blk_t) + READ_BUFF_CNT];
} read_fifo_t;

declare_simple_fsm(dequeue);
declare_simple_fsm(stream_read_flush);
declare_simple_fsm(time_out);

extern_fsm_implementation(dequeue,
    args(byte *ptByte
    )) ;

extern_fsm_implementation(stream_read_flush); 

extern_fsm_implementation(time_out); 


extern_simple_fsm(stream_read_flush,
    def_params(           
        zero_strm_read_t  *ptStreamRead; 
        zero_strm_mem_blk_t **pptByteFifo;
        zero_strm_mem_blk_t *ptByteFifo;
        zero_strm_dma_start_rx_fn *fnDmaStartRx;
        stream_read_flush_fn  *fnFlush;
    )
)

extern_simple_fsm(dequeue,
    def_params(          
        zero_strm_read_t *ptStreamRead;
        zero_strm_mem_blk_t *ptByteFifo;
        dequeue_fn *fnDequeue;
    )
)

extern_simple_fsm(time_out,
    def_params(          
        zero_strm_read_t *ptStreamRead;
        time_out_fn *fnTimeOut;
        fsm(stream_read_flush) fsmFlush;
    )
)

struct zero_strm_read_t {
    bool bBusy ;
    bool bUartIdle;
    bool bTimerStart;
    zero_strm_mem_blk_fifo_t tMemBlockFifo ;   
    zero_strm_byte_fifo_t tByteFifo ; 
    zero_strm_mem_blk_t *ptByteFifoDmaRx ; 
    zero_strm_mem_blk_t *ptByteFifoEmpty ;     
    uint16_t hwDmaCntUartIdle;
    uint32_t wSetTime;
    uint32_t wTimeStamp;
    zero_strm_dma_start_rx_fn *fnDmaStartRx;
    zero_strm_dma_cnt_get_fn *fnDmaCntGet;
    fsm(stream_read_flush) fsmFlushHt ;  
    fsm(dequeue) fsmDequeue ;
    fsm(time_out)  fsmTimeOut ;
    uint16_t hwDmaSizeTotal;
} ;







extern
void zero_strm_read_init(zero_strm_read_t *ptThis,zero_strm_read_cfg_t *ptCfg ) ;
extern
void zero_strm_uart_dma_get_data_insert_to_dma_irq_event_handler(zero_strm_read_t *ptThis) ;
extern
void zero_strm_uart_idle_insert_to_uart_irq_event_handler(zero_strm_read_t  *ptThis) ;
extern
bool zero_strm_read(zero_strm_read_t *ptThis,byte *pchChar) ;
extern
void zero_strm_uart_wait_time_out_insert_to_hard_timer_irq_event_handler(zero_strm_read_t *ptThis) ;
#endif

