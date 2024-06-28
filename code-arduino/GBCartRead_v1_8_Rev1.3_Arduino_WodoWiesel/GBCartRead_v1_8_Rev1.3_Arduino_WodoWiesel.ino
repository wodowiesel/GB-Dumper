/*
  GBCartRead
  Version: 1.8 Rev 1.2

  Author: Alex from insideGadgets (https://www.insidegadgets.com)
  github: (https://github.com/insidegadgets/GBCartRead)
  Created: 18/03/2011
  Modified: 26/05/2016

  GBCartRead is an Arduino based Gameboy Cartridge Reader which uses a C program or python
  script to interface with the Arduino. GBCartRead allows you to dump your ROM,
  save the RAM and write to the RAM.
  Works with Arduino Duemilanove, Uno and Nano.
  Will work for other Arduinos too with VCC = +5V,
  but requires wiring changes!!
  Speed increase thanks to Frode vdM.(fvdm1992@yahoo.no) and David R.

  UPDATE: This code was changed to be compatible for Arduino NANO and a custom PCB!!

  Version: 1.8 Rev 1.3
  Author: WodoWiesel
  github: (https://github.com/wodowiesel/GB-Dumper)
  Last Modified: 23. February 2024
*/

// for REVISION 0.6 of the GB-Dumper Board
// compatible with Cartridges Classic (Original) + Color + Pocket + Micro :
// Class/Type A (grey+colors), Class B Dual (black), Type C (transparent/clear)
// NOT compatible with Class D (Advance GBA) -> 3.3V Logic
// cartridge internal coin battery Li-cell normally +3V CR2025, sometimes 2032, CR1616 GBA
#include "pindeclarations.h"
#include <SPI.h>
#include <util/delay.h>
#include <Wire.h>
#include <string.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>  // OLED_I2C

int baud = 9600; // rate for serial connection 

// Edit these in pindelcarations.h too if needed!
// Digital Pins-> D-Numbers
uint8_t gb30cPin = 0; // /CS2 on D0 digital
uint8_t gb22Pin = 1;   // GB_D0 -> Ard_Pin2 PD1 D1 RX
uint8_t gb31iPin = 2;  // GB_IRQ digital -> Ar_Pin5 PD2 INT0 if needed for interrupts too!
uint8_t gb23Pin = 3;   // GB_D1 -> Ar_Pin6 PD3 INT1
uint8_t gb24Pin = 4;   // GB_D2 -> Ar_Pin7 PD4 D4
uint8_t gb25Pin = 5;   // GB_D3 -> Ar_Pin8 PD5 D5
uint8_t gb26Pin = 6;   // GB_D4 -> Ar_Pin9 PD6 D6
uint8_t gb27Pin = 7;   // GB_D5 -> Ar_Pin10 PD7 D7
uint8_t gb28Pin = 8;    // GB_D6 -> Ar_Pin11 PB0 D8
uint8_t gb29Pin = 9;    // GB_D7 -> Ar_Pin12 PB1 D9
uint8_t latchPin = 10;  // RCLK -> Ar_Pin13 PB2 D10 SS/CS
uint8_t dataPin = 11;   // SER -> Ar_Pin14 PB3 D11 MOSI
uint8_t sd_miso = 12;   // sd_spi & -> Ar_Pin15 PB4 D12 MISO https://github.com/rust-console/gbatek-gbaonly/blob/gh-pages/index.md#gbainterruptcontrol idk why? old
uint8_t clockPin = 13;  // /SRCLK -> Ar_Pin16 PB5 D13 SCK(SPI), switchable clock for cartridge 4MHz
uint8_t gb30Pin = 28; // /RST + /RES + /RESET + SCLR + VCC -> Ar_Pin3&28 PB6 PCINT14 -> also CS2 for advance

uint8_t gb32Pin = 29; // GND + /OE -> Ar_Pin4&29

// Analog Pins
uint8_t mosfetControlPin = A0; // Mosfet -> Ar_Pin19 PC0 internal
uint8_t BTNPin = A1;  // /BTN -> Ar_Pin20 PC1 PCINT9 Button for resets
uint8_t AudioPin = A2;  // VIN/AUDIO_IN DMG gn31 -> Ar_PIN21
uint8_t wrPin = A3;   // /WR -> Ar_Pin22 PC3
uint8_t mreqPin = A4; // /CS -> Ar_Pin23 PC4 SDA
uint8_t rdPin = A5;   // /RD -> Ar_Pin24 PC5 SCL

