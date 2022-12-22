#include "accelerometer.h"
#include "displayitems.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* Declarations of uint_32 and the like */
#include <stdbool.h>
#include "pic32mx.h" /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include "mipslabfunc.h"
#include "accelerometer.c"

void render_image(char *data);
void translate_image(char toTranslate[32][128]);
extern void enable_interrupt();

// setting timer period
#define TMR2PERIOD ((80000000 / 256) / 10)
// catch too large period
#if TMR2PERIOD > 0xffff
#error "Timer period is too big."
#endif

volatile int *porte = (volatile int *)0xbf886110;

// accl data variables
char x[17] = "0000000000000000";
char y[17] = "0000000000000000";
char z[17] = "0000000000000000";
int xInt;
int yInt;
int zInt;
int xRot = 0;      // rotation of x that is a value between -180 and 180 (brake)
int yRot = 0;      // rotation of y that is a value between -180 and 180 (left/right)
int zRot = 0;      // rotation of z that is a value between -180 and 180
int rightRot = 30; // the value of y when it should start/stop blinking right
int leftRot = -30; // the value of y when it should start/stop blinking left
int brakeRot = 30; // the value of x when it should start/stop displaying brake

/*
 * Converts a binary char to an integer
 * @param xChars[17] The list of 16 binary numbers in X
 * @param yChars[17] The list of 16 binary numbers in Y
 * @param zChars[17] The list of 16 binary numbers in Z
 */
void binaryToInt(int xChars[17], int yChars[17], int zChars[17])
{
    // converts binary to int
    xInt = 0;
    yInt = 0;
    zInt = 0;
    int numbers[16] = {32744, 16372, 8186, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};
    int i;
    for (i = 0; i < 16; i++)
    {
        if (xChars[i] == '1')
        {
            xInt += numbers[i];
        }
        if (yChars[i] == '1')
        {
            yInt += numbers[i];
        }
        if (zChars[i] == '1')
        {
            zInt += numbers[i];
        }
    }
}

void converter(uint16_t mydata, char accl)
{
    /*
     converts from uint16 to a binary char
    */
    char thebits[17] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
    int i;
    int onebit;
    for (i = 0; i < 16; i++)
    {
        // uint8_t onebit = mydata & 1;
        // onebit = (mydata & (1 << (7 - i)));
        if (mydata & (1 << (15 - i)))
        {
            thebits[i] = '1';
        }
    }

    if (accl == 'x')
    {
        int h;
        for (h = 0; h < 16; h++)
        {
            x[h] = thebits[h];
        }
    }
    else if (accl == 'y')
    {
        int h;
        for (h = 0; h < 16; h++)
        {
            y[h] = thebits[h];
        }
    }
    else if (accl == 'z')
    {
        int h;
        for (h = 0; h < 16; h++)
        {
            z[h] = thebits[h];
        }
    }
    binaryToInt(x, y, z);
}

void addLeftManualArrow()
{
    /*
     adds the array from LeftManualArrow into the Screen Array

     special handling because the LeftManualArrow array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 27; j++)
        {
            if (LeftManualArrow[i][j] == 1)
            {
                Screen[i][j] = 1;
            }
        }
    }
}
void removeLeftManualArrow()
{
    /*
     remove the array from LeftManualArrow into the Screen Array

     special handling because the LeftManualArrow array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 27; j++)
        {
            if (LeftManualArrow[i][j] == 1)
            {
                Screen[i][j] = 0;
            }
        }
    }
}

void addRightManualArrow()
{
    /*
     adds the array from RightManualArrow into the Screen Array

     special handling because the RightManualArrow array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 27; j++)
        {
            if (RightManualArrow[i][j] == 1)
            {
                Screen[i][j + 101] = 1;
            }
        }
    }
}
void removeRightManualArrow()
{
    /*
     remove the array from RightManualArrow into the Screen Array

     special handling because the RightManualArrow array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 27; j++)
        {
            if (RightManualArrow[i][j] == 1)
            {
                Screen[i][j + 101] = 0;
            }
        }
    }
}

void addLeftAutomaticArrow()
{
    /*
     remove the array from LeftAutomaticArrow into the Screen Array

     special handling because the LeftAutomaticArrow array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 27; j++)
        {
            if (LeftManualArrow[i][j] == 1)
            {
                Screen[i][j] = 0;
            }
        }
    }
}

void addRightAutomaticArrow()
{
    /*
     remove the array from RightAutomaticArrow into the Screen Array

     special handling because the RightAutomaticArrow array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 27; j++)
        {
            if (RightAutomaticArrow[i][j] == 1)
            {
                Screen[i][j + 101] = 0;
            }
        }
    }
}

void addBrakeLight()
{
    /*
     adds the array from BrakeLight into the Screen Array

     special handling because the BrakeLight array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 68; j++)
        {
            if (BrakeLight[i][j] == 1)
            {
                Screen[i][j + 30] = 1;
            }
        }
    }
}

void removeBrakeLight()
{
    /*
     remove the array from BrakeLight into the Screen Array

     special handling because the BrakeLight array only contains parts
     that change
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 68; j++)
        {
            if (BrakeLight[i][j] == 1)
            {
                Screen[i][j + 30] = 0;
            }
        }
    }
}

void addXYZ()
{
    /*
     adds the array from XYZ into the Screen Array

     special handling because the XYZ array only contains parts
     that change

     this has two for loops because we change the area of two rectangles
    */
    int i, j;
    for (i = 0; i < 7; i++)
    {
        for (j = 0; j < 128; j++)
        {
            if (XYZScreen1[i][j] == 1)
            {
                Screen[i][j] = 1;
            }
        }
    }
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (XYZScreen2[i][j] == 1)
            {
                Screen[i][j] = 1;
            }
        }
    }
}

