
# HD44780 16x2 LCD Driver for AVR (ATmega8)

## Overview

This project provides a lightweight and highly functional C library for interfacing an **HD44780-based 16x2 LCD** using a **4-bit data interface** with an **ATmega8 AVR microcontroller**. It includes:

- Full initialization and instruction handling
- Custom bold font digits using CGRAM
- Print support for strings and integers
- Application demo: a simple weather monitor UI using custom fonts

---

## File Structure

| File                  | Description                                                  |
|-----------------------|--------------------------------------------------------------|
| `lcd_16x2_lib.h`      | Header file containing macros, pin mappings, and prototypes  |
| `lcd_16x2_lib.c`      | Implementation of LCD functions and custom digit graphics    |
| `weather_monitor_demo.c` | Demo application simulating a weather display on LCD         |

---

## Features

- ✅ **4-bit LCD mode support**
- ✅ **Cursor positioning and display control**
- ✅ **Display of integers and strings**
- ✅ **Custom 5x8 font digits for bold display**
- ✅ **Efficient use of CGRAM for visual enhancement**
- ✅ **Busy flag checking for robust timing**

---

## LCD Pin Configuration

- LCD is connected using 4-bit data mode.
- Control and data lines are defined in `lcd_16x2_lib.h`.

```
LCD Pins:
 RS -> PD0
 RW -> PD1
 EN -> PD2
 PWR -> PD3
 D4 -> PC0
 D5 -> PC1
 D6 -> PC2
 D7 -> PC3
```

---

## Demo Output (`weather_monitor_demo.c`)

A sample UI is displayed on the LCD in rotation:
1. Title screen: `"Weather Monitor"`, version info.
2. **Temperature and Humidity** in bold digits.
3. **Pressure** in hPa, also in bold.

```c
T_int = 24;      // °C
H_int = 53;      // %
P_int = 1017;    // hPa
```

---

## How to Use

1. Include the files in your AVR C project.
2. Adjust I/O pin mapping in `lcd_16x2_lib.h` if needed.
3. Call `lcd_init()` once in `main()`.
4. Use utility functions like `print_string_upto_16_char()` and `print_bold_int()` to display data.

---

## Requirements

- **AVR-GCC**
- **WinAVR / AVRDUDE** for flashing
- **ATmega8** or compatible microcontroller
- **16x2 HD44780 LCD**

---

## License

MIT License © 2025 Jawad T. Bin Taufik  
Feel free to use, modify, and distribute.

---

## Author

**Jawad T. Bin Taufik**  
Embedded Systems Developer | June 2025
EMAIL: jawad.tahmeed.71@gmail.com
---
