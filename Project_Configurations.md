# 📘 STM32 RTOS-Based Multi-Sensor Data Logger  
This project uses **STM32F767ZI** MCU with **FreeRTOS**, interfacing a **DHT11 Temperature & Humidity Sensor**, a **Capacitive Soil Moisture Sensor**, and a **SH1106 OLED Display (I2C)**. 
UART output is provided via Virtual COM (USART3) for serial debugging.
---

## 🛠️ STM32CubeMX Configuration Summary

### 🔧 System Core & Clock

| Category         | Setting                | Value                     | Purpose                                                                 |
|------------------|------------------------|----------------------------|-------------------------------------------------------------------------|
| RCC              | High Speed Clock (HSE) | Crystal/Ceramic Resonator | Provides stable external clock for precise timing.                     |
| System Clock Mux |                        | PLLCLK                     | Uses PLL to boost frequency for system clock.                          |
| HCLK Frequency   |                        | 216 MHz                    | Maximum clock frequency of STM32F767ZI.                                |
| SYS              | Debug                  | Serial Wire                | Enables SWD debugging via ST-LINK.                                     |
| Timebase Source  |                        | TIM6                       | Uses TIM6 instead of SysTick for FreeRTOS timing (to avoid conflicts). |

---

### ⚙️ Peripherals Configuration

| Peripheral | Setting           | Value          | Purpose                                                                 |
|------------|-------------------|----------------|-------------------------------------------------------------------------|
| ADC1       | Channel           | IN0 (PA0)      | Reads analog data from the soil moisture sensor.                        |
| I2C1       | Mode              | I2C            | Communicates with SH1106 OLED over PB8/PB9.                             |
| TIM7       | Activation        | Enabled        | Used by DHT11 driver for precise bit-level timing.                      |
| TIM7       | Prescaler (PSC)   | 215            | Configured for 1 µs delay (`delay_us` function).                        |
| USART3     | Mode              | Asynchronous   | Transmits sensor logs to PC terminal via Virtual COM (USB).             |

---

### 🧵 FreeRTOS Middleware Setup

| Component | Name             | Parameters                            | Description                                                              |
|-----------|------------------|----------------------------------------|-------------------------------------------------------------------------|
| Task      | `sensorTask`     | Priority: Normal, Stack: 256 words     | Periodically reads DHT11 + soil sensors and sends data to a queue.      |
| Task      | `displayTask`    | Priority: BelowNormal, Stack: 512 words| Waits on queue, updates OLED display with sensor readings.              |
| Queue     | `sensorDataQueue`| Size: 5 items, 8 bytes each            | Buffers data from `sensorTask` to `displayTask`.                        |
| Mutex     | `i2cMutex`       | Type: Mutex                           | Prevents I2C contention between display and any other peripherals.       |

---

### 🔌 GPIO Pin Mapping

| STM32 Pin | Function       | Mode / Config          | Connected To                         |
|-----------|----------------|------------------------|--------------------------------------|
| **PA0**   | ADC1_IN0       | Analog                 | Soil Moisture Sensor (AOUT)          |
| **PC7**   | GPIO Output    | Push-Pull Output       | DHT11 Sensor (DATA)                  |
| **PB8**   | I2C1_SCL       | AF, Open-Drain         | OLED Display SCL (Clock)             |
| **PB9**   | I2C1_SDA       | AF, Open-Drain         | OLED Display SDA (Data)              |
| **PA9**   | USART3_TX      | AF, Push-Pull          | UART TX (ST-LINK VCP output)         |
| **PA10**  | USART3_RX      | AF, Push-Pull          | UART RX (ST-LINK VCP input)          |

---

📎 **Note:** This configuration is designed to be modular, thread-safe (via FreeRTOS), and optimized for real-time sensor logging and display with minimal resource usage.
