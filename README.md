#EcoLogger: Advanced Environmental Data Logging System
Description: EcoLogger is an advanced environmental data logging system based on the STM32F103C8T6 microcontroller. It integrates various sensors to monitor environmental parameters such as temperature and humidity, logs the data to an SD card, and displays real-time information on an OLED screen. The system also supports Modbus RTU communication for data exchange with other devices, ensuring compatibility with industrial IoT systems.

Features:
Data Logging: Collects temperature and humidity data and stores it on an SD card using SPI protocol.
Real-Time Display: Displays sensor readings on an OLED screen using the I2C protocol.
Modbus RTU Communication: Communicates with Modbus master devices via RS-485 using the MAX485 transceiver.
RTC Integration: Logs data with accurate timestamps using the DS1307 real-time clock.
Sensor Integration: Supports AHT21B sensor for temperature and humidity readings.
Components:
Microcontroller: STM32F103C8T6 (ARM Cortex-M3)
Sensors: AHT21B Environmental Sensor (I2C)
Display: OLED SSD1306 (I2C)
SD Card Module: SPI-based for data logging
Real-Time Clock (RTC): DS1307 (I2C)
Communication: MAX485 Transceiver for Modbus RTU communication
Libraries Used:
ModbusRTUSlave: For implementing Modbus RTU communication.
U8x8lib: For interfacing with the OLED SSD1306 display.
Wire: For I2C communication.
SPI: For SPI-based SD card communication.
SD: For handling data storage on the SD card.
RTClib: For managing timekeeping with the DS1307 RTC.
How to Use:
Hardware Setup: Connect the STM32 microcontroller to the sensors, OLED, SD card module, and MAX485 transceiver as per the schematics.
Software Requirements: Install Arduino IDE or STM32CubeIDE and upload the code from the src/ folder to the microcontroller.
Library Dependencies: All required libraries are included in the libraries/ folder or can be installed from the Arduino Library Manager.
Run the Project: Power up the system, and it will automatically start logging data, displaying real-time sensor values, and communicating via Modbus.
