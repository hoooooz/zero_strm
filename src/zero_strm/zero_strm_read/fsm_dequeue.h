#ifndef __FSM_DEQUEUE_H__
#define __FSM_DEQUEUE_H__

/*============================ INCLUDES ======================================*/

#include ".\zero_strm_read.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
extern 
    void record_current_data_count(zero_strm_read_t *ptThis);
extern 
    void set_dma_idle(zero_strm_read_t *ptThis);
extern
    void set_uart_idle(zero_strm_read_t *ptThis);
extern
    void set_uart_busy(zero_strm_read_t *ptThis);
extern
    void  set_target_time(zero_strm_read_t *ptThis);
#endif