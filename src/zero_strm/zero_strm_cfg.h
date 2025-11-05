
#ifndef __ZERO_STRM_CFG_H__
#define __ZERO_STRM_CFG_H__

#include "..\common\common.h"

#define   READ_BUFF_CNT     (16)
#define   WRITE_BUFF_CNT    (4)

typedef void(dma_start_rx_fn)(mem_blk_t *ptFifoSend);
typedef uint16_t(dma_cnt_get_fn)(void);

typedef void(dma_start_fn)(mem_blk_t *ptFifoSend);
typedef void (dma_send_data_fn)(mem_blk_t *ptThis);

typedef struct  {
    uint8_t *pchBuffer;
    uint16_t hwSize;
    uint32_t wTimeOutMs;
    dma_start_rx_fn *fnDmaStartRx;
    dma_cnt_get_fn *fnDmaCntGet;
} zero_strm_read_cfg_t;

typedef struct {
    uint8_t *pchBuffer;
    uint16_t hwSize;
    dma_send_data_fn *fnDmaSendData;     
} zero_strm_write_cfg_t ;


#endif