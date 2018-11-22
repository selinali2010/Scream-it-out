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
int FULLCAN = 750;
unsigned long GAMELENGTH = 30000;
String PLAYERS[] = {"Sulley", "Randall", "You"};
int topScores[2] = {13125, 11850};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, INPUT);
  display.begin(16, 2);
  display.print("Scream it out!");
  display.setCursor(0,1);
  display.print("By hk4li & sk6li");
  display.createChar(0, bar);
  display.createChar(1, smiley);
  display.createChar(2, canister);
  delay(1000);
}

int countBars = 0, sensorValue = 0, buttonValue = 0, state = STARTUP;
int gauge = 0, cans = 0;
unsigned long gameTime = 0;

void printCans(int numCans){
  display.setCursor(0,0);
  display.write(byte(2));
  display.print(numCans);
}

void clearBar(){
  display.setCursor(0,1);
  display.print("                ");
}

void updateRanking(int score){
  display.setCursor(3, 0);
  switch (score) {
  case 0 ... 2:
    Serial.print("B");
    display.print("Beginner");
    break;
  case 3 ... 5:
    Serial.print("T");
    display.print(" Trainee");
    break;
  case 6 ... 8:
    Serial.print("W");
    display.print("  Worker");
    break;
   case 9 ... 12:
    Serial.print("M");
    display.print("  Master");
    break;
  default:
    Serial.print("G");
    display.print("  Genius");
    break;
  }
}

void printTime(int gameTime){
  display.setCursor(13, 0);
  int displayT = GAMELENGTH/60 - (millis() - gameTime)/60;
  if(displayT <= 0){
    display.print("  0");
  } else if (displayT < 10){
    display.print("  ");
    display.print(displayT);
  } else if(displayT < 99){
    display.print(" ");    
    display.print(displayT);
  } else {
    display.print(displayT); 
  }
}

void printScores(int a, int b, int c, String s1, String s2, String s3){
  display.setCursor(0,1);
  display.print("1." + String(s1) + ": " + a + "     "); 
  delay(3000);
  display.setCursor(0,0);
  display.print("2." + String(s2) + ": " + b + "     ");
  display.setCursor(0,1);
  display.print("3." + String(s3) + ": " + c + "     ");
}

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
      display.setRGB(90, 255, 0);
      gameTime = millis();
      display.clear();
      printCans(cans);
      updateRanking(cans);
    }
  } else if(state == SCREAM){
    if(millis() - gameTime > GAMELENGTH){
      state = END;
    } else {// if(buttonValue == HIGH){
      if(millis() - gameTime > 9*GAMELENGTH/10){
        display.setRGB(230, 0, 0);
      }
      sensorValue = analogRead(A1);
      if(sensorValue >= 600){
        gauge += 2*sensorValue - 600;
      }
      
      Serial.println(sensorValue);
      
      display.setCursor(countBars,1);
      if(gauge > FULLCAN){
        gauge -= FULLCAN;
        countBars++;
        display.write(byte(0)); 
      }
      
      if(countBars == 17){
        cans++;
        countBars = 0;
        printCans(cans);
        updateRanking(cans);
        clearBar();
      }

      printTime(gameTime);
    }
  } else if(state == END){
    printCans(cans);
    updateRanking(cans);
    printTime(gameTime);
    display.setRGB(255, 51, 51);
    display.setCursor(0,1);
    display.print("Tiiiiime's up!!!");
    delay(3000);
    display.setColorWhite();
    display.setCursor(0,0);
    display.print("Leaderboard     ");
    int score = (gauge+countBars*750+cans*750*17)/10;
    if(score > topScores[0]){
      printScores(score,topScores[0],topScores[1],PLAYERS[2],PLAYERS[0],PLAYERS[1]);
    } else if(score > topScores[1]){
      printScores(topScores[0],score,topScores[1],PLAYERS[0],PLAYERS[2],PLAYERS[1]);
    } else {
      printScores(topScores[0],topScores[1],score,PLAYERS[0],PLAYERS[1],PLAYERS[2]);
    }
    delay(4000);
    state = REPLAY;
  } else if(state == REPLAY){
    display.setCursor(0,0);
    display.print("Thnx 4 play'n!  ");
    display.setCursor(0,1);
    display.print("Click to replay  ");
    if(buttonValue == 1){
      state = READY;
    }
  }
  
  delay(100);
}
