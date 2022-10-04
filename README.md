# GB-Dumper - Gameboy Cartridge Dumper

The GB-Dumper is a **PCB** project for Gameboy cartridge dumps/backups via Arduino __Nano__ &amp; USB-connection,

so you can read ROM and read / write to RAM.

The new configured software and custom PCB is compatible with +5V (mostly USB) Arduino Nano or similar.

![PCB](/docs/GB-Dumper-front%2Bchips.png)

Compatibility list:

- Gameboys

:heavy_check_mark: Classic/Original

:heavy_check_mark: Color

:heavy_check_mark: Pocket

:heavy_check_mark: Micro

:heavy_minus_sign: Advance

- Cartridges

:heavy_check_mark: Class/Type A (grey+colors)

:heavy_check_mark: Class B Dual (black)

:heavy_check_mark: Class C (transparent/clear)

:x: Class D (Advance GBA) -> 3.3V Logic

# DISCLAIMERS

**This is still "work-in-progress (WIP)" so i do NOT take any kind of warranty/responibility for damages or data losses!**

**You can use this at your own risk without guarantees!**

If you like the new design yo are free to share it!

Have fun and save the retro world :)

## HARDWARE

- 1x Arduino Nano (or Uno for old versions)
- 1x Custom PCB for Arduino Nano by WodoWiesel (abx@gmx.de)
  (or from orig. GBCartReader for Uno)
- 2x SN-74HC595 Shift-Registers from Texas Instruments
- 5x 470 Ohm Resistors – recommended for pull-ups on active low pins & to prevent damage
- 1x Gameboy Cartridge 32-Pin Slot

- 3x Female Pin headers - optional
- 2x Jumpers - optional

- 1x Soldering iron
- solder + flux

## SOFTWARE

GBCartRead v1.8 Rev 1.2 - Gameboy Cart Reader
by insideGadgets (https://www.insidegadgets.com/ & support@insidegadgets.com)

GBCartRead is an Arduino based Gameboy Cartridge Reader which uses a C program or python script to interface with Arduino.

GBCartRead allows you to dump your ROM, save the RAM and write to the RAM.

Works with Arduino Duemilanove and Uno, but requires wiring & code definition changes.

Speed increases thanks to Frode vdM. (fvdm1992@yahoo.no) and David R.

## HOW TO USE

1. Open the \GBCartRead_v(xxx)_Arduino\GBCartRead_v(xxx).ino file and program it to your Arduino.

   Note down the COM port number in use; you can find this out from the Arduino software by going to Tools -> Serial port.

2. There are two choices to communicate with the Arduino – using the C program or the Python program.

  - Python Program

  a. Download and install Python 3.2+ or higher (https://www.python.org/download/) and

     pySerial (https://pypi.python.org/pypi/pyserial)

  b. Open up the “GBCartRead_v(xxx)_Python_Interface.py” script by right clicking it and selecting “Edit with IDLE”.

  c. Change the “COM2” serial port to the serial port that your Arduino is connected on, save the file and press F5 to run it.

  d. A new window will appear, after 2-3 seconds you’ll have some options available.

 - C Program

  a. Open config.ini and change the COM port number to the serial port that your Arduino is connected on and save
     the file.

  b. Run “GBCartRead_v(xxx)_C_Interface.exe” in Windows or re-compile the source for your OS

3. At this stage you should insert your Gameboy cartridge and press the power button, the power LED should light up.

4. Press 0 to read the header and verify that it looks correct. If it doesn’t look correct, press the power button
   to power off the Gameboy cartridge, remove and re-insert it and power it up again.

5. Press 1 to Dump the ROM, 2 to Backup your RAM or 3 to Load your RAM file. Hashes (#) will start printing every
   few seconds and a file called <gametitle>.gb or .sav will be created if you chose option 1 or 2.
   If you choose option 3, it will load the save from <gametitle>.sav.

We recommend verifying your Gameboy ROM using BGB (a Gameboy emulator) or "xgbfix.exe -v -d <your_rom.rom>"
found in the project called "ASMotor" (we've included it).
It’s a good idea to verify your save files too by running the ROM when the save file is present in BGB.

## SPECIAL CASES

There are some cartridges which don't quite conform to cartridge header standards or require something out of the ordinary.

List can be found in the documentations folder!

Wisdom_Tree.txt - Provided by Benjamin E.

## REVISION HISTORY

v1.8 Rev 1.2 (29 September 2022) by WodoWiesel
- adapted pin configuration for new Arduino Nano
- added reset pin interrupt
- added optional display pins
- added logos & pin labels

v1.8 Rev 1.1 (11 March 2022) by WodoWiesel
- Optimized & detailed Makefile
- added pull-up active low resistors
- added audio/Vin pin support for cartridges
- added more stable common ground-plane connectivity
- added M3 mounting holes

v1.8 Rev 1 (28 May 2016)
- Re-add checking for non-printable ASCII characters code that was removed when cleaning up code
- Added custom cartridge files for Wisdom Tree games (thanks Benjamin E)

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
- Added interrupt for button on A1 to switch P mosfet on A0 that powers the cartridge
- Changed so at startup it doesn't display the Game Cartridge header

v1.6 (3 January 2015)
- Speed increase by using a higher baud rate (400 Kbit from 57.6Kbit) and using hardware SPI (thanks David R.)
- Changed SCK from pin 12 to pin 13 for SPI
- RAM writing speed increase by continually sending 64bytes data from PC and pausing for 5 ms in-between

  (Compatible with Gameboy Cart Shield v1.2 if you connect pins 12 and 13 together)

v1.5 (15 November 2014)
- Speed increases by using direct PORT writing/reading and sending 64 bytes at a time
  (thanks Frode vdM. fvdm1992@yahoo.no)

- Compacting functions (thanks Frode vdM. fvdm1992@yahoo.no)
- Delays when pulsing WR/RD/MREQ pins reduced to a few NOP cycles

v1.4.2 (14 November 2014)
- Fixed gameTitle array length issue (thanks Frode vdM. and David R.)
- Added Gameboy Camera support (thanks Frode vdM.)

v1.4.1 (3 January 2014)
- Changed wrPin from pin 13 to A3 to suit Gameboy Cart Shield v1.2

v1.4 (30 August 2013)
- Added gbcartread.exe program for Windows (with source) which can be used instead of using the Python script
  and is a bit faster

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

 :copyright: 2021-2022 WodoWiesel (https://github.com/wodowiesel/ & https://www.twitch.com/wodowiesel/)

The new design was checked by the electrical engineer TheBrutzler

(https://github.com/theBrutzler/ & https://ww.twitch.com/thebrutzler/)

The new optimized PCB was designed with free KiCAD software (https://www.kicad.org/)

:heavy_exclamation_mark: The PCB should de produced at a manufacturer (like JLCPCB) with the RoHS lead-less option for environmental protection.

Original: :copyright: 2011-2016 by insideGadgets ( https://github.com/insidegadgets/GBCartRead/ & http://www.insidegadgets.com)

## LICENCES

This work is licensed under a Creative Commons Attribution-NonCommercial 3.0 Unported License.

(http://creativecommons.org/licenses/by-nc/3.0/)
