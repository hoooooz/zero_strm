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
#include "zero_strm_byte_fifo.h"


#undef  this
#define this    (*ptThis)

bool zero_strm_init_byte_fifo_empty(zero_strm_byte_fifo_t *ptThis,byte *ptBuff,uint16_t hwSize)
{
    bool bRet = false;
    
    if (    (NULL == ptThis) 
       ||   (NULL == ptBuff)  
       ||   (0 == hwSize)) {
        return bRet;
    }
       
    this.pchBuffer = ptBuff;
    this.hwSize    = hwSize;
    this.hwHead    = 0;
    this.hwTail    = 0;
    this.hwLength  = 0;
    
    bRet = true;
    return bRet;
}

bool zero_strm_init_byte_fifo_full(zero_strm_byte_fifo_t *ptThis,byte *ptBuff,uint16_t hwSize)
{
    bool bRet = false;
    
    if (    (NULL == ptThis)  
       ||   (NULL == ptBuff)  
       ||   (0 == hwSize)) {
        return bRet;
    }
         
    this.pchBuffer = ptBuff;
    this.hwSize    = hwSize;
    this.hwHead    = 0;
    this.hwTail    = 0;
    this.hwLength  = hwSize;
    
    bRet = true;
    return bRet;
}





bool zero_strm_enqueue_byte_fifo(zero_strm_byte_fifo_t *ptThis,byte tByteToIn)
{
    bool bRet = false;
    
    if (NULL == ptThis) {
        return bRet;
    }
    /* fifo full */
    if (    (this.hwHead == this.hwTail) 
       &&   (0 != this.hwLength)) {
        return  bRet; 
    }
    
    this.pchBuffer[this.hwTail++] = tByteToIn;
    
    if (this.hwTail >= this.hwSize) {
        this.hwTail = 0;
    }
    this.hwLength++;
    bRet = true;
    return bRet;
}

bool zero_strm_dequeue_byte_fifo(zero_strm_byte_fifo_t *ptThis,byte *ptToOut)
{
    bool bRet = false;
    
    if (    (NULL == ptThis) 
       ||   (NULL == ptToOut)) {
        return bRet;
    }    
    /* fifo empty */
    if (    (this.hwHead == this.hwTail) 
       &&   (0 == this.hwLength)) {
        return bRet;   
    }
    
    *ptToOut = this.pchBuffer[this.hwHead++];
    
    if (this.hwHead >= this.hwSize) {
        this.hwHead = 0;
    }
    this.hwLength--;
    bRet = true;
    return bRet;
}

bool zero_strm_is_byte_fifo_empty(zero_strm_byte_fifo_t *ptThis)
{
    bool bRet = false;
    
    if (NULL == ptThis) {
        return bRet;
    }
    
    if (    (this.hwHead == this.hwTail) 
       &&   (0 == this.hwLength)) {
        bRet  = true;   
    }
    return bRet;
}




