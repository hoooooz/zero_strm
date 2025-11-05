# zero_strm

# zero_strm_read 和 zero_strm_write 使用文档

这是一份关于 `zero_strm` 项目中 `zero_strm_read` 和 `zero_strm_write` 模块的使用文档。这两个模块提供了基于DMA的UART数据收发抽象层,通过有限状态机(FSM)协调异步数据传输和缓冲区管理。

## zero_strm_read - 数据接收

### 功能概述

`zero_strm_read` 模块通过DMA从UART硬件接收数据,并提供简单的字节流API供应用层读取。

### 初始化

#### 配置结构体

使用 `zero_strm_read_cfg_t` 配置接收参数: 

| 字段             | 类型               | 说明                           |
| ---------------- | ------------------ | ------------------------------ |
| `pchBuffer`      | `uint8_t*`         | 接收缓冲区内存池指针           |
| `hwSize`         | `uint16_t`         | 缓冲区总大小                   |
| `wTimeOutMs`     | `uint32_t`         | 定时器产生中断的设置时间(毫秒) |
| `fnDmaStartRx`   | `dma_start_rx_fn*` | DMA启动回调函数                |
| `fnDmaCntGet`    | `dma_cnt_get_fn*`  | 获取DMA剩余计数回调            |
#### 时间

用户需要将`zero_strm_uart_wait_time_out_insert_to_hard_timer_irq_event_handler(&g_tZStrmRead)`放入一个以`1ms`为周期的中断服务函数中



#### 初始化函数

调用 `zero_strm_read_init()` 初始化模块: 

```c
zero_strm_read_t g_tZStrmRead;
zero_strm_read_cfg_t s_tZStrmReadCfg = {
    .pchBuffer     = s_chReadBuffer,
    .hwSize        = sizeof(s_chReadBuffer),
    .wTimeOutMs    = 2000,
    .fnDmaStartRx  = uart_dma_data_get,
    .fnDmaCntGet   = get_dma_cnt,
};

zero_strm_read_init(&g_tZStrmRead, &s_tZStrmReadCfg);
```

### 读取数据

使用 `zero_strm_read()` 函数逐字节读取数据: 

```c
uint8_t chByte;
if (zero_strm_read(&g_tZStrmRead, &chByte)) {
    // 成功读取一个字节
    printf("%c", chByte);
}

函数返回 `true` 表示成功读取,`false` 表示当前无数据可读。
```

### 中断处理

需要在相应的中断服务程序中调用以下事件处理函数:

#### DMA半满中断

当dma接收到字节数为设置的一半时调用：

```c
void DMA1_Channel5_IRQHandler(void) {
    if (RESET != DMA_GetITStatus(DMA1_IT_TC5)) {
        DMA_ClearITPendingBit(DMA1_IT_TC5);
        zero_strm_uart_dma_get_data_insert_to_dma_irq_event_handler(&g_tZStrmRead);
    }
}
```

#### UART空闲中断

当UART检测到空闲线路时调用: 

```c
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        zero_strm_uart_idle_insert_to_uart_irq_event_handler(&g_tZStrmRead);
    }
}
```
#### 超时定时器中断

当超时定时器到期时调用: 

```c
void TIMx_IRQHandler(void) {
    zero_strm_uart_wait_time_out_insert_to_hard_timer_irq_event_handler(&g_tZStrmRead);
}
```
## zero_strm__write - 数据发送

### 功能概述

`zero_strm_write` 模块提供通过DMA向UART发送数据的高层接口,自动管理缓冲和DMA事务协调。 

### 初始化

#### 配置结构体

使用 `stream_write_cfg_t` 配置发送参数: 

| 字段            | 类型                | 说明                 |
| --------------- | ------------------- | -------------------- |
| `pchBuffer`     | `uint8_t*`          | 发送缓冲区内存池指针 |
| `hwSize`        | `uint16_t`          | 缓冲区总大小         |
| `fnDmaSendData` | `dma_send_data_fn*` | DMA发送启动回调函数  |

#### 初始化函数

调用 `zero_strm_write_init()` 初始化模块: 

```c
zero_strm_write_t g_tZStrmWrite;
zero_strm_write_cfg_t s_tZStrmWriteCfg = {
    .pchBuffer     = s_chWriteBuffer,
    .hwSize        = sizeof(s_chWriteBuffer),
    .fnDmaSendData = uart_dma_data_send    
};

zero_strm_write_init(&g_tZStrmWrite, &s_tZStrmWriteCfg);
```

### 发送数据

使用 `zero_strm_write()` 函数逐字节发送数据: 

```c
uint8_t chByte = 'A';
if (zero_strm_write(&g_tZStrmWrite, chByte)) {
    // 字节已成功加入发送队列
}

函数返回 `true` 表示字节已入队,`false` 表示需要稍后重试(缓冲区满)。
```

