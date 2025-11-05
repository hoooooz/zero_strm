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