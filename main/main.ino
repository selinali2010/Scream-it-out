#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd display;

byte bar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, INPUT);
  display.begin(16, 2);
  display.print("hk4li and sk6li");
  display.createChar(0, bar);
  display.createChar(1, smiley);
  delay(1000);
}

int countBars = 0, sensorValue = 0, buttonValue = 0;
int prvSensorValue = 0;

void loop() {
  // put your main code here, to run repeatedly:
  buttonValue = digitalRead(4);
  if(buttonValue == HIGH){
    sensorValue = analogRead(A1);
    Serial.println(sensorValue);
    if(sensorValue >= 400){
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
    display.setCursor(countBars,1);
    if(prvSensorValue >= 400 && sensorValue >= 400 && countBars != -1){//buttonValue == HIGH && countBars != -1){
      countBars++;
      display.write(byte(0)); 
    }
    if(countBars == 15){
      display.write(byte(1));
      countBars = -1;
    } 
    prvSensorValue = sensorValue;
  }
  //display.clear();
  //display.print(sensorValue);
  delay(500);
}
