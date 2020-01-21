# SH1106-ULTIMATE
__SH1106-ULTIMATE__ is a C library for manipulations with OLED displays with __SH1106__ internal drive chip. This library is written for __STM32F411E-DISCOVERY__ and uses the __HAL__ library.

The current version of the library supports only displays with __128x64__ resolution.

The communication between the display and __STM32F411E-DISCOVERY__ is implemented through the __I2C__ bus (__I2C3__ on the __STM__ kit by default).

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

In the "examples/" directory you can find a project which uses some of the functions as an example. To see the pinout and wiring scheme go to __HARDWARE.pdf__ or see the comments in the __sh1106Ultimate.c__ or open the given example in STM32CubeMX.

---
---

## Issues and future plans

* Add support for SSD1306 and multiple displays at the same time;
* Add support for displays with different resolution;
* Add ability to configure the size of display;
* Library optimization;
* Implement util checks for more reliable work process;

## Bugs
* shu_DrawLines does not work properly with diagonal lines, especially ascending ones;
* When choosing BLACK colour when drawing a char or a string, the letters above or below will be partially erased. This is due to bit shifting when writing on __y__ coords that are not divisible by 8;
---
---