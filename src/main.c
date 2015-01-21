#include "stm32f4xx.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"

#include "hw_conf.h"
#include "myio.h"

/* semaphores, queues declarations */
xQueueHandle xQueueUART1Recvie;
SemaphoreHandle_t xSemUSART1send;

xQueueHandle xQueueUSART2Recvie;
SemaphoreHandle_t xSemUSART2send;

/* Queue structure used for passing messages. */
typedef struct {
	char str[50];
} serial_str_msg;

typedef struct {
	char ch;
} serial_ch_msg;

/*Private variables ---------------------------------------*/
//SD_Error Status = SD_OK;

/* Private functions ---------------------------------------------------------*/
char receive_byte()
{
	serial_ch_msg msg;

	/* Wait for a byte to be queued by the receive interrupts handler. */
	while (xQueueReceive(xQueueUART1Recvie, &msg, 0) == pdFALSE);
	return msg.ch;
}

void vUsartInputResponse(void *pvParameters)
{
	while(1){
		uprintf("%c", receive_byte());
	}
}

void vATask(void *pvParameters)
{	
	char A[] = "Task A running... ";
	char who = 'A';
	while(1){
		vTaskDelay(1000);
		uprintf("\nTask %c running\n", who);	
		qprintf(xSemUSART2send, "USART2> %s\n", A);
		//USART_SendData(USART2, 'A');
	}
}

void vBTask(void *pvParameters)
{
	char A[] = "Task B running... ";
	while(1){
		vTaskDelay(1000);
		uprintf("\n%s\n", A);	
	}
}

int main(void)
{
	/*a queue for tansfer the senddate to USART1 task*/
	xQueueUART1Recvie = xQueueCreate(15, sizeof(serial_ch_msg));
    /*for UASRT1 Tx usage token*/
	xSemUSART1send = xSemaphoreCreateBinary();

	/*a queue for tansfer the senddate to USART2 task*/
	xQueueUSART2Recvie = xQueueCreate(15, sizeof(serial_ch_msg));
    /*for UASRT2 Tx usage token*/
	xSemUSART2send = xSemaphoreCreateBinary();

	/* initialize USART hardware... */
	prvSetupHardware();
	_print("USART initialize finish...\n\r", xSemUSART1send);

	xTaskCreate( vATask, "send", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate( vBTask, "send", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate( vUsartInputResponse, "vUsartInputResponse", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);	


	vTaskStartScheduler();
	while(1);
	return 0;
}
