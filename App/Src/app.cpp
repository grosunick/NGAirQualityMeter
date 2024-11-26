#include "app.hpp"
#include "gpio.h"
#include <stdio.h>

extern "C" void app_c(void) {
    app();
}

class TEST
{
public:
    static void toggle_print();
};

void TEST::toggle_print() {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

[[noreturn]] void app() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

    while (true) {
        TEST::toggle_print();
        HAL_Delay(100);
        printf("Hello world!!!\n");
    }
}

extern "C" int _write(int file, char *ptr, int len) {
    (void)file; // hack for compiler warning about unused parameter file
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}