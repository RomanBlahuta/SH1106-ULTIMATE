////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                      HEADER                                                      ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************************************************
 *
 * SH1106 ULTIMATE
 * SHU
 *
************************************************************************************************************************
 * Created by Roman Blahuta on 03.12.2019 for UCU Principles of Computer Organization course as a semester project;
 * STM32F411E-DISCOVERY C library for OLED displays;
 *
 * OLED display description:
 * - Resolution: 128x64;
 * - Internal drive chip: SH1106;
 * - Communication protocol: I2C;
 * - I2C3 is used for this library;
 *
 * I2C3 Pinout:
 * - SCL — PA8;
 * - SDA — PC9;
 *
 * Library consists of the following files:
 * - sh1106Ultimate.c;
 * - sh1106Ultimate.h;
 * - fonts.h;
************************************************************************************************************************
 *
 * © Roman Blahuta 2019;
 *
***********************************************************************************************************************/




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                     INCLUDES                                                     ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "fonts.h"
#include "sh1106Ultimate.h"
#include <math.h>

//TODO BLACK LETTERS OVERLAP ERASE
//TODO BITMAPS
//TODO CHECK FONT CHANGE


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                               STRUCTURES AND VARIABLES REPRESENTING DISPLAY STATE                                ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Display display = {
        0,
        0,
        0,
        0, 0
};


uint8_t DisplayBuffer[BUFFERSIZE] = { 0 };
uint16_t *CurrentFont = Font5x7;
uint16_t CurrentFontWidth = 5;
uint16_t CurrentFontHeight = 7;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                    FUNCTIONS                                                     ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                       UTIL                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

void shu_WriteCommand(uint16_t command) {
    display.temp[0] = COMMAND;
    display.temp[1] = command;

    HAL_I2C_Master_Transmit(&hi2c3,OLED_ADDR,display.temp,2,100);
}

/**********************************************************************************************************************/

void shu_WriteData(uint16_t data) {
    display.temp[0] = DATA;
    display.temp[1] = data;

    HAL_I2C_Master_Transmit(&hi2c3,OLED_ADDR,display.temp,2,100);
}

/**********************************************************************************************************************/

uint16_t min(uint16_t a, uint16_t b) {
    return (a < b) ? a : b;
}


uint16_t max(uint16_t a, uint16_t b) {
    return (a > b) ? a : b;
}

