/*
 * thermo-hygrometer.ino
 *
 * This example shows how to:
 * 1. define a temperature sensor accessory and its characteristics (in my_accessory.c).
 * 2. report the sensor value to HomeKit (just random value here, you need to change it to your real sensor value).
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 *	Modified on: 2022-01-19
 *		Modifier: ywake
 *
 * Note:
 *
 * You are recommended to read the Apple's HAP doc before using this library.
 * https://developer.apple.com/support/homekit-accessory-protocol/
 *
 * This HomeKit library is mostly written in C,
 * you can define your accessory/service/characteristic in a .c file,
 * since the library provides convenient Macro (C only, CPP can not compile) to do this.
 * But it is possible to do this in .cpp or .ino (just not so conveniently), do it yourself if you like.
 * Check out homekit/characteristics.h and use the Macro provided to define your accessory.
 *
 * Generally, the Arduino libraries (e.g. sensors, ws2812) are written in cpp,
 * you can include and use them in a .ino or a .cpp file (but can NOT in .c).
 * A .ino is a .cpp indeed.
 *
 * You can define some variables in a .c file, e.g. int my_value = 1;,
 * and you can access this variable in a .ino or a .cpp by writing extern "C" int my_value;.
 *
 * So, if you want use this HomeKit library and other Arduino Libraries together,
 * 1. define your HomeKit accessory/service/characteristic in a .c file
 * 2. in your .ino, include some Arduino Libraries and you can use them normally
 *                  write extern "C" homekit_characteristic_t xxxx; to access the characteristic defined in your .c file
 *                  write your logic code (eg. read sensors) and
 *                  report your data by writing your_characteristic.value.xxxx_value = some_data; homekit_characteristic_notify(..., ...)
 * done.
 */
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <DHT.h>
#include "wifi_connect.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

#define DATA_PIN 4 // D2

DHT dht(DATA_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
  wifi_connect();
  my_homekit_setup();
}

void loop() {
  delay(1000);
  // float humid = dht.readHumidity();
  // float temp = dht.readTemperature();

  // if (isnan(humid) || isnan(temp)) {
  //   Serial.println("Failed...");
  //   return;
  // }

  // char humidFloatString[10];
  // char tempFloatString[10];
  // dtostrf(humid, 4, 2, humidFloatString);
  // dtostrf(temp, 4, 2, tempFloatString);

  // char bufHumid[20];
  // char bufTemp[20];

  // sprintf(bufHumid, "Humidity: %s", humidFloatString);
  // sprintf(bufTemp, "Temperature: %s", tempFloatString);

  // Serial.println(bufHumid);
  // Serial.println(bufTemp);

  my_homekit_loop();
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t g_config;
extern "C" homekit_characteristic_t g_cha_current_temperature;
extern "C" homekit_characteristic_t g_cha_humidity;

static uint32_t g_next_heap_millis = 0;
static uint32_t g_next_report_millis = 0;

void my_homekit_setup() {
  arduino_homekit_setup(&g_config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t now = millis();
  if (now > g_next_report_millis) {
    // report sensor value every 10 seconds
    g_next_report_millis = now + 10 * 1000;
    my_homekit_report();
  }
  if (now > g_next_heap_millis) {
    // show heap info every 5 secounds
    g_next_heap_millis = now + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
      ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}

void my_homekit_report() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  LOG_D("%.1f °C, %.1f %%", temperature, humidity);
  g_cha_current_temperature.value.float_value = temperature;
  homekit_characteristic_notify(
    &g_cha_current_temperature, g_cha_current_temperature.value);
  g_cha_humidity.value.float_value = humidity;
  homekit_characteristic_notify(&g_cha_humidity, g_cha_humidity.value);
}
