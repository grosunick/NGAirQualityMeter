#include "app.hpp"
#include "gpio.h"
#include "usart.h"
#include <gpio/ShiftRegister.hpp>
#include <segment/driver/DriverHC595.hpp>
#include <segment/SegmentDisplay.hpp>

using namespace ng;
using namespace std;

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
    char msg[] = "Hello world from stm32!\n";
    debug(msg);


    while (true) {
        Segment::setNumber(val);
    }
}

extern "C" void debug(const char* data) {
#ifdef DEBUG
    static bool isInit = false;

    if (!isInit) {
        hdma_usart2_tx.XferCpltCallback = DMATransferComplete;
        isInit = true;
    }

    HAL_DMA_Start_IT(
        &hdma_usart2_tx,
        (uint32_t)data,
        (uint32_t)&huart2.Instance->DR,
        strlen(data)
    );

    huart2.Instance->CR3 |= USART_CR3_DMAT;
//    HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), HAL_MAX_DELAY);
#endif
}

extern "C" int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }

    return len;
}