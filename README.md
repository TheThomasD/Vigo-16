# Vigo-16 GRBL Offline Controller Info &amp; Custom Firmware (if I have the time to do it ;))

*Note: if you want to reach me, feel free to create an issue on this repo. I can then open a Telegram channel where we can exchange.*

I have a Vevor CNC 3018-PRO. This machine comes with an offline controller that does not work as I want it to, basically because it does not meet all my requirements. Actually, I can't really remember which of the features is not implemented, but I wanted to play with an ESP32 chip anyway, so why not use this one as a project :) My requirements are:

* Connect to Wifi
* Allow Upload of files via web
* Allow homing and unlocking the board (I installed some endstops)
* Allow control (maybe a little bit more user friendly)
* Allow the display to be connected while the usb port of the GRBL port is also in use (if that is possible)

And as nice to haves:

* Allow for remote connection of GRBL control software via WiFi

For now, I have uploaded the latest GRBL firmware and configured it in the way I wanted it. I also 3D-printed some improvements. Apart from that, I have some experience with the Marlin software and also with reverse engineering of Arduino compatible boards (done for at least 3 3D printers). I also have limited experinece with ESP32-CAM systems. But all this makes me confident that I can improve the offline controller.

## The Controller

The controller features 8 buttons, one micro-SD card slot, a 1.8" 128x160 RGB TFT LCD and an ESP32-WROOM-32E chip. The display shows "Driver IC:ST7735" and "SW-M35 V2.0 210506" printed on its back. The controller board shows the print "VIGO-16 V3.3 211105". There is also another connecter that is usally used for powering devices, but I haven't figured out yet what it is for.

I first idea was to maybe use the cable connection to connect to the board. However, there is another connector on the board named P3 that features 5 pins. I soldered some metal pins to the conenctor to be able to use them. After that, I tried to connect a USB connector directly to the pins. For that I tested which bpin is GND (left-most pin is GND, right-most is nost likely 5V, I guessed). When connected to a PC, the device starts, but no device is recognized on the PC. My theory at this point: the board does not have an integrated USB-to-serial converter.