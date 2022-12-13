void begin(void)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;

    txBufferIndex = 0;
    txBufferLength = 0;

    twi_init();
    twi_attachSlaveTxEvent(onRequestService); // default callback must exist
    twi_attachSlaveRxEvent(onReceiveService); // default callback must exist
}

void twi_init(void)
{
    // initialize state
    twi_state = TWI_READY;
    twi_sendStop = true;		// default value
    twi_inRepStart = false;

    // activate internal pullups for twi.
    digitalWrite(SDA, 1);
    digitalWrite(SCL, 1);

    // initialize twi prescaler and bit rate
    cbi(TWSR, TWPS0);
    cbi(TWSR, TWPS1);
    TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

    /* twi bit rate formula from atmega128 manual pg 204
    SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
    note: TWBR should be 10 or higher for master mode
    It is 72 for a 16mhz Wiring board with 100kHz TWI */

    // enable twi module, acks, and twi interrupt
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}