//DISPLAY CONFIG
// right Bit (LowBit) not counted -> 0x78/2=0x3C
//#define I2C_ADDRESS 0x3C // 0x78->3C, 0x7A->3D, 0x74->3A alternative
//display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);
//display.cp437(true);
//display.clearDisplay();
//display.setTextColor(WHITE);
//display.setTextSize(1);
//display.setCursor(0,0);

// https://gbdev.io/pandocs/Power_Up_Sequence.html
uint8_t nintendoLogo[] = {0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
                          0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
                          0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
                         };

char gameTitle[17];
//char gameTitle[16];
uint16_t cartridgeType = 0;
uint16_t romSize = 0;
uint16_t romBanks = 0;
uint16_t ramSize = 0;
uint16_t ramBanks = 0;
uint16_t ramEndAddress = 0;

void setup() {
  //analogReference(DEFAULT); // AREF
  pinMode(latchPin, OUTPUT); // RCLK
  pinMode(clockPin, OUTPUT); // SRCLK + SCK
  pinMode(dataPin, OUTPUT); // D11 MOSI
  pinMode(rdPin, OUTPUT); // A5
  pinMode(wrPin, OUTPUT); // A3
  pinMode(mreqPin, OUTPUT); // /CS
  pinMode(mosfetControlPin, OUTPUT); // A0
  pinMode(gb30Pin, OUTPUT); // /RES + /RST + /RESET -> Ar_Pin3&27 INPUT => std as active (if GND) LOW -> pull-up
  pinMode(gb30cPin, OUTPUT); // /CS2 PD0/D0/TX
  pinMode(AudioPin, OUTPUT); // AUDIO_IN gb_31
  pinMode(gb31iPin, OUTPUT); // IRQ

  //mosfetPin_high; // mosfet off
  PORTC |= (1 << PC0);

  // Set pins as inputs
  DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB4)); // D8 to D13 datapin | (1<<PB3) , D12 clock | (1<<PB5) , latch (1<<PB2) , AUDIO_IN
  // DDRC &= ~((1<<PC0) | (1<<PC3) | (1<<PC4) | (1<<PC5)); // A0,3,4,5
  DDRD &= ~((1 << PD1) | (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7)); // D0 to D7

  rd_wr_mreq_off();

  // Setup pin interrupt for button
  PCMSK1 = (1 << PCINT9); // A1 Ar_Pin20 PC1
  PCICR = (1 << PCIE1);

  Serial.begin(baud); // for 4/16 MHz clock
  //Serial.println("\nInit done \n");
}

