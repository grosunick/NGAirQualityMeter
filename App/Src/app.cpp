#include "app.hpp"
#include "gpio.h"
#include "usart.h"
#include <gpio/ShiftRegister.hpp>
#include <segment/driver/DriverHC595.hpp>
#include <segment/SegmentDisplay.hpp>

using namespace ng;

using HC595 = DriverHC595<ShiftRegister<GPIOA_BASE, 8, GPIOA_BASE, 11, GPIOA_BASE, 12>>;
using Segment = SegmentDisplay<HC595>;
int16_t val = 0;

extern "C" void app_c(void) {
    app();
}

void app() {
    debug("Hello world from stm32!\n");

    while (true) {
        Segment::setNumber(val);
    }
}

void debug(char* data) {
    HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), HAL_MAX_DELAY);
}

extern "C" int _write(int file, char *ptr, int len) {
    if (file == 1) {
        HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    } else {
        for (int i = 0; i < len; i++) {
            ITM_SendChar(*ptr++);
        }
    }

    return len;
}