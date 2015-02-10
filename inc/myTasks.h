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


typedef struct GSP_Data{
    char timeUTC[11]; //hhmmss.sss
    char status[2]; //V/A
    char latitude[12]; //plus N/S indicator, Nddmm.mmmm
    char longitude[13]; //plus E/W indicator, Edddmm.mmmm
    char speed[10]; //Speed over Ground(knots)
    char COG[10]; //Course over Ground(degrees)
    char date[7]; //ddmmyy
}GPSdata;

void vMyTaskCreate(uint16_t usStackSize, UBaseType_t uxPriority );
void vGPSRawDataParsingTask(void *pvParameters);

#endif