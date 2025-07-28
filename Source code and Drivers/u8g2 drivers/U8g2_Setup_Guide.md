## ✅ How to Add U8g2 OLED Display Driver to STM32 Project

This guide explains how to integrate the **U8g2 graphics library** (for SH1106 or SSD1306 OLED displays) into an STM32 project using STM32CubeIDE.

---

### 📁 1. File Placement

Organize the U8g2 files as follows in your STM32 project directory:

| U8g2 Component | Copy To STM32 Project Folder |
| -------------- | ---------------------------- |
| `u8g2.h`       | `/Inc/`                      |
| `u8x8.h`       | `/Inc/`                      |
| `csrc/`        | `/Drivers/u8g2/csrc/`        |

> 💡 You can create a folder like:
> `/Drivers/u8g2/` and place the `csrc` folder inside it.

---

### ⚙️ 2. Include Paths Configuration

To let the compiler find the U8g2 header files:

#### Steps:

1. Right-click your STM32 project in **Project Explorer**.
2. Select **Properties**.
3. Navigate to: `C/C++ General → Paths and Symbols → Includes`.
4. Under **Languages**, select **GNU C**.
5. Click **Add...** and add:

   * `Drivers/u8g2/csrc`
   * `Inc`
6. Repeat the above for **GNU C++** if needed.

---

### 🔗 3. (Optional) GCC Linker Flags

Usually no extra flags are needed, but for float printing support (in U8g2 callbacks), add:

Go to:

* `Project Properties → C/C++ Build → Settings → Tool Settings`
* `MCU GCC Linker → Miscellaneous`
* Add the following to **Linker Flags**:

```
-u _printf_float
```

---

### 💬 4. Initialize U8g2 in Code

In your `main.c` or a display module:

```c
#include "u8g2.h"
#include "u8x8.h"
```

Then create and initialize the display (example for SH1106 via I2C):

```c
u8g2_t u8g2;
u8g2_Setup_sh1106_i2c_128x64_noname_f(
    &u8g2,
    U8G2_R0,
    u8x8_byte_hw_i2c,
    u8x8_gpio_and_delay_stm32
);
u8g2_InitDisplay(&u8g2);
u8g2_SetPowerSave(&u8g2, 0);
```

> 📌 Ensure you’ve configured your I2C peripheral (e.g., `I2C1`) using STM32CubeMX.

---

### ✅ Done!

Your U8g2 OLED display library is now integrated with your STM32 project.

Let me know if you want to generate example FreeRTOS tasks or provide `.ioc` snippets for display setup.