void addToScreen(char toAdd[32][128])
{
    /*
     adds the array from toAdd into the Screen Array
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 128; j++)
        {
            if (toAdd[i][j] == 1)
            {
                Screen[i][j] = 1;
            }
        }
    }
}

void removeFromScreen(char toRemove[32][128])
{
    /*
     removes the array from toAdd into the Screen Array
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 128; j++)
        {
            if (toRemove[i][j] == 1)
            {
                Screen[i][j] = 0;
            }
        }
    }
}

void update_screen()
{
    /*
     updates the screen by translating Screen into DrawingScreen that
     is in a format that render_image understands

     render_image draws on the screen
    */
    translate_image(Screen);
    render_image(DrawingScreen);
}

void clearScreen()
{
    /*
     sets every pixel in Screen to 0, erasing the screen contents
    */
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 128; j++)
        {
            Screen[i][j] = 0;
        }
    }
}

void addHistogramBar(int place, int num)
{
    /*
     creates a histogram bar
     uses a manual for-loop to draw the bar
     contains special handling to display numbers > 16
    */
    /*
    Place is 0-4
    0 <|  4-7
    1 |> 10-13
    2 || 16-19
    3 <  22-25
    4 >  28-31

    num is 1+ (17+ is drawn the same)
    num is adjusted to 0+
    */
    num = num - 1;
    if (num > 15)
    {
        num = 15;
        int i, j;
        for (i = (4 + (6 * place)); i < (8 + (6 * place)); i++)
        {
            for (j = 98; j < 100; j++)
            {
                Screen[i][j] = 1;
            }
        }
    }
    if (num < 0)
    {
        return;
    }
    int i, j;
    for (i = (4 + (6 * place)); i < (8 + (6 * place)); i++)
    {
        for (j = 5; j < (8 + (6 * num)); j++)
        {
            Screen[i][j] = 1;
        }
    }
}

void drawOne(int x, int y)
{
    /*
     draws a one with the upper left corners in x, y
    */
    Screen[x][y + 1] = 1;
    Screen[x + 1][y + 1] = 1;
    Screen[x + 2][y + 1] = 1;
    Screen[x + 3][y + 1] = 1;
    Screen[x + 4][y + 1] = 1;
    Screen[x + 1][y] = 1;
    Screen[x + 4][y] = 1;
    Screen[x + 4][y + 2] = 1;
}

void drawZero(int x, int y)
{
    /*
     draws a zero with the upper left corners in x, y
    */
    Screen[x][y] = 1;
    Screen[x][y + 1] = 1;
    Screen[x][y + 2] = 1;

    Screen[x + 1][y] = 1;
    Screen[x + 2][y] = 1;
    Screen[x + 3][y] = 1;

    Screen[x + 1][y + 2] = 1;
    Screen[x + 2][y + 2] = 1;
    Screen[x + 3][y + 2] = 1;

    Screen[x + 4][y] = 1;
    Screen[x + 4][y + 1] = 1;
    Screen[x + 4][y + 2] = 1;
}

void addXYZPlace(int place, char numbers[17])
{
    /*
     adds the array from LeftManualArrow into the Screen Array

     special handling because the LeftManualArrow array only contains parts
     that change
    */
    /*
    Place is 0-2
    0 x
    1 y
    2 z
    */
    int i;
    for (i = 0; i < 16; i++)
    {
        if (numbers[i] == '1')
        {
            // draws a one with the upper left corners in x, y
            drawOne((10 + (place * 8)), (10 + (i * 4)));
        }
        else
        {
            // draws a zero with the upper left corners in x, y
            drawZero((10 + (place * 8)), (10 + (i * 4)));
        }
    }
}

