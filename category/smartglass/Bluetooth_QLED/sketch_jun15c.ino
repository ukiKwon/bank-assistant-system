#include <SoftwareSerial.h>
#include <Wire.h>
#include <OzOLED.h>

#define BT_TXD 2
#define BT_RXD 3
SoftwareSerial bluetooth(BT_TXD, BT_RXD);
String mString ="";

void setup()
{
  OzOled.init();
  OzOled.clearDisplay();
  OzOled.printString("Ready Now");
  
  Serial.begin(9600);
  Serial.println("Hi, there?");
  bluetooth.begin(9600);
  bluetooth.println("Type value!");
}
 
void loop()
{
  while (bluetooth.available()) {
    OzOled.clearDisplay();
    char mchar = (char)bluetooth.read();
    mString += mchar;
    delay(5);
  }
  if (!mString.equals("")) {
    Serial.println("input value: " + mString);
    char* str = (char*)malloc(sizeof(char) * mString.length());
    mString.toCharArray(str, mString.length() + 2);  
    
    OzOled.printString(str);
    mString = "";
  }
}
