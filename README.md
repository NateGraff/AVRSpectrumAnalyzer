# AVR Spectrum Visualizer Project
Nate Graff
Created as a final project for Cal Poly CPE 329

## What does it do?

Displays the magnitude of the audio spectrum from about 700 Hz to about 3 kHz on an 8x8 LED Matrix

## What parts does it use?
* ATmega328P (courtesy of an Arduino Duemilanove)
* MAXIM MAX7219CNG SPI-Interfaced 8-digit 7-segment display driver
* 8x8 LED matrix, common-cathode (Vf ~ 2.0 V, If ~ 20 mA)
* Adafruit Electret Microphone Amplifier - MAX9814 with Auto Gain Control
* 28 kohm resistor

## Can/Should I build one?
Yes/No. Don't use the MAX7219CNG. Get a prebuilt 8x8 matrix controller module and use that. The pinout of this chip is inane. If you do use a better matrix driver, you'll need to modify the encode_display() function, which transforms the vram matrix to match what I had to do to have a sane wiring harness.

## I want to use the MAX7219CNG, how did you wire it?
![MAX7219CNG Wiring Diagram](https://raw.githubusercontent.com/NateGraff/AVRSpectrumAnalyzer/master/MAX7219CNGWiringDiagram.png)

## What FFT Library did you use?
I used [Open Music Labs' Arduino FHT Library](http://wiki.openmusiclabs.com/wiki/ArduinoFHT). I used the Hartley transform so I wouldn't bother wasting cycles computing phase data.
