#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,1); // RX, TX

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

Adafruit_MQTT_Publish pi_lcd = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/pi/lcd");             //give rpi messages for printing
Adafruit_MQTT_Publish pi_ir = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/pi/ir");               // publish ir commands for rpi to handle
Adafruit_MQTT_Publish pi_notif = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/pi/notif");         //give rpi notifications

Adafruit_MQTT_Subscribe esp_lamp = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/esp2/lamp");        // get messages for Lamp

void MQTT_connect();

char mystr[10] = "kir";
char data[10];

void setup()
{
//  Serial.begin(9600);
  mySerial.begin(9600);
//  Serial.println(F("RPi-ESP2-MQTT"));
//  
//  Serial.println(); Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }
//  Serial.println();
//  Serial.println("WiFi connected");
//  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
  mqtt.subscribe(&esp_lamp);
}

void loop() {
  MQTT_connect();

//  mySerial.readBytes(mystr,7);
  
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription())) {
    if (subscription == &esp_lamp) {
      char *message = (char *)esp_lamp.lastread;
//      Serial.print(F("Got: "));
//      Serial.println(message);
      
      if (strncmp(message, "off", 3) == 0) {
        pi_notif.publish("esp_lamp Turned OFF");
      }
      else if (strncmp(message, "on", 2) == 0) {
        pi_notif.publish("esp_lamp Turned ON");
      }
      else if (strncmp(message, "rob", 3) == 0) {
//        data = mySerial.read();
        Serial.readBytes(data,7);
        pi_notif.publish(mystr); 
        pi_notif.publish(data);
      }
    }
  }
}


void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
//    Serial.println(mqtt.connectErrorString(ret));
//    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
   pi_notif.publish("ESP8266 MQTT connected.");
}
