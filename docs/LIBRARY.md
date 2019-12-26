# Library Contents



># sh1106Ultimate.c

## Functions

Function | Documentation
-------- | ---------
__void shu_InitDisplay(void)__ | Initializes the display by sending the necessary data and commands.
__void shu_WriteCommand(uint16_t command)__ | Sends commands in hex to the display through I2C. _Util function_.
__void shu_WriteData(uint16_t data)__ | Sends data in hex to the display through I2C. _Util function_.
__void shu_GotoXY(uint16_t x, uint16_t y)__ | Sets the cursor to given display coordinates.
__uint16_t shu_GetX();__ | Returns the current X coordinate of the cursor.
__uint16_t shu_GetY();__ | Returns the current Y coordinate of the cursor.
__void shu_ClearDisplay(void)__ | Sets all pixels on the display to Black.
__void shu_ChooseFont(FontChoice choice)__ | Sets the current font to given __FontChoice__.
__void shu_DrawChar(char chr)__ | Draws the character on the display on current coordinates.
__void shu_DrawString(char *string)__ | Draws a string of multiple characters on the display on current coordinates.
__void shu_DrawHeart(void)__ | Draws a 16x16 heart on the display. _Demo function_.



---
---



># sh1106Ultimate.h

## Constants

Constant | Value
-------- | ---------
__SETCONTRAST__ | 0x81
__DISPLAYALLON_RESUME__ | 0xA4
__DISPLAYALLON__ | 0xA5
__NORMALDISPLAY__ | 0xA6
__INVERTEDISPLAY__ | 0xA7
__DISPLAYOFF__ | 0xAE
__DISPLAYON__ | 0xAF
__SETDISPLAYOFFSET__ | 0xD3
__SETCOMPINS__ | 0xDA
__SETVCOMDETECT__ | 0xDB
__SETDISPLAYCLOCKDIV__ | 0xD5
__SETPRECHARGE__ | 0xD9
__SETMULTIPLEX__ | 0xA8
__SETLOWCOLUMN__ | 0x00
__SETHIGHCOLUMN__ | 0x10
__SETSTARTLINE__ | 0x40
__MEMORYMODE__ | 0x20
__COLUMNADDR__ | 0x21
__PAGEADDR__ | 0x22
__COMSCANINC__ | 0xC0
__COMSCANDEC__ | 0xC8
__SEGREMAP__ | 0xA0
__CHARGEPUMP__ | 0x8D
__SWITCHCAPVCC__ | 0x2
__NOP__ | 0xE3
__OLED_ADDR__ | 0x78
__WIDTH__ | 128
__HEIGHT__ | 64
__BUFFERSIZE__ | (__WIDTH__*__HEIGHT__)/8
__DEFAULT_SPACE__ | 5
__COMMAND__ | 0x00
__DATA__ | 0x40

---

## Structures

Structure | Documentation
--------- | ----------
Colour | Constants representing black and white colours of the pixels.
FontChoice | Constants for int numbers to represent a choice of some font. Example: _Font\_5x7_ = __0__.
Display | Represents the state of the display: current coordinates, buffers.



## Variables

Variable | Documentation
--------- | ---------
__uint16_t isInitialized__ | __1__ if display has been initialized, __0__ otherwise.
__uint16_t *CurrentFont__ | Pointer to current used bitmap array. Influenced by __shu_ChooseFont(choice)__.
__uint16_t CurrentFontWidth__ | Width of the font in pixels. Influenced by __shu_ChooseFont(choice)__.
__uint16_t CurrentFontHeight__ | Height of the font in pixels. Influenced by __shu_ChooseFont(choice)__.



---
---



># fonts.h

## Variables

Variable | Documentation
--------- | ---------
__uint16_t Font5x7[0x0500]__ | Bitmap array for the font with width __5__ and height __7__

---
---