#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
void app_c(void);
void debug(const char* data);
void sendFromBuffer(void);

#ifdef __cplusplus
}
#endif

extern int16_t val;
void app();

