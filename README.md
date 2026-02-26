# ESP32 Jammer - Single or Dual Antennas (nrf24l01)

> [!WARNING]
> This code was built using Artificial Intelligence and by a person who has no knowledge in the area...
> There may possibly be inconsistencies in the code that may not be perceived by someone who has no knowledge in the area (me), such as poorly commented or redundant code snippets. If you notice this, feel free to send a PR or report an Issue to the project :)

> [!NOTE]
> This project exists due to an external request from an acquaintance of mine. Therefore, there will probably be no changes to the project, as I have already "fulfilled" this request.
> The reason I made this code was because of an error when buying a different ESP32 model than another code required.

> [!IMPORTANT]
> It is crucial to understand that this device, operating in the 2.4 GHz band, does not exclusively interfere with Bluetooth signals. The 2.4 GHz band is widely used by a variety of wireless technologies, such as: WI-FI, Bluetooth, and other devices like cordless phones, baby monitors, drones, and IoT devices. Its use is the sole responsibility of the user. I am not responsible for any damages, interferences, losses, or consequences resulting from the use of this system.

## Web Flasher (Quick Installation)

The easiest and fastest way to install the firmware on your ESP32 is by using the **Web Flasher** directly in your browser.

**Requirements:**
1. A browser compatible with the Web Serial API (Chrome, Edge, Opera).
2. Your ESP32 connected to the computer via USB cable.

**Instructions:**
1. **Access the Web Flasher:** [Click Here to Access the Web Flasher](https://newbox22.github.io/jammer-esp32/flasher/webflasher.html)
2. Click the **"Connect to Serial Port"** button and select your ESP32's serial port.
3. Click the **"Flash Firmware"** button and wait for the process to complete.

---

## Wiring Diagram

The circuit uses an ESP32 and two nRF24L01 modules, each with its respective decoupling capacitor to ensure power stability.

**Wokwi Project Link:** [Click Here!](https://wokwi.com/projects/450785045943583745)

The following image illustrates the complete wiring:

![ESP32 Jammer Wiring Diagram](https://github.com/NeWBoX22/jammer-esp32/blob/main/image.png)

**Attention:** In the diagram, the nRF24L01 modules and the ESP32 are represented from the rear view (where the contact pins are visible) to facilitate the visualization of connections. Make sure to connect the wires to the correct pins in the physical assembly.

## Physical Mapping of Modules

To ensure that the Wokwi diagram wiring corresponds to the code logic (`main.cpp`), the physical position of the nRF24L01 modules is important:

| Position in Diagram | Control Pins (CE/CSN) | Module in Code |
| :--- | :--- | :--- |
| **Left Module** | `GPIO16` / `GPIO15` | **Module 1** |
| **Right Module** | `GPIO4` / `GPIO2` | **Module 2** |

**Note:** If you want to invert the physical position of the modules, you must adjust the pin definitions in the `src/main.cpp` file so that Module 1 (code) corresponds to the module you want to be the main one.


## Pin Connections

The connections between the ESP32 and the two nRF24L01 modules are identical, sharing the SPI pins (SCK, MOSI, MISO) and using separate GPIO pins for Chip Enable (CE) and Chip Select Not (CSN).

### 1. Power and Decoupling (Capacitors)

It is crucial to use a decoupling capacitor (typically 10uF to 100uF) in parallel with the power supply of each nRF24L01 module to handle current peaks during transmission.

| Component | Pin | Connection |
| :--- | :--- | :--- |
| **ESP32** | `3V3` | VCC (3.3V Power Supply) |
| **ESP32** | `GND` | GND (Ground) |
| **Capacitor Breakout** | `VCC` | nRF24L01 `VCC` |
| **Capacitor Breakout** | `GND` | nRF24L01 `GND` |

### 2. nRF24L01 Module #1 (Chip 1)

This module uses a set of GPIO pins for control, sharing the standard ESP32 SPI pins.

| nRF24L01 Pin | ESP32 Pin (GPIO) | Function |
| :--- | :--- | :--- |
| `VCC` | `3V3` | 3.3V Power Supply |
| `GND` | `GND` | Ground |
| `CE` | `GPIO16` (RX2) | Chip Enable |
| `CSN` | `GPIO15` (D15) | Chip Select Not |
| `SCK` | `GPIO14` (D14) | Serial Clock (SPI) |
| `MOSI` | `GPIO13` (D13) | Master Out Slave In (SPI) |
| `MISO` | `GPIO12` (D12) | Master In Slave Out (SPI) |
| `IRQ` | **Not Connected** | Interrupt |

### 3. nRF24L01 Module #2 (Chip 2)

This module shares the SPI pins (SCK, MOSI, MISO) with Module #1, but uses a different set of GPIO pins for CE and CSN.

| nRF24L01 Pin | ESP32 Pin (GPIO) | Function |
| :--- | :--- | :--- |
| `VCC` | `3V3` | 3.3V Power Supply |
| `GND` | `GND` | Ground |
| `CE` | `GPIO4` (D4) | Chip Enable |
| `CSN` | `GPIO2` (D2) | Chip Select Not |
| `SCK` | `GPIO14` (D14) | Serial Clock (SPI) |
| `MOSI` | `GPIO13` (D13) | Master Out Slave In (SPI) |
| `MISO` | `GPIO12` (D12) | Master In Slave Out (SPI) |
| `IRQ` | **Not Connected** | Interrupt |
