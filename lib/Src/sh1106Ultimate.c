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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                               STRUCTURES AND VARIABLES REPRESENTING DISPLAY STATE                                ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Display display = {
    0,
    0,
    0,
    0, 0,
    0, 0, 0, 0, 0, 0, 0
};

uint16_t *CurrentFont = Font5x7;
uint16_t CurrentFontWidth = 5;
uint16_t CurrentFontHeight = 7;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                    FUNCTIONS                                                     ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void shu_GotoXY(uint16_t x, uint16_t y) {
    display.Current_X = x;
    display.Current_Y = y;

    shu_WriteCommand(0xB0 + y);
    shu_WriteCommand((x+2) & 0xf);
    shu_WriteCommand(0x10 | ((x+2) >> 4));
}

/**********************************************************************************************************************/

void shu_ClearDisplay(void) {
    uint16_t i;
    uint16_t x=0;
    uint16_t y=0;
    shu_GotoXY(0,0);

    for (i = 0; i < (BUFFERSIZE); i++) {
        shu_DrawChar(' ');
        x++;
        if(x >= WIDTH) {
            x = 0;
            y++;
            shu_GotoXY(0,y);
        }
    }
    display.Current_X = DEFAULT_SPACE;
    display.Current_Y = 0;
}

/**********************************************************************************************************************/

void shu_DrawChar(char chr) {
    unsigned int character = chr;
    uint16_t x = 0;
    display.Char_Buffer[0] = 0x40;
    for (x=0; x < CurrentFontWidth; x++)
    {
        display.Char_Buffer[x+1] = CurrentFont[character*5+x];
    }
    //if (CurrentFontWidth == 5)
    display.Char_Buffer[6] = 0x00;
    int foo = 12345;
    HAL_I2C_Master_Transmit(&hi2c3, OLED_ADDR, display.Char_Buffer, 7,1000);

    display.Current_X += 8;
    if(display.Current_X>WIDTH)
    {
        display.Current_X = DEFAULT_SPACE;
    }
}

/**********************************************************************************************************************/

void shu_DrawString(char *string) {
    while(*string != '\0')
    {
        shu_DrawChar(*string);
        string++;
    }
}

/**********************************************************************************************************************/

//IMPLEMENT DN

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
        *CurrentFont = Font5x7;
        CurrentFontWidth = 5;
        CurrentFontWidth = 7;
    }
}

/**********************************************************************************************************************/

//TODO DOES NOT WORK PROPERLY
void shu_DrawPixel(Colour colour) {
    uint16_t x = 0;
    display.Char_Buffer[0] = 0x40;
    for (x=0; x<CurrentFontWidth; x++) {
        if (x == 0)
            display.Char_Buffer[x+1] = 0x01;
        else
            display.Char_Buffer[x+1] = 0x00;
    }
    display.Char_Buffer[6] = 0;
    HAL_I2C_Master_Transmit(&hi2c3, OLED_ADDR, display.Char_Buffer, 7,1000);

    display.Current_X += 8;
    if(display.Current_X>WIDTH) {
        display.Current_X = DEFAULT_SPACE;
    }
}

/**********************************************************************************************************************/
//TODO: CREATES SOMETHING LIKE PROGRESS BAR, DOESN'T FILL
/*
void shu_FillDisplay(Colour colour) {
    uint16_t x = 0;
    display.Char_Buffer[0] = 0x40;
    for (x=0; x < WIDTH; x++) {
        display.Char_Buffer[x+1] = 0xFF;
    }
    display.Char_Buffer[6] = 0xFF;
    HAL_I2C_Master_Transmit(&hi2c3, OLED_ADDR, display.Char_Buffer, 7,1000);

    display.Current_X += 8;
    if(display.Current_X>WIDTH) {
        display.Current_X = DEFAULT_SPACE;
    }
}
*/
/**********************************************************************************************************************/

//TODO IMPLEMENT
void shu_DrawBitmap(uint16_t* bitmap) {

}

/**********************************************************************************************************************/

uint16_t shu_GetX() {
    return display.Current_X;
}

/**********************************************************************************************************************/

uint16_t shu_GetY() {
    return display.Current_Y;
}

/**********************************************************************************************************************/

/** Functions for demonstration **/

void DrawHeartL(void) {
    uint16_t x = 0;
    uint8_t HeartBitmapLeft[] = {0xf0, 0xc, 0x2, 0x1, 0x1, 0x2, 0xc, 0x10, 0x1, 0x6, 0x8, 0x10, 0x20, 0x40, 0x80, 0x00};
    display.Char_Buffer[0] = 0x40;
    for (x = 0; x < 8; x++) {
        display.Char_Buffer[x + 1] = HeartBitmapLeft[x];
    }
    HAL_I2C_Master_Transmit(&hi2c3, OLED_ADDR, display.Char_Buffer, 9, 1000);
    shu_GotoXY(display.Current_X, display.Current_Y + 1);

    for (x = 0; x < 8; x++) {
        display.Char_Buffer[x + 1] = HeartBitmapLeft[x + 8];
    }
    HeartBitmapLeft[12] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c3, OLED_ADDR, display.Char_Buffer, 13, 1000);
    display.Current_X += 8;
}



void DrawHeartR(void) {
    uint16_t x = 0;
    uint8_t HeartBitmapRight[] = {0xc, 0x2, 0x1, 0x1, 0x2, 0xc, 0xf0, 0x00, 0x80, 0x40, 0x20, 0x10, 0x4, 0x6, 0x1,
                                  0x00};
    display.Char_Buffer[0] = 0x40;

    shu_GotoXY(display.Current_X, display.Current_Y);
    for (x = 0; x < 11; x++) {
        display.Char_Buffer[x + 1] = HeartBitmapRight[x];
    }
    HeartBitmapRight[12] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c3, OLED_ADDR, display.Char_Buffer, 9, 1000);

    shu_GotoXY(display.Current_X, display.Current_Y + 1);
    for (x = 0; x < 8; x++) {
        display.Char_Buffer[x + 1] = HeartBitmapRight[x + 8];
    }
    HeartBitmapRight[12] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c3, OLED_ADDR, display.Char_Buffer, 13, 1000);
    display.Current_X += 8;
    if (display.Current_X > WIDTH) {
        display.Current_X = DEFAULT_SPACE;
    }
}



void shu_DrawHeart(void) {
    uint16_t x = display.Current_X;
    uint16_t y = display.Current_Y;
    DrawHeartL();
    shu_GotoXY(x+8, y);
    DrawHeartR();
    shu_GotoXY(x+16, y+1);
    shu_DrawChar(' ');
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////