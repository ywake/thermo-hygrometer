/*
 * wifi_connect.h
 *
 *	Created on: 2020-05-15
 *		Author: Mixiaoxiao (Wang Bin)
 *	Modified on: 2022-01-19
 *		Modifier: ywake
 */

#ifndef WIFI_CONNECT_H
# define WIFI_CONNECT_H

# if defined(ESP8266)
#  include <ESP8266WiFi.h>
# elif defined(ESP32)
#  include <WiFi.h>
# endif

# include "wifi_info.h"

void	wifi_connect(void)
{
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoReconnect(true);
	WiFi.begin(g_ssid, g_password);
	Serial.println("WiFi connecting...");
	while (!WiFi.isConnected())
	{
		delay(100);
		Serial.print(".");
	}
	Serial.print("\n");
	Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
}

#endif
