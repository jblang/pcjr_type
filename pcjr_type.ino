#include "pcjr_type.h"

void testKeys()
{
  // Test typing all graphical keys
  typeString("cls\n");
  typeString("' 1234567890\n");
  typeString("' !@#$%^&*()\n");
  typeString("' abcdefghijklmnopqrstuvwxyz\n");
  typeString("' ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
  typeString("' `~-_=+[{]}\\|;:'\",<.>/?\n");  
  delay(1000);
}

void setup()
{
  // must be called once in setup
  pcjrInit();
  
  // comment out if you don't want to run a test at startup
  testKeys(); 
  
  // The keyboard PCjr can only receive around 100 CPS via IR
  // so lower baud rates work best. Setting this higher than ~600
  // will result in dropped characters
  Serial.begin(600);
}

void loop() {
  // Listen on serial port and relay any characters received
  if (Serial.available())
    typeAscii(Serial.read());
}
