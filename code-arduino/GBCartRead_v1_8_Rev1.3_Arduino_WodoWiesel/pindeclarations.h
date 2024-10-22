 /*
 UPDATE: This code was changed to be compatible for Arduino NANO and a custom PCB!!

 Author: WodoWiesel
 github: (https://github.com/wodowiesel/GB-Dumper)
 Last Modified: 28 June 2024
 Compativble with PCB v0.83!
 */

#ifndef PINDECLARATIONS_H_
#define PINDECLARATIONS_H_
#define HW 3 // hardware revision v3.x nano
#define FW 4.4
#define OZ 28.800 //xtal oscilator 28,800 MHz
/*
// AVR definitions for Arduino nano! -> check your cocnnections again!

Registers:
https://docs.arduino.cc/retired/hacking/software/PortManipulation/
https://deepbluembedded.com/arduino-port-manipulation-registers-example/
// PORTA (only nano 33/uno?)
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7 // ??
*/

// PORTB
#define PB0 0 // D8
#define PB1 1 // D9
#define PB2 2 // D10
#define PB3 3 // D11/MOSI
#define PB4 4 // D12/MISO
#define PB5 5 // D13/SCK(SPI)
#define PB6 6 // /RES
//#define PB7 7 // not on nano

// PORTC
#define PC0 0 // A0
#define PC1 1 // A1
#define PC2 2 // A2
#define PC3 3 // A3
#define PC4 4 // A4
#define PC5 5 // A5
#define PC6 6 // A6
#define PC7 7 // A7

// PORTD
#define PD0 0 // D0/TX
#define PD1 1 // D1/RX
#define PD2 2 // D2/INT0
#define PD3 3 // D3/INT1
#define PD4 4 // D4
#define PD5 5 // D5
#define PD6 6 // D6
#define PD7 7 // D7

// Assignments for functions to specific ports
#define mosfetPin_high PORTC  |= (1<<PC0); // Pin19 A0
#define mosfetPin_low  PORTC  &= ~(1<<PC0); // not sure if | or & inverted?

#define BTNPin_high   PORTC |= (1<<PC1); // /RST Button for soft reset, Pin20 A1, not used atm
#define BTNPin_low    PORTC &= ~(1<<PC1); // /RST Button for soft reset, Pin20 A1, not used atm


// A2/PC2 not used

#define wrPin_high    PORTC |= (1<<PC3); // /WR Pin22 A3
#define wrPin_low     PORTC &= ~(1<<PC3);

#define mreqPin_high  PORTC |= (1<<PC4); // /CS Pin23 A4
#define mreqPin_low   PORTC &= ~(1<<PC4);

#define rdPin_high    PORTC |= (1<<PC5); // /RD Pin24 A5
#define rdPin_low     PORTC &= ~(1<<PC5);

#define latchPin_high PORTB |= (1<<PB2); // RCLK/SS Pin13 D10  ->CS???
#define latchPin_low  PORTB &= ~(1<<PB2);

#define dataPin_high  PORTB |= (1<<PB3); // MOSI Pin14 D11
#define dataPin_low   PORTB &= ~(1<<PB3);

#define clockPin_high PORTB |= (1<<PB5); // SRCLK/SCK Pin 16 D13 (SPI)
#define clockPin_low PORTB &= ~(1<<PB5);

// extra pins
/*
#define gb22Pin_high  PORTD |= (1<<PD1); // GB_D0  Pin2 D1 RX    
#define gb22Pin_low   PORTD &= ~(1<<PD1);

#define gb23Pin_high  PORTD |= (1<<PD3); // GB_D1 Pin6 INT1 D3
#define gb23Pin_low   PORTD &= ~(1<<PD3);

#define gb24Pin_high  PORTD |= (1<<PD4); // GB_D2 Pin7 D4
#define gb24Pin_low   PORTD &= ~(1<<PD4);

#define gb25Pin_high  PORTD |= (1<<PD5); // GB_D3 Pin8 D5
#define gb25Pin_low   PORTD &= ~(1<<PD5);

#define gb26Pin_high  PORTD |= (1<<PD6); // GB_D4 Pin9 D6
#define gb26Pin_low   PORTD &= ~(1<<PD6);

#define gb27Pin_high  PORTD |= (1<<PD7); // GB_D5 Pin10 D7
#define gb27Pin_low   PORTD &= ~(1<<PD7);

#define gb28Pin_high  PORTB |= (1<<PB0); // GB_D6 Pin11 D8
#define gb28Pin_low   PORTB &= ~(1<<PB0);

#define gb29Pin_high  PORTB |= (1<<PB1); // GB_D7 Pin12 D9
#define gb29Pin_low   PORTB &= ~(1<<PB1);
*/

// special pins

#define gb30Pin_high  PORTB |= (1<<PB6); // /RES Pin3&29
#define gb30Pin_low   PORTB &= ~(1<<PB6);

#define gb30cPin_high  PORTD |= (1<<PD0); // /CS2(digital) on D0/PD0 Pin1
#define gb30cPin_low   PORTD &= ~(1<<PD0);

#define gb31Pin_high  PORTC |= (1<<PC1); // AUD_IN analog on A1 PC1 Pin21 (bits 7 & 3 of NR_50 register ) 
#define gb31Pin_low   PORTC &= ~(1<<PC1);

#define gb31iPin_high  PORTB |= (1<<PD2); // IRQ (digital) Pin5 INT0 D2 (on MISO Pin15 D12 old)
#define gb31iPin_low   PORTB &= ~(1<<PD2);

#endif /* PINDECLARATIONS_H_ */
