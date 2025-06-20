#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>  // Biblioteka do Wi-Fi dla Wemos D1 mini

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

WiFiInfo networks[20];  // Tablica na maksymalnie 20 sieci

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Inicjalizacja OLED
  Wire.begin(D2, D1);  // Twoje piny I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Błąd inicjalizacji OLED!");
    while (true);
  }

  display.clearDisplay();
  
  // Intro
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  String introText = "/D3h420/";
  display.setCursor(0, 0);  // Zaczynamy od początku ekranu
  display.println(introText);

  // Mniejszy tekst - "WiFi scanner"
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 10 * 8) / 2, 20);  // Wyśrodkowanie tekstu
  display.println("Ph4ntom W4R");

  // Dodanie linii na dole: "/niewidzialny wpierdol/"
  String bottomText = "/niewidzialny wpierdol/";
  int bottomTextWidth = bottomText.length() * 6;  // Szerokość tekstu
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - bottomTextWidth) / 2, SCREEN_HEIGHT - 10);  // Wyśrodkowanie na dole
  display.println(bottomText);

  display.display();
  delay(3000);  // Intro trwa 3 sekundy
  display.clearDisplay();

  // Skanowanie sieci Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Rozłączenie, aby rozpocząć skanowanie

  Serial.println("Skanowanie dostępnych sieci Wi-Fi...");
  int networksFound = WiFi.scanNetworks();

  if (networksFound == 0) {
    Serial.println("❌ Brak sieci Wi-Fi w pobliżu");
    display.setCursor(0, 0);
    display.println("Brak sieci Wi-Fi");
    display.display();
  } else {
    for (int i = 0; i < networksFound; i++) {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      float distance = rssiToDistance(rssi);

      // Zapisujemy sieć w tablicy
      networks[i].ssid = ssid;
      networks[i].rssi = rssi;
      networks[i].distance = distance;
    }

    // Sortowanie sieci po odległości (najbliższa pierwsza)
    for (int i = 0; i < networksFound - 1; i++) {
      for (int j = i + 1; j < networksFound; j++) {
        if (networks[i].distance > networks[j].distance) {
          // Zamiana miejscami
          WiFiInfo temp = networks[i];
          networks[i] = networks[j];
          networks[j] = temp;
        }
      }
    }

    // Wyświetlanie wyników
    display.setTextSize(1);
    display.setCursor(0, 0);

    // Pierwsza linia z napisem "Wi Fi"
    display.setTextSize(1);
    display.setCursor((SCREEN_WIDTH - 16 * 8) / 2, 0);  // Wyśrodkowanie
    display.println("------/WIFI/------");

    // Wyświetlamy każdą sieć w jednej linijce z nazwą, RSSI i dystansem
    for (int i = 0; i < networksFound; i++) {
      display.setTextSize(1);  // Mała czcionka
      display.setCursor(0, (i + 1) * 10);  // Wysokość dla każdej sieci
      
      // Wyświetlanie nazwy sieci i dystansu
      display.print(networks[i].ssid);
      display.print(" ");
      display.print(networks[i].distance, 1);  // Wyświetlamy dystans z 1 miejscem po przecinku
      display.println("m");

      display.display();
      delay(3000);  // Pauza 3 sekundy na każdą sieć
    }
  }
}

void loop() {
  // Pętla nie jest potrzebna w tym przypadku
}