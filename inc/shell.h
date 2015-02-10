#ifndef SHELL_H
#define SHELL_H

/* Standard includes. */
#include "string.h"
#include "stdio.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Example includes. */
#include "FreeRTOS_CLI.h"

/* print to USART includes. */
#include "myio.h"

#include "ShellCommands.h"

/*-----------------------------------------------------------*/

/*
 * The task that implements the command console processing.
 */
void prvUARTCommandConsoleTask( void *pvParameters );
void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );

#endif