uint16_t lineLen(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2) {
    return (uint16_t) sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

/**********************************************************************************************************************/

void shu_DrawHorizontalLine(uint16_t lineLen, Colour colour) {
    uint16_t originX = display.Current_X-2;
    uint16_t originY = display.Current_Y;

    for (int i = 0; i < lineLen; i++) {

        shu_GotoXY(originX + i, originY);
        shu_DrawPixel(colour);
    }

    shu_GotoXY(originX + 2 + lineLen, originY);
}

/**********************************************************************************************************************/

void shu_DrawVerticalLine(uint16_t lineLen, Colour colour) {
    uint16_t originX = display.Current_X-2;
    uint16_t originY = display.Current_Y;

    for (int i = 0; i < lineLen; i++) {

        shu_GotoXY(originX, originY + i);
        shu_DrawPixel(colour);
    }

    shu_GotoXY(originX+2, originY);
}

/**********************************************************************************************************************/

void shu_DrawDiagonalLine(uint16_t x1, uint16_t  y1, uint16_t  x2, uint16_t  y2, Colour colour) {
    uint16_t sX = 0, sY = 0, eX = 0, eY = 0;
    int type = 0;

    //Set up start and end points
    if ( ((x1<x2)&&(y1<y2)) || ((x2<x1)&&(y2<y1)) ) {
        sX = min(x1, x2);
        eX = max(x1, x2);

        sY = min(y1, y2);
        eY = max(y1, y2);

        type = 0;
    }
    else {
        sX = min(x1, x2);
        eX = max(x1, x2);

        sY = max(y1, y2);
        eY = min(y1, y2);
        type = 1;
    }

    shu_GotoXY(sX, sY);
    shu_DrawPixel(colour);
    shu_GotoXY(eX, eY);
    shu_DrawPixel(colour);

    //Check for difference of coordinates == 1;
    int dx = (int)eX - (int)sX - 1;
    int dy = (int)eY - (int)sY - 1;

    if(dy == 0) {
        shu_GotoXY(sX+1, sY);
        shu_DrawHorizontalLine((uint16_t) ceil(dx/2), colour);
        if(type==0) {
            shu_GotoXY(sX + 1 + ceil(dx / 2), sY + 1);
            shu_DrawLine(sX + 1 + ceil(dx / 2) + (uint16_t) ceil(dx / 2) + 1, sY + 1, colour);
        }
        else{
            shu_GotoXY(sX + 1 + ceil(dx / 2), sY - 1);
            shu_DrawLine(sX + 1 + ceil(dx / 2) + (uint16_t) ceil(dx / 2) + 1, sY - 1, colour);
        }
        return;
    }
    else if (dx == 0) {
        shu_GotoXY(sX, sY+1);
        shu_DrawVerticalLine((uint16_t) ceil(dy/2), colour);
        if(type==0) {
            shu_GotoXY(sX + 1, sY + 1 + ceil(dy / 2));
            shu_DrawLine(sX+1, sY + 1 + ceil(dy / 2)+(uint16_t) ceil(dy/2) + 1, colour);
        }
        else {
            shu_GotoXY(sX - 1, sY - 1 - ceil(dy / 2));
            shu_DrawLine(sX-1, sY - 1 - ceil(dy / 2)+(uint16_t) ceil(dy/2) - 1, colour);
        }
        return;
    }

    //Analyze the angle and draw
    int barLen = 0;
    int currX = sX+1;
    int currY = sY+1;


    if(dx > dy) {
        barLen = dx/dy;
        while (currX != eX && currY != eY) {
            shu_GotoXY(currX, currY);
            shu_DrawLine(currX+barLen, currY, colour);
            currX += barLen;
            currY = (type==1) ? (currY-1): (currY+1);
        }
        if (currX != eX) {
            shu_GotoXY(currX, eY-1);
            shu_DrawLine(currX+(eX-currX), eY-1, colour);
        }
        return;
    }
    else {
        barLen = (type==1) ? (dy/dx * -1):(dy/dx);;
        while (currX != eX && currY != eY) {
            shu_GotoXY(currX, currY);
            shu_DrawLine(currX, currY+barLen, colour);
            currY += barLen;
            currX++;
        }
        if (currY != eY) {
            shu_GotoXY(eX-1, currY);
            shu_DrawLine(eX-1, currY+(eY-currY), colour);
        }
        return;
    }


}

/**********************************************************************************************************************/




/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                     DISPLAY                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

void shu_InitDisplay(void) {
    HAL_Delay(100);                 //Delay

    shu_WriteCommand(DISPLAYOFF);

    shu_WriteCommand(SETDISPLAYCLOCKDIV);
    shu_WriteCommand(0x80);

    shu_WriteCommand(SETMULTIPLEX);
    shu_WriteCommand(0x3F);     //0x3F --> 128x64; 0x1F --> 128x32

    shu_WriteCommand(SETDISPLAYOFFSET);
    shu_WriteCommand(0x00);

    shu_WriteCommand(SETSTARTLINE | 0x00);

    shu_WriteCommand(CHARGEPUMP);
    shu_WriteCommand(0x14);     //Internal Charge Pump

    shu_WriteCommand(MEMORYMODE);
    shu_WriteCommand(0x00);

    shu_WriteCommand(SEGREMAP | 0x1);

    shu_WriteCommand(COMSCANDEC);

    shu_WriteCommand(SETCOMPINS);
    shu_WriteCommand(0x12);     //0x12 --> 128x64; 0x02 --> 128x32

    shu_WriteCommand(SETCONTRAST);
    shu_WriteCommand(0xCF);     //Max contrast

    shu_WriteCommand(SETPRECHARGE);
    shu_WriteCommand(0xF1);

    shu_WriteCommand(SETVCOMDETECT);
    shu_WriteCommand(0x40);

    shu_WriteCommand(DISPLAYALLON_RESUME);

    shu_WriteCommand(NORMALDISPLAY);

    shu_WriteCommand(DISPLAYON);

    shu_ClearDisplay();
    display.isInitialized = 1;
}

/**********************************************************************************************************************/

void shu_ChooseFont(FontChoice choice) {
    if (choice == Font_5x7) {
        CurrentFont = &Font5x7;
        CurrentFontWidth = 5;
        CurrentFontHeight = 7;
    }
    else if (choice == Font_4x6) {
        CurrentFont = &Font4x6;
        CurrentFontWidth = 4;
        CurrentFontHeight = 6;
    }
    else if (choice == Font_6x8) {
        CurrentFont = &Font6x8;
        CurrentFontWidth = 6;
        CurrentFontHeight = 8;
    }
    else if (choice == Font_8x8) {
        CurrentFont = &Font8x8;
        CurrentFontWidth = 8;
        CurrentFontHeight = 8;
    }
}

/**********************************************************************************************************************/


void shu_UpdateDisplay(void) {
    for (uint8_t i = 0; i < 8; i++) {
        shu_WriteCommand(0xB0 + i);
        shu_WriteCommand(0x00);
        shu_WriteCommand(0x10);

        HAL_I2C_Mem_Write(&hi2c3, OLED_ADDR, 0x40,
                          1, &DisplayBuffer[WIDTH * i],
                          WIDTH, 100);
    }
}

/**********************************************************************************************************************/





/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                     DRAWING                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

void shu_DrawPixel(Colour colour) {
    if (colour == WHITE) {
        DisplayBuffer[display.Current_X + (display.Current_Y / 8) * WIDTH] |= 1 << (display.Current_Y % 8);
    }
    else {
        DisplayBuffer[display.Current_X + (display.Current_Y / 8) * WIDTH] &= ~(1 << (display.Current_Y % 8));
    }
}

/**********************************************************************************************************************/

void shu_FillDisplay(Colour colour) {
    for(uint32_t i = 0; i < sizeof(DisplayBuffer); i++)
    {
        DisplayBuffer[i] = (colour == BLACK) ? 0x00 : 0xFF;
    }
}

/**********************************************************************************************************************/

void shu_ClearDisplay(void) {
    shu_FillDisplay(BLACK);
}

/**********************************************************************************************************************/

void shu_DrawChar(char chr, Colour colour) {
    uint16_t x = display.Current_X;
    uint16_t y = display.Current_Y;

    if (colour == WHITE) {
        DisplayBuffer[x + (y / 8) * WIDTH] = 0x00;
        DisplayBuffer[x + ((y+8) / 8) * WIDTH] = 0x00;
        x++;

        for (int i = 0; i < CurrentFontWidth; i++) {
            DisplayBuffer[x + (y / 8) * WIDTH] |= CurrentFont[chr*CurrentFontWidth+i] << (y%8);
            DisplayBuffer[x + ((y+8) / 8) * WIDTH] |= CurrentFont[chr*CurrentFontWidth+i] >> (8 - y%8);
            x++;
        }

        DisplayBuffer[x + (y / 8) * WIDTH] = 0x00;
        DisplayBuffer[x + ((y+8) / 8) * WIDTH] = 0x00;
    }
    else {
        DisplayBuffer[x + (y / 8) * WIDTH] = 0xFF;
        DisplayBuffer[x + ((y+8) / 8) * WIDTH] = 0xFF;
        x++;

        for (int i = 0; i < CurrentFontWidth; i++) {
            DisplayBuffer[x + (y / 8) * WIDTH] = ~(CurrentFont[chr*CurrentFontWidth+i] << (y%8));
            DisplayBuffer[x + ((y+8) / 8) * WIDTH] = ~(CurrentFont[chr*CurrentFontWidth+i] >> (8 - y%8));
            x++;
        }
        DisplayBuffer[x + (y / 8) * WIDTH] = 0xFF;
        DisplayBuffer[x + ((y+8) / 8) * WIDTH] = 0xFF;
    }
    x -= 2;
    if (CurrentFontWidth >= 8) {
        x--;
    }
    shu_GotoXY(x, y);
}

/**********************************************************************************************************************/

void shu_DrawString(char *string, Colour colour) {
    while(*string != '\0')
    {
        shu_DrawChar(*string, colour);
        string++;
    }
}

/**********************************************************************************************************************/

uint16_t shu_DrawBitmap(uint16_t xsize, uint16_t ysize, uint16_t* bitmap) {
    if (ysize % 8 != 0) {
        return 0;
    }

    uint16_t cx = display.Current_X;
    uint16_t cy = display.Current_Y;
    uint16_t i = 0;

    for (int y = 0; y < ysize; y+=8) {
        for (int x = 0; x < xsize; x++) {
            DisplayBuffer[cx+x + ((y+cy) / 8) * WIDTH] = bitmap[x];
            i++;
        }
    }
    return 1;
}

/**********************************************************************************************************************/

void shu_DrawLine(uint16_t x, uint16_t y, Colour colour) {
    uint16_t startX, startY, endX, endY;
    uint16_t currX = display.Current_X-2;
    uint16_t currY = display.Current_Y;

    startX = min(x, currX);
    startY = min(y, currY);
    endX = max(x, currX);
    endY = max(y, currY);

    if (currX == x) {
        shu_GotoXY(startX, startY);
        shu_DrawVerticalLine(endY-startY+1, colour);
        shu_GotoXY(endX, endY+1);
    }
    else if (currY == y) {
        shu_GotoXY(startX, startY);
        shu_DrawHorizontalLine(endX-startX+1, colour);
        shu_GotoXY(endX+1, endY);
    }
    else {
        shu_GotoXY(startX, startY);
        shu_DrawDiagonalLine(currX, currY, x, y, colour);
        shu_GotoXY(endX+1, endY+1);
    }
}

/**********************************************************************************************************************/

void shu_DrawRectangle(uint16_t x, uint16_t y, Colour colour) {
    uint16_t startX, startY, endX, endY;
    uint16_t currX = display.Current_X;
    uint16_t currY = display.Current_Y;

    startX = min(x, currX);
    startY = min(y, currY);
    endX = max(x, currX);
    endY = max(y, currY);

    shu_GotoXY(startX, startY);
    shu_DrawLine(startX, endY, colour);

    shu_GotoXY(startX, startY);
    shu_DrawLine(endX, startY, colour);

    shu_GotoXY(endX, endY);
    shu_DrawLine(endX, startY, colour);

    shu_GotoXY(endX, endY);
    shu_DrawLine(startX, endY, colour);
}

/**********************************************************************************************************************/

void shu_FillArea(uint16_t x, uint16_t y, Colour colour) {
    uint16_t startX, startY, endX, endY;
    uint16_t currX = display.Current_X;
    uint16_t currY = display.Current_Y;

    startX = min(x, currX);
    startY = min(y, currY);
    endX = max(x, currX);
    endY = max(y, currY);

    for (int i = startX; i < endX+1; i++) {
        shu_GotoXY(i, startY);
        shu_DrawVerticalLine(endY-startY+1, colour);
    }


}

/**********************************************************************************************************************/





/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                   COORDINATES                                                      */
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

void shu_GotoXY(uint16_t x, uint16_t y) {
    display.Current_X = x+2;
    display.Current_Y = y;
}

/**********************************************************************************************************************/

uint16_t shu_GetX() {
    return display.Current_X;
}

/**********************************************************************************************************************/

uint16_t shu_GetY() {
    return display.Current_Y;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////