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

The controller features 8 buttons, one micro-SD card slot, a 1.8" 128x160 RGB TFT LCD and an [ESP32-WROOM-32E](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf) chip. The display shows "Driver IC:ST7735" and "SW-M35 V2.0 210506" printed on its back ([this](https://techoverflow.net/2023/05/30/minimal-esp32-platformio-128x160px-tft-display-example-using-adafruit-st7735/) might be helpful in the future). The controller board shows the print "VIGO-16 V3.3 211105". There is also another connecter that is usally used for powering devices, but I haven't figured out yet what it is for.

### Connecting the controller

I first idea was to maybe use the cable connection to connect to the board. However, there is another connector on the board named P3 that features 5 pins. I soldered some metal pins to the conenctor to be able to use them. After that, I tried to connect a USB connector directly to the pins. For that I tested which bpin is GND (left-most pin is GND, right-most is nost likely 5V, I guessed). When connected to a PC, the device starts, but no device is recognized on the PC. My theory at this point: the board does not have an integrated USB-to-serial converter.

I measured that one of the pins is connected to the TXD 0 pin of the ESP chip ([see pinout here](https://mischianti.org/2021/05/26/esp32-wroom-32-high-resolution-pinout-and-specs/)). This seemed to confirm my guess. This now leaves me with several options that I think I could try:

* use an USB-to-serial converter I bought a while ago to debug serial connections
* try use an USB ASP/ISP device (would that work?)
* try to use the ESP32-CAM-MB board that came with my ESP32-CAM devices

I went for the last option as I thought this setup was already very close to the setup I found.

The first thing I want to do is store a backup of the flash on the chip so I can restore the original firmware (so I don't end up with an unsable display during the development process). On [Youtube](https://www.youtube.com/watch?v=2GwzbBn7uRw) I found a video on how to backup an ESP32. However, I only used the first few minutes to start my work. In there he basically explains on how to find the right commands via ArduinoIDE to use the `esptool` to dump the flash. This is done by configuring the ArduinoIDE correctly for an ESP32 board and run the upload with the USB-to-serial device connected, but without an ESP32 board connected.

Next, I had to figure out which pins are connected to what. The ESP32-CAM-MB connects to the back of the [ESP32-CAM](https://mischianti.org/2021/09/02/esp32-cam-high-resolution-pinout-and-specs/) module. Based on this I figured out the connections (pins numbers from left to right on the VIGO-16 board):

1. GND -> second pin from top left
2. Reset -> bottom right pin
3. TX -> second pin from bottom right
4. RX -> third pin from bottom right
5. 5V -> top left pin

This connection allows for automatic reset via the ESP32-CAM-MB. Now I tried to determine the ESP32 system info for the connected board, but the `esptool` reported that the device was in the wrong boot mode. From the ESP32-CAM modules I know that a specific button has to be pressed to put the board into upload mode. With my multimeter I could find that the ESC button on the VIGO-16 board is connected to `GPIO0`. After keeping the ESC button pressed during conenction, the `esptool` can finally connect to the chip.

### Making a Backup

First, I need to find out which kind of chip is used. I ran the following command (use the COM port you have; as you can see, I use Windows BTW ;) Should work similarly on Linux):

```
esptool.exe --port "COM8" flash_id
```

The output is:

```
esptool.py v4.5.1
Serial port COM8
Connecting............
Detecting chip type... Unsupported detection protocol, switching and trying again...
Connecting....
Detecting chip type... ESP32
Chip is ESP32-D0WD-V3 (revision v3.0)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: xx:xx:xx:xx:xx:xx (replaced)
Uploading stub...
Running stub...
Stub running...
Manufacturer: 20
Device: 4016
Detected flash size: 4MB
Hard resetting via RTS pin...
```

This gives me the info that I need to pull a backup: the flash size. With this, I can pull the backup of the chip with the followin command:

```
esptool.exe --chip esp32 --port "COM8" --baud 921600 read_flash 0 0x400000 flash_contents.bin
```

The resulting file can be found in the "backup" folder (I created two additional backups, just to be sure ;)).

### Restoring a Backup

Then, I tried to restore the backup. This is done via this command:

```
esptool.exe --chip esp32 --port "COM8" --baud 921600  --before default_reset --after hard_reset write_flash  -z --flash_mode dio --flash_freq 40m --flash_size detect 0x0 flash_contents.bin
```

This worked for me, the display seems to work as before. Hence, I am now safe to run another software on the system I just need to start implementing :)

## Display connection

To configure the connection to the display correctly, I need to figure out the connections of the pins first. Then, I also need to understand how the configuration of the display libraries work. I measured the pins of the display and this is the result and added the pin descriptions that are very hard to read from the LCD board (LCD1 pins from top to bottom seen from the top of the controller board):

1. BL? -> via R19 to pin 27
2. CS? -> via R8 to pin 21
3. DC? -> via R7 to pin 18
4. RES -> via R6 to pin 22
5. SDA? -> via R5 to pin 17
6. SCL? -> via R4 to pin 20
7. VCC
8. GND