### 中断处理

#### DMA传输完成中断

当DMA发送完成时调用: 

```c
void DMA1_Channel4_IRQHandler(void) {
    if (RESET != DMA_GetITStatus(DMA1_IT_TC4)) {
        DMA_Cmd(DMA1_Channel4, DISABLE);
        DMA_ClearITPendingBit(DMA1_IT_TC4);
        zero_strm_dma_send_data_cpl_event_handler(&g_tZStrmWrite);
    }
}
```



## 完整示例

以下是一个完整的使用示例,展示了如何初始化和使用这两个模块: 

```c
#include "zero_strm.h"

__attribute__((aligned(32)))
static uint8_t s_chReadBuffer[1024];
__attribute__((aligned(32)))
static uint8_t s_chWriteBuffer[1024];


static zero_strm_read_cfg_t s_tZStrmReadCfg = {
    .pchBuffer     = s_chReadBuffer,
    .hwSize        = sizeof(s_chReadBuffer),
    .wTimeOutMs    = 2000,
    .fnDmaStartRx  = uart_dma_data_get,
    .fnDmaCntGet   = get_dma_cnt,
};

static zero_strm_write_cfg_t s_tZStrmWriteCfg = {
    .pchBuffer     = s_chWriteBuffer,
    .hwSize        = sizeof(s_chWriteBuffer),
    .fnDmaSendData = uart_dma_data_send    
};


stream_read_t g_tZStrmRead;
stream_write_t g_tZStrmWrite;

int main(void) {
    bsp_init();
    
    // 初始化读写模块
    zero_strm_read_init(&g_tZStrmRead, &s_tZStrmReadCfg);
    zero_strm_write_init(&g_tZStrmWrite, &s_tZStrmWriteCfg);
    
    // 主循环:读取数据并回显
    while(1) {
        uint8_t chByte;
        if (zero_strm_read(&g_tZStrmRead,&chByte)) {
            zero_strm_write(&g_tZStrmWrite,chByte);
        }
    }
}
```
## Notes

这两个模块都使用有限状态机(FSM)来管理复杂的异步操作。`zero_strm_read` 使用三个FSM(dequeue、flush、timeout)协调数据接收,  而 `zero_strm_write` 使用两个FSM(enqueue、flush)管理数据发送。

两个模块都依赖于底层的块内存管理器(`mem_blk_fifo_t`)和字节FIFO缓冲区(`byte_fifo_t`)来实现高效的内存管理和数据缓冲。



# zero_strm

# zero_strm_read and zero_strm_write Usage Documentation

This document provides an overview of the `zero_strm_read` and `zero_strm_write` modules in the `zero_strm` project. These modules offer a DMA-based UART data transmission and reception abstraction layer, coordinating asynchronous data transfer and buffer management through a finite state machine (FSM).

## zero_strm_read - Data Reception

### Overview

The `zero_strm_read` module receives data from UART hardware via DMA and provides a simple byte stream API for application-level reading.

### Initialization

#### Configuration Structure

Configure reception parameters using the `zero_strm_read_cfg_t` structure:

| Field          | Type               | Description                                 |
| -------------- | ------------------ | ------------------------------------------- |
| `pchBuffer`    | `uint8_t*`         | Pointer to the reception buffer pool        |
| `hwSize`       | `uint16_t`         | Total size of the buffer                    |
| `wTimeOutMs`   | `uint32_t`         | Timer interrupt setting time (milliseconds) |
| `fnDmaStartRx` | `dma_start_rx_fn*` | DMA start callback function                 |
| `fnDmaCntGet`  | `dma_cnt_get_fn*`  | DMA remaining count retrieval callback      |

#### Timing

Users need to insert `zero_strm_uart_wait_time_out_insert_to_hard_timer_irq_event_handler(&g_tZStrmRead)` into an interrupt service function with a 1ms period.

#### Initialization Function

Call `zero_strm_read_init()` to initialize the module:

```c
zero_strm_read_t g_tZStrmRead;
zero_strm_read_cfg_t s_tZStrmReadCfg = {
    .pchBuffer     = s_chReadBuffer,
    .hwSize        = sizeof(s_chReadBuffer),
    .wTimeOutMs    = 2000,
    .fnDmaStartRx  = uart_dma_data_get,
    .fnDmaCntGet   = get_dma_cnt,
};

zero_strm_read_init(&g_tZStrmRead, &s_tZStrmReadCfg);
```

### Reading Data

Use the `zero_strm_read()` function to read data byte by byte:

```c
uint8_t chByte;
if (zero_strm_read(&g_tZStrmRead, &chByte)) {
    // Successfully read a byte
    printf("%c", chByte);
}

The function returns `true` if a byte is successfully read, and `false` if no data is currently available.
```

### Interrupt Handling

The following event handler functions need to be called in the corresponding interrupt service routines:

