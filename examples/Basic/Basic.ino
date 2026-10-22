
/*
 Name:		Basic.ino
 Created:	10/20/2026 7:39:39 AM
 Author:	Daen
*/

#include <Create_Lib_Use_Test.h>
#include <WiFiProv.h>
#include <WiFi.h>
#include "nvs_flash.h"

TestWifiProv Prov;

void setup() {
	Serial.begin(115200);
	waitForSerial(5000);
	//Prov.eraseNVS();//erase credentials and etcetera
	delay(5000);
	Prov.startWifi();
}

// the loop function runs over and over again until power down or reset
void loop() {
	waitForSerial(5000);
}
void waitForSerial(unsigned long timeout_millis) {
	unsigned long start = millis();
	while (!Serial) {
		if (millis() - start > timeout_millis)
			break;
	}
}
