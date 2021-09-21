#include "MCP3912.h"
#include <SPI.h>

SPIClass *vspi = new SPIClass(VSPI);

MCP3912::MCP3912(uint8_t CsPin, uint8_t DRpin) {
    _CSpin = CsPin;
    _DRpin = DRpin;

    pinMode(_CSpin, OUTPUT);
    pinMode(_DRpin, INPUT_PULLUP);
    digitalWrite(_CSpin, HIGH);
}

MCP3912::~MCP3912() {

}

/* config here */
void MCP3912::setup_MCP_configuration(ADCConfig &activeConfig) {

    activeConfig.DitherMode = 3;
    activeConfig.PreScale = 0;          /* Table 4.1 for prescaler values for AMCLK */
    activeConfig.OSR = 255;             /* Table 4.3 for OSR */
    activeConfig.boost = 0;
    activeConfig.PGA_CH0 = 0;           /* Table 5.1 */
    activeConfig.PGA_CH1 = 0;           /* for gain */
    activeConfig.PGA_CH2 = 0;
    activeConfig.PGA_CH3 = 0;

}

void MCP3912::write_MCP_configuration(byte DitherMode, byte PreScale, byte OSR, byte boost,
                                      byte PGA_CH0, byte PGA_CH1, byte PGA_CH2, byte PGA_CH3) {

    Serial.print(F("\r\n[MCP]\tEntering ADC configuration"));

    vspi->begin();
    Settings.Boost = boost;
    Settings.DitherMode = DitherMode;
    Settings.OSR = OSR;
    Settings.PGA_CH0 = PGA_CH0;
    Settings.PGA_CH1 = PGA_CH1;
    Settings.PGA_CH2 = PGA_CH2;
    Settings.PGA_CH3 = PGA_CH3;
    Settings.PreScale = PreScale;

    byte _Gain[3] = {0, 0, 0};
    byte _Config0[3], _Config1[3], _StatusCom[3] = {0, 0, 0};

    _Gain[1] = (PGA_CH3 << 1) | (PGA_CH2 >> 2);
    _Gain[2] = (PGA_CH2 << 6) | (PGA_CH1 << 3) | PGA_CH0;

    /* Address counter auto-increments, and loops on register TYPES,
     * Address counter auto-increments and loops on writable part of the register map (DEFAULT),
     * The DR pin state is a logic high when data is NOT ready
     * CRC = 16-bit, ADC Data = 24-bit */
    _StatusCom[0] = 0xB9;
    /* CRC is disabled
     * The interrupt flag for the CRCREG checksum verification is disabled. The CRCREG<15:0> bits are
     * still calculated properly and can still be read in this mode. No interrupt is generated even when a
     * CRCREG checksum error happens (Default) */
    _StatusCom[1] = 0x00;
    /* DRSTATUS<3:0>: Data ready status bit for each individual ADC channel
     * DRSTATUS<n> = 1 - Channel CHn data is not ready (DEFAULT)
     * DRSTATUS<n> = 0 - Channel CHn data is ready. The status bit is set back to '1' after reading the
     * STATUSCOM register. The status bit is not set back to '1' by the read of the corresponding channel ADC data */
    _StatusCom[2] = 0x0F;

    _Config0[0] = (DitherMode << 4) | (boost << 2) | PreScale;
    _Config0[1] = OSR << 5;
    _Config0[2] = 0x50; /* See page 36 in datasheet for vrefcal description */

    _Config1[0] = 0x00; /* No re-set mode active */
    _Config1[1] = 0x00; /* No shut-down active */
    _Config1[2] = 0x40; /* Internal voltage reference, external clock */

    digitalWrite(_CSpin, LOW);
    vspi->beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    vspi->transfer(MCP3912_WRITE_MASK | (Addresses.GAIN << 1));
    vspi->transfer(_Gain[0]);
    vspi->transfer(_Gain[1]);
    vspi->transfer(_Gain[2]);
    vspi->transfer(_StatusCom[0]);
    vspi->transfer(_StatusCom[1]);
    vspi->transfer(_StatusCom[2]);
    vspi->transfer(_Config0[0]);
    vspi->transfer(_Config0[1]);
    vspi->transfer(_Config0[2]);
    vspi->transfer(_Config1[0]);
    vspi->transfer(_Config1[1]);
    vspi->transfer(_Config1[2]);
    vspi->endTransaction();
    digitalWrite(_CSpin, HIGH);

    Serial.print(F("\r\n[MCP]\tSent configuration data:"));
    Serial.print(F("\r\n[<-]\tGAIN:\t"));
    print_binary_byte(_Gain, 3);

    Serial.print(F("\r\n[<-]\tSTATUSCOM:\t"));
    print_binary_byte(_StatusCom, 3);

    Serial.print(F("\r\n[<-]\tCONFIG0:\t"));
    print_binary_byte(_Config0, 3);

    Serial.print(F("\r\n[<-]\tCONFIG1:\t"));
    print_binary_byte(_Config1, 3);

}

