#include <SoftwareSerial.h>
SoftwareSerial softSerial(13, 14); // RX, TX

String str;
char input[10];

void setup()  
{
  Serial.begin(9600);
  softSerial.begin(115200);
  Serial.println("Hello World");
} 
void loop()  
{ 
  str =String("Hooradk");
  softSerial.print(str);
  delay (100);
//  softSerial.readBytes(input,10);
//  Serial.print(input);
  
}
