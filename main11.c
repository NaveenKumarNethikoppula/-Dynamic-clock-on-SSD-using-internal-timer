

#include "header11.h"
static unsigned char ssd[MAX_SSD_CNT];
static unsigned char digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
int hours, mins, sec; //declare the variables globally

void main(void) {
    int key, flag = 0, select_flag = 0; //declare the variables
    unsigned int delay = 0, wait = 0; //declare variables for delay
    init_config(); //call the configuration function
    while (1) {
        key = read_digital_keypad(STATE_CHANGE); //continuously read the keypad
        if (key == SWITCH4) { //if switch4 is pressed
            TMR0ON = !TMR0ON; //turn off the timer
            flag = !flag; //toggle the flag
            wait = 0; //initialize the wait as 0
        }
        if (flag == 1) { //if flag is 1
            if (key == SWITCH3) { //if switch 3 is pressed
                select_flag = !select_flag; //toggle the select flag
                wait = 0; //make wait as 0
                sec = 0; //start seconds to 0
            }
            if (select_flag == 0) { //if select flag is 0
                ssd[0] = digit[hours / 10]; //keep hours as constant
                ssd[1] = digit[hours % 10];

                if (wait++ == 100) { //for blinking the mins ssd 
                    ssd[2] = digit[mins / 10];
                    ssd[3] = digit[mins % 10];
                } else if (wait == 200) { //after delay blink again
                    wait = 0; //make delay as 0
                    ssd[2] = 0x40;
                    ssd[3] = 0x40;
                }

                if (key == SWITCH1) { //if key is switch 1
                    mins++; //increment
                    if (mins > 59) { //if mins is greater than 59 reset as 0
                        mins = 0;
                    }

                } else if (key == SWITCH2) { //if key is switch 2
                    mins--; //decrement the mins
                    if (mins < 0) { //if mins is lesss than 0 make as 59
                        mins = 59;
                    }
                }
            } else if (select_flag == 1) { //if select flag is 1
                ssd[2] = digit[mins / 10]; //keep mins ssd as constant
                ssd[3] = digit[mins % 10];
                if (wait++ == 100) //for delay 100
                {
                    ssd[0] = digit[hours / 10]; //blink the hours ssd
                    ssd[1] = digit[(hours % 10)];
                } else if (wait == 200) {
                    wait = 0;
                    ssd[0] = 0x40;
                    ssd[1] = 0x40;
                }
                if (key == SWITCH1) { //if key switch 1
                    hours++; //increment hours
                    if (hours > 23) { //if hours is greater than 23
                        hours = 0; //make them 0
                    }
                } else if (key == SWITCH2) { //if key is switch 2
                    hours--; //decrement hours
                    if (hours < 0) {
                        hours = 23; //reset them to 23
                    }
                }
            }
        } else {
            if (sec % 2 == 0) {
                delay = 0;
                ssd[1] = digit[(hours % 10)] | DOT; //display ssd with dot for even sec
            } else {
                ssd[1] = digit[hours % 10]; //for odd remove led
            }
            ssd[0] = digit[hours / 10];
            ssd[2] = digit[mins / 10]; //display ssd with hours and mins
            ssd[3] = digit[mins % 10];
        }
        display(ssd); //display the value on ssd independently
    }
}

void init_config() {
    init_digital_keypad(); //configure the digital keypad
    init_ssd_control(); //configure the ssd port
    init_timer0(); //configure the timer0 
}

void init_timer0() {
    PEIE = 1;
    ADCON1 = 0x0F;
    T08BIT = 1;

    /* Selecting internal clock source */
    T0CS = 0;

    /* Enabling timer0*/
    TMR0ON = 1;

    /* disabling prescaler*/
    PSA = 1;

    TMR0 = 6;

    /* Clearing timer0 overflow interrupt flag bit */
    TMR0IF = 0;

    /* Enabling timer0 overflow interrupt */
    TMR0IE = 1;
    GIE = 1;
}