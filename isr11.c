

#include"header11.h"
extern int hours, mins, sec;

void __interrupt() isr(void) {
    static unsigned int count;
    if (TMR0IF) { //if timer interrupt is generated
        if (++count == 20000) { //if count is 20000
            count = 0; //count as 0
            if (++sec == 60) { //if sec is 60
                sec = 0;
                mins++; //increment count
                if (mins > 59) { //if mins is greater than 59
                    mins = 0; //make mins as 0
                    hours++; //increment hour
                    if (hours > 23) {
                        hours = 0; //reset hour to 0 if limit is reached
                    }
                }
            }
        }
        TMR0IF = 0; //make timer interrupt flag as 0
    }
}