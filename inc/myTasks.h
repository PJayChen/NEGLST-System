#ifndef MYTASKS_H
#define MYTASKS_H

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"

extern xQueueHandle xQueueUART1Recvie;
extern xQueueHandle xQueueUSART2Recvie;
extern xQueueHandle xQueueUSART3Recvie;

extern SemaphoreHandle_t xSemUSART1send;
extern SemaphoreHandle_t xSemUSART2send;
extern SemaphoreHandle_t xSemUSART3send;

void vMyTaskCreate(uint16_t usStackSize, UBaseType_t uxPriority );
void vGPSRawDataParsingTask(void *pvParameters);

#endif