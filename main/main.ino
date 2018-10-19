#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd display;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  display.begin(16, 2);
  display.print("hk4li and sk6li");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A1);
  Serial.println(sensorValue);
  if(sensorValue >= 400){
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  display.setCursor(0,1);
  display.clear();
  display.print(sensorValue);
  delay(100);
}