void debug_image(char toDebug[32][128])
{
    /*
     this function prints the screen contents into the terminal

     when code is run on the chipKit it has to be commented out because
     the chipKit doesn't recognize the function printf
    */
    /*
    int i, j;
    printf("\n");
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 128; j++)
        {
            if (toDebug[i][j] == 1)
            {
                printf("□");
            }
            else
            {
                printf("■");
            }
        }
        printf("\n");
    }
    //*/
}

// translate into format render_image can understand
void translate_image(char toTranslate[32][128])
{
    // init loop values
    int i, j, k;
    int placement_index = 0;
    // iterating over the 4 segments of the column
    for (i = 0; i < 4; i++)
    {
        // control segments added
        int index_start, index_end;
        if (i == 0)
        {
            index_start = 0;
            index_end = 7;
        }
        else if (i == 1)
        {
            index_start = 8;
            index_end = 15;
        }
        else if (i == 2)
        {
            index_start = 16;
            index_end = 23;
        }
        else if (i == 3)
        {
            index_start = 24;
            index_end = 31;
        }

        // iterating over columns
        for (j = 0; j < 128; j++)
        {

            int n = 0;
            for (k = index_start; k < index_end; k++)
            {
                // clear bit if zero
                if (toTranslate[k][j] == (char)0)
                {
                    DrawingScreen[placement_index] &= ~((char)1 << n);
                }
                // set bit if 1
                if (toTranslate[k][j] == (char)1)
                {
                    DrawingScreen[placement_index] |= (char)1 << n;
                }
                n++;
            }

            placement_index++;
        }
    }
}

int getsw()
{
    int returnValue = PORTD;
    //               4321
    returnValue &= 0b111100000000;
    returnValue = returnValue >> 8;

    // returning status on SW1-SW4
    // return is 4321
    return returnValue;
}

int getbtns()
{
    int returnValue = PORTD;
    // btns          432
    returnValue &= 0b11100000;
    returnValue = returnValue >> 5;

    int btn1 = PORTF;
    // btn    1
    btn1 &= 0b10;
    btn1 = btn1 >> 1;

    // returning status on B1-B4
    // return is 4321
    returnValue = returnValue << 1;
    returnValue += btn1;
    return returnValue;
}

void ChangeScreen(enum ScreenType ChangeTo)
{
    /*
     changes the screen to ScreenType and draws the main data
    */
    clearScreen();
    if (ChangeTo == MainMenu)
    {
        CurrentScreen = MainMenu;
        addToScreen(MainScreen);
    }
    else if (ChangeTo == Bike)
    {
        CurrentScreen = Bike;
    }
    else if (ChangeTo == XYZ)
    {
        CurrentScreen = XYZ;
        addXYZ();

        // xyz data is updated here
        xyz_data temporary;
        temporary = getACCL_XYZ_I2C(OUT_X_L_XL);

        // converts uint16_t to binary representation
        converter(temporary.x_data, 'x');
        converter(temporary.y_data, 'y');
        converter(temporary.z_data, 'z');

        addXYZPlace(0, x);
        addXYZPlace(1, y);
        addXYZPlace(2, z);
    }
    else if (ChangeTo == Histogram)
    {
        CurrentScreen = Histogram;
        addToScreen(HistogramScreen);
        addHistogramBar(0, 10);
        addHistogramBar(1, 0);
        addHistogramBar(2, 1);
        addHistogramBar(3, 16);
        addHistogramBar(4, 20);
    }
    else
    {
        ChangeScreen(MainMenu);
    }
}

