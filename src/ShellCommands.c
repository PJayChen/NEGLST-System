#include "ShellCommands.h"

extern GPSdata GPS;

/*
 * Implements the USART2 serial port forward to USART1 command.
 */
static BaseType_t prvGetGPSDataCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/*
 * Implements the USART3 serial port forward to USART1 command.
 */
static BaseType_t prvUSART3ForwardingCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/*
 * The function that registers the commands that are defined within this file.
 */
void vRegisterSampleCLICommands( void );

static const CLI_Command_Definition_t xGetGPS =
{
	"GPS", /* The command string to type. */
	"GPS: Get Parsed GPS data",
	prvGetGPSDataCmd, /* The function to run. */
	0
};

static const CLI_Command_Definition_t xU3ForwardU1 =
{
	"GSM", /* The command string to type. */
	"GSM: Forward USART3(Connected to sim800h module) to USART1",
	prvUSART3ForwardingCmd, /* The function to run. */
	0
};

static BaseType_t prvGetGPSDataCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char readChar = '\0';

    (void) pcCommandString;

    uprintf("UTC Time: %s\n", GPS.timeUTC);
    uprintf("Status: %s\n", GPS.status);
    uprintf("Latitude: %s\n", GPS.latitude);
    uprintf("Longitude: %s\n", GPS.longitude);
    uprintf("Date: %s\n", GPS.date);
    

    return pdFALSE;
}

static BaseType_t prvUSART3ForwardingCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	char readChar = '\0';

	(void) pcCommandString;

	while(1){
        //vSerialGetChar(xQueueUSART3Recvie, &readChar, 0);
        //uprintf("%c", readChar);
        vSerialGetChar(xQueueUART1Recvie, &readChar, 0);
        qprintf(xSemUSART3send, "%c", readChar);
	}

	return pdFALSE;
}

void vRegisterSampleCLICommands(void)
{
	FreeRTOS_CLIRegisterCommand( &xGetGPS );
	FreeRTOS_CLIRegisterCommand( &xU3ForwardU1 );
}