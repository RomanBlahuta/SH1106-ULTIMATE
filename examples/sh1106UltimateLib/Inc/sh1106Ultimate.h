////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                     INCLUDES                                                     ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "i2c.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                    CONSTANTS                                                     ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** OPERATION CONSTANTS **/
#define SETCONTRAST 0x81
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYALLON 0xA5
#define NORMALDISPLAY 0xA6
#define INVERTEDISPLAY 0xA7
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define SETDISPLAYOFFSET 0xD3
#define SETCOMPINS 0xDA
#define SETVCOMDETECT 0xDB
#define SETDISPLAYCLOCKDIV 0xD5
#define SETPRECHARGE 0xD9
#define SETMULTIPLEX 0xA8
#define SETLOWCOLUMN 0x00
#define SETHIGHCOLUMN 0x10
#define SETSTARTLINE 0x40
#define MEMORYMODE 0x20
#define COLUMNADDR 0x21
#define PAGEADDR   0x22
#define COMSCANINC 0xC0
#define COMSCANDEC 0xC8
#define SEGREMAP 0xA0
#define CHARGEPUMP 0x8D
#define SWITCHCAPVCC 0x2
#define NOP 0xE3

/** OLED PARAMETERS **/
#define OLED_ADDR 0x78
#define WIDTH 132
#define HEIGHT 64
#define BUFFERSIZE (WIDTH*HEIGHT)/8
#define DEFAULT_SPACE 5

/** COMMAND/DATA CONSTANTS **/
#define COMMAND 0x00
#define DATA   0x40


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 TYPE DEFINITIONS                                                 ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** COLOUR TYPE ENUMERATION **/
typedef enum {
    BLACK = 0x00,
    WHITE = 0x01
} Colour;


/** ENUMERATION OF CONSTANTS FOR SUPPORTED FONT CHOICES **/
typedef enum {
    Font_4x6 = 0,
    Font_5x7 = 1,
    Font_6x8 = 2,
    Font_8x8 = 3
} FontChoice;


/** DISPLAY STRUCTURE **/
typedef struct {
    int isInitialized;
    uint16_t Current_X;
    uint16_t Current_Y;
    uint8_t temp[2];
} Display;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                              FUNCTION DECLARATIONS                                               ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** Function for sending the command to the display controller **/
void shu_WriteCommand(uint16_t command);

/** Function for sending data to the display controller **/
void shu_WriteData(uint16_t data);

/** Function for calculating the length of a line given by the coordinates of its endpoints **/
uint16_t lineLen(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);




/** Function for initializing the display **/
void shu_InitDisplay(void);

/** Function for setting the current used font **/
void shu_ChooseFont(FontChoice choice);

/** Function for sending the edited buffer to the display **/
void shu_UpdateDisplay(void);




/** Function for setting a pixel either to 0(Black) or 1(White) **/
void shu_DrawPixel(Colour colour);

/** Function for drawing a character on the display **/
void shu_DrawChar(char chr, Colour colour);

/** Function for drawing a string of characters on the display**/
void shu_DrawString(char *string, Colour colour);

/** Function for clearing the display **/
void shu_ClearDisplay(void);

/** Function for filling the display with chosen colour **/
void shu_FillDisplay(Colour colour);

/** Function for drawing a given bitmap that returns a boolean indicator of this operation's success**/
uint16_t shu_DrawBitmap(uint16_t x, uint16_t y, uint16_t* bitmap);

/** Function for drawing a line from current coordinates to the given ones in chosen colour **/
void shu_DrawLine(uint16_t x, uint16_t y, Colour colour);

/** Function for drawing a rectangle in chosen colour with vertices defined by the current and given coordinates **/
void shu_DrawRectangle(uint16_t x, uint16_t y, Colour colour);

/** Function for filling the rectangular area with the colour of choice with vertices defined by the current and given coordinates **/
void shu_FillArea(uint16_t x, uint16_t y, Colour colour);




/** Function for setting the current coordinates on the display **/
void shu_GotoXY(uint16_t x, uint16_t y);

/** Function for getting the X coordinate of the cursor **/
uint16_t shu_GetX();

/** Function for getting the Y coordinate of the cursor **/
uint16_t shu_GetY();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////