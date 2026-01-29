# ESP32 Wroom with Screen and GPIO Extension.

For those who want to build a project using the ESP32 and an integrated 320×240 pixel screen, there is a range of very interesting boards available. They come with either a resistive or capacitive touchscreen.

## Resistive touchscreen

  - ESP32-2432S028R
    - Display2.8′
    - Display type ILI9341
    - Back Light gpio 27
    
  - ESP32-2432S028
    - Display 2.8′
    - Display type ST7789
    - Back Light gpio 27
    
  - ESP32-2432S024
    - Display 2.4”
    - Display type ILI9341
    - Back Light gpio 21
    
  - ESP32-024
    - Display 2.4′
    - Display type ST7789
    - Back Light gpio 21
    
    
## Capacitiv Touchscreen

  - ESP32-2432S024
    - Display 2.4′
    - Display type ILI9341
    - Back Light gpio 21
    
  - JC2432W328
    - Display 2.8′
    - Display type ST7789
    - Back Light gpio 27

## Board includes:

- an ESP32 Wroom (dual-core)
- 320*240 pixel touchscreen
- an ambient light sensor
- a 3-color LED on the front or back (red: GPIO 4, green and blue: GPIO 16 or 17 depending on the board)
- an XPT2046 resistive touch sensor or a CST820 capacitive touch sensor

## Cabling and pinout
Details are available at [f1atb.fr](https://f1atb.fr/esp32-2432s028-esp32-2432s024-jc2432w328/)

These boards are also called “ESP32-Cheap-Yellow-Display” or CYD and can be found on [Aliexpress](https://s.click.aliexpress.com/e/_c3ACEwsb)..

## Code installation
Install the LovyanGFX library in the Arduino IDE. A Google search will provide the instruction set for controlling the graphics generator.

Using the 115000 baud serial port, if you send H or ?, you will receive a summary of the commands for selecting the card model and display rotation. For devices with a resistive touchscreen, touching the screen will initiate a calibration phase.


