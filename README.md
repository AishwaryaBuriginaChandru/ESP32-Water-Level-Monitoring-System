# ESP32 Water Level Monitoring System

An ESP32-based smart water level monitoring and automatic motor control system with a web dashboard for monitoring and configuring the device over Wi-Fi.

## Features

- Real-time water level monitoring
- Automatic motor ON/OFF control
- OLED display for local status monitoring
- Relay control for water pump
- Web-based dashboard
- Wi-Fi configuration page
- ESP-NOW communication support
- JSON-based configuration storage
- PlatformIO project structure

---

## Hardware Used

- ESP32 Development Board
- Ultrasonic Water Level Sensor
- OLED Display (I2C)
- Relay Module
- Water Pump (through relay)
- Power Supply

---

## Software Used

- PlatformIO
- Arduino Framework
- ESPAsyncWebServer
- ArduinoJson
- LittleFS
- Bootstrap
- jQuery

---

## Folder Structure

```
ESP32-Water-Level-Monitoring-System
│
├── data/                 # Web dashboard files (HTML, CSS, JS)
├── src/                  # Source code
├── Architecture.md       # System architecture
├── platformio.ini        # PlatformIO configuration
└── README.md
```

---

## Project Architecture

The project is divided into multiple modules:

- Wi-Fi Controller
- Web Server Controller
- Configuration Manager
- Water Level Controller
- Display Controller
- Motor Controller
- ESP-NOW Controller
- Logger

The web interface communicates with the ESP32 using REST APIs to configure Wi-Fi settings and monitor the system.

---

## Getting Started

### Clone the repository

```bash
git clone https://github.com/<your-username>/ESP32-Water-Level-Monitoring-System.git
```

### Open in PlatformIO

Open the project using Visual Studio Code with the PlatformIO extension installed.

### Upload File System

Upload the contents of the `data` folder to LittleFS using PlatformIO.

### Build and Upload

Compile and upload the firmware to the ESP32.

---

## Web Dashboard

The web dashboard allows users to:

- View water level
- Configure Wi-Fi
- Restart the ESP32
- Monitor device status

---

## Technologies

- C++
- HTML
- CSS
- JavaScript
- Bootstrap
- jQuery
- ESP32
- PlatformIO

---

## Future Improvements

- User authentication
- OTA firmware updates
- MQTT integration
- Cloud monitoring
- Mobile application
- Historical data logging

---
