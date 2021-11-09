#include <SoftwareSerial.h>
SoftwareSerial softSerial(13, 14); // RX, TX

String str;

void setup()  
{
  softSerial.begin(115200);
} 
void loop()  
{ 
//  softSerial.write(123);
  str =String("Hoorad");
  softSerial.println(str);
  delay (100);
}