void MCP3912::write_MCP_configuration(ADCConfig activeConfig) {

    Serial.print(F("\r\n[MCP]\tEntering ADC configuration"));

    vspi->begin();
    Settings.Boost = activeConfig.boost;
    Settings.DitherMode = activeConfig.DitherMode;
    Settings.OSR = activeConfig.OSR;
    Settings.PGA_CH0 = activeConfig.PGA_CH0;
    Settings.PGA_CH1 = activeConfig.PGA_CH1;
    Settings.PGA_CH2 = activeConfig.PGA_CH2;
    Settings.PGA_CH3 = activeConfig.PGA_CH3;
    Settings.PreScale = activeConfig.PreScale;

    byte _Gain[3] = {0, 0, 0};
    byte _Config0[3], _Config1[3], _StatusCom[3] = {0, 0, 0};

    _Gain[1] = (activeConfig.PGA_CH3 << 1) | (activeConfig.PGA_CH2 >> 2);
    _Gain[2] = (activeConfig.PGA_CH2 << 6) | (activeConfig.PGA_CH1 << 3) | activeConfig.PGA_CH0;

    /* Address counter auto-increments, and loops on register TYPES,
     * Address counter auto-increments and loops on writable part of the register map (DEFAULT),
     * The DR pin state is a logic high when data is NOT ready
     * CRC = 16-bit, ADC Data = 24-bit */
    _StatusCom[0] = 0xB9;
    /* CRC is disabled
     * The interrupt flag for the CRCREG checksum verification is disabled. The CRCREG<15:0> bits are
     * still calculated properly and can still be read in this mode. No interrupt is generated even when a
     * CRCREG checksum error happens (Default) */
    _StatusCom[1] = 0x00;
    /* DRSTATUS<3:0>: Data ready status bit for each individual ADC channel
     * DRSTATUS<n> = 1 - Channel CHn data is not ready (DEFAULT)
     * DRSTATUS<n> = 0 - Channel CHn data is ready. The status bit is set back to '1' after reading the
     * STATUSCOM register. The status bit is not set back to '1' by the read of the corresponding channel ADC data */
    _StatusCom[2] = 0x0F;

    _Config0[0] = (activeConfig.DitherMode << 4) | (activeConfig.boost << 2) | activeConfig.PreScale;
    _Config0[1] = activeConfig.OSR << 5;
    _Config0[2] = 0x50; /* See page 36 in datasheet for vrefcal description */

    _Config1[0] = 0x00; /* No re-set mode active */
    _Config1[1] = 0x00; /* No shut-down active */
    _Config1[2] = 0x40; /* Internal voltage reference, external clock */

    digitalWrite(_CSpin, LOW);
    vspi->beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    vspi->transfer(MCP3912_WRITE_MASK | (Addresses.GAIN << 1));
    vspi->transfer(_Gain[0]);
    vspi->transfer(_Gain[1]);
    vspi->transfer(_Gain[2]);
    vspi->transfer(_StatusCom[0]);
    vspi->transfer(_StatusCom[1]);
    vspi->transfer(_StatusCom[2]);
    vspi->transfer(_Config0[0]);
    vspi->transfer(_Config0[1]);
    vspi->transfer(_Config0[2]);
    vspi->transfer(_Config1[0]);
    vspi->transfer(_Config1[1]);
    vspi->transfer(_Config1[2]);
    vspi->endTransaction();
    digitalWrite(_CSpin, HIGH);

    Serial.print(F("\r\n[MCP]\tSent configuration data:"));
    Serial.print(F("\r\n[<-]\tGAIN:\t\t"));
    print_binary_byte(_Gain, 3);

    Serial.print(F("\r\n[<-]\tSTATUSCOM:\t"));
    print_binary_byte(_StatusCom, 3);

    Serial.print(F("\r\n[<-]\tCONFIG0:\t"));
    print_binary_byte(_Config0, 3);

    Serial.print(F("\r\n[<-]\tCONFIG1:\t"));
    print_binary_byte(_Config1, 3);

}

