#include "String.h"


#define MaxDigit 6
/*
* Main part of itoa and xtoa
* Utilize the concept of long division to implement
*/
void _toa(int in_num, char *out_str, int base, int digit){

    int Mdigit = digit;
    int neg = 0;
    out_str[digit--] = '\0';
    
    if(in_num == 0) out_str[digit--] = '0';
    else if(in_num < 0){
        in_num = -in_num;
        neg = 1;
    }

    while(in_num > 0){

        if(base == 16 && in_num % base >= 10)
            out_str[digit--] = (in_num % base) + 'A' - 10;
        else
            out_str[digit--] = (in_num % base) + '0';
        
        in_num /= base;
    }//End of while(in_num > 0)
    
    if(base == 16){
        out_str[digit--] = 'x';
        out_str[digit--] = '0';
    }

    if(neg) out_str[digit--] = '-'; //negative number

digit++;
    //reorder
    int j = 0;
    while(digit < Mdigit + 1){
        out_str[j++] = out_str[digit++];
    }
}

void xtoa(int in_num, char *out_str){
    
    _toa(in_num, out_str, 16, MaxDigit + 4);//MaxDigit + 4 that can contain address
}


void itoa(int in_num, char *out_str){
   
    _toa(in_num, out_str, 10, MaxDigit);
}

size_t strlen(const char *string)
{
    size_t chars = 0;

    while(*string++) {
        chars++;
    }
    return chars;
}

/*
 * Concatenate src on the end of dst.  At most strlen(dst)+n+1 bytes
 * are written at dst (at most n+1 bytes being appended).  Return dst.
 */
// char *
// strncat(char * dst, const char * src, int n)
// {
    
//     if (n != 0) {
//         char *d = dst;
//         const char *s = src;
 
//         while (*d != '\0')
//             d++;
        
//         do {
//             if ((*d = *s++) == '\0')
//                 break;
//             d++;
//         } while (--n != 0);
        
//         *d = '\0';
//     }
//     return (dst);
// }

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