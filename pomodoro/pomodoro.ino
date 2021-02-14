#include "M5CoreInk.h"

Ink_Sprite InkPageSprite(&M5.M5Ink);

unsigned short STATE_IDLE = 0;
unsigned short STATE_WORKING = 1;
unsigned short STATE_BREAK = 2;

//unsigned int POMODORO_MINS = 25;
unsigned int POMODORO_MINS = 5;
unsigned int SHORT_BREAK_MINS = 5;
unsigned int LONG_BREAK_MINS = 15;
//unsigned long MILLIS_IN_ONE_MIN = 60000;
unsigned long MILLIS_IN_ONE_MIN = 2000;
unsigned long startTime = 0;
unsigned int currIteration = 1;
unsigned int minsRemaining = 0;
unsigned short currentState = STATE_IDLE;

bool running = false;

void drawText(char* str) {
    InkPageSprite.clear();
    InkPageSprite.drawString(35, 59, str);
    InkPageSprite.pushSprite();
}

void drawMinsRemaining(int numMins) {
    char minsBuf[30];
    sprintf(minsBuf, "%d mins left.", numMins);
    InkPageSprite.drawString(35, 89, minsBuf);
    InkPageSprite.pushSprite();
}

void playTone(int howMany) {
    while (howMany > 0) {
      M5.Speaker.tone(2700);
      delay(100);
      M5.Speaker.mute();

      howMany--;

      if (howMany > 0) {
        delay(100);
      }
    }
}

void setup() {
    M5.begin();
    
    if (!M5.M5Ink.isInit()) {
        Serial.printf("Ink Init failed");
    }
    
    M5.M5Ink.clear();
    delay(1000);
    
    if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0) {
        Serial.printf("Ink Sprite create failed");
    }

    drawText("Press to start!");
    playTone(1);
}

void loop() {
    if (M5.BtnEXT.wasPressed()) {
      playTone(3);
      minsRemaining = POMODORO_MINS;
      currIteration = 1;
      
      if (currentState == STATE_IDLE) {
        drawText("Time to work... 1/4");
        drawMinsRemaining(minsRemaining);
        startTime = millis();  
        currentState = STATE_WORKING;
      } else {
        drawText("Press to start!");
        startTime = 0;
        currentState = STATE_IDLE;
      }
    }
    
    if (M5.BtnPWR.wasPressed()) {
        drawText("Bye, love you!");
        playTone(1);
        delay(3000);
        M5.M5Ink.clear();
        delay(1000);
        currentState = STATE_IDLE;
        M5.PowerDown();
    }

    if (currentState != STATE_IDLE) {
      unsigned long timeNow = millis();
      if (timeNow - startTime >= MILLIS_IN_ONE_MIN) {
        minsRemaining--;

        if (minsRemaining == 0) {
          playTone(3);
          
          // Work out what state comes next...
          if (currentState == STATE_WORKING) {
            currIteration++;

            if (currIteration == 5) {
              currIteration = 1;
              minsRemaining = LONG_BREAK_MINS;
              currentState = STATE_BREAK;
              drawText("Long break...");
            } else {
              minsRemaining = SHORT_BREAK_MINS;
              currentState = STATE_BREAK;
              drawText("Short break...");
            }
          } else if (currentState == STATE_BREAK) {
            char workBuf[30];
            sprintf(workBuf, "Time to work... %d/4", currIteration);
            drawText(workBuf);
            minsRemaining = POMODORO_MINS;
            currentState = STATE_WORKING;
          }   
        }

        drawMinsRemaining(minsRemaining);
        startTime = millis();     
      }
    }
    
    M5.update();
}
