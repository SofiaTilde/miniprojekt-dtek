#include <stdio.h>

volatile int *porte;
enum ScreenType
{
    MainMenu,
    Bike,
    XYZ,
    Histogram
};
enum ScreenType CurrentScreen = MainMenu;

int ActiveLeftAutomaticArrow = 0;
int ActiveRightAutomaticArrow = 0;
int ActiveLeftManualArrow = 0;
int ActiveRightManualArrow = 0;
int ActiveBrakeLight = 0;

int sensitivity = 0;
int timeoutcount = 0;

/* char x[17] = "0000000000000000";
char y[17] = "0000000000000000";
char z[17] = "0000000000000000"; */