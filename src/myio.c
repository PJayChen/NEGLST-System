#include "myio.h"
#include "String.h"

extern SemaphoreHandle_t xSemUSART1send;
extern SemaphoreHandle_t xSemUSART2send;
extern SemaphoreHandle_t xSemUSART3send;

extern xQueueHandle xQueueUART1Recvie;
extern xQueueHandle xQueueUSART2Recvie;
extern xQueueHandle xQueueUSART3Recvie;

/* USART1 receive data queue */
extern xQueueHandle xQueueUART1Recvie;

/*
 * The function that reads a char from queue
 */
BaseType_t vSerialGetChar(xQueueHandle xQueueH, char *cReadChar, TickType_t xTicksToWait)
{
    while (xQueueReceive(xQueueH, cReadChar, xTicksToWait) == pdFALSE);

    return pdPASS;
}

void _print(char *str, SemaphoreHandle_t SemUSART)
{
    
    USART_TypeDef * whichUSART;

    /*Deside should print string by using which USART*/
    if(SemUSART == xSemUSART1send){
        whichUSART = USART1;            
    }else if(SemUSART == xSemUSART2send){
        whichUSART = USART2;
    }else if(SemUSART == xSemUSART3send){
        whichUSART = USART3;
    }else{
        whichUSART = USART1;
    }


    while( xSemaphoreTake(SemUSART,  portMAX_DELAY) == pdFALSE);

    while(*str){
        if(USART_GetFlagStatus(whichUSART, USART_FLAG_TXE) == SET){
            USART_SendData(whichUSART, *str++);
        }
    }
    //The Interrup will always assert when the TXE bit (SR register) is empty(USART_IT_TXE == 1)
    USART_ITConfig(whichUSART, USART_IT_TXE, ENABLE);
}


//Print String using specific USART
void qprintf(SemaphoreHandle_t SemWhichUSART, const char *format, ...){
    va_list ap;
    va_start(ap, format);
    int curr_ch = 0;
    char out_ch[2] = {'\0', '\0'};
    char newLine[3] = {'\n' , '\r', '\0'};
    char percentage[] = "%";
    char *str;
    char str_num[10];
    char str_out[100] = "";
    int nCnt = 0;

    while( format[curr_ch] != '\0' ){
        
        if(format[curr_ch] == '%'){

            switch(format[curr_ch + 1]){
                case 's':
                    str = va_arg(ap, char *);
                    //prevent str_out is no more space
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, str, nCnt - 1);
                    break;
                case 'd':
                    itoa(va_arg(ap, int), str_num);
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, str_num, nCnt - 1);
                    //str_out = str_num;
                    break;
                case 'c':                  
                    out_ch[0] = (char)va_arg(ap, int);
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, out_ch, nCnt - 1);
                    //str_out = out_ch;
                    break;
                case 'x':
                    xtoa(va_arg(ap, int), str_num);
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, str_num, nCnt - 1);
                    //str_out = str_num;
                    break;
                case '%':
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, percentage, nCnt - 1);
                    //str_out = percentage;
                    break;
                default:;
            }//End of switch(format[curr_ch + 1])

            curr_ch++;

        }else if(format[curr_ch] == '\n'){
            
            //str_out = newLine;
            //prevent str_out is no more space
            nCnt = sizeof(str_out) - strlen(str_out);
            strncat(str_out, newLine, nCnt - 1);
            
        }else{
            
            out_ch[0] = format[curr_ch];
            
            //prevent str_out is no more space
            nCnt = sizeof(str_out) - strlen(str_out);
            strncat(str_out, out_ch, nCnt);
        }
        curr_ch++;
    }//End of while( format[curr_ch] != '\0' )
    _print(str_out, SemWhichUSART); //print on Specific USART
    va_end(ap);
}

//print String using USART1 defaultly.
void uprintf(const char *format, ...){
    va_list ap;
    va_start(ap, format);
    int curr_ch = 0;
    char out_ch[2] = {'\0', '\0'};
    char newLine[3] = {'\n' , '\r', '\0'};
    char percentage[] = "%";
    char *str;
    char str_num[10];
    char str_out[100] = "";
    int nCnt = 0;

    while( format[curr_ch] != '\0' ){
        
        if(format[curr_ch] == '%'){

            switch(format[curr_ch + 1]){
                case 's':
                    str = va_arg(ap, char *);
                    //prevent str_out is no more space
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, str, nCnt - 1);
                    break;
                case 'd':
                    itoa(va_arg(ap, int), str_num);
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, str_num, nCnt - 1);
                    //str_out = str_num;
                    break;
                case 'c':                  
                    out_ch[0] = (char)va_arg(ap, int);
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, out_ch, nCnt - 1);
                    //str_out = out_ch;
                    break;
                case 'x':
                    xtoa(va_arg(ap, int), str_num);
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, str_num, nCnt - 1);
                    //str_out = str_num;
                    break;
                case '%':
                    nCnt = sizeof(str_out) - strlen(str_out);
                    strncat(str_out, percentage, nCnt - 1);
                    //str_out = percentage;
                    break;
                default:;
            }//End of switch(format[curr_ch + 1])

            curr_ch++;

        }else if(format[curr_ch] == '\n'){
            
            //str_out = newLine;
            //prevent str_out is no more space
            nCnt = sizeof(str_out) - strlen(str_out);
            strncat(str_out, newLine, nCnt - 1);
        }else{            
            out_ch[0] = format[curr_ch];
            
            //prevent str_out is no more space
            nCnt = sizeof(str_out) - strlen(str_out);
            strncat(str_out, out_ch, nCnt);
        }
        curr_ch++;
    }//End of while( format[curr_ch] != '\0' )
    _print(str_out, xSemUSART1send); //print on USART1
    va_end(ap);
}//End of void printf(const char *format, ...)
