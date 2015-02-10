#include "myTasks.h"
#include "myio.h"

void vMyTaskCreate(uint16_t usStackSize, UBaseType_t uxPriority )
{
/* Create that task that handles the console itself. */
    xTaskCreate(    vGPSRawDataParsingTask,  /* The task that implements the command console. */
                    "GPS Parsing Task",                      /* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
                    usStackSize,                /* The size of the stack allocated to the task. */
                    NULL,                       /* The parameter is not used, so NULL is passed. */
                    uxPriority,                 /* The priority allocated to the task. */
                    NULL );                     /* A handle is not required, so just pass NULL. */
}


/*Zero(\0) padding strtok*/
/*This function is the special case of strtok*/
/*Only support one char delimiter to parsing string*/
char *
strtok_Zero(char *s, const char delim)
{
    char c;
    char *tok;
    static char *last;

    //reload last string address
    if (s == NULL && (s = last) == NULL)
        return (NULL);

    tok = s;
    
    //find delimiter or end of string '\0'
    for(c = *s++; ; c = *s++){
        if((c == delim) || (*s == '\0'))break;
    }

    if((c != delim) && (*s == '\0')){
        last = NULL;
        return tok;
    }else{
        //replace comma ',' with '\0'
        s[-1] = '\0';
        last = s;
        return tok;  //return string only with "\0"
    }
}

/* Store needed data into struct */
void vParsing(GPSdata *gps, char *str)
{
    int i = 0;

    char *dest;

    dest = strtok_Zero(str, ',');

    while(dest != NULL){
        
       /* if(*dest == '\0'){
            uprintf("0\n");
        }else{
            uprintf("%s\n", dest);
        }*/

        switch(i++){
            //UTC Time
            case 0:
                strcpy(gps->timeUTC, dest);
                break;
            case 1:
                strcpy(gps->status, dest);
                break;
            case 2:
                strcpy(gps->latitude, dest);
                break;
            case 3:
                strncat(gps->latitude, dest, 1);
                break;
            case 4:
                strcpy(gps->longitude, dest);
                break;
            case 5:
                strncat(gps->longitude, dest, 1);
                break;
            case 6:
                strcpy(gps->speed, dest);
                break;
            case 7:
                strcpy(gps->COG, dest);
                break;
            case 8:
                strcpy(gps->date, dest);
                break;
            default:;
        }
        dest = strtok_Zero(NULL, ',');
    }
}

GPSdata GPS;
void vGPSRawDataParsingTask(void *pvParameters)
{   
    uint16_t GPS_STATE, GPRMC_STATE;
    char readChar;
    char strGPRMC[80];
    char *ptrGPRMC = strGPRMC;
    
    GPS_STATE = 0;
    GPRMC_STATE = 0;

    while(1){
        vSerialGetChar(xQueueUSART2Recvie, &readChar, 0);
        //uprintf("%c", readChar);
        switch(GPS_STATE){
            //Find $ char
            case 0: 
                if(readChar == '$') GPS_STATE = 1;
                break;
            //Find the string "GPRMC"
            case 1: 
                switch(GPRMC_STATE){
                    case 0:
                        if(readChar == 'G') GPRMC_STATE = 1;
                        break;
                    case 1:
                        if(readChar == 'P') GPRMC_STATE = 2;
                        break;
                    case 2:
                        if(readChar == 'R') GPRMC_STATE = 3;
                        break;
                    case 3:
                        if(readChar == 'M') GPRMC_STATE = 4;
                        break;
                    case 4:
                        if(readChar == 'C'){
                            GPRMC_STATE = 0;
                            GPS_STATE = 2;
                            vSerialGetChar(xQueueUSART2Recvie, &readChar, 0);
                        }
                        break;
                    default:;
                }
                break;
            //Pick out the $GPRMC string but exclude $GPRMS at the start of the string
            case 2:
                *ptrGPRMC++ = readChar;
                if(readChar == '\r') GPS_STATE = 3;
                break;
            //Parsing the $GPRMC String
            case 3:
                
                *ptrGPRMC = '\0';
                //Print the picked out string from GPS module
                //uprintf("%s\n", strGPRMC);
                
                vParsing(&GPS, strGPRMC);

                //Reset State and string point.
                GPS_STATE = 0;
                *strGPRMC = '\0';
                ptrGPRMC = strGPRMC;
                break;
            default:;
        }

	}
}