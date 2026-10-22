/*
 Name:		wifi_connectapp.cpp
 Created:	10/20/2026 8:59:31 PM
 Author:	Daen
 Editor:	http://www.visualmicro.com
*/

#include "wifi_connectapp.h"

#include "WiFiProv.h"
#include <Arduino.h>
#include "nvs_flash.h"

#if CONFIG_ESP_HOSTED_ENABLED
#error "WiFiProv is only supported in SoCs with native Wi-Fi support"
#endif

// Ensure a security level is defined
#if defined(CONFIG_ESP_PROTOCOMM_SUPPORT_SECURITY_VERSION_1)
#define NETWORK_PROV_SECURITY NETWORK_PROV_SECURITY_1//for android app
#endif

const char* service_name = "my_device";//the name of the wifi network
const char* service_key = "password";//this is the password for the provisioning service. It is used to secure the communication between the device and the provisioning application. The service key should be kept secret and not shared publicly.
const char* pop = "abcd1234";//this is the next step to secure the provisioning process. The Proof of Possession (PoP) is a secret that is known only to the device and the provisioning application. It is used to authenticate the device during the provisioning process, ensuring that only authorized devices can be provisioned. The PoP should be kept secret and not shared publicly.
bool reset_provisioned = true;

//namespace {

void wifi_connectapp::SysProvEvent(arduino_event_t* sys_event)
{
    if (!sys_event) {
        return;  //Null would crash this function
    }
    switch (sys_event->event_id) {
        //case ARDUINO_EVENT_WIFI_READY:
        //    Serial.println("WiFi interface ready");
        //    break;
        //case ARDUINO_EVENT_WIFI_SCAN_DONE:
        //    Serial.println("Completed scan for access points");
        //    break;
        //case ARDUINO_EVENT_WIFI_STA_START:
        //    Serial.println("WiFi client started");
        //    break;
        //case ARDUINO_EVENT_WIFI_STA_STOP:
        //    Serial.println("WiFi clients stopped");
        //    //auto reconnect is enabled
        //    break;
        //case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        //    Serial.println("Connected to access point");
        //    break;
        //case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        //    Serial.println("\nDisconnected. Connecting to the AP again... ");
        //    //auto reconnect is enabled
        //    break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("\nConnected IP address : ");
        Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
        WiFi.setSleep(false);
        //WiFiProv.endProvision();
        //WiFi.removeEvent(TestWifiProv::SysProvEvent);
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("\nDisconnected. Connecting to the AP again... ");
        break;
    case ARDUINO_EVENT_PROV_START:
        Serial.println("\nProvisioning started\nGive Credentials of your access point using \" Android app \"");
        break;
    case ARDUINO_EVENT_PROV_CRED_RECV: {
        Serial.println("\nReceived Wi-Fi credentials");
        Serial.print("\tSSID : ");
        Serial.println((const char*)sys_event->event_info.prov_cred_recv.ssid);
        Serial.print("\tPassword : ");
        Serial.println((char const*)sys_event->event_info.prov_cred_recv.password);
        break;
    }
    case ARDUINO_EVENT_PROV_CRED_FAIL: {
        Serial.println("\nProvisioning failed!\nPlease reset to factory and retry provisioning\n");
        if (sys_event->event_info.prov_fail_reason == NETWORK_PROV_WIFI_STA_AUTH_ERROR) {
            Serial.println("\nWi-Fi AP password incorrect");
            wifi_connectapp::eraseNVS();
            ESP.restart();
        }
        else {
            Serial.println("\nWi-Fi AP not found....Add API \" nvs_flash_erase() \" before beginProvision()");
            wifi_connectapp::eraseNVS();
            ESP.restart();
        }
        break;
    }
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
        Serial.println("\nProvisioning Successful");
        break;
    case ARDUINO_EVENT_PROV_END:
        Serial.println("\nProvisioning Ends");
        //WiFiProv.endProvision();
        //WiFi.removeEvent(TestWifiProv::SysProvEvent);
        break;
    default:
        break;
    }
}

//void TestWifiProv::deiniWifi() {
//    Serial.println("Deintialize WiFi");
//    WiFi.removeEvent(TestWifiProv::SysProvEvent);
//    WiFi.disconnect(true);// delete old config
//    WiFi.mode(WIFI_OFF);//espWiFiStop
//    //eraseNVS();//erase credentials and etcetera
//}
void wifi_connectapp::startWifi() {
    Serial.println("login to WiFi");
    //eraseNVS();//erase credentials and etcetera
    delay(100);
    WiFi.onEvent(&wifi_connectapp::SysProvEvent);
    //WiFi.onEvent(SysProvEvent);
    WiFi.setAutoReconnect(true);
    Serial.println("Begin Provisioning using Soft AP");
    WiFiProv.beginProvision(NETWORK_PROV_SCHEME_SOFTAP, NETWORK_PROV_SCHEME_HANDLER_NONE, NETWORK_PROV_SECURITY, pop, service_name, service_key);
    //Serial.print("Security: ");
    //Serial.println(NETWORK_PROV_SECURITY);

}
/*ERASE FLASH SETTINGS*/
void wifi_connectapp::eraseNVS() {
    esp_err_t ret;
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
    Serial.println("nvs flash erased");
    ESP_ERROR_CHECK(ret);
}
bool wifi_connectapp::testWifi(void)
{
    int c = 0;
    Serial.println("Waiting for Wifi to connect");
    while (c < 20) {
        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }
        delay(500);
        Serial.print("*");
        c++;
    }
    Serial.println("");
    Serial.println("Connect timed out, opening AP");
    return false;
}
//}