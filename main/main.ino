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

byte canister[8] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

int STARTUP = 0, READY = 1, SCREAM = 2, END = 3, REPLAY = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, INPUT);
  display.begin(16, 2);
  display.print("hk4li and sk6li");
  display.createChar(0, bar);
  display.createChar(1, smiley);
  display.createChar(2, canister);
  delay(1000);
}

int countBars = 0, sensorValue = 0, buttonValue = 0, state = STARTUP;
int gauge = 0, cans = 0;
unsigned long gameTime = 0;

void loop() {
  buttonValue = digitalRead(4);
  
  if(state == STARTUP){
    if(buttonValue == 1){
       state = READY;
    }
  } else if(state == READY){
    display.setCursor(0,0);
    display.print("Click button and");
    display.setCursor(0,1);
    display.print("start screaming!");
    cans = 0;
    countBars = 0;
    gauge = 0;
    
    if(buttonValue == 1){
      state = SCREAM;
      gameTime = millis();
      display.clear();
      display.write(byte(2));
      display.print(cans);
    }
  } else if(state == SCREAM){
    if(millis() - gameTime > 30000){
      state = END;
    } else {// if(buttonValue == HIGH){
      sensorValue = analogRead(A1);
      if(sensorValue >= 300){
        gauge += sensorValue - 300;
      }
      
      Serial.println(sensorValue);
      
      display.setCursor(countBars,1);
      if(gauge > 1000){
        gauge -= 1000;
        countBars++;
        display.write(byte(0)); 
      }

      /*
      display.setCursor(4,0);
      display.print(gauge);
      display.print("  ");
      display.setCursor(countBars,1);
      */
      
      if(countBars == 17){
        cans++;
        countBars = 0;
        display.clear();
        display.write(byte(2));
        display.print(cans);
        display.setCursor(0,1);
        display.write(byte(0)); 
      }
    }
  } else if(state == END){
    display.clear();
    display.write(byte(2));
    display.print(cans);
    display.setCursor(0,1);
    display.print("Thnx 4 play'n!");
    delay(5000);
    state = REPLAY;
  } else if(state == REPLAY){
    display.setCursor(0,1);
    display.print("Play again?     ");
    if(buttonValue == 1){
      state = READY;
    }
  }
  
  delay(100);
}
