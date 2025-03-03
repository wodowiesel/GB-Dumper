
# GB-Dumper - Gameboy Cartridge Dumper

The GB-Dumper is a **PCB** project for Gameboy cartridge dumps/backups via Arduino __Nano__

&amp; USB-connection with the SPI-Protocol, so you can read ROM and read / write to RAM.

The new configured software and custom PCB is compatible with +5V (mostly USB) Arduino Nano or similar.

![PCB](/docs/GB-Dumper-front%2Bchips.png)

Compatibility list:

- Gameboys (Revisions):

:heavy_check_mark: Classic & Original (DMG0 & DMG)

:heavy_check_mark: Color (CGB0 & CGB)

:heavy_check_mark: Pocket/Lite (MGB)

:heavy_check_mark: Micro (MGB)

:heavy_check_mark: Advance & SP (AGB0 & AGB)

- Cartridges:

:heavy_check_mark: Class/Type A (grey+colors)

:heavy_check_mark: Class B Dual (black)

:heavy_check_mark: Class C (transparent/clear)

:heavy_check_mark: Class D (Advance GBA) -> 3.3V Logic, manual switch

:white_circle: Class X (special)

:heavy_check_mark: compatibility jumpers for pin 30 /RES + /CS2 & pin 31 VIN/AUDIO + IRQ
                  (functions wip!)

- Functions:

  - GB:

    :heavy_check_mark: Reading full header on PC/Serial

    :heavy_check_mark: Dump ROM on PC

    :heavy_check_mark: Dump RAM on PC

    :heavy_check_mark: Write RAM

    :white_circle: Dump ROM/RAM on SD

  - GBA:

    :white_circle: Reading full header on PC/Serial

    :white_circle: Dump ROM on PC

    :white_circle: Dump RAM on PC

    :white_circle: Write RAM

    :white_circle: Dump ROM/RAM on SD

# DISCLAIMERS

**This is still "work-in-progress (WIP)/experimental"

I do NOT take any kind of warranty/responsibility for damages/data losses!**

**You can use this at your own risk without guarantees and put device on non conductive surface or enclosure!**

If you like the new design you are free to share it!

Have fun and save the retro world :)

## HARDWARE

- 1x Arduino Nano v3.x (or Uno for old versions) + mini-USB-Cable + IDE

- 1x Custom PCB by WodoWiesel (wodowiesel@quantentunnel.de)

  (or from orig. GBCartReader for Uno)

- 2x flat M3 screws and an enclosure - optional

- 2x SN-74HC595 Shift-Registers from Texas Instruments

- 5x 470 (or 330) Ohm Resistors – pull-ups on active low & impedance matching

- 1x 2-position (3-pin) switch SPDT

- 1x Gameboy Cartridge 32-pin Slot

- 1x push-button - optional

- 1x ~10 uF Capacitor (pol.) - optional/recommended

- 3x Jumpers (required) -> default to GB DMG (marked as dot)

- Female Pin headers/sockets for the chips - or solder it directly

  * 1x 30-pin for Arduino

  * 2x 16-pin for shift register

- Male headers

  * 3x 3-pin (required)

  * 1x 4-pin (I²C RTC/Display - optional)

  * 1x 6-pin (SPI SD-Card - optional)

- 1x Soldering iron

- solder + flux + IPA etc.

## SOFTWARE

- It is based on GBCartRead v1.8 Rev 1.2 - Gameboy Cart Reader by insideGadgets, but impoved and smaller.

- Board based Gameboy Cartridge Reader & Writer which uses

  a C program or Python script to interface via USB with Cartridge directly.

- It allows you to dump your ROM (game), read & write the RAM (saves).

- Works with old Arduino Due-milanove and Uno, but requires wiring & code changes!)

