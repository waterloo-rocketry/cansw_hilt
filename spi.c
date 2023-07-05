#include "spi.h"
#include <xc.h>

void spi_init()
{
    TRISDbits.TRISD4 = 0; // SCK
    TRISDbits.TRISD5 = 0; // MOSI
    TRISCbits.TRISC2 = 1; // MISO
    TRISEbits.TRISE4 = 0; // CS
    
    ANSDbits.ANSELD4 = 0;
    ANSDbits.ANSELD5 = 0;
    
    RPINR20bits.SDI1R = 39; // MISO RPI39
    RPOR12bits.RP25R = 8; // SCK
    RPOR10bits.RP20R = 7; // MOSI

    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.MSTEN = 1;
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 1;
}

void spi_write(uint8_t data)
{
    uint8_t tmp = SPI1BUF;
    SPI1BUF = data;
    while (!SPI1STATbits.SPIRBF);
}

uint8_t spi_read(void)
{
    uint8_t tmp = SPI1BUF;
    SPI1BUF = 0;
    while (!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}

void cs_drive(uint8_t state)
{
    LATEbits.LATE4 = state > 0;
}