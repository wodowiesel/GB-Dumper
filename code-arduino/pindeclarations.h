 /*
 UPDATE: This code was changed to be compatible for Arduino NANO and a custom PCB!!

 Author: WodoWiesel
 github: (https://github.com/wodowiesel/GB-Dumper)
 Last Modified: 14 November 2023
 */

#ifndef PINDECLARATIONS_H_
#define PINDECLARATIONS_H_
#define HW6

// AVR definitions for Arduino NANO!
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define PB0 0
#define PB1 1
#define PB2 2
#define PB3 3
#define PB4 4
#define PB5 5
#define PB6 6
#define PB7 7

#define PC0 0
#define PC1 1
#define PC2 2
#define PC3 3
#define PC4 4
#define PC5 5
#define PC6 6
#define PC7 7

#define PD0 0
#define PD1 1
#define PD2 2
#define PD3 3
#define PD4 4
#define PD5 5
#define PD6 6
#define PD7 7

// Assignment
#define wrPin_high    PORTC |= (1<<PC3); // /WR Pin22 A3
#define wrPin_low     PORTC &= ~(1<<PC3);

#define mreqPin_high  PORTC |= (1<<PC4); // /CS Pin23 A4
#define mreqPin_low   PORTC &= ~(1<<PC4);

#define rdPin_high    PORTC |= (1<<PC5); // /RD Pin24 A5
#define rdPin_low     PORTC &= ~(1<<PC5);

#define latchPin_high PORTB |= (1<<PB2); // RCLK Pin13 D10
#define latchPin_low  PORTB &= ~(1<<PB2);

#define dataPin_high  PORTB |= (1<<PB3); // Pin14 D11
#define dataPin_low   PORTB &= ~(1<<PB3);

#define clockPin_high PORTB |= (1<<PB5); // SRCLK Pin 16 D13
#define clockPin_low  PORTB &= ~(1<<PB5);

#define mosfetPin_high  PORTC  |= (1<<PC0); // Pin19 A0
#define mosfetPin_low   PORTC  &= ~(1<<PC0);

// extra pins
#define gb22Pin_high   PORTD |= (1<<PD2); // GB_D0 Pin5 INT0 D2
#define gb22Pin_low    PORTD &= ~(1<<PD2);

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

#define gb30Pin_high  PORTB |= ~(1<<PB6); // /RES & CS2 Pin3&29
#define gb30Pin_low   PORTB &= ~(1<<PB6);

#define gb31Pin_high  PORTB |= (1<<PB4); // AUD_IN & IRQ Pin15 D12
#define gb31Pin_low   PORTB &= ~(1<<PB4);

#define gb32Pin_ground 0 //Pin 4&29

#endif /* PINDECLARATIONS_H_ */