-  Dependencies:

   a. Py

   - Download and install Python 3.2+ or higher (https://www.python.org/download/)

   - Lib: pySerial (https://pypi.python.org/pypi/pyserial/)

   b. C

   - Check dependencies cygwin64 & gcc compiler are installed.

## HOW TO USE

0. Set GB clock-pin jumper to default GND pin (arduino clock only for development/debug purposes 4/16 MHz).

   Reset pin can be set optionally to low to +5V pin if you want to soft-reset control (may need extra code).

1. Open the \GBCartRead_v1.3.x_Arduino_WodoWiesel\GBCartRead_v1.3.x_Arduino_WodoWoiesel.ino file

   with Arduino IDE. Note down and select your COM port number in use (std. Nr.3).

   You can find this out from the Arduino IDE (2) by going to Tools -> Serial port.  

   Then set the identical baudrate as in the py/c-program and flash it to your Arduino.

2. There are two choices to communicate with the Arduino – using the C program or the Python program.

  - Python Program:

  a. Open up the “GB-Dumper-v1.8_Rev1.3.x_Py_Interface.py” script by right clicking it and selecting “Edit with IDLE”.

  b. Then set the identical COM port & baudrate as in the arduino ino-program,

   save and press F5 to run it via py-reader.bat as admin.

  c. A new window will appear, after 2-3 seconds you’ll have some options available.

 - C Program:

   a. Open config.ini and change the COM port & baudrate identical to your Arduino is connected on and save the file.

   b.  Check paths in Makefile and cmaker.bat and run it as admin to compile it.

   c. Run “GBCartRead_v1.3.x_C_Interface.exe” in Windows terminal as admin or re-compile the source for your OS.

3. At this stage you should insert your Gameboy cartridge and press the power button,

   the power LED should light up.

4. Press 0 to read the header and verify that it looks correct. If it doesn’t look correct,

   press the power button to power off the Gameboy cartridge, remove and re-insert it and power it up again.

5. Press 1 to Dump the ROM, 2 to Backup your RAM or 3 to Load your RAM file.

   Hashes (#) will start printing every few seconds and a file called <gametitle>[.gb]

   or *.sav will be created if you chose option 1 or 2.

   If you choose option 3, it will load the save from <gametitle>.sav.

   It’s a good idea to verify your save files too by running the ROM when the save file is present in BGB.

## SPECIALS, CHECKSUMS & MEMORY

1. There are some cartridges which don't quite conform to cartridge header standards or require something out of the ordinary.
   
   For example the EVERDRIVE cartridges uses .srm instead of the more common .sav format and consumes less power than the EZ-Flash.
   
   For further technical infos and datasheets take a good look into the Documentation folder!!

3. GB audio pin (VIN analog) / IRQ (digital) base function is already built-in (wip!), but needs changes for rare games! :construction:

   The /RST and /CS2 pins got own compatibility functions thanks to [joaopauloap](https://github.com/joaopauloap/arduino-gba-dumper).

4. Formatting

   If you're on Linux or Mac make sure the Windows style line endings (CRLF) don't get removed and write a shell script.

5. CRC/Verification/Validation

   This file [GB/C](/docs/gb.txt) stores the ROM names and the CRC32 checksums of the complete ROM and are used only for

   verification at the end of the dumping process.

   Example:

   From: [OSCR](https://github.com/sanni/cartreader/tree/master/sd/)

   Name: 007 - The World Is Not Enough (USA, Europe).gb

   CRC32: E038E666

   It is recommend verifying your Gameboy ROM using BGB (a Gameboy emulator) or

   run validator.bat as admin via ```xgbfix.exe -v -p -d <gametitle>[.gb(c)]``` (should not contain spaces).

   Original can be found in the git project called [ASMotor](https://github.com/asmotor/asmotor/).

   -> My easier validation can be done via the [validator.bat](/code-pc/validator/validator.bat)

   or executable [xgbfix.exe](/code-pc/validator/xgbfix.exe).

6. Memory

  A memory IO-map on the standard GB can be found in the docs [here](/docs/gb-memory-maps.png).

   The file [WisdomTree](/docs/Wisdom_Tree.txt) contains memory address informations and exceptions.

   Additional articles (https://gbdev.gg8.se/wiki/articles/MBC7#Ax8x_-_EEPROM_.28Read.2FWrite.29)

7. CI

   Arduino Workflow [CI](.github/workflows/main.yaml) just for debug.

## REVISION HISTORY

v1.8 Rev 1.3.1 (28 June 2024) by WodoWiesel
- pcb 0.83
- lot more comments and documentation
- CLK switchable (default to GND or unconnected)
- added optional button pin
- added debug test point A0 for mosfet
- /RST & /CS2 (digital) switchable + functions wip
- VIN (analog) & IRQ (digital) switchable + functions wip

v1.8 Rev 1.2 (29 September 2023) by WodoWiesel
- pcb 0.7
- added CRC game informations
- added memory info, links & CI
- adapted pin configuration for new Arduino Nano
- Optimized & detailed Makefile
- added optional display pins
- added logos & pin labels

v1.8 Rev 1.1 (11 March 2022) by WodoWiesel
- pcb v0.6
- added reset pin interrupt
- added pull-up active low resistors
- added Audio/VIN (into the card) pin support for cartridges
- added more stable common ground-plane connectivity
- added M3 mounting holes

-------

v1.8 Rev 1 (28 May 2016)
- Re-add checking for non-printable ASCII characters code that was removed when cleaning up code
- Added custom cartridge files for Wisdom Tree games (thanks Benjamin E.)

v1.8 (21 March 2016)
- Added Nintendo Logo check and compare so you can tell if the cartridge is being read correctly
  (sometimes the title was read ok but the ROM dump wasn't correct)
- Added missing cartridge RAM support
- Cleaned up code

v1.7 Rev 2 (10 February 2016)
- Added checking for non-printable ASCII characters in Gameboy header and replaced them with underscores

v1.7 Rev 1 (17 February 2015)
- Fixed clockPin to change correct port - PB5 (thanks Anna)

v1.7 (7 February 2015) *Only compatible with Gameboy Cart Shield v1.3*
- Added interrupt for button on A1 to switch P-mosfet on A0 that powers the cartridge
- Changed so at startup it doesn't display the Game Cartridge header

v1.6 (3 January 2015)
- Speed increase by using a higher baud rate (400 Kbit from 57.6Kbit) and using hardware SPI (thanks David R.)
- Changed SCK from pin 12 to pin 13 for SPI
- RAM writing speed increase by continually sending 64bytes data from PC and pausing for 5 ms in-between
  (Compatible with Gameboy Cart Shield v1.2 if you connect pins 12 and 13 together)

v1.5 (15 November 2014)
- Speed increases by using direct PORT writing/reading and sending 64 bytes at a time
  (thanks Frode vdM.)
- Compacting functions (thanks Frode vdM.)
- Delays when pulsing WR/RD/MREQ pins reduced to a few NOP cycles

v1.4.2 (14 November 2014)
- Fixed gameTitle array length issue (thanks Frode vdM. and David R.)
- Added Gameboy Camera support (thanks Frode vdM. fvdm1992@yahoo.no)

v1.4.1 (3 January 2014)
- Changed wrPin from pin 13 to A3 to suit Gameboy Cart Shield v1.2

v1.4 (30 August 2013)
- Added gbcartread.exe program for Windows (with source)
  which can be used instead of using the Python script and is a bit faster

v1.3.1 (2 May 2011)
- Updated schematic as R19, R20 and R31 were actually acting as voltage dividers

v1.3 (9 April 2011)
- Added RAM writing ability
- Fixed MBC2 reading RAM overflow relating to the first 4 bits not being ignored properly
- Cleaned up bank select code to use BitRead instead of checking each bit ourselves
- Fixed selecting wrong RAM size when reading
- Fixed incorrect check if RAM was present before reading
- Removed unnecessary changing of data pins to inputs after MBC was initialized when reading RAM

v1.2 (2 April 2011)
- Added RAM saving ability.
- Now prints the amount of data read in 64K increments (i.e. ###64K###128K###192K)
- Changed <gametitle>.rom to <gametitle>.gb

v1.1 (21 March 2011)
- ROM reading is now fully automated.
- The Game Title, MBC type, ROM size and RAM size are presented in the Python script.

v1.0 (19 March 2011)
- Reading ROM only, user to change the maximum ROM bank to read

## CREDITS

 :copyright: 2021-2024 WodoWiesel

 Github (https://github.com/wodowiesel/)

 Twitch (https://www.twitch.com/wodowiesel/)

 Instagram (https://www.instagram.com/wodowiesel)

 YT (https://www.youtube.com/@wodowiesel)

 The new design was checked by the electrical engineer theBrutzler

 (https://github.com/theBrutzler/ & https://www.twitch.com/theBrutzler/)

 The new optimized PCB was designed with free KiCAD software (https://www.kicad.org/)

 :heavy_exclamation_mark: The PCB should be produced at a manufacturer

 (like PCBWay/JLCPCB) with the RoHS lead-free option for environmental protection.

 Original: :copyright: 2011-2016 by [insideGadgets](https://github.com/insidegadgets/GBCartRead/) support@insidegadgets.com

## LICENSES

Not for commercial and/or criminal usage!

- Hardware

   CERN Open Hardware License Version 2 - Weakly Reciprocal

   Short identifier CERN-OHL-W-2.0

   [LICENSE Source](https://spdx.org/licenses/CERN-OHL-W-2.0.html)

- Software

   This work is licensed under a Creative Commons Attribution-NonCommercial 3.0 Unported License.

   [CCANC](http://creativecommons.org/licenses/by-nc/3.0/)
