#include <TFT_eSPI.h>
#include <DHT.h>
#include <Adafruit_VEML7700.h>

#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 27
#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
TFT_eSPI tft = TFT_eSPI();
Adafruit_VEML7700 veml = Adafruit_VEML7700();

const int BUTTON_1 = 32;
const int BUTTON_2 = 33;

bool lastButton1State = HIGH;
bool lastButton2State = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  dht.begin();
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Started", tft.width() / 2, tft.height() / 2);

  if (!veml.begin()) {
    Serial.println("no sensor nooo");
    while (1) {
      delay(10);
    }
  }
}

void loop() {
  bool currentButton1State = digitalRead(BUTTON_1);
  bool currentButton2State = digitalRead(BUTTON_2);

  if (lastButton1State == HIGH && currentButton1State == LOW) {
    float t = dht.readTemperature();
    
    tft.fillScreen(TFT_BLACK);

    if (isnan(t)) {
      Serial.println("noooo failed to read dht sensor nooo");
      tft.drawString("sensor error", tft.width() / 2, tft.height() / 2);
    } else {
      String tempString = "Temp: " + String(t, 1) + " C";
      tft.drawString(tempString, tft.width() / 2, tft.height() / 2);
    }
    
    delay(50);
  }

  if (lastButton2State == HIGH && currentButton2State == LOW) {
    float lux = veml.readLux();
    
    tft.fillScreen(TFT_BLACK);
    String luxString = "Lux: " + String(lux, 1);
    tft.drawString(luxString, tft.width() / 2, tft.height() / 2);
    delay(50);
  }

  lastButton1State = currentButton1State;
  lastButton2State = currentButton2State;
}
