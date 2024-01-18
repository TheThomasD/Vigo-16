# Vevor CNC 3018 PRO Vigo-16 GRBL Offline Controller Info &amp; Custom Firmware (if I have the time to do it ;))

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

The controller features 8 buttons, one micro-SD card slot, a 1.8" 128x160 RGB TFT LCD and an [ESP32-WROOM-32E](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf) chip. The display shows "Driver IC:ST7735" and "SW-M35 V2.0 210506" printed on its back ([this](https://techoverflow.net/2023/05/30/minimal-esp32-platformio-128x160px-tft-display-example-using-adafruit-st7735/) might be helpful in the future). The controller board shows the print "VIGO-16 V3.3 211105". There is also another connecter that is usally used for powering devices, but I haven't figured out yet what it is for (I think it is just another way to power the device ;)).

<image src="images/Board-with-Display.jpg" width="50%" />
<image src="images/Board-with-Display-Back.jpg" width="50%" />
<image src="images/Board-with-ESP32-WROOM-32E.jpg" width="50%" />

### Connecting the controller

I first idea was to maybe use the cable connection to connect to the board. However, there is another connector on the board named P3 that features 5 pins. I soldered some metal pins to the conenctor to be able to use them. After that, I tried to connect a USB connector directly to the pins. For that I tested which bpin is GND (left-most pin is GND, right-most is nost likely 5V, I guessed). When connected to a PC, the device starts, but no device is recognized on the PC. My theory at this point: the board does not have an integrated USB-to-serial converter.

