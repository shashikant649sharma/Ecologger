# EcoLogger: Advanced Environmental Data Logging System
EcoLogger is a sophisticated environmental data logging system designed to monitor and record various environmental parameters using the STM32F103C8T6 microcontroller. It integrates multiple sensors, logs data to an SD card, and provides real-time information on an OLED screen. The system also supports Modbus RTU communication, ensuring compatibility with industrial IoT systems.

#Features
- Data Logging: Collects temperature and humidity data and stores it on an SD card using the SPI protocol.
- Real-Time Display: Shows sensor readings on an OLED screen using the I2C protocol.
- Modbus RTU Communication: Communicates with Modbus master devices via RS-485 using the MAX485 transceiver.
- RTC Integration: Logs data with accurate timestamps using the DS1307 real-time clock.
- Sensor Integration: Utilizes the AHT21B sensor for temperature and humidity measurements.
# Components
- Microcontroller: STM32F103C8T6 (ARM Cortex-M3)
- Sensors: AHT21B Environmental Sensor (I2C)
- Display: OLED SSD1306 (I2C)
- SD Card Module: SPI-based for data logging
- Real-Time Clock (RTC): DS1307 (I2C)
- Communication: MAX485 Transceiver for Modbus RTU communication
#Libraries Used
- ModbusRTUSlave: For implementing Modbus RTU communication.
- U8x8lib: For interfacing with the OLED SSD1306 display.
- Wire: For I2C communication.
- SPI: For SPI-based SD card communication.
- SD: For handling data storage on the SD card.
RTClib: For managing timekeeping with the DS1307 RTC.
# Hardware Setup
- Connect the STM32 microcontroller to the sensors, OLED display, SD card module, and MAX485 transceiver as per the provided schematics.
- Ensure all connections are secure and correctly mapped according to the pin configurations.
# Software Requirements
- Install Arduino IDE or STM32CubeIDE.
- Upload the code from the src/ folder to the microcontroller.
- Ensure all required libraries are available. They are included in the libraries/ folder or can be installed via the Arduino Library Manager.
- Running the Project
- Power up the system.
- The EcoLogger will automatically start:
- Logging temperature and humidity data to the SD card.
- Displaying real-time sensor values on the OLED screen.
- Communicating data via Modbus RTU.
- Additional Information
* For detailed schematics, code explanations, and library documentation, please refer to the respective folders and documentation files in this repository.
  # Arduino IDE setup
  add given link to File -> prefrence -> additional Board manager URLs:
  https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
  
 ![image](https://github.com/user-attachments/assets/7376a448-b247-4215-ba8a-106853687952)

![image](https://github.com/user-attachments/assets/9e0a17c8-052f-44af-9db5-ff5236466acc)



License
- This project is licensed under the MIT License. See the LICENSE file for details.

