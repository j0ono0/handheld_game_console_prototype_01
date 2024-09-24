# Handheld game console and Arduino game

## Hardware

- PBC designed with KiCad (manufactured by PCBWay)
- Teensy 4 development board 
- TFT display with incorporated touchscreen and SD card reader

### TFT display

The display includes a screen, touch interface, and SD card reader. Each of these features are controlled as separate SPI devices.

The SD card reader was successfully enabled by connecting the jumper 'J1' point. This allowed the display to be **powered with 5v**.

### Breadboard limitations (probably)
On the breadboard initialising up to two features of the TFT display worked reliably. On attempting all three (display, touch, and SD card reader) behaviour became unpredictable with features inconsistently failing/passing initialisation Rewiring the breadboard with shorter wires improved reliability to some extent. All issues were entirely resolved on the final PCB. So presumably a limitation of prototyping on a breadboard