/*
GPL V3.  Copyright 2009, tz.
Max speed (3.2khz) ADXL345 accelerometer
INT1 pin to D2, sda and scl to proper pins on a 3.3v arduino, 8mhz.
1000000 baud UART, A5A5 are sync bytes.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>

#define CMDSTART ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN))
#define CMDXFER ((1<<TWINT)|(1<<TWEN))
#define CMDXFERACK ((1<<TWINT)|(1<<TWEN)|(1<<TWEA))
#define CMDSTOP ((1<<TWINT)|(1<<TWEN)|(1<<TWSTO))

#define STASTART 0x08
#define STAREPSTART 0x10
#define STASLAWACK 0x18
#define STASLAWNAK 0x20
#define STADATWACK 0x28
#define STADATWNAK 0x30
#define LOSTARB 0x38
#define STASLARACK 0x40
#define STASLARNAK 0x48
#define STADATRACK 0x50
#define STADATRNAK 0x58

unsigned char readreg(unsigned char reg)
{
    do {
        TWCR = CMDSTART;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STASTART)
            break;
        TWDR = 0x3a;            //0x3a;//a6;                // write
        TWCR = CMDXFER;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STASLAWACK)
            break;
        TWDR = reg;             // register
        TWCR = CMDXFER;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STADATWACK)
            break;
        TWCR = CMDSTART;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STAREPSTART)
            break;
        TWDR = 0x3b;            //0x3b;//a7;             // read
        TWCR = CMDXFER;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STASLARACK)
            break;
        TWCR = CMDXFER;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STADATRNAK)
            break;
    } while (0);
    unsigned char r = TWDR;
    TWCR = CMDSTOP;
    return r;
}

void writereg(unsigned char reg, unsigned char val)
{
    do {
        TWCR = CMDSTART;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STASTART)
            break;
        TWDR = 0x3a;            //0x3a;//a6;                // write
        TWCR = CMDXFER;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STASLAWACK)
            break;
        TWDR = reg;             // register
        TWCR = CMDXFER;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STADATWACK)
            break;
        TWDR = val;
        TWCR = CMDXFER;
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xf8) != STADATWACK)
            break;
    } while (0);
    TWCR = CMDSTOP;
}

unsigned char read6(unsigned char *buf)
{
    unsigned char j = 0;
    UDR0 = 0xa5;

    TWCR = CMDSTART;
    while (!(TWCR & (1 << TWINT)));
    TWDR = 0x3a;                //0x3a;//a6;                // write
    TWCR = CMDXFER;
    while (!(TWCR & (1 << TWINT)));
    TWDR = 0x32;                // register
    TWCR = CMDXFER;
    UDR0 = 0xa5;
    while (!(TWCR & (1 << TWINT)));
    TWCR = CMDSTART;
    while (!(TWCR & (1 << TWINT)));
    TWDR = 0x3b;                //0x3b;//a7;             // read
    TWCR = CMDXFER;
    while (!(TWCR & (1 << TWINT)));
    for (j = 0; j < 6; j++) {
        TWCR = j == 5 ? CMDXFER : CMDXFERACK;
        while (!(TWCR & (1 << TWINT)));
        UDR0 = TWDR;
    }
    TWCR = CMDSTOP;
    return j;
}

int main(void)
{
    Serial.println("Here");
    printf("hello?\n");
    DDRB = 0x20;
    PORTB &= ~0x20;             // LED

    // Crystal based: xtal/16 or xtal/8 for clk / baud - 1
#define BAUD (9600)
    UBRR0 = 0;                  //1000000 / BAUD - 1;

    UCSR0C = 0x06;              //8N1 (should be this from reset)
    UCSR0A = 0xE2;              // clear Interrupts, UART at 2x (xtal/8)
    UCSR0B = 0x18;              // oring in 0x80 would enable rx interrupt

    PORTC |= 0x30;
    TWCR = CMDSTOP;
    TWAMR = TWDR = TWAR = 0;
    DDRD = 0;                   // input
    PORTD = 4;                  //pull up

    TWSR = 0;
    TWBR = 2;                   //2

    sei();

    unsigned char i;

    i = readreg(0);

    writereg(0x2d, 8);
    writereg(0x2c, 0x0f);
    writereg(0x31, 0xb);
    writereg(0x38, 0x9f);
    writereg(0x2e, 0x80);       // data available on int 1

    unsigned char buf[6];
    for (;;) {
        Serial.println("Here");
        PORTB |= 0x20;          // LED
        if (!(PIND & 4))        // no more data, wait for edge to interrupt
            continue;
        PORTB &= ~0x20;         // LED
        i = read6(buf);
    }
    sleep_mode();
    Serial.println("Here");
}
