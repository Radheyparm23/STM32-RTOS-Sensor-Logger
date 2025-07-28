# STM32 FreeRTOS Multi-Sensor Data Logger

> *(Action: Replace the URL below with a link to a GIF or a high-quality photo of your final working project!)*

![Project Demo](https://your-image-url-here) <!-- Replace this URL -->

---

This repository contains the full source code and documentation for a real-time, multi-sensor environmental data logger built on an **STM32F767ZI Nucleo** board using the **FreeRTOS** operating system.

The system concurrently reads data from a **DHT11** (temperature/humidity) and a **capacitive soil moisture sensor**, processes the data for accuracy, and displays it on a **graphical OLED screen**. This project is a comprehensive example of a real-world embedded system, covering hardware interfacing, custom driver development, RTOS architecture, and a polished user interface.

---

## 🔑 Key Features

- **Real-Time & Multitasking**  
  Built on FreeRTOS with a pre-emptive scheduler, featuring two concurrent tasks for sensing and display that communicate safely using a **message queue** and **mutex**.

- **Robust Sensor Drivers**  
  Includes a custom, RTOS-aware driver for the DHT11 with a pre-scheduler "cranking" routine to handle hardware instability and a context-aware critical section to protect timing from RTOS interrupts.

- **Advanced Data Filtering**  
  Uses **ADC oversampling** to reduce noise from the analog soil sensor and applies a **linear mapping function** with user-calibrated values for accurate percentage readings.

- **Graphical User Interface (GUI)**  
  A clean, professional UI on an SH1106 OLED display using the **U8g2 library**, featuring a **startup splash screen** and **graphical progress bars**.

- **System Status Indicators**  
  Utilizes the onboard LEDs to provide at-a-glance feedback: a **blinking/solid blue LED** for the DHT11's startup status and a **green LED** that serves as a "soil is dry" warning.

---

## 🔧 Hardware Components

| Component           | Model                                  |
|---------------------|----------------------------------------|
| Microcontroller     | STM32F767ZI Nucleo-144                 |
| Temperature Sensor  | DHT11 Sensor Module                    |
| Soil Moisture       | Capacitive Soil Moisture Sensor v1.2   |
| Display             | 1.3" SH1106 I2C OLED                   |

---

## 💻 Software & Tools

- **IDE:** STM32CubeIDE  
- **OS:** FreeRTOS (CMSIS-RTOS v2 API)  
- **Libraries:** STM32 HAL, U8g2 Graphics Library

---

## ⚙️ How It Works

The system architecture is designed around two main tasks:

- **`sensorTask`**  
  This task runs periodically every two seconds. It reads the raw sensor values, applies **oversampling** and **calibration logic**, and then sends the final, processed data to a **message queue**.

- **`displayTask`**  
  This task is **event-driven**. It waits for data to arrive in the message queue, and upon receiving it, it takes control of the I2C bus via a **mutex** to update the **OLED display** with the new information.

This **decoupled design** ensures that the sensor reading and display updates operate independently, creating a **responsive and stable system**.
