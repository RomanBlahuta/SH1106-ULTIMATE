# SH1106-ULTIMATE
__SH1106-ULTIMATE__ is a C library for manipulations with OLED displays with __SH1106__ internal drive chip. This library is written for __STM32F411E-DISCOVERY__ and uses the __HAL__ library.

The current version of the library supports only displays with __128x64__ resolution.

The communication between the display and __STM32F411E-DISCOVERY__ is implemented through the __I2C__ bus (__I2C3__ on the __STM__ kit by default).

### \-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-
### __UNDER DEVELOPMENT__
### \-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

---
---

## Documentation

docs/ | Contents
----- | ------
__LIBRARY.md__ | Description of all functions, constants and structures of the library
__REQUIREMENTS.md__ | Necessary libraries for __SH1106-ULTIMATE__
__HARDWARE.pdf__ | Description of the hardware devices for this library

---
---

## Library Contents

lib/ | Contents
------ | ------
sh1106Ultimate.c | Function implementations, structures etc.
sh1106Ultimate.h | Constants for OLED initialization, function declarations, structure definitions etc.
fonts.h | Bitmaps for fonts of different sizes

---
---

## Examples

### In the "examples/" directory you can find a project which uses some of the functions as an example. To see the pinout and wiring scheme go to __HARDWARE.pdf__ or see the comments in the __sh1106Ultimate.c__ or open the given example in STM32CubeMX.

---
---

## Issues and future plans

* ### Current Library is still being developed. There are possibly many bugs!!!!!!!
* ### Add functions for drawing geometric figures on the display;
* ### Add support for SSD1306;
* ### Currently there is only 5x7 font available;
* ### Add support for displays with different resolution;
* ### Add ability to configure the size of display;
* ### Library optimization;

## Bugs
* ### __shu_DrawChar__ works only with 5x7 font. Cause: magic number in implementation. Usage of variables creates distortion on the display.
* ### Writing on same coordinates overrides characters.
* ### Structure attributes behaved unpredictably when there was an attempt to perform a check if display.isInitialized == 1;

---
---