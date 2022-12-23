bool begin() {
    // hardware SPI
    pinMode(_CS, OUTPUT);
    digitalWrite(_CS, HIGH);

    _spi->begin();
    mySPISettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
    m_spiMode = SPI_MODE0;

    // try mode0
    if (getVersion() != 0x811) {
        // try mode1
        mySPISettings = SPISettings(1000000, MSBFIRST, SPI_MODE1);
        m_spiMode = SPI_MODE1;
        if (getVersion() != 0x811) {
            return false;
        }
    }

    writeRegister8(STMPE_SYS_CTRL2, 0x0); // turn off clock
    writeRegister8(STMPE_TSC_CTRL, STMPE_TSC_CTRL_XYZ | STMPE_TSC_CTRL_EN); // XYZ and enable
    writeRegister8(STMPE_INT_EN, STMPE_INT_EN_TOUCHDET);
    writeRegister8(STMPE_ADC_CTRL1, STMPE_ADC_CTRL1_12BIT | (1 << 4)); // 96 clocks per conversion
    writeRegister8(STMPE_ADC_CTRL2, STMPE_ADC_CTRL2_6_5MHZ);
    writeRegister8(STMPE_TSC_CFG, STMPE_TSC_CFG_4SAMPLE | STMPE_TSC_CFG_DELAY_1MS | STMPE_TSC_CFG_SETTLE_5MS);
    writeRegister8(STMPE_TSC_FRACTION_Z, 0x6);
    writeRegister8(STMPE_FIFO_TH, 1);
    writeRegister8(STMPE_FIFO_STA, STMPE_FIFO_STA_RESET);
    writeRegister8(STMPE_FIFO_STA, 0); // unreset
    writeRegister8(STMPE_TSC_I_DRIVE, STMPE_TSC_I_DRIVE_50MA);
    writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
    writeRegister8(STMPE_INT_CTRL, STMPE_INT_CTRL_POL_HIGH | STMPE_INT_CTRL_ENABLE);

    return true;
}