void loop() {

  // Decode input
  char readInput[10];
  uint8_t readCount = 0;

  // Wait for serial input
  while (Serial.available() <= 0) {
    delay(200);
  }
  while (Serial.available() > 0) {
    char c = Serial.read();
    readInput[readCount] = c;
    readCount++;
  }
  //readInput[readCount] = '\0';
  readInput[16] = '\0';
  // Cartridge Header
  if (strstr(readInput, "HEADER")) {
    rd_wr_mreq_reset();

    gameTitle[16] = '\0';

    
    // Read cartridge title and check for non-printable text
    for (uint16_t romAddress = 0x0134; romAddress <= 0x143; romAddress++) {
      char headerChar = (char) read_byte(romAddress);
      if ((headerChar >= 0x30 && headerChar <= 0x57) || // 0-9
          (headerChar >= 0x41 && headerChar <= 0x5A) || // A-Z
          (headerChar >= 0x61 && headerChar <= 0x7A)) { // a-z
          gameTitle[(romAddress - 0x0134)] = headerChar;
          Serial.println(headerChar);
        /*
         * MULICARD MBCs & EMS etc https://gbdev.gg8.se/wiki/articles/Memory_Bank_Controllers
          A header matching any of the following is detected as EMS mapper:
          Header name is "EMSMENU", NUL-padded
          Header name is "GB16M", NUL-padded
          Cartridge type ($0147) = $1B and region ($014A) = $E1
        */
      }

      cartridgeType = read_byte(0x0147); // MBC type should be specified in the byte at 0147h of the ROM
      romSize = read_byte(0x0148);
      ramSize = read_byte(0x0149);

      // ROM banks
      romBanks = 2; // Default 32K
      if (romSize >= 1) { // Calculate rom size
        romBanks = 2 << romSize;
      }

      // RAM banks
      ramBanks = 0; // Default 0K RAM
      if (cartridgeType == 6) {
        ramBanks = 1;
      }
      if (ramSize == 2) {
        ramBanks = 1;
      }
      if (ramSize == 3) {
        ramBanks = 4;
      }
      if (ramSize == 4) {
        ramBanks = 16;
      }
      if (ramSize == 5) {
        ramBanks = 8;
      }

      // RAM end address
      if (cartridgeType == 6) {
        ramEndAddress = 0xA1FF;  // MBC2 512 bytes (nibbles)
      }
      if (ramSize == 1) {
        ramEndAddress = 0xA7FF;  // 2K RAM
      }
      if (ramSize > 1) {
        ramEndAddress = 0xBFFF;  // 8K RAM
      }

   
    }
 
    // Nintendo Logo Check
    uint8_t logoCheck = 1;
    uint8_t x = 0;
    for (uint16_t romAddress = 0x0104; romAddress <= 0x133; romAddress++) {
      if (nintendoLogo[x] != read_byte(romAddress)) {
        logoCheck = 0;
        break;
      }
      x++;
    }
    
      Serial.println(gameTitle);
      Serial.println(cartridgeType);
      Serial.println(romSize);
      Serial.println(ramSize);
      Serial.println(logoCheck);
    }
  

    // Dump ROM
   else if (strstr(readInput, "READROM")) {
      rd_wr_mreq_reset();
      uint16_t romAddress = 0;

      // Read number of banks and switch banks
      for (uint16_t bank = 1; bank < romBanks; bank++) {
        if (cartridgeType >= 5) { // MBC2 and above
          write_byte(0x2100, bank); // Set ROM bank
        }
        else { // MBC1
          write_byte(0x6000, 0); // Set ROM Mode
          write_byte(0x4000, bank >> 5); // Set bits 5 & 6 (01100000) of ROM bank
          write_byte(0x2000, bank & 0x1F); // Set bits 0 & 4 (00011111) of ROM bank
        }
        if (bank > 1) {
          romAddress = 0x4000;
        }

        // Read up to 7FFF per bank
        while (romAddress <= 0x7FFF) {
          uint8_t readData[64];
          for (uint8_t i = 0; i < 64; i++) {
            readData[i] = read_byte(romAddress + i);
          }

          Serial.write(readData, 64); // Send the 64 byte chunk
          romAddress += 64;
        }
      }
   }

    // Read RAM
   else if (strstr(readInput, "READRAM")) {
      rd_wr_mreq_reset();

      // MBC2 Fix (unknown why this fixes reading the ram, maybe has to read ROM before RAM?)
      read_byte(0x0134);

      // if cartridge have RAM test
      if (ramEndAddress > 0) {
        if (cartridgeType <= 4) { // MBC1
          write_byte(0x6000, 1); // Set RAM Mode
        }

        // Initialise MBC
        write_byte(0x0000, 0x0A);

        // Switch RAM banks
        for (uint8_t bank = 0; bank < ramBanks; bank++) {
          write_byte(0x4000, bank);

          // Read RAM
          for (uint16_t ramAddress = 0xA000; ramAddress <= ramEndAddress; ramAddress += 64) {
            uint8_t readData[64];
            for (uint8_t i = 0; i < 64; i++) {
              readData[i] = read_byte(ramAddress + i);
            }

            Serial.write(readData, 64); // Send the 64 byte chunk
          }
        }

        // Disable RAM
        write_byte(0x0000, 0x00);
      }
    }

    // Write RAM
    else if (strstr(readInput, "WRITERAM")) {
      rd_wr_mreq_reset();

      // MBC2 Fix (unknown why this fixes it, maybe has to read ROM before RAM?)
      read_byte(0x0134);

      // Does cartridge have RAM
      if (ramEndAddress > 0) {
        if (cartridgeType <= 4) { // MBC1
          write_byte(0x6000, 1); // Set RAM Mode
        }

        // Initialise MBC
        write_byte(0x0000, 0x0A);

        // Switch RAM banks
        for (uint8_t bank = 0; bank < ramBanks; bank++) {
          write_byte(0x4000, bank);

          // Write RAM
          for (uint16_t ramAddress = 0xA000; ramAddress <= ramEndAddress; ramAddress++) {
            // Wait for serial input
            while (Serial.available() <= 0);

            // Read input
            uint8_t readValue = (uint8_t) Serial.read();

            // Write to RAM
            mreqPin_low;
            write_byte(ramAddress, readValue);
            asm volatile("nop");
            asm volatile("nop");
            asm volatile("nop");
            mreqPin_high;
          }
        }

        // Disable RAM
        write_byte(0x0000, 0x00);
        Serial.flush(); // Flush any serial data that wasn't processed
      }
    }

      rd_wr_mreq_off();
    ///Serial.println("Loop done \n");
   }
  

  uint8_t read_byte(uint16_t address) {
    shiftout_address(address); // Shift out address

    mreqPin_low;
    rdPin_low;
    asm volatile("nop"); // Delay a little (minimum is 2 nops, using 3 to be sure)
    asm volatile("nop");
    asm volatile("nop");
    uint8_t bval = ((PINB << 6) | (PIND >> 2)); // Read data
    rdPin_high;
    mreqPin_high;

    return bval;
  }

  void write_byte(uint16_t address, uint8_t data) {
    // Set pins as outputs
    DDRB |= ((1 << PB0) | (1 << PB1) | (1 << PB4)); // | (1<<PB2) | (1<<PB4),D8 to D13, datapin | (1<<PB3), clock | (1<<PB5)
    // DDRC |= ((1<<PC0) | (1<<PC3) | (1<<PC4) | (1<<PC5)); // A0, 3 to 5
    DDRD |= ((1 << PD1) | (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7)); // D0 to D7

    shiftout_address(address); // Shift out address

    // Clear outputs and set them to the data variable
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB4)); // | (1<<PB2) | (1<<PB4)D8 to D13, clock | (1<<PB3) | (1<<PB5)
    // DDRC &= ~((1<<PC0) | (1<<PC3) | (1<<PC4) | (1<<PC5)); // A0, 3 to 5
    PORTD &= ~((1 << PD2) | (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7)); // D0 to D7

    PORTD |= (data << 2);
    //PORTC |= (data << 2); // bitshift legth unknown atm!
    PORTB |= (data >> 6);

    // Pulse WR
    wrPin_low;
    asm volatile("nop");
    wrPin_high;

    // Set pins as inputs
    DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB4)); // | (1<<PB2) | (1<<PB4), D8 to D13, datapin | (1<<PB3), clock | (1<<PB5)
    // DDRC &= ~((1<<PC0) | (1<<PC3) | (1<<PC4) | (1<<PC5)); // A0, 3 to 5
    DDRD &= ~((1 << PD0) | (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7)); // D0 to D7
  }

  // Use the shift registers to shift out the address
  void shiftout_address(uint16_t shiftAddress) {
    SPI.transfer(shiftAddress >> 8);
    SPI.transfer(shiftAddress & 0xFF);
    //SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0)); // alternative

    latchPin_low;
    asm volatile("nop");
    latchPin_high;
  }

  // Turn /RD, /WR and MREQ to high so they are deselected (reset state)
  void rd_wr_mreq_reset(void) {
    rdPin_high; // /RD off
    wrPin_high; /// WR off
    mreqPin_high; // MREQ off
  }

  // Turn /RD, /WR and MREQ off as no power should be applied to GB Cart
  void rd_wr_mreq_off(void) {
    rdPin_low;
    wrPin_low;
    mreqPin_low; // /CS 
  }

  // Setup pin interrupt for button
  void (* resetFunc)(void) = 0; // create a standard reset function

  void soft_reset(void) {
    //Serial.println("Button Reset check ");
    resetFunc();
    //digitalWrite(gb30Pin, LOW); // deactivate /RST
    //digitalWrite(gb30Pin, HIGH); // activate /RST (method without button / soft reset)
  }

  void reset_button(void) { // active-low jumper on board should be set!
    //Serial.println("Reset button check ");
    // Serial.println("Reset pressed! ");
    // https://forums.nesdev.org/viewtopic.php?t=15689
    // gb30Pin_low;  // is open-drain, 100h-00h /CS2+/RST addr
    // gb30Pin_high;
  }

  // --------------experimental!!! if ready it will be built-in to the upper code!!!
  // joaopauloap suggestions
  unsigned int readDataBus() {
    Serial.println("CS2 read databus ");
    unsigned int s = ((PINB & 0b00000011) << 6) | ((PIND & 0b11111100) >> 2);
    return (s);
  }

  void latchaddress(unsigned int addr) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (addr >> 8)); // clock ? idk
    shiftOut(dataPin, clockPin, MSBFIRST, addr);
    digitalWrite(latchPin, HIGH);
  }

  void cs2_dumpsave() { // digital active-low
    Serial.println("Dumping GBA Save ");
    //digitalWrite(mreqPin, HIGH);  // CS Disable ROM
    digitalWrite(gb30cPin, LOW);
    digitalWrite(gb30cPin, LOW);
    for (unsigned long addr = 0; addr < 0x1FFF; addr++) {
      latchaddress(addr);
      digitalWrite(rdPin, LOW);
      delayMicroseconds(2);
      digitalWrite(rdPin, HIGH);
      int d = readDataBus();
      Serial.write(d);
    }

    digitalWrite(gb30cPin, HIGH);
    Serial.println("CS2 dump done");
    // may ned adaption in C/Py code on pc app!
    // -> not sure what data is extly tranfered ? need more info !!
  }
  // end  suggestion

  /*
    The problem is that the GB is using a base 2 frequency clock crystal,
    not a base 10 one. Ie, 1MiHz = 1024*1024 Hz = 1048576 Hz,
    where "normally" you'd use 1 MHz = 1000000 Hz. But that's easy to overcome with new frequency tables.
    https://gbdev.gg8.se/forums/viewtopic.php?id=10
    https://dhole.github.io/post/gameboy_cartridge_emu_2/ only for STM32
  */

  void IRQ_GBA(void) {
    Serial.println("attach check ");
  }

  ISR(gb31iPin_vect) { // alternative for attached pin
    Serial.println("ISR check ");
  }

  void IRQ_handler(void) {
    Serial.println("IRQ check ");
    pinMode(gb31iPin, INPUT);  // INPUT_PULLUP Set the pin as input with pull-up resistor, not sure normally no resistor needed , std output
    gb31iPin_high;
    attachInterrupt(digitalPinToInterrupt(gb31iPin), IRQ_GBA, RISING);
    // https://www.arduino.cc/reference/de/language/functions/external-interrupts/attachinterrupt/
    // maybe only with INT0&1, D2&3 on nano for attaching digital,wip -> may need change for the pin gb23 (d0) or gb24 (d1)
    // => gb23 from d2 to d1!!!!! change stuff in main funtions!!!
    // Handle PC0 interrupt (rising edge of the gameboy CLK)
    gb31iPin_low;
  }

  void AUDIO_GB(void) {
    Serial.println("AUDIO check ");
    //may need PORTB activation above! wip but no priority/ wip
    /*
      VIN/AUDIO can output up to 0.6V and total stays less than 3V (5 x 0.6V = 3V).
      So I guess that software using VIN should not put an audio volume higher
      than 12.8 on each channel, which would mean 0xC or 0xD.
    */
  }

  // sd card via spi
  void sd_card_action(void) {
    Serial.println("sd_card_action check \n");
    //add later with thebrutzler
  }

  //------------- end experimental
  // interrupt check for main
  ISR(PCINT1_vect) {
    _delay_ms(200);
    if (PINC & (1 << PC1)) { // Invert mosfet control pin on A1
      PORTC ^= (1 << PC0);
    }

    while (PINC & (1 << PC1)); // Wait for button release
    _delay_ms(200);

    // Check if mosfet is on or off (inverted as it's a P mosfet)
    if (PINC & (1 << PC0)) {
      SPI.end(); // End SPI
      rd_wr_mreq_off(); // Set everying low
      latchPin_low;
      dataPin_low;
      clockPin_low;
      gb31Pin_low; // AUDIO
      gb31iPin_low; // IRQ
      gb30cPin_low;  //CSs2
      //gb30Pin_low; // maybe use reset pins for hardware reset
      _delay_ms(200);
      asm volatile ("jmp 0"); // Jump to address 0 (soft reset)

    }
    else {
      SPI.begin(); // Setup SPI
      // alternative SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
      // Max SPI speed is always half of the CPU clock.
      SPI.setClockDivider(SPI_CLOCK_DIV2); // half cock speed, gb dmg clock 4 MHz (DIV4 default)
      SPI.setBitOrder(MSBFIRST); // most-significant bit first
      SPI.setDataMode(SPI_MODE0); // clock polarity and phase (leading edge Sample rising, trailing edge Setup falling)
    }

  }
 
  //EOF