#### DMA Half Full Interrupt

Called when the number of bytes received by DMA is half of the set value:

```c
void DMA1_Channel5_IRQHandler(void) {
    if (RESET != DMA_GetITStatus(DMA1_IT_TC5)) {
        DMA_ClearITPendingBit(DMA1_IT_TC5);
        zero_strm_uart_dma_get_data_insert_to_dma_irq_event_handler(&g_tZStrmRead);
    }
}
```

#### UART Idle Interrupt

Called when the UART detects an idle line:

```c
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        zero_strm_uart_idle_insert_to_uart_irq_event_handler(&g_tZStrmRead);
    }
}
```

#### Timeout Timer Interrupt

Called when the timeout timer expires:

```c
void TIMx_IRQHandler(void) {
    zero_strm_uart_wait_time_out_insert_to_hard_timer_irq_event_handler(&g_tZStrmRead);
}
```

## zero_strm_write - Data Transmission

### Overview

The `zero_strm_write` module provides a high-level interface for sending data to UART via DMA, automatically managing buffering and coordinating DMA transactions.

### Initialization

#### Configuration Structure

Configure transmission parameters using the `zero_strm_write_cfg_t` structure:

| Field           | Type                | Description                             |
| --------------- | ------------------- | --------------------------------------- |
| `pchBuffer`     | `uint8_t*`          | Pointer to the transmission buffer pool |
| `hwSize`        | `uint16_t`          | Total size of the buffer                |
| `fnDmaSendData` | `dma_send_data_fn*` | DMA send start callback function        |

#### Initialization Function

Call `zero_strm_write_init()` to initialize the module:

```c
zero_strm_write_t g_tZStrmWrite;
zero_strm_write_cfg_t s_tZStrmWriteCfg = {
    .pchBuffer     = s_chWriteBuffer,
    .hwSize        = sizeof(s_chWriteBuffer),
    .fnDmaSendData = uart_dma_data_send    
};

zero_strm_write_init(&g_tZStrmWrite, &s_tZStrmWriteCfg);
```

### Sending Data

Use the `zero_strm_write()` function to send data byte by byte:

```c
uint8_t chByte = 'A';
if (zero_strm_write(&g_tZStrmWrite, chByte)) {
    // Byte has been successfully added to the send queue
}

The function returns `true` if the byte is queued, and `false` if a retry is needed (buffer full).
```

### Interrupt Handling

#### DMA Transfer Complete Interrupt

Called when DMA transmission is complete:

```c
void DMA1_Channel4_IRQHandler(void) {
    if (RESET != DMA_GetITStatus(DMA1_IT_TC4)) {
        DMA_Cmd(DMA1_Channel4, DISABLE);
        DMA_ClearITPendingBit(DMA1_IT_TC4);
        zero_strm_dma_send_data_cpl_event_handler(&g_tZStrmWrite);
    }
}
```

## Complete Example

The following is a complete usage example, demonstrating how to initialize and use these modules:

```c
#include "zero_strm.h"

__attribute__((aligned(32)))
static uint8_t s_chReadBuffer[1024];
__attribute__((aligned(32)))
static uint8_t s_chWriteBuffer[1024];

static zero_strm_read_cfg_t s_tZStrmReadCfg = {
    .pchBuffer     = s_chReadBuffer,
    .hwSize        = sizeof(s_chReadBuffer),
    .wTimeOutMs    = 2000,
    .fnDmaStartRx  = uart_dma_data_get,
    .fnDmaCntGet   = get_dma_cnt,
};

static zero_strm_write_cfg_t s_tZStrmWriteCfg = {
    .pchBuffer     = s_chWriteBuffer,
    .hwSize        = sizeof(s_chWriteBuffer),
    .fnDmaSendData = uart_dma_data_send    
};

zero_strm_read_t g_tZStrmRead;
zero_strm_write_t g_tZStrmWrite;

int main(void) {
    bsp_init();
    
    // Initialize read and write modules
    zero_strm_read_init(&g_tZStrmRead, &s_tZStrmReadCfg);
    zero_strm_write_init(&g_tZStrmWrite, &s_tZStrmWriteCfg);
    
    // Main loop: read data and echo
    while(1) {
        uint8_t chByte;
        if (zero_strm_read(&g_tZStrmRead, &chByte)) {
            zero_strm_write(&g_tZStrmWrite, chByte);
        }
    }
}
```

## Notes

Both modules use finite state machines (FSMs) to manage complex asynchronous operations. The `zero_strm_read` module uses three FSMs (dequeue, flush, timeout) to coordinate data reception, while the `zero_strm_write` module uses two FSMs (enqueue, flush) to manage data transmission. Both modules rely on the underlying block memory manager (`mem_blk_fifo_t`) and byte FIFO buffer (`byte_fifo_t`) for efficient memory management and data buffering.
