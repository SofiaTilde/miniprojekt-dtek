#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void)
{
    /*
    The four least significant bits of the return value should contain data from switches SW4, SW3, SW2, and SW1. SW1 corresponds to the least significant bit. All other bits of the return
    value must be zero
    */

    int returnvalue = PORTD;
    // returnvalue &= 0b 1111 0000 0000;
    returnvalue &= 0x00000F00;
    // do some bitshift left
    returnvalue = returnvalue >> 8;

    // display_string(1, returnvalue);

    // returning status on SW1-SW4
    return returnvalue;
}

int getbtns(void)
{

    /*
    The 3 least significant bits of the return value must contain current data from push buttons BTN4, BTN3, and BTN2. BTN2 corresponds to the least significant bit. All other bits of the return value must be zero
    */
    int returnvalue = PORTD;
    // returnvalue &= 0b 1110 0000;
    returnvalue = returnvalue & 0xE0;
    // do some bitshift left
    returnvalue = returnvalue >> 5;

    /*
        char stringbtns[10];
        time2string(stringbtns, returnvalue);
        display_string(2, stringbtns); */

    // returning status on B2-B4
    return returnvalue;
}