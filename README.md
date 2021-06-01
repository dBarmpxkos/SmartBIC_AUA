# SmartBIC Biosample OCV measurement system
## Technologies

### Hardware
- `ESP32` Xtensa LX6 MCU
- 1 x 16bit Analog Front End - AFE 4 channels SPI [`MCP3912A1-E/SS`]
- PCB designed with Altium Designer
    - PCB prototype design (AFE, MCU, Power) [status : ✅]
    - PCB prototype fabrication 
### Software
- `USB` data availability via custom `Python 3.6` host [status : 🚧]
    - PySimpleGui 
    - pySerial
- Wireless connectivity 
    - `BLE` [status : 🚧]
    - `WiFi` [status: 🚧]
