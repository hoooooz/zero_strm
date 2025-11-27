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
#include ".\zero_strm_read.h"
#include ".\fsm_dequeue.h"


extern_fsm_initialiser(dequeue,
    args(zero_strm_read_t *ptStreamRead
    )) ;

extern_fsm_initialiser(stream_read_flush,
    args(zero_strm_read_t *ptStreamRead,
    zero_strm_mem_blk_t **pptByteFifo,
    zero_strm_dma_start_rx_fn *fnDmaStartRx
    )) ;

extern_fsm_initialiser(time_out,
    args(zero_strm_read_t *ptStreamRead
    )) ;


void zero_strm_read_init(zero_strm_read_t *ptThis,zero_strm_read_cfg_t *ptCfg )
{
    if (   NULL == ptThis 
        || NULL == ptCfg ) {
        return ;
    }
        
    read_fifo_t *ptAddr = ptCfg->pchBuffer;
    uint8_t chCnt       = ptCfg->hwSize/sizeof(read_fifo_t);
    memset(&this,0,sizeof(zero_strm_read_t));

    zero_strm_block_fifo_init(&(this.tMemBlockFifo)); 
    for(uint8_t chIndex = 0;chIndex < chCnt;chIndex++) {
        ptAddr[chIndex].tList.tSizeInByte = READ_BUFF_CNT;
        zero_strm_block_free(&(this.tMemBlockFifo),&ptAddr[chIndex].tList);
    }
    
    this.hwDmaSizeTotal = READ_BUFF_CNT;
    this.fnDmaCntGet = ptCfg->fnDmaCntGet ;
    this.bBusy = false;
    this.bUartIdle = false;
    this.bTimerStart = false;
    this.wSetTime = ptCfg->wTimeOutMs;        
    this.fnDmaStartRx = ptCfg->fnDmaStartRx;

    this.ptByteFifoDmaRx  = zero_strm_block_new(&(this.tMemBlockFifo));
    (*this.fnDmaStartRx)(this.ptByteFifoDmaRx);
    
    this.ptByteFifoEmpty  = zero_strm_block_new(&(this.tMemBlockFifo));
    zero_strm_init_byte_fifo_empty(&(this.tByteFifo),this.ptByteFifoEmpty->chMemory,
        this.ptByteFifoEmpty->tSizeInByte);
   
    
    init_fsm(stream_read_flush,&(this.fsmFlushHt),
        args(&this,&this.ptByteFifoDmaRx,this.fnDmaStartRx) );
    init_fsm(dequeue,&(this.fsmDequeue),args(&this));
    init_fsm(time_out,&(this.fsmTimeOut),args(&this));
    
    init_fsm(stream_read_flush,&this.fsmTimeOut,
        args(&this,&this.ptByteFifoDmaRx,this.fnDmaStartRx));

}


bool zero_strm_read(zero_strm_read_t *ptThis,byte *pchChar) 
{
    bool bRet = false ;
    if (   NULL == ptThis 
        || NULL == pchChar ) {
        return bRet;
    }

    if ( fsm_rt_cpl == call_fsm(dequeue,&this.fsmDequeue,args(pchChar) ) ) {
        bRet = true;        
    }

    return bRet;
}

void zero_strm_uart_dma_get_data_insert_to_dma_irq_event_handler(zero_strm_read_t *ptThis)
{
    if (NULL == ptThis) {
        return ;
    }
    
    set_dma_idle(&this);
    set_uart_busy(&this);
    call_fsm(stream_read_flush,&this.fsmFlushHt);
}


void zero_strm_uart_idle_insert_to_uart_irq_event_handler(zero_strm_read_t *ptThis)
{
    if ( NULL == ptThis ) {
        return;
    }
    
    set_uart_idle(&this);
    record_current_data_count(&this);
    set_target_time(&this);
    
}

void zero_strm_uart_wait_time_out_insert_to_hard_timer_irq_event_handler(zero_strm_read_t *ptThis) 
{ 
    
    if ( NULL == ptThis ) {
        return ;
    }
   
    call_fsm(time_out,&(this.fsmTimeOut)) ;

   
  
}



