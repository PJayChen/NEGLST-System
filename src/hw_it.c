#include "stm32f4xx.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"

extern xQueueHandle xQueueUART1Recvie;
extern SemaphoreHandle_t xSemUSART1send;

extern xQueueHandle xQueueUSART2Recvie;
extern SemaphoreHandle_t xSemUSART2send;

/* Queue structure used for passing characters. */
typedef struct {
        char ch;
} serial_ch_msg;

/* IRQ handler to handle USART1 interruptss (both transmit and receive
* interrupts). */
void USART1_IRQHandler()
{
    static signed portBASE_TYPE xHigherPriorityTaskWoken;
    serial_ch_msg rx_msg;

    /* If this interrupt is for a transmit... */
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
            
           
            /* Diables the transmit interrupt. */
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
            xSemaphoreGiveFromISR(xSemUSART1send, &xHigherPriorityTaskWoken); 

    /* If this interrupt is for a receive... */
    }else if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
            /* Receive the byte from the buffer. */
            rx_msg.ch = USART_ReceiveData(USART1);

            /* Queue the received byte. */
            xQueueSendToBackFromISR(xQueueUART1Recvie, &rx_msg, &xHigherPriorityTaskWoken);  
    }
#if 0
	else {
            /* Only transmit and receive interrupts should be enabled.
             * If this is another type of interrupt, freeze.
             */
            while(1);
    }
#endif
    if (xHigherPriorityTaskWoken) {
            taskYIELD();
    }

}

/* IRQ handler to handle USART2 interruptss (both transmit and receive
* interrupts). */
void USART2_IRQHandler()
{
    static signed portBASE_TYPE xHigherPriorityTaskWoken;
    serial_ch_msg rx_msg;

    /* If this interrupt is for a transmit... */
    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
            
        /* Diables the transmit interrupt. */
        //USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        //xSemaphoreGiveFromISR(xSemUSART2send, &xHigherPriorityTaskWoken); 

    /* If this interrupt is for a receive... */
    }else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
            /* Receive the byte from the buffer. */
            rx_msg.ch = USART_ReceiveData(USART2);

            /* Queue the received byte. */
            xQueueSendToBackFromISR(xQueueUSART2Recvie, &rx_msg, &xHigherPriorityTaskWoken);  
    }
    if (xHigherPriorityTaskWoken) {
            taskYIELD();
    }
}

