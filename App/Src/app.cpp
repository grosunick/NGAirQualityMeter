#include "app.hpp"
#include "gpio.h"
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
    while (true) {
//        Pin<GPIOA_BASE, 5, Write>::toggle();
//        HAL_Delay(1);

        Segment::setNumber(val);
    }
}

extern "C" int _write(int file, char *ptr, int len) {
    (void)file; // hack for compiler warning about unused parameter file
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}