bool MCP3912::initialize_with_conf(ADCConfig &activeConfig){
    MCP3912::setup_MCP_configuration(activeConfig);
    MCP3912::write_MCP_configuration(activeConfig);
    MCP3912::read_MCP_configuration();
    return true;
}

int32_t MCP3912::read_values(int32_t *Data, uint8_t chToRead) {
    if (chToRead == 0)
        return 0;

    byte Buf[4] = {0, 0, 0, 0};
    int32_t temp = 0;

    digitalWrite(_CSpin, LOW);
    unsigned char readAddress = Addresses.CHANNEL0 << 1; // Channel 0 address = 0x00;
    readAddress |= MCP3912_READ_MASK;

    vspi->beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    Buf[0] = vspi->transfer(readAddress);
    Buf[1] = vspi->transfer(0); // Byte 23-16
    Buf[2] = vspi->transfer(0); // Byte 15-8
    Buf[3] = vspi->transfer(0); // Byte 7-0

    temp = (
            ((uint32_t) Buf[1] << 16) |
            ((uint32_t) Buf[2] << 8) |
            ((uint32_t) Buf[3])
    );

    //Convert to 32bit 2's complement
    if ((temp & 0x00800000) > 0) {
        temp |= 0xFF000000;
    } else {
        temp &= 0x00FFFFFF;
    }
    Data[0] = temp;

    if (chToRead >= 2) {
        temp = 0;

        Buf[1] = vspi->transfer(0); // Byte 23-16
        Buf[2] = vspi->transfer(0); // Byte 15-8
        Buf[3] = vspi->transfer(0); // Byte 7-0

        temp = (
                ((uint32_t) Buf[1] << 16) |
                ((uint32_t) Buf[2] << 8) |
                ((uint32_t) Buf[3])
        );

        //Convert to 32bit 2's complement
        if ((temp & 0x00800000) > 0) {
            temp |= 0xFF000000;
        } else {
            temp &= 0x00FFFFFF;
        }

        Data[1] = temp;
    }
    if (chToRead >= 3) {
        temp = 0;
        Buf[1] = vspi->transfer(0); // Byte 23-16
        Buf[2] = vspi->transfer(0); // Byte 15-8
        Buf[3] = vspi->transfer(0); // Byte 7-0

        temp = (
                ((uint32_t) Buf[1] << 16) |
                ((uint32_t) Buf[2] << 8) |
                ((uint32_t) Buf[3])
        );

        //Convert to 32bit 2's complement
        if ((temp & 0x00800000) > 0) {
            temp |= 0xFF000000;
        } else {
            temp &= 0x00FFFFFF;
        }
        Data[2] = temp;
    }

    if (chToRead >= 4) {
        temp = 0;
        Buf[1] = vspi->transfer(0); // Byte 23-16
        Buf[2] = vspi->transfer(0); // Byte 15-8
        Buf[3] = vspi->transfer(0); // Byte 7-0

        temp = (
                ((uint32_t) Buf[1] << 16) |
                ((uint32_t) Buf[2] << 8) |
                ((uint32_t) Buf[3])
        );

        //Convert to 32bit 2's complement
        if ((temp & 0x00800000) > 0) {
            temp |= 0xFF000000;
        } else {
            temp &= 0x00FFFFFF;
        }
        Data[3] = temp;
    }

    vspi->endTransaction();
    digitalWrite(_CSpin, HIGH);

    return (int32_t) chToRead;
}

