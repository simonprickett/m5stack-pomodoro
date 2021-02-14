
#include "M5CoreInk.h"
#include "icon.h"

Ink_Sprite InkPageSprite(&M5.M5Ink);

unsigned short STATE_IDLE = 0;
unsigned short STATE_WORKING = 1;
unsigned short STATE_BREAK = 2;

unsigned int POMODORO_MINS = 25;
unsigned int SHORT_BREAK_MINS = 5;
unsigned int LONG_BREAK_MINS = 15;
unsigned long MILLIS_IN_ONE_MIN = 60000;
unsigned long startTime = 0;
unsigned int currIteration = 1;
unsigned int minsRemaining = 0;
unsigned short currentState = STATE_IDLE;

bool running = false;
char statusBuf[30];

void playTone(int howMany) {
  while (howMany > 0) {
    M5.Speaker.tone(2700);
    
    delay(100);
    howMany--;

    M5.Speaker.mute();

    if (howMany > 0) {
      delay(100);
    }
  }
}

void drawImageToSprite(int posX, int posY, image_t* imagePtr, Ink_Sprite* sprite) {
  sprite->drawBuff(posX, posY, imagePtr->width, imagePtr->height, imagePtr->ptr);
}

void drawScreen(char *str, int num) {
  InkPageSprite.clear();

  int n;

  if (num > 9) {
    n = num;
      
    while (n >= 10) {
      n = n / 10;
    }
  } else {
    n = 0;
  }
    
  drawImageToSprite(60, 78, &num55[n], &InkPageSprite);
  drawImageToSprite(100, 78, &num55[num % 10], &InkPageSprite);

  // Let's say each char is 8 and we can do 25 across
  int gap = ((25 - strlen(str)) / 2) * 8;
    
  InkPageSprite.drawString(gap, 48, str);
  InkPageSprite.pushSprite();
}

void setup() {
  M5.begin();

  M5.M5Ink.clear();

  InkPageSprite.creatSprite(0, 0, 200, 200);
        
  drawScreen("Press to start!", 0);
  playTone(1);  
}

void loop() {
  if (M5.BtnEXT.wasPressed()) {
    playTone(3);
    minsRemaining = POMODORO_MINS;
    currIteration = 1;
      
    if (currentState == STATE_IDLE) {
      sprintf(statusBuf, "Time to work... 1/4");
      drawScreen(statusBuf, minsRemaining);
      startTime = millis();  
      currentState = STATE_WORKING;
    } else {
      drawScreen("Press to start!", 0);
      currentState = STATE_IDLE;
    }
  }
    
  if (M5.BtnPWR.wasPressed()) {
    drawScreen("Bye, love you!", 0);
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
            sprintf(statusBuf, "Long break...");
          } else {
            minsRemaining = SHORT_BREAK_MINS;
            currentState = STATE_BREAK;
            sprintf(statusBuf, "Short break...");
          }
        } else if (currentState == STATE_BREAK) {    
          minsRemaining = POMODORO_MINS;
          currentState = STATE_WORKING;
          sprintf(statusBuf, "Time to work... %d/4", currIteration);
        }   
      }

      drawScreen(statusBuf, minsRemaining);
      startTime = millis();     
    }
  }
    
  M5.update();
}
