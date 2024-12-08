#include "app.hpp"
#include "gpio.h"
#include "usart.h"
#include <gpio/ShiftRegister.hpp>
#include <segment/driver/DriverHC595.hpp>
#include <segment/SegmentDisplay.hpp>
#include <common/RingBuffer.hpp>

using namespace ng;
using namespace std;

using RingBuf = RingBuffer<char, 128>;

using HC595 = DriverHC595<ShiftRegister<GPIOA_BASE, 8, GPIOA_BASE, 11, GPIOA_BASE, 12>>;
using Segment = SegmentDisplay<HC595>;
int16_t val = 0;

extern "C" void app_c(void) {
    app();
}

void DMATransferComplete(DMA_HandleTypeDef *hdma) {
    if (hdma->Instance == DMA1_Channel7) {
        huart2.Instance->CR3 &= ~USART_CR3_DMAT; // disable DMA mode
    }
}

void app() {
    debug("Hello world from stm32!\n");

    int cnt = 0;
    char b[100];
    while (1) {
        Segment::setNumber(val);

        sprintf(b, "cnt = %d\n", cnt++);
        debug(b);
    }
}

extern "C" void sendFromBuffer() {
    if (RingBuf::isEmpty()) {
        huart2.Instance->CR3 |= USART_CR3_DMAT;
        return;
    }

    auto len = min(RingBuf::length(), (uint16_t)10);
    auto ptr = RingBuf::remove(len);

    HAL_DMA_Start_IT(
        &hdma_usart2_tx,
        (uint32_t)ptr,
        (uint32_t)&huart2.Instance->DR,
        len
    );

    huart2.Instance->CR3 |= USART_CR3_DMAT;
}

extern "C" void debug(const char* data) {
#ifdef DEBUG
    static bool isInit = false;

    if (!isInit) {
        hdma_usart2_tx.XferCpltCallback = DMATransferComplete;
        isInit = true;
    }

    auto res = HAL_DMA_Start_IT(
        &hdma_usart2_tx,
        (uint32_t)data,
        (uint32_t)&huart2.Instance->DR,
        strlen(data)
    );

    if (res == HAL_BUSY) {
        auto res = RingBuf::add(data, strlen(data));
        if (res == BufferError::maxSize) {
            debug("max_size\n");
            HAL_Delay(10);
        }
    }

    huart2.Instance->CR3 |= USART_CR3_DMAT;
#endif
}

extern "C" int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }

    return len;
}