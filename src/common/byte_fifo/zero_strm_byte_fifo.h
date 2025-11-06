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
#ifndef __ZERO_STRM_BYTE_FIFO_H__
#define __ZERO_STRM_BYTE_FIFO_H__

#include ".\app_cfg.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef  uint8_t byte;
typedef struct {
    byte *pchBuffer;
    uint16_t hwSize;
    uint16_t hwHead;
    uint16_t hwTail;
    uint16_t hwLength;
} zero_strm_byte_fifo_t;

#define ENQUEUE_BYTE_FIFO(__FIFO, __OBJ)                 zero_strm_enqueue_byte_fifo(__FIFO,__OBJ)
#define DEQUEUE_BYTE_FIFO(__FIFO, __ADDR)                zero_strm_dequeue_byte_fifo(__FIFO,__ADDR)
#define IS_BYTE_FIFO_EMPTY(__FIFO)                       zero_strm_is_byte_fifo_empty(__FIFO)
#define INIT_BYTE_FIFO_EMPTY(__FIFO, __BUFFER, __SIZE)   zero_strm_init_byte_fifo_empty(__FIFO,__BUFFER,__SIZE)
#define INIT_BYTE_FIFO_FULL(__FIFO, __BUFFER, __SIZE)    zero_strm_init_byte_fifo_full(__FIFO,__BUFFER,__SIZE)



extern 
    bool zero_strm_enqueue_byte_fifo(zero_strm_byte_fifo_t *ptThis,byte tByteToIn);
extern 
    bool zero_strm_dequeue_byte_fifo(zero_strm_byte_fifo_t *ptThis,byte *ptToOut);
extern 
    bool zero_strm_is_byte_fifo_empty(zero_strm_byte_fifo_t *ptThis);
extern 
    bool zero_strm_init_byte_fifo_empty(zero_strm_byte_fifo_t *ptThis,byte *ptBuff,uint16_t hwSize);
extern 
    bool zero_strm_init_byte_fifo_full(zero_strm_byte_fifo_t *ptThis,byte *ptBuff,uint16_t hwSize);

#endif

