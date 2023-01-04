#include <avr/io.h>

void EEPROM_write(unsigned short uiAddress, unsigned char ucData)
{
    while (EECR & (1 << EEPE))
        ;
    EEAR = uiAddress;
    EEDR = ucData;
    EECR |= 1 << EEMPE;
    EECR |= 1 << EEPE;
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
    while (EECR & (1 << EEPE))
        ;
    EEAR = uiAddress;
    EECR |= (1 << EERE);
    return EEDR;
}

void EEPROM_update(unsigned short uiAddress, unsigned char ucData)
{
    unsigned char ucOldData = EEPROM_read(uiAddress);
    if (ucOldData != ucData)
    {
        EEPROM_write(uiAddress, ucData);
    }
}