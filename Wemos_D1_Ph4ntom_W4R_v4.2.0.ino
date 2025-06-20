#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED podłączony do Wemos D1 Mini:
// - SDA: D2 (GPIO 4)
// - SCL: D1 (GPIO 5)
// - VCC: 3.3V lub 5V (zależnie od modułu OLED)
// - GND: GND

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
  Serial.println("Inicjalizacja...");

  // Inicjalizacja OLED
  Wire.begin(D2, D1);  // I2C: D2 (SDA), D1 (SCL)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Błąd inicjalizacji OLED!");
    while (true);
  }

  // Wyświetlanie logo
  display.clearDisplay();
  display.drawBitmap(0, 0, wifi_logo, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();
  Serial.println("Wyświetlono logo intro");
  delay(3000);  // Logo wyświetlane przez 3 sekundy

  // Test wyświetlacza
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Test OLED");
  display.display();
  Serial.println("Wyświetlono komunikat testowy OLED");
  delay(2000);  // Test widoczny przez 2 sekundy
  display.clearDisplay();
  display.display();

  // Inicjalizacja Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("Wi-Fi zainicjalizowane");
}

void scanAndDisplay() {
  display.clearDisplay();
  Serial.println("Rozpoczynanie skanowania sieci Wi-Fi...");

  // Skanowanie sieci Wi-Fi
  int networksFound = WiFi.scanNetworks();
  Serial.print("Znaleziono sieci: ");
  Serial.println(networksFound);

  if (networksFound <= 0) {
    Serial.println("❌ Brak sieci Wi-Fi w pobliżu lub błąd skanowania");
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Brak sieci Wi-Fi");
    display.display();
    Serial.println("Wyświetlono komunikat o braku sieci");
  } else {
    // Zapisujemy sieci
    for (int i = 0; i < networksFound && i < 6; i++) {
      networks[i].ssid = WiFi.SSID(i);
      networks[i].rssi = WiFi.RSSI(i);
      networks[i].distance = rssiToDistance(networks[i].rssi);
      Serial.print("Sieć ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(networks[i].ssid);
      Serial.print(", RSSI: ");
      Serial.print(networks[i].rssi);
      Serial.print(", Odległość: ");
      Serial.print(networks[i].distance, 1);
      Serial.println("m");
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
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    for (int i = 0; i < networksFound && i < 6; i++) {
      display.setCursor(0, i * 10);
      // Ograniczamy SSID do 12 znaków, aby zmieścić się na ekranie
      String displaySSID = networks[i].ssid.substring(0, 12);
      display.print(displaySSID);
      display.print(" ");
      display.print(networks[i].distance, 1);
      display.println("m");
      Serial.print("Wyświetlam sieć ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(displaySSID);
    }
    display.display();
    Serial.println("Wyświetlono listę sieci");
  }
}

void loop() {
  scanAndDisplay();
  delay(10000);  // Odświeżanie co 10 sekund
}