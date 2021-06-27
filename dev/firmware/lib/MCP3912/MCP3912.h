//
// Created by euphoria on 6/27/2021.
//
#ifndef FIRMWARE_MCP3912_H
#define FIRMWARE_MCP3912_H

#include <cstdint>
#include <Arduino.h>

#define ArduinoSPISpeed 4000000 //12MHz
#define UseArduinoSPI

#define WIDTH_ADC0_16bit 0x0
#define WIDTH_ADC0_24bit 0x8
#define WIDTH_ADC1_16bit 0x0
#define WIDTH_ADC1_24bit 0x10

class MCP3912{
public:
    MCP3912(uint8_t CsPin, uint8_t DRpin, uint8_t CLKIpin, float CLKIspeed);
    ~MCP3912();
    void Configuration(byte DitherMode,byte PreScale, byte OSR, byte boost, byte PGA_CH0, byte PGA_CH1, byte PGA_CH2, byte PGA_CH3);
    int32_t ReadValue(int32_t Data[], uint8_t chToRead);
    int32_t ReadSingleValue(uint8_t chAddr);
    void ReadConfiguration(void);
    void printBinaryByte(byte *data, uint8_t length);
    void printBinaryInt16(uint16_t *data, uint8_t length);
    void printBinaryInt32(uint32_t *data, uint8_t length);
    uint8_t _DRpin;

private:

    /*
    TABLE 8-1: MCP3912 REGISTER MAP
    Address		Name		Bits	R/W		Description
    0x00		CHANNEL0	24		R		Channel 0 ADC Data <23:0>, MSB first
    0x01		CHANNEL1	24		R		Channel 1 ADC Data <23:0>, MSB first
    0x02		CHANNEL2	24		R		Channel 2 ADC Data <23:0>, MSB first
    0x03		CHANNEL3	24		R		Channel 3 ADC Data <23:0>, MSB first
    0x04		Unused		24		U		Unused
    0x05		Unused		24		U		Unused
    0x06		Unused		24		U		Unused
    0x07		Unused		24		U		Unused
    0x08		MOD			24		R/W		Delta-Sigma Modulators Output Value
    0x09		Unused		24		R/W		Unused
    0x0A		PHASE		24		R/W		Phase Delay Configuration Register - Channel pairs 0/1 and 2/3
    0x0B		GAIN		24		R/W		Gain Configuration Register
    0x0C		STATUSCOM	24		R/W		Status and Communication Register
    0x0D		CONFIG0		24		R/W		Configuration Register
    0x0E		CONFIG1		24		R/W		Configuration Register
    0x0F		OFFCAL_CH0	24		R/W		Offset Correction Register - Channel 0
    0x10		GAINCAL_CH0 24		R/W		Gain Correction Register - Channel 0
    0x11		OFFCAL_CH1	24		R/W		Offset Correction Register - Channel 1
    0x12		GAINCAL_CH1 24		R/W		Gain Correction Register - Channel 1
    0x13		OFFCAL_CH2	24		R/W		Offset Correction Register - Channel 2
    0x14		GAINCAL_CH2 24		R/W		Gain Correction Register - Channel 2
    0x15		OFFCAL_CH3	24		R/W		Offset Correction Register - Channel 3
    0x16		GAINCAL_CH3 24		R/W		Gain Correction Register - Channel 3
    0x17		Unused		24		U		Unused
    0x18		Unused		24		U		Unused
    0x19		Unused		24		U		Unused
    0x1A		Unused		24		U		Unused
    0x1B		Unused		24		U		Unused
    0x1C		Unused		24		U		Unused
    0x1D		Unused		24		U		Unused
    0x1E		Unused		24		U		Unused
    0x1F		LOCK/CRC	24		R/W		Security Register (Password and CRC-16 on Register Map)
    */
    const byte MCP3912_READ_MASK = 0b01000001;  //Device Address are always 00 at start unless specified to Microchip
    const byte MCP3912_WRITE_MASK = 0b01000000; //Device Address are always 00 at start unless specified to Microchip
    struct{
        const byte CHANNEL0	= 0x00;
        const byte CHANNEL1	= 0x01;
        const byte CHANNEL2	= 0x02;
        const byte CHANNEL3 = 0x03;
        const byte MOD = 0x08;
        const byte PHASE = 0x0A;
        const byte GAIN	= 0x0B;
        const byte STATUSCOM = 0x0C;
        const byte CONFIG0 = 0x0D;
        const byte CONFIG1 = 0x0E;
        const byte OFFCAL_CH0 = 0x0F;
        const byte GAINCAL_CH0 = 0x10;
        const byte OFFCAL_CH1 = 0x11;
        const byte GAINCAL_CH1 = 0x12;
        const byte OFFCAL_CH2 = 0x13;
        const byte GAINCAL_CH2 = 0x14;
        const byte OFFCAL_CH3 = 0x15;
        const byte GAINCAL_CH3 = 0x16;
        const byte LOCK_CRC = 0x1F;
    }Addresses;

    struct
    {
        uint8_t PGA_CH0;
        uint8_t PGA_CH1;
        uint8_t PGA_CH2;
        uint8_t PGA_CH3;
        uint8_t DitherMode;
        uint8_t PreScale;
        uint8_t OSR;
        uint8_t Boost;
        double Vref; //for calibation
    }Settings;

    struct{
        byte _GainSettings[3];
        byte _StatusComSettings[3];
        byte _Config0Register[3];
        byte _Config1Register[3];
    }Registers;
    //HW settings
    uint8_t _CSpin;
    uint8_t _CLKIpin;

};


#endif //FIRMWARE_MCP3912_H
