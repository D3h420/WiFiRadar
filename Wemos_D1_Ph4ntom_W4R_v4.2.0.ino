#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Placeholder for full-screen 128x64 bitmap logo
const unsigned char wifi_logo [] PROGMEM = {
  // INSERT YOUR 128x64 BITMAP HERE
  // Generate using tools like Image2CPP and replace this placeholder
  // Expected size: 128x64 pixels, 1024 bytes (128 * 64 / 8)
};

// Funkcja do przeliczania RSSI na odległość (w metrach)
float rssiToDistance(int rssi) {
  return pow(10, (27.55 - (20 * log10(2400)) + abs(rssi)) / 20.0);
}

// Struktura do przechowywania informacji o sieci Wi-Fi
struct WiFiInfo {
  String ssid;
  int rssi;
  float distance;
};

WiFiInfo networks[6];  // Tablica na maksymalnie 6 sieci (pasuje do ekranu 128x64 przy 10 pikselach na linię)

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Inicjalizacja OLED
  Wire.begin(D2, D1);  // Twoje piny I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Błąd inicjalizacji OLED!");
    while (true);
  }

  // Wyświetlanie logo
  display.clearDisplay();
  display.drawBitmap(0, 0, wifi_logo, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(3000);  // Logo wyświetlane przez 3 sekundy
  display.clearDisplay();

  // Inicjalizacja Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void scanAndDisplay() {
  display.clearDisplay();
  
  // Skanowanie sieci Wi-Fi
  Serial.println("Skanowanie dostępnych sieci Wi-Fi...");
  int networksFound = WiFi.scanNetworks();

  if (networksFound == 0) {
    Serial.println("❌ Brak sieci Wi-Fi w pobliżu");
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Brak sieci Wi-Fi");
    display.display();
  } else {
    // Zapisujemy sieci
    for (int i = 0; i < networksFound && i < 6; i++) {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      float distance = rssiToDistance(rssi);

      networks[i].ssid = ssid;
      networks[i].rssi = rssi;
      networks[i].distance = distance;
    }

    // Sortowanie sieci po odległości
    for (int i = 0; i < networksFound - 1 && i < 5; i++) {
      for (int j = i + 1; j < networksFound && j < 6; j++) {
        if (networks[i].distance > networks[j].distance) {
          WiFiInfo temp = networks[i];
          networks[i] = networks[j];
          networks[j] = temp;
        }
      }
    }

    // Wyświetlanie wyników
    display.setTextSize(1);
    display.setCursor(0, 0);

    // Wyświetlamy każdą sieć w jednej linijce
    for (int i = 0; i < networksFound && i < 6; i++) {
      display.setCursor(0, i * 10);
      display.print(networks[i].ssid);
      display.print(" ");
      display.print(networks[i].distance, 1);
      display.println("m");
    }
    display.display();
  }
}

void loop() {
  scanAndDisplay();
  delay(10000);  // Odświeżanie co 10 sekund
}