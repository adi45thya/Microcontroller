# STM32 Smart Irrigation System

An automated, intelligent irrigation system powered by the **STM32F103C8T6 (ARM Cortex-M3)** microcontroller. The system monitors soil moisture, ambient temperature, relative humidity, and rainfall in real time, automatically controlling a mechanical water valve operated by an **SG90 servo motor**.

---

## 📌 Features

- **Soil Moisture Monitoring**: Measures soil moisture level (0%–100%) via a YL-69 resistive probe and LM393 comparator on a 12-bit ADC channel.
- **Environmental Sensing**: Samples ambient temperature and relative humidity using a DHT11 sensor via cycle-accurate microsecond handshaking (using Cortex-M3 hardware DWT counter).
- **Rain Detection Override**: Automatically overrides and aborts watering cycles when rain is detected by an MH-RD rain sensor to conserve water.
- **Dynamic Watering Calculation**: Dynamically scales valve open duration based on soil moisture deficit, adjusted for high ambient temperature and dry air.
- **Mechanical Valve Actuation**: Drives a TowerPro SG90 micro servo using hardware TIM1 PWM (50 Hz) between 0° (Closed) and 90° (Open).
- **Serial Telemetry (USART1)**: Streams live sensor metrics and decision states to a PC serial monitor at 115200 baud.

---

## 🛠️ Hardware Components

| Component | Description |
| :--- | :--- |
| **Microcontroller** | STM32F103C8T6 ("Blue Pill") |
| **Programmer / Debugger** | ST-Link V2 (SWD interface) |
| **Soil Moisture Sensor** | YL-69 probe + LM393 module (Analog Out) |
| **Raindrop Sensor** | MH-RD module (Analog Out) |
| **Temperature & Humidity** | DHT11 digital module |
| **Actuator** | TowerPro SG90 9g Micro Servo Motor |
| **Power Supply** | External 5V DC supply for servo, 3.3V from STM32 for sensors |

---

## 🔌 Pin Connections

| Component Pin | STM32 Pin | Function / Peripheral | Voltage Level |
| :--- | :--- | :--- | :--- |
| **ST-Link SWDIO** | `PA13` | `SYS_JTMS-SWDIO` | 3.3V |
| **ST-Link SWCLK** | `PA14` | `SYS_JTCK-SWCLK` | 3.3V |
| **Soil Moisture A0** | `PA0` | `ADC1_IN0` | 3.3V (from STM32) |
| **Raindrop Sensor A0** | `PA1` | `ADC1_IN1` | 3.3V (from STM32) |
| **DHT11 Data Pin** | `PA2` | `GPIO_Output / Input` (`DHT11_DATA`) | 3.3V (from STM32) |
| **SG90 Servo Signal** | `PA8` | `TIM1_CH1` (PWM 50 Hz) | 3.3V Logic |
| **SG90 Servo VCC** | — | **External 5V (+)** | 5V DC |
| **SG90 Servo GND** | — | **Common GND** (Tied to STM32 GND) | 0V |
| **UART TX (Telemetry)** | `PA9` | `USART1_TX` (115200 baud) | 3.3V |

> [!WARNING]
> Both the Soil Moisture and Raindrop sensor comparator boards **must be powered from 3.3V** to prevent exceeding the STM32 ADC's maximum input voltage. The SG90 servo motor **must be powered from an external 5V supply** sharing a common ground with the STM32.

---

## 📂 Project Structure

```
├── Core
│   ├── Inc
│   │   ├── dht11.h        # DHT11 driver header & DWT delay
│   │   ├── sensors.h      # Analog sensor (soil & rain) driver header
│   │   ├── servo.h        # SG90 PWM valve controller header
│   │   └── main.h
│   └── Src
│       ├── dht11.c        # DHT11 bit-bang implementation & checksum
│       ├── sensors.c      # ADC multichannel sampling & moisture mapping
│       ├── servo.c        # TIM1 PWM generation (0°–90°)
│       └── main.c         # Irrigation state machine & control logic
├── Drivers
│   ├── CMSIS
│   └── STM32F1xx_HAL_Driver
└── Microcontroller-1.ioc  # STM32CubeMX configuration file
```

---

## 🚀 Getting Started

1. **Clone the repository**:
   ```bash
   git clone https://github.com/adi45thya/Microcontroller.git
   ```
2. **Open in STM32CubeIDE**:
   - Go to `File` → `Open Projects from File System...`
   - Select the cloned project directory and click `Finish`.
3. **Build the Project**:
   - Press `Ctrl + B` (or click the Hammer icon).
4. **Flash to Target**:
   - Connect your ST-Link V2 to the Blue Pill SWD pins (`SWCLK`, `SWDIO`, `GND`, `3.3V`).
   - Click `Run` (Ctrl + F11).
5. **View Serial Telemetry**:
   - Connect a USB-to-UART module to `PA9` (TX) and `GND` at **115200 baud** to view live telemetry.
