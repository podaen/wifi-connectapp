/*
 Name:		wifi_connectapp.h
 Created:	10/20/2026 8:59:31 PM
 Author:	Daen
 Editor:	http://www.visualmicro.com
*/

#ifndef _wifi_connectapp_h
#define _wifi_connectapp_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define wifi_connectapp_VERSION        (F("1.0.0"))

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <WiFiProv.h>
#include <WiFi.h>


// Configuration (declarations only)
extern const char* service_name;
extern const char* service_key;
extern const char* pop;
extern bool reset_provisioned;

class wifi_connectapp
{
public:
	//void deiniWifi();

	static void eraseNVS();
	void startWifi();
	static void SysProvEvent(arduino_event_t* sys_event);
	//void SysProvEvent(arduino_event_t* sys_event);
	bool testWifi(void);
protected:
};
#endif

