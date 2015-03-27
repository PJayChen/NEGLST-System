#ifndef COMTASKS_H
#define COMTASKS_H

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"

#include "string.h"
#include "String.h"
#include "myio.h"
#include "myTasks.h"

extern xQueueHandle xQueueUSART3Recvie;

extern SemaphoreHandle_t xSemUSART3send;

extern SemaphoreHandle_t xMutexGPS;

extern GPSdata GPS;

void vBluetoothSPPTask(void *);

#endif