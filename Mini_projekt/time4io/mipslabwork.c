/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2022-09-27 by Sara Danielsson

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

volatile int *porte = (volatile int *)0xbf886110;

/* Interrupt Service Routine */
void user_isr(void)
{
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{
  // initializing volatile pointer to TRISE
  volatile int *trise = (volatile int *)0xbf886100;

  // setting lowest 8 bits to output
  *trise &= 0x00;

  // set intial value to 0
  *porte = 0;
  // return;
}

/* This function is called repetitively from the main program */
void labwork(void)
{
  TRISD &= 0b000000011111;
  TRISD += 0b111111100000;

  delay(1000);
  time2string(textstring, mytime);
  display_string(3, textstring);
  display_update();
  tick(&mytime);
  // increase value for each tick
  *porte += 1;
  display_image(96, icon);

  int btns = getbtns();
  if (btns > 0)
  {
    int sws = getsw();
    if ((btns & 0b001) == 0b001) // BTN2
    {
      /*
      Pressing BTN2 copies the value from SW4 through SW1 into the third digit of mytime. If SW4 through SW1 are set to 0100, time would change from, say, 16:53 into 16:43.
      */
      mytime &= 0xFF0F;
      mytime += (sws << 4);
    }
    if ((btns & 0b010) == 0b010) // BTN3
    {
      /*
      Pressing BTN3 copies the value from SW4 through SW1 into the second digit of mytime. If SW4 through SW1 are set to 0100, time would change from, say, 16:53 into 14:53.
      */
      mytime &= 0xF0FF;
      mytime += (sws << 8);
    }
    if ((btns & 0b100) == 0b100) // BTN4
    {
      /*
      Pressing BTN4 copies the value from SW4 through SW1 into the first digit of mytime. If SW4 through SW1 are set to 0100, time would change from, say, 16:53 into 46:53.
      */
      mytime &= 0x0FFF;
      mytime += (sws << 12);
    }
  }
}
