#include <avr/io.h>

// Function to write a byte of data to the EEPROM at a specified address
void EEPROM_write(unsigned short uiAddress, unsigned char ucData)
{
    while (EECR & (1 << EEPE)); // Wait for the previous write operation to complete
    EEAR = uiAddress; // Set the address to write to in the EEAR register
    EEDR = ucData; // Place the data to be written in the EEDR register
    EECR |= 1 << EEMPE; // Set the EEMPE bit to indicate a write operation is coming
    EECR |= 1 << EEPE; // Set the EEPE bit to start the write operation
}

// Function to read a byte of data from the EEPROM at a specified address
unsigned char EEPROM_read(unsigned int uiAddress)
{
    while (EECR & (1 << EEPE)); // Wait for the previous write operation to complete
    EEAR = uiAddress; // Set the address to read from in the EEAR register
    EECR |= (1 << EERE); // Set the EERE bit to start the read operation
    return EEDR; // Return the data from the EEDR register
}

// Function to update a byte of data in the EEPROM at a specified address
void EEPROM_update(unsigned short uiAddress, unsigned char ucData)
{
    unsigned char ucOldData = EEPROM_read(uiAddress); // Read the current data at the specified address

    // If the new data is different from the old data, write the new data
    if (ucOldData != ucData)
    {
        EEPROM_write(uiAddress, ucData);
    }
}

void EEPROM_clear_entire_mem()
{
    for (uint8_t i = 0; i < EEPROM_SIZE; ++i)
    {
        EEPROM_update(i, 0);
    }
}