void MCP3912::read_MCP_configuration(void) {

    Serial.print(F("\r\n[MCP]\tReading ADC configuration"));

    digitalWrite(_CSpin, LOW);
    vspi->beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    vspi->transfer(MCP3912_READ_MASK | (Addresses.GAIN << 1));
    Registers._GainSettings[0] = vspi->transfer(0xFF);
    Registers._GainSettings[1] = vspi->transfer(0xFF);
    Registers._GainSettings[2] = vspi->transfer(0xFF);
    Registers._StatusComSettings[0] = vspi->transfer(0xFF);
    Registers._StatusComSettings[1] = vspi->transfer(0xFF);
    Registers._StatusComSettings[2] = vspi->transfer(0xFF);
    Registers._Config0Register[0] = vspi->transfer(0xFF);
    Registers._Config0Register[1] = vspi->transfer(0xFF);
    Registers._Config0Register[2] = vspi->transfer(0xFF);
    Registers._Config1Register[0] = vspi->transfer(0xFF);
    Registers._Config1Register[1] = vspi->transfer(0xFF);
    Registers._Config1Register[2] = vspi->transfer(0xFF);
    vspi->endTransaction();
    digitalWrite(_CSpin, HIGH);


    Serial.print(F("\r\n[MCP]\tGot configuration data:"));
    Serial.print(F("\r\n[->]\tGAIN:\t\t"));
    print_binary_byte(Registers._GainSettings, 3);

    Serial.print(F("\r\n[->]\tSTATUSCOM:\t"));
    print_binary_byte(Registers._StatusComSettings, 3);

    Serial.print(F("\r\n[->]\tCONFIG0:\t"));
    print_binary_byte(Registers._Config0Register, 3);

    Serial.print(F("\r\n[->]\tCONFIG1:\t"));
    print_binary_byte(Registers._Config1Register, 3);
}

int32_t MCP3912::read_single_value(uint8_t chAddr) {
    byte channel;
    if (chAddr == 0) {
        channel = Addresses.CHANNEL0;
    } else if (chAddr == 1) {
        channel = Addresses.CHANNEL1;
    } else if (chAddr == 2) {
        channel = Addresses.CHANNEL2;
    } else if (chAddr == 3) {
        channel = Addresses.CHANNEL3;
    } else {
        channel = Addresses.CHANNEL0; //default
    }

    byte Buf[4] = {0, 0, 0, 0};

    digitalWrite(_CSpin, LOW);

    vspi->beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    Buf[3] = vspi->transfer(MCP3912_READ_MASK | (channel << 1));
    Buf[0] = vspi->transfer(0); // Byte 23-16
    Buf[1] = vspi->transfer(0); // Byte 15-8
    Buf[2] = vspi->transfer(0); // Byte 7-0

    digitalWrite(_CSpin, HIGH);
    vspi->endTransaction();

    int32_t temp = 0;
    temp = (
            ((uint32_t) Buf[0] << 16) |
            ((uint32_t) Buf[1] << 8) |
            ((uint32_t) Buf[2])
    );

    //Convert to 32bit 2's complement
    if ((temp & 0x00800000) > 0) {
        temp |= 0xFF000000;
    } else {
        temp &= 0x00FFFFFF;
    }
    return temp;
}

void MCP3912::print_binary_byte(byte *data, uint8_t length) {
    for (int i = 0; i < length; i++) {
        for (unsigned int mask = 0x80; mask; mask >>= 1) {
            Serial.print(mask & data[i] ? '1' : '0');
        }
    }
}

void MCP3912::print_binary_int16(uint16_t *data, uint8_t length) {
    for (int i = 0; i < length; i++) {
        for (unsigned int mask = 0x8000; mask; mask >>= 1) {
            Serial.print(mask & data[i] ? '1' : '0');
        }
    }
}

void MCP3912::print_binary_int32(uint32_t *data, uint8_t length) {
    for (int i = 0; i < length; i++) {
        for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
            Serial.print(mask & data[i] ? '1' : '0');
        }
    }
}

long MCP3912::mcp3912_read_reg(byte reg) {

    byte Buf[4] = {0, 0, 0, 0};

    digitalWrite(_CSpin, LOW);
    vspi->beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
/* S */ byte treg = ((0b01)<<6) | (reg<<1) | 1;
/* P */ vspi->transfer(treg);
/* I */ Buf[0] = 0;
        Buf[1] = vspi->transfer(0);
        Buf[2] = vspi->transfer(0);
        Buf[3] = vspi->transfer(0);
    digitalWrite(_CSpin, HIGH);
    vspi->endTransaction();

    return (Buf[0] << 24) | (Buf[1] << 16) | (Buf[2] << 8) | Buf[3];
}

byte MCP3912::mcp_data_ready(byte statComRegister) {
    long st = mcp3912_read_reg(statComRegister); /* STATUSCOM */
    byte str = st & 0b01111;
    return (str == 0);
}