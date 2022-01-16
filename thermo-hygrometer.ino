// ref. https://101010.fun/iot/arduino-dht11.html
#include "DHT.h"
# define DATA_PIN 4 // D2
DHT dht(DATA_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(3000);
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(humid) || isnan(temp)) {
    Serial.println("Failed...");
    return;
  }

  char humidFloatString[10];
  char tempFloatString[10];
  dtostrf(humid, 4, 2, humidFloatString);
  dtostrf(temp, 4, 2, tempFloatString);

  char bufHumid[20];
  char bufTemp[20];

  sprintf(bufHumid, "Humidity: %s", humidFloatString);
  sprintf(bufTemp, "Temperature: %s", tempFloatString);

  Serial.println(bufHumid);
  Serial.println(bufTemp);

}
