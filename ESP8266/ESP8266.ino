#include <ESP8266WiFi.h>
//#include <SoftwareSerial.h>

//SoftwareSerial softSerial(3,1); // RX, TX

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID        "Shabdiz"
#define WLAN_PASS        "hoorad123"

#define MQTT_SERVER      "192.168.137.1"
#define MQTT_PORT        1883
#define MQTT_USERNAME    ""
#define MQTT_PASSWORD    ""

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD);

Adafruit_MQTT_Publish pi_notif = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/pi/notif");         //give rpi notifications

Adafruit_MQTT_Subscribe esp_lamp = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/esp2/lamp");        // get messages for Lamp

void MQTT_connect();

//char mystr[10];
//int data;
//char ip;

void setup()
{
  Serial.begin(115200);
  
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  mqtt.subscribe(&esp_lamp);
}

void loop() {
  MQTT_connect();

//  mySerial.readBytes(mystr,7);
  
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription())) {
    if (subscription == &esp_lamp) {
      char *message = (char *)esp_lamp.lastread;      
      if (strncmp(message, "off", 3) == 0) {
        pi_notif.publish("esp_lamp Turned OFF");
      }
      else if (strncmp(message, "on", 2) == 0) {
        pi_notif.publish("esp_lamp Turned ON");
      }
      else if (strncmp(message, "rob", 3) == 0) {
//        data = Serial.read();
//        Serial.readBytes(data,7);
//        pi_notif.publish(data); 
        if (Serial.available()) {
          delay(100);
          pi_notif.publish("kir");
          pi_notif.publish(Serial.read());
        }
        
//        if (Serial.available()){
//          ip=softSerial.read();
//          Serial.print(ip);
//          pi_notif.publish('h');
//        }
      }
    }
  }
}


void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
    mqtt.disconnect();
    delay(5000);  
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
   pi_notif.publish("ESP8266 MQTT connected.");
}