I measured that one of the pins is connected to the TXD 0 pin of the ESP chip ([see pinout here](https://mischianti.org/2021/05/26/esp32-wroom-32-high-resolution-pinout-and-specs/) and [data sheet here](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf)). This seemed to confirm my guess. This now leaves me with several options that I think I could try:

* use an USB-to-serial converter I bought a while ago to debug serial connections
* try use an USB ASP/ISP device (would that work?)
* try to use the ESP32-CAM-MB board that came with my ESP32-CAM devices

I went for the last option as I thought this setup was already very close to the setup I expected to work.

<image src="images/ESP32-CAM-MB.jpg" width="50%" />

The first thing I want to do is store a backup of the flash on the chip so I can restore the original firmware (so I don't end up with an unsable display during the development process). On [Youtube](https://www.youtube.com/watch?v=2GwzbBn7uRw) I found a video on how to backup an ESP32. However, I only used the first few minutes to start my work. In there he basically explains on how to find the right commands via ArduinoIDE to use the `esptool` to dump the flash. This is done by configuring the ArduinoIDE correctly for an ESP32 board and run the upload with the USB-to-serial device connected, but without an ESP32 board connected.

Next, I had to figure out which pins are connected to what. The ESP32-CAM-MB connects to the back of the [ESP32-CAM](https://mischianti.org/2021/09/02/esp32-cam-high-resolution-pinout-and-specs/) module. Based on this I figured out the connections (pins numbers from left to right on the VIGO-16 board):

1. GND -> second pin from top left
2. Reset -> bottom right pin
3. TX -> second pin from bottom right
4. RX -> third pin from bottom right
5. 5V -> top left pin

<image src="images/USB-Connector-Display.jpg" width="50%" />
<image src="images/USB-Connector-ESP32-CAM-MB.jpg" width="50%" />

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

1. BLK -> via R19 to pin 28 -> GPIO 17 (Backlight)
2. CS  -> via R8 to pin 23  -> GPIO 15 (Chip Select/Slave Select)
3. D/C -> via R7 to pin 14  -> GPIO 12 (Data/Command; =RS Register Select?)
4. RES -> via R6 to pin 24  -> GPIO  2
5. SDA -> via R5 to pin 13  -> GPIO 14 (SPI Data; connect to MOSI?)
6. SCL -> via R4 to pin 16  -> GPIO 13 (SPI Clock)
7. VCC
8. GND

## Programming the ESP32

I created a simple "Hello World!" example for the ESP32 in basically no time and it was running sucessfully. While uploading the program to the ESP32 worked, I couldn't see any output when connecting the serial monitor. A little googeling helped me to figure out the correct settings in the PlatformIO ini file.

### Controlling the Display

My next goal then was to show some output on the display to give me some feedback (beside the serial monitor that I can use for this). However, I had a steep learning curve to tackle at this point. I tried to simply use the pin numbers from the sites linked above. No luck. The numbers in the black squares in the picture from `mischianti.org` didn't work and neither did the pin numbers from the data sheet. Sometimes the display flickered and I heard something coming from the speaker, but the display did not show anything. This led me to two ideas:

1. the pins could be incorrect (but I verified them again with my multimeter)
2. maybe the default pin settings of the ESP32 variant I am using are conflicting with my setup

First, I introduced my own variant of the ESP32 so I would be able to define all the pins to my requirements. Figuring out how this works was not that easy, but my default program worked even after using my own variant. Then, I tried to connect to the display again, again with no luck.

This led me to read something about the different communication methods of an ESP32: I2C and SPI. After understanding the difference and seeing that the display was basically connected to one of the SPI interfaces (HSPI), I was rather sure that I have to configure the SPI correctly to communicate with the display.

While trying to set the correct pins in the `pins_arduino.h` of my variant, I recognized that the pins that are mentioned there are actually not pins, but GPIO numbers. That one was a hard learning for me. The default RX/TX settings finally lead me to this idea and after setting the correct GPIOs for the SPI and also setting the right GPIOs for the display in the library I use, the display finally works!

Next steps:

1. make the speaker work
2. make the SD card reader work
3. make the buttons work
4. make the Wifi (and Bluetooth?) work
5. connect to the GRBL board via serial connection
6. implement nice menus etc.

### Controlling the Speaker

There is a small speaker that could provide some feedback as well. And after connecting the display maybe this a simpler task with a "low hanging fruit" :) I knew there was a speaker, but I didn't know where it was located and I did not know how the component looked like. Nevertheless, I found the speaker and via some electronic parts (Q3 and R25) it is connected to pin 33 (GPIO 21). Maybe this is good starting point.

Actually, implementing the speaker was really easy. With some googeling I found a nice `pitches.h` file that can be used with the standard Arduino `tones()` function. Now, I can also give audio feedback.

Next steps:

1. ~~make the speaker work~~
2. make the SD card reader work
3. make the buttons work
4. make the Wifi (and Bluetooth?) work
5. connect to the GRBL board via serial connection
6. implement nice menus etc.

### Accessing the microSD card

As always, figuring out the pin connections (from left to right):

1. unused? (SD: Card Inserted = GND)
2. unused            (SD pin 8) 
3. pin 31 -> GPIO 19 (SD pin 7: Data Out)
4. GND               (SD pin 6)
5. pin 30 -> GPIO 18 (SD pin 5: Serial Clock)
6. VCC               (SD pin 4)
7. pin 37 -> GPIO 23 (SD pin 3: Data In)
8. pin 29 -> GPIO  5 (SD pin 2: Chip Select)
9. unused            (SD pin 1)

The pins (in reverse order, pin 1 is the "card in" signal, which is pulled to GND if card is inserted) correspond the different microSD contacts as shown [here](https://mischianti.org/2021/03/28/how-to-use-sd-card-with-esp32-2/). The pin connections seem to indicate that the second SPI interface (VSPI) is used for the SD card access.

I had the feeling that I should be using the VSPI as the default and set up the HSPI specifically of the TFT. I had to figure out how to set up the second SPI as I didn't find any indication that two SPIs are instantiated by default.

After I got the HSPI for the SPI switched over to manual setup, I followed the abovementioned guide to run some basic SD card access steps. All worked well as now the default SPI is VSPI (to wich the SD card reader is attached). Listing the files worked flawlessly.

Next steps:

1. ~~make the speaker work~~
2. ~~make the SD card reader work~~
3. make the buttons work
4. make the Wifi (and Bluetooth?) work
5. connect to the GRBL board via serial connection
6. implement nice menus etc.

### Reading the buttons

The button pins are as follows:

* +X  -> pin  4 -> GPIO 36
* -X  -> pin  5 -> GPIO 39
* +Y  -> pin  6 -> GPIO 34
* -Y  -> pin  7 -> GPIO 35
* SET -> pin  8 -> GPIO 32
* +Z  -> pin 10 -> GPIO 25
* -Z  -> pin 11 -> GPIO 26
* ESC -> pin 25 -> GPIO  0

With this information I tried to figure out how to react to button presses wihtout having to regularly poll the button state. This is possible with external interrupts as shown [here](https://microcontrollerslab.com/esp32-external-interrupts-tutorial-arduino-ide/). With some macro-magic and a debouncing library I added all the button code. This currently supports single, double and long press. There is another example [here](https://forum.arduino.cc/t/adding-a-double-click-case-statement/283504) that does not use interrupts. Maybe I have to look at this later.

Next steps:

1. ~~make the speaker work~~
2. ~~make the SD card reader work~~
3. ~~make the buttons work~~
4. make the Wifi (and Bluetooth?) work
5. connect to the GRBL board via serial connection
6. implement nice menus etc.

### Wifi Connection

For connecting the WiFi I used [this tutorial](https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/). AP mode as well as STA mode work like a charm. There is also some information about reacting to WiFi events, which seems quite useful. I also saw that if I want to use both the AP and the STA mode at the same time [I need to determine the channel of the WiFi STA I want to use first and then prepare the AP with that channel for it to be able to work](https://forum.arduino.cc/t/esp8266-wifi_ap_sta-mode-wie-richtig-benutzen/556529/9).

I didn't bother about Bluetooth right now, as I currently don't see any real requirement apart from mobile app development (which I currently don't want to dig into).

#### Intermezzo: Web Server

I definitely want to run a webserver on the ESP32 to be able to upload files through the server to the SD card and ideally to directly communicate with the server via a simulated COM port. On the client side I might be able to use the software from [this](https://tibbo.com/soi/software.html) page for the connection with Windows and Linux.

For the an asynchronous server I found [this page](https://myhomethings.eu/de/esp32-asynchroner-webserver/) with a simple introduction. However, I think I'll have to implement a more capable system that is comparable to the [ESP3D project](https://github.com/luc-github/ESP3D). I think I might have to borrow some of the ideas from there to speed up my project. While looking at that project, I also found the [FludiNC](https://github.com/bdring/FluidNC) project, which sounds also very interesting (although I'm not going to switch my CNC and laser controller boards for an ESP32 anytime soon).

No implementation as of yet... But I recognized that the size of the image is already 805177 bytes of 1310720. I guess I will have to resize the partitions or store some data somewhere else to make use of the 4MB size of the ESP32 flash. Or maybe this is actually the limit?

Next steps:

1. ~~make the speaker work~~
2. ~~make the SD card reader work~~
3. ~~make the buttons work~~
4. ~~make the Wifi (and Bluetooth?) work~~
5. connect to the GRBL board via serial connection
6. implement nice menus etc.

### Connecting to the GRBL board

I have a spare GRBL board that can be used for my laser cutter. I think the Vevor CNC board will have a similar serial connection. Hence, I tried to derive the correct pins of the connector from the [board details](https://github.com/makerbase-mks/MKS-DLC/tree/master/hardware/MKS%20DLC%20V2.1_001), which show the pinout.

Based on this and looking at the connection cable, I would assume the following pin connection setup (looking at the port from the front):

```
1 3 5 7
2 4 6 8
```

* 1 (not connected)
* 2 TX  -> via R38 to pin 12       -> GPIO 27
* 3 RST -> via Q1 and R1 to pin 36 -> GPIO 22
* 4 RX  -> via R37 to pin 9        -> GPIO 33
* 5+6 GND
* 7+8 5V

I didn't find any connection to pin 1, but if these pin connections are correct, then the pinout from the offline controller board should be exactly the same as on the GRBL board (with TX and RX being switched).

I am not sure why pin 3 is connected to Q1, but I would expect that this allows for e.g. a reset of the GRBL board or similar. Indeed, if I pull up GPIO 22, pin 3 of the connector is connected to GND. I will have to test with the Vevor board whether this pull-down has any effect.

I checked the behavior of the CNC if I pull the pin to GND manually. Indeed, the CNC is reset. Also, if one of the CNC endstops is hit during the CNC process, the display (with the original firmware) shows an alarm and offers to reset the GRBL board. I guess this is the functionality that they implemented.

## Current Display and Web Views

### Display Views
<image src="images/Display-Boot.jpg" width="30%" />
<image src="images/Display-Home.jpg" width="30%" />
<image src="images/Display-Move.jpg" width="30%" />
<image src="images/Display-SPN.jpg" width="30%" />
<image src="images/Display-Alarm.jpg" width="30%" />
<image src="images/Display-Files.jpg" width="30%" />
<image src="images/Display-Run.jpg" width="30%" />
<image src="images/Display-Settings.jpg" width="30%" />
<image src="images/Display-About.jpg" width="30%" />

### Web Views
<image src="images/Web-Control.jpg" width="30%" />
<image src="images/Web-Files.jpg" width="30%" />
<image src="images/Web-Print.jpg" width="30%" />
<image src="images/Web-Running.jpg" width="30%" />
<image src="images/Web-Settings.jpg" width="30%" />
<image src="images/Web-About.jpg" width="30%" />

## Implementation

### First Steps

After preparing the knowledge I needed about the offline controller, I wanted to start implementing the boot screen. For doing that, I took a Vevor image from the internet, altered it a little bit to look similar to the old image and then converted it to an `.h` file to be able to draw it. I used [this online converter](http://www.rinkydinkelectronics.com/t_imageconverter565.php) and [this software](https://sourceforge.net/projects/lcd-image-converter/) to convert the image into the required format.

While doing so I recognized that the display didn't show the right colors, but inverted some of them. According to my Google search, this is caused by the incorrect setup of the graphics library. The bad thing: if I configured the library to show the colors correctly, the column and row offsets of the pixels where incorrect. Setting the column and row offset, however, is done via a protected method on the library class. So I had to derive my own class of the graphics library that acually does what I want: correct colors AND correct offsets.

After fixing this, I was able to show the bitmap as I wanted. I also had to fix the initialization of the speaker stuff, as it produced an error message.

### Basic Connection to the GRBL Board

After setting the correct RX/TX pins for the second serial, I am able to connect my serial monitor via the display to the GRBL contoller. By just pushing everything that is received via one serial to the other, I am able to controle the GRBL board directly. During my tests I also checked whether it is possible to use the USB connection in parallel to the display connection. As soon as the display is connected, the USB connection does not work anymore. No commands are accepted. It seems to me that I need to alter the [GRBL firmware](https://github.com/gnea/grbl) to make use of two ports in parallel (which would be my aim). However, I also read that on some boards the RX/TX pins of the display connector and the USB connector on the GRBL board are actually the same, which would not allow for parallel use.

Next steps:

1. ~~make the speaker work~~
2. ~~make the SD card reader work~~
3. ~~make the buttons work~~
4. ~~make the Wifi (and Bluetooth?) work~~
5. ~~connect to the GRBL board via serial connection~~
6. implement nice menus etc.

### Implementation Planning

Now I am at the point where I need to sketch out how to move forward. There are a lot of things I want to achieve, but of course I don't want to do everything by myself. I'll rather crawl through some implementations from others, think of what I need and integrate the ideas into my own firmware (hoping that it all fits into the 4MB flash size of the chip). I would like to have the following points, and I think I am going to implement them in the following order:

1. WiFi STA and AP in parallel
    * if STA is not connected, start AP anayway
2. configure STA and AP mode over a web interface
3. allow for OTA updates via a web interface

After those, I want the following things in addiotion (unordered for now):

* Web interface for control, files, printing and serial terminal (wifi config and update is ideally already there, see above)
* Web serial connection
* Display implementation for control, files, printing and network info
* (GRBL board topic; maybe not required with web serial connection) allow USB and display in parallel

### Implementation Planning - revised

I fiddled around a little bit with the SPIFFS storage for files and it works really well and easy. While I was implementing that, I decided to change my plan:

1. ~~WiFi STA and AP in parallel~~
    * ~~if STA is not connected, start AP anyway~~ (both already done)
2. implement OTA updates via the web interface
3. configure STA and AP mode over a web interface

I want to switch the two points because of "bootstrapping" reasons. With the OTA update, I will be able to exchange the current app image on the ESP32 easily via the web interface. In the initial phase, I will just create a simple web site (non-compressed html; possibly without CSS) stored on the SPIFFS partition to upload the new image. With that done, I can upload a new app image OTA any time (and I will potentially reduce wear by using a different partition on every update). I expect that the upload process is quite similar for OTA and SPIFFS files.

When this is done, I will create a new web page and a new firmware, that also allows for uploading the web file for the SPIFFS partition. With this, I can exchange the image or the web part whenever I want via the web interface. That sounds very convenient to me. After reading [this](https://randomnerdtutorials.com/esp32-ota-over-the-air-arduino/) solution, I feel I can just use the suggested library and I can be done with mostly everything for the webserver I need in now time. It does not allow for single file upload to SPIFFS, but I can generate the SPIFFS binary file from my VS Code project just fine, so the upload of a bin file is totally sufficient for me.

And wow, this was a quick one. With the [AsyncElegantOTA](https://randomnerdtutorials.com/esp32-ota-over-the-air-arduino/) library, the updates are already working. Looking at the library, the code isn't really that complicated, but I don't mind the small "buy me a coffee" icon on the page for now ;).

### Implementation next steps

Again, I changed my plans a little bit. I'm working very agile ;). I added the menu screen, the control screen and the info screen to the display already. Buttons work on the screens, but not all of them have some logic behind them that has an effect. OTA is working nicely so far. However, sometimes I still need to connect the device to the serial port to flash and monitor it when I run into errors. Didn't happen to often, though, so for now I run the display off an USB charger.

The next steps for me are now:
1. ~~WiFi STA and AP in parallel~~
    * ~~if STA is not connected, start AP anyway~~ (both already done)
2. ~~implement OTA updates via the web interface~~
3. Display implementation for settings (only feed rate and baud rate?)
4. configure STA and AP mode over a web interface

and after that (most likely in this order):

* GRBL board connection
* Web serial connection
* Web interface for control, files, printing and serial terminal (wifi config and update is ideally already there, see above)
* Display implementation for files and printing
* (GRBL board topic; maybe not required with web serial connection) allow USB and display in parallel

### Settings implemented

The settings are now implemented, altough I only implemented feed rate and baud rate (for the serial to the GRBL board). The information is stored in the NVM (non-volatile memory), so it will survive any reboot and update. The only missing thing on the display is the file implementation. However, as this requires some testing with the GRBL board (is this file really processed?), I'm going to push this back a little bit. One idea that I had: maybe if I stop the communication of the serial port to the GRBL board, the board can be connected via USB? This would solve my problem, but I fear that this will not work. As far as I can remember this worked for my Laser cutter, but I'll have to test that with the machine directly. So, next points in the order I see them now:

1. ~~WiFi STA and AP in parallel~~
    * ~~if STA is not connected, start AP anyway~~ (both already done)
2. ~~implement OTA updates via the web interface~~
3. ~~Display implementation for settings (only feed rate and baud rate?)~~
    * ~~maybe add "disconnect" functionality if that frees the USB connection?~~ (does not work)
4. ~~GRBL board connection~~ (done)
    * ~~maybe a good point in time to look also at this: (GRBL board topic; maybe not required with web serial connection) allow USB and display in parallel~~ (seems impossible with my GRBL board as the USB and the display port connect to the same pins of the processor)
5. configure STA and AP mode over a web interface (pushed back)
6. ~~Web serial connection~~ (works, yay! But consumes a lot of processing power making OTA nearly impossible :()
7. Web interface for control, files, printing and serial terminal (wifi config ~~and update~~ (done) is ideally already there, see above)
8. Display implementation for files and printing

NOTE: in the current state the OTA functionality is not really usable. I think I have to change my programming style to consume less resources. That will require some refactoring, I think :(

### Performance increase

Success! After some changes I am now able to quickly upload an OTA update. However, when Candle is connected to the Web Serial Port, the control screen becomes very unresponsive. I think I'll have to look into that later. It works for now, although I have to test "the real thing", i.e. running a gcode file through Candle.

### Preparations for web improvements

I want to use Bootstrap 5 for my web pages, so I downloaded the required CSS files etc. I read [here](https://github.com/me-no-dev/ESPAsyncWebServer/issues/556) that the ESPAsyncWebServer is already capable of serving gzipped files. Hence, I zipped the files I want to serve and I created a `data-unzipped` folder for the files that I will use during web development. In addition to that, I added a `pre` python script that zips the `index.html` automatically whenever a new SPIFFS files is built. Additional files can easily be added, if needed. I got the ideas from [here](https://docs.platformio.org/en/latest/scripting/actions.html) and [here](https://docs.python.org/3/library/gzip.html#examples-of-usage).

## It has been some days...

... but I managed to implement some stuff and I was also able to improve performance a little bit (including by using the two cores of the ESP32 more effectively for the UI and the buttons). Printing via network seems flawlessly possible right now (I use Candle with the Tibbo remote serial software), control via web UI works as well and printing a file from the SD card is also possible. Nevertheless, there are still some things that I might want to implement in the future (most likely with this priority):

1. Files in the Web UI (show files, start file, upload/download/delete files)
2. Implement overrides (feed, rapid, spindle) via display (see [here](https://github.com/gnea/grbl/blob/master/doc/markdown/commands.md#extended-ascii-realtime-command-descriptions))
3. Make file printing better (nicer UI, easier usage and maybe react to GRBL replies?)
4. Implement Web UI settings

## Later

* ~~[partition tables and embedding binary data](https://docs.platformio.org/en/latest/platforms/espressif32.html#partition-tables), see also [here](https://community.platformio.org/t/unable-to-build-and-upload-spiffs-filesystem-image-with-framework-esp-idf/17820/2) and [here](https://github.com/espressif/arduino-esp32/blob/master/tools/partitions/default.csv)~~
* ~~[OTA updates](https://randomnerdtutorials.com/esp32-over-the-air-ota-programming/) and [AsyncElegantOTA](https://randomnerdtutorials.com/esp32-ota-over-the-air-arduino/)~~
* ~~[Preferences](https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/)~~
* [GRBL quick reference](https://www.sainsmart.com/blogs/news/grbl-v1-1-quick-reference) and [GRBL command documentation](https://github.com/gnea/grbl/tree/master/doc/markdown)
* [Websockets](https://randomnerdtutorials.com/esp32-websocket-server-arduino/) and [here as well](https://github.com/me-no-dev/ESPAsyncWebServer#async-websocket-plugin)
* [onReceive for Serial](https://github.com/espressif/arduino-esp32/blob/master/libraries/ESP32/examples/Serial/OnReceive_Demo/OnReceive_Demo.ino)
* [RGB565 Color Picker](https://barth-dev.de/online/rgb565-color-picker/)
* [Pixel editor](https://apps.lospec.com/pixel-editor) (good for figuring out how to draw something)
* [Online Arduino ESP32 simulator](https://wokwi.com/projects/new/esp32) (unfortunately, very slow when working with a display, but still allows to try some things...)
* [Bootstrap icons](https://icons.getbootstrap.com/#icons)