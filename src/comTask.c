/* Tasks of Communicating functnion with SIM800h module */
/* Contains the function of Bluetooth and GSM */

/* Initial the SIM800h bluetooth as the server,
 * then waiting for the client(Android phone) to connect.
 * After paring with clinet device,
 * waiting for client to connect with SPP(Serial Port Profile),
 * after that we can communicate between MCU and Android phone 
 * through a wireless serial port(rs232).
 */

#include "comTask.h"

void vBluetoothSPPTask(void *pvParameters)
{
    int sppflag = 0, txFlag = 0;
    char rxStr[80];
    char substitute = 26; //^Z

    vTaskDelay( 2000 / portTICK_PERIOD_MS );
    uprintf("Start initial SIM800h and Bluetooth SPP...\n");
    qprintf(xSemUSART3send, "AT\n");
    

    while(1){
        
        if(vSerialGetLine(xQueueUSART3Recvie, rxStr, 2) == pdPASS)
            uprintf("SIM800h> %s\n", rxStr);

        if(strncmp(rxStr, "+BTPAIRING", sizeof("+BTPAIRING") - 1) == 0){
            qprintf(xSemUSART3send, "AT+BTPAIR=1,1\n");
            uprintf("Bluetooth> Accepts pairing request\n");
        }else if(strncmp(rxStr, "+BTCONNECTING", sizeof("+BTCONNECTING") - 1) == 0){
            qprintf(xSemUSART3send, "AT+BTACPT=1\n");
            uprintf("Bluetooth> Accepts connection\n");
        }else if( strncmp(rxStr, "+BTSPPDATA: 1,15,SIMCOMSPPFORAPP", sizeof("+BTSPPDATA: 1,15,SIMCOMSPPFORAPP") - 1) == 0){
            uprintf("Bluetooth> APP Mode\n");
            sppflag = 1;
        }else if(strncmp(rxStr, "+BTDISCONN", sizeof("+BTDISCONN") - 1) == 0){
            sppflag = 0;
        }
        
        //uprintf("[Flag] %d, %d\n", sppflag, txFlag);

        //Check client is still connected or not.
        if(sppflag){
            int cnt = 0;
            qprintf(xSemUSART3send, "AT+BTSTATUS?\n");
            do{
                vTaskDelay( 20 / portTICK_PERIOD_MS );
                vSerialGetLine(xQueueUSART3Recvie, rxStr, 20);                
                uprintf("SIM800h> %s\n", rxStr);
                if(strncmp(rxStr, "C: 1,", sizeof("C: 1,") - 1) == 0 ){
                    sppflag = 1;
                    txFlag = 1;
                    break;
                }else{
                    sppflag = 0;
                    txFlag = 0;
                }
            }while(cnt++ < 10);
        }

        //client connected and SPP is APP mode
        if(txFlag){
            qprintf(xSemUSART3send, "AT+BTSPPSEND\n");
            vTaskDelay( 20 / portTICK_PERIOD_MS );
            //clear the echo char from SIM800h
            xQueueReset( xQueueUSART3Recvie);
            
            if(xSemaphoreTake(xMutexGPS, 2 / portTICK_PERIOD_MS) == pdTRUE)
                qprintf(xSemUSART3send, "[%s, %s]: %s, %s\n%c",GPS.date, GPS.timeUTC, GPS.latitude, GPS.longitude, substitute);
            while(xSemaphoreGive(xMutexGPS) != pdTRUE);
            
            //The response from SIM800h will delay a few moment.
            vTaskDelay( 50 / portTICK_PERIOD_MS );
            //clear the echo char from SIM800h
            xQueueReset( xQueueUSART3Recvie);
        }
    }
}