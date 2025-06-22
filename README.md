# WiFiRadar

## Overview
WiFiRadar is a simple WiFi scanning tool built for the ESP8266 microcontroller. It scans nearby WiFi networks, measures their signal strength, and converts it to an estimated distance in meters. Results are displayed on a 0.96" OLED screen or via serial monitor. 

## Features
- **Sniff**: Detects nearby 2.4GHz WiFi networks.
- **Screen output**: Displays SSID and estimated distance via serial monitor.

## Preview


## Example Output
```
WiFiRadar Scan Results:
 HomeWiFi, Distance: ~2.5 m
 GuestNetwork, Distance: ~10.3 m
 CoffeeShop, Distance: ~15.7 m
```
## Hardware Connections
Connect the SDA and SCL pins accordingly to Your board
In my example it is Wemos D1 Mini

Display - Board
VCC → 3.3V
GND → GND
SCL → D1 (GPIO 5)
SDA → D2 (GPIO 4)

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/D3h420/WiFiRadar.git
   ```
2. Open the project in Arduino IDE or PlatformIO.
3. Connect your ESP8266 board to your computer.
   Needed libraries:
    -ESP8266WiFi
    -Wire (for I2C communication)
    -Adafruit_GFX and Adafruit_SSD1306 (for OLED display)
4. Upload the code to the board using the IDE.

## License && Contributing
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
Fork the repo, make changes, and submit pull requests

## 🐲 Author

**/D3h420/**  
GitHub: [D3h420](https://github.com/D3h420)

