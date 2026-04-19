# 🌱 ESP32 Growtent

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE)
[![Arduino](https://img.shields.io/badge/Arduino-ESP32-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Matter](https://img.shields.io/badge/Matter-Smart%20Home-4B8BBE?logo=homebridge&logoColor=white)](https://csa-iot.org/csa_iot_wp/what-is-matter/)
[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)](https://cplusplus.com/)

> Smart home grow tent controller with **Matter** support — control LED brightness and fan speed via Apple Home, Google Home, or any Matter-compatible hub.

## ✨ Features

- 🏠 **Matter Integration** — Full Matter protocol support for seamless smart home integration.
- 💡 **Dimmable LED Light** — PWM-controlled LED with smooth brightness adjustment (0-255).
- 🌀 **Variable Speed Fan** — Three-speed fan control (LOW, MEDIUM, HIGH) or continuous speed mode.
- 🔌 **WiFi + Matter** — Connect via WiFi and Matter simultaneously.
- 💾 **Persistent State** — Last state saved and restored on power cycle.
- 🔐 **Secure Credentials** — WiFi SSID/password managed via `.env` file (not hardcoded).
- 🎛️ **Button Control** — Long-press button to decommission device.

## 🛠️ Hardware

- **Board**: XIAO ESP32-C6 (or compatible ESP32 variant)
- **LED**: PWM-controlled via GPIO 18 (250 Hz, 8-bit resolution)
- **Fan**: PWM-controlled via GPIO 19 (3000 Hz, 8-bit resolution)
- **Button**: Boot pin for decommissioning (5-second hold)

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/temmiland/esp32-growtent.git
cd esp32-growtent
```

### 2. Configure WiFi credentials

```bash
nano .env  # Edit with your WiFi SSID and password
```

### 3. Build and upload

```bash
./build_and_upload.sh
```

The script will:
- Load WiFi credentials from `.env`
- Compile the sketch using Arduino CLI
- Prompt you to confirm upload
- Flash the XIAO ESP32-C6

## 📦 Build Cache

Subsequent builds use cached compiled files for faster compilation:

```bash
./build_and_upload.sh  # First build: ~2 min, subsequent: ~30 sec
```

Clear cache if needed:

```bash
rm -rf ~/.cache/Arduino
```

## 🔧 Project Structure

```
├── esp32-growtent.ino   # Main entry point (setup/loop)
├── settings.ino                   # WiFi & hardware configuration
├── hardware.ino                   # LED/Fan PWM control, button handling
├── wifi.ino                        # WiFi initialization
├── matter.ino                      # Matter protocol & commissioning
├── matter_light.ino               # Dimmable light endpoint
├── matter_fan.ino                 # Variable speed fan endpoint
├── config.h                        # Global declarations
├── .env                            # WiFi credentials (not in repo)
└── build_and_upload.sh            # Build & upload script
```

## ⚙️ Configuration

Edit `.env` to set WiFi credentials:

```bash
WIFI_SSID="YourNetwork"
WIFI_PASSWORD="YourPassword"
```

Modify GPIO pins in `settings.ino`:

```cpp
const uint8_t ledPin = 18;   // LED PWM pin
const uint8_t fanPin = 19;   // Fan PWM pin
const uint8_t buttonPin = BOOT_PIN;  // Decommission button
```

## 🔌 Commissioning

1. Power on the device
2. Look for **"Light Matter Accessory"** in your Matter app (Apple Home, Google Home, etc.)
3. Scan the QR code or enter the pairing code shown in the serial console
4. Add to your Matter hub

To decommission: **Hold the boot button for 5 seconds**

## 🐛 Debugging

### Serial Monitor

View real-time serial output (auto-detects ESP32 across platforms):

```bash
./serial_monitor.sh
```

The script automatically finds your ESP32 on:
- **macOS**: `/dev/cu.usbmodem*`, `/dev/cu.SLAB_USBtoUART*`, etc.
- **Linux**: `/dev/ttyUSB*`, `/dev/ttyACM*`
- **Windows** (WSL/Git Bash): `COM*` ports

Or specify port manually:

```bash
./serial_monitor.sh /dev/cu.usbmodem101
```

### Verbose Compile Output

Enable verbose output during build:

```bash
./build_and_upload.sh  # Already enabled via `-v` flag
```

## 📝 Known Limitations

- OTA (Over-The-Air) updates not yet implemented
- Fan speed sensor not implemented
- Matter only supports IP commissioning (no BLE pairing code display)

## 🤝 Contributing

Pull requests welcome! For significant changes, please open an issue first to discuss.

## 📄 License

Licensed under **MIT License**. See [LICENSE](./LICENSE) for details.

---

Built with ❤️ using [Arduino CLI](https://arduino.cc/pro/cli) and the [Matter SDK](https://csa-iot.org/)
