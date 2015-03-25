#ifndef __MYIO_H
#define __MYIO_H

#include <stdarg.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

void _print(char *str, SemaphoreHandle_t);
void qprintf(SemaphoreHandle_t, const char *format, ...);
void uprintf(const char *format, ...);

BaseType_t vSerialGetChar(xQueueHandle, char *, TickType_t);
BaseType_t vSerialGetLine(xQueueHandle, char *, TickType_t);

#endif

