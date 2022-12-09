
// /* Screen simplification
//  < | O | >
// */

// //#include "IOShieldOled.h"

// uint8_t *fillWhite;

// enum Arrow
// {
//     Left,
//     Right
// };

// void setup()
// {
//     IOShieldOled.begin();
//     IOShieldOled.displayOn();
//     IOShieldOled.clear();
//     IOShieldOled.setCursor(0, 0);
//     IOShieldOled.setDrawMode(IOShieldOled.modeSet);
//     fillWhite = IOShieldOled.getStdPattern(1);
//     IOShieldOled.setFillPattern(fillWhite);
// }

// void drawArrow(enum Arrow direction)
// {
//     // triangle
//     int toDraw[] =
//         {
//             // magenta rectangels
//             {0, 15, 4, 2},
//             {5, 18, 2, 2},
//             {8, 20, 2, 2},
//             {11, 22, 3, 2},
//             {15, 24, 2, 2},
//             {18, 26, 2, 2},
//             {21, 28, 2, 2},
//             {5, 12, 2, 2},
//             {8, 10, 2, 2},
//             {11, 8, 3, 2},
//             {15, 6, 2, 2},
//             {18, 4, 2, 2},
//             {21, 2, 2, 2},
//             {25, 0, 2, 32},
//             // blue rectangles
//             {3, 17, 2, 2},
//             {6, 19, 3, 2},
//             {10, 21, 2, 2},
//             {13, 23, 2, 2},
//             {16, 25, 2, 2},
//             {19, 27, 3, 2},
//             {23, 29, 2, 2},
//             {3, 13, 2, 2},
//             {7, 11, 2, 2},
//             {10, 9, 2, 2},
//             {13, 7, 2, 2},
//             {16, 5, 3, 2},
//             {20, 3, 2, 2},
//             {23, 1, 2, 2},
//             // green squares
//             {24, 0, 1, 1},
//             {2, 14, 1, 1},
//             {2, 17, 1, 1},
//             {24, 31, 1, 1}};

//     if (direction == Left)
//     {
//         drawAll(toDraw, sizeof(toDraw) / sizeof(toDraw[0]));
//     }
//     else if (direction == Right)
//     {
//         drawMirrorHorizontalAll(toDraw, sizeof(toDraw) / sizeof(toDraw[0]));
//     }
//     else
//     {
//         //! throw error
//     }
// }

// void drawAll(int list[], int length)
// {
//     // call draw for all values
//     for (int i = 0; i < length; i++)
//     {
//         int row[4] = list[i];
//         draw(row[0], row[1], row[2], row[3]);
//     }
// }

// void drawMirrorHorizontalAll(int list[], int length)
// {
//     // call draw for all values
//     for (int i = 0; i < length; i++)
//     {
//         int row[4] = list[i];
//         draw(128 - row[0] + row[2], row[1], row[2], row[3]);
//     }
// }

// void drawMirrorVericalAll(int list[], int length)
// {
//     // call draw for all values
//     for (int i = 0; i < length; i++)
//     {
//         int row[4] = list[i];
//         draw(row[0], 32 - row[1] + row[3], row[2], row[3]);
//     }
// }

// // draws a square on the screen
// void draw(int Xplace, int Yplace, int Xsize, int Ysize)
// {
//     //! error checking
//     IOShieldOled.moveTo(Xplace, Yplace);
//     IOShieldOled.drawRect(Xsize, Ysize);
//     IOShieldOled.updateDisplay();
// }