#ifndef __MYIO_H
#define __MYIO_H

#include <stdarg.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "semphr.h"

void _print(char *str, SemaphoreHandle_t);
void qprintf(SemaphoreHandle_t, const char *format, ...);
void uprintf(const char *format, ...);

#endif