void doMainMenu(int btn1, int btn2, int btn3, int btn4)
{
    if (btn1 == 1)
    {
        ChangeScreen(Bike);
    }
    else if (btn2 == 1)
    {
        ChangeScreen(XYZ);
    }
    else if (btn3 == 1)
    {
        ChangeScreen(Histogram);
    }
    update_screen();
}
void doBike(int btn1, int btn2, int btn3, int btn4)
{
    if (btn4 == 1)
    {
        if (ActiveLeftManualArrow)
        {
            ActiveLeftManualArrow = false;
            removeLeftManualArrow();
        }
        else
        {
            ActiveLeftManualArrow = true;
            addLeftManualArrow();
        }
    }
    if (btn3 == 1)
    {
        if (ActiveRightManualArrow)
        {
            ActiveRightManualArrow = false;
            removeRightManualArrow();
        }
        else
        {
            ActiveRightManualArrow = true;
            addRightManualArrow();
        }
    }
    if (btn2 == 1)
    {
        if (ActiveBrakeLight)
        {
            ActiveBrakeLight = false;
            removeBrakeLight();
        }
        else
        {
            ActiveBrakeLight = true;
            addBrakeLight();
        }
    }
    if (btn1 == 1)
    {
        ChangeScreen(MainMenu);
    }

    // xyz data is updated here
    xyz_data temporary;
    temporary = getACCL_XYZ_I2C(OUT_X_L_G);

    // converts uint16_t to binary representation
    converter(temporary.x_data, 'x');
    converter(temporary.y_data, 'y');
    converter(temporary.z_data, 'z');

    if (ActiveLeftAutomaticArrow && yRot > leftRot)
    {
        ActiveLeftAutomaticArrow = false;
        ActiveLeftManualArrow = false;
        removeLeftManualArrow();
    }
    else if (!ActiveLeftAutomaticArrow && yRot <= leftRot)
    {
        ActiveLeftAutomaticArrow = true;
        if (!ActiveLeftManualArrow)
        {
            addLeftAutomaticArrow();
        }
    }
    if (ActiveRightAutomaticArrow && yRot < rightRot)
    {
        ActiveRightAutomaticArrow = false;
        ActiveRightManualArrow = false;
        removeRightManualArrow();
    }
    else if (!ActiveRightAutomaticArrow && yRot >= rightRot)
    {
        ActiveRightAutomaticArrow = true;
        if (!ActiveRightManualArrow)
        {
            addRightAutomaticArrow();
        }
    }
    if (ActiveBrakeLight && xRot < brakeRot)
    {
        ActiveBrakeLight = false;
        removeBrakeLight();
    }
    else if (!ActiveBrakeLight && xRot >= brakeRot)
    {
        ActiveBrakeLight = true;
        addBrakeLight();
    }

    update_screen();
}
void doXYZ(int btn1, int btn2, int btn3, int btn4)
{
    if (btn1 == 1)
    {
        ChangeScreen(MainMenu);
        update_screen();
    }
    if (btn2 == 1)
    {
        ChangeScreen(XYZ);
        update_screen();
    }
}
void doHistogram(int btn1, int btn2, int btn3, int btn4)
{
    if (btn1 == 1)
    {
        ChangeScreen(MainMenu);
        update_screen();
    }
}

void doScreenStuff()
{
    /*
     this function polls the buttons do decide if the screen should change

     what this function does depends on what the current screen is
    */
    int btns = getbtns();
    int btn1 = btns & 0b0001;
    int btn2 = (btns & 0b0010) >> 1;
    int btn3 = (btns & 0b0100) >> 2;
    int btn4 = (btns & 0b1000) >> 3;

    if (CurrentScreen == MainMenu)
    {
        doMainMenu(btn1, btn2, btn3, btn4);
    }
    else if (CurrentScreen == Bike)
    {
        doBike(btn1, btn2, btn3, btn4);
    }
    else if (CurrentScreen == XYZ)
    {
        doXYZ(btn1, btn2, btn3, btn4);
    }
    else if (CurrentScreen == Histogram)
    {
        doHistogram(btn1, btn2, btn3, btn4);
    }
    else
    {
        ChangeScreen(MainMenu);
        update_screen();
    }
}

void screen_init()
{
    // mask
    TRISD &= 0b000000011111;
    TRISF &= 0b111111111101;
    // set to input
    TRISD += 0b111111100000;
    TRISF += 0b000000000010;

    // initializing volatile pointer to TRISE
    volatile int *trise = (volatile int *)0xbf886100;

    // setting lowest 8 bits to output
    *trise &= 0x00;

    // set intial value to 0
    *porte = 0;

    // setting prescale to 1:256
    T2CON = 0x70;
    // init timer
    PR2 = TMR2PERIOD;

    // setting timer
    TMR2 = 0;

    // setting the on bit (bit 15) to 1 to start the timer
    T2CONSET = 0x8000;

    // enable interrupts globally
    IEC(0) = (1 << 8);

    // enable interrupts from Timer 2
    IPC(2) = 4;

    // init external interrupt for INT2
    IEC(0) += (1 << 11);
    IPC(2) += 0x1F000000;

    // enable_interrupt();
    clearScreen();
    return;
}

int main(void)
{
    // display setup
    initiate_spi();
    display_init();
    display_update();
    screen_init();
    ChangeScreen(MainMenu);
    update_screen();

    // start I2C
    I2C_setup();

    // set up input pins
    TRISDSET = 1 << 8;
    TRISFSET = 1 << 1;

    // acceletometer configurations setup
    ACCEL_config_I2C();

    // continously update screen
    while (1)
    {
        delay_I2C();
        doScreenStuff();
    }

    return 0;
}
