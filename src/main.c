#include "stm32f4xx.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"

#include "hw_conf.h"
#include "myio.h"

#include "shell.h"
#include "myTasks.h"
#include "comTask.h"
/*--------------------------------------------------------- */

/* semaphores, queues declarations */
xQueueHandle xQueueUART1Recvie;
SemaphoreHandle_t xSemUSART1send;

xQueueHandle xQueueUSART2Recvie;
SemaphoreHandle_t xSemUSART2send;

xQueueHandle xQueueUSART3Recvie;
SemaphoreHandle_t xSemUSART3send;

SemaphoreHandle_t xMutexGPS;

/* Queue structure used for passing messages. */
typedef struct {
	char str[50];
} serial_str_msg;

typedef struct {
	char ch;
} serial_ch_msg;

/*Private variables ---------------------------------------*/


/* Private functions ---------------------------------------------------------*/
void vCreateQueues(void)
{
	/*a queue for tansfer the senddate to USART1 task*/
	xQueueUART1Recvie = xQueueCreate(15, sizeof(serial_ch_msg));
    /*for UASRT1 Tx usage token*/
	xSemUSART1send = xSemaphoreCreateBinary();

	/*a queue for tansfer the senddate to USART2 task*/
	xQueueUSART2Recvie = xQueueCreate(500, sizeof(serial_ch_msg));
    /*for UASRT2 Tx usage token*/
	xSemUSART2send = xSemaphoreCreateBinary();

	/*a queue for tansfer the senddate to USART2 task*/
	xQueueUSART3Recvie = xQueueCreate(400, sizeof(serial_ch_msg));
    /*for UASRT2 Tx usage token*/
	xSemUSART3send = xSemaphoreCreateBinary();

	/*Mutex for read/write GPS data*/
    xMutexGPS = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutexGPS);
}

int main(void)
{
	vCreateQueues();

	/* initialize USART hardware... */
	prvSetupHardware();
	_print("USART initialize finish...\n\r", xSemUSART1send);
	
	vUARTCommandConsoleStart( configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY);
    vMyTaskCreate( configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY);
	
 	xTaskCreate(    vBluetoothSPPTask,  /* The task that implements the command console. */
                    "Bluetooth Task",                      /* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
                    configMINIMAL_STACK_SIZE,                /* The size of the stack allocated to the task. */
                    NULL,                       /* The parameter is not used, so NULL is passed. */
                    tskIDLE_PRIORITY,                 /* The priority allocated to the task. */
                    NULL );                     /* A handle is not required, so just pass NULL. */

	vTaskStartScheduler();
	while(1);
	return 0;
}
