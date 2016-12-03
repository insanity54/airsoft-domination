/**
  night game airsfoft king of the hill
**/

#include <avr/power.h> // fix for trinket
#include <Adafruit_NeoPixel.h>
#define PIXELPIN 0
#define BUZZERPIN 1
#define GREENBUTTONPIN 2
#define REDBUTTONPIN 3
#define NUMBEROFPIXELS 16
#define LONGPRESSTIME 2000
#define INDICATORTIME 500
#define BUZZTIME 500
#define WINBUZZTIME 2000
#define BLINKRATE 800
#define WINBLINKRATE 300
#define COUNTDOWNTIME 1000 // @todo change to 15000 for gameplay
#define ANIMATIONINTERVAL 250 // update the display this fast

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMBEROFPIXELS, PIXELPIN);

// global (global)
//uint32_t prevTime;
uint32_t red     = 0xff0000; // red
uint32_t blue    = 0x0000ff; // blue
uint32_t purple  = 0xff6699; // purple
uint32_t yellow  = 0xFFE600; // yellow
uint32_t green   = 0x00FF00; // green
uint8_t mode = 0; // operation mode. 0 program, 1 prep, 2 game
uint32_t buzzTimer;
boolean pressedLastCycle = 0;
boolean longPressLastCycle = 0;

// global (programming mode)
boolean firstRun = true;
uint8_t programMode = 0; // 0 is timetowin, 1 is nothing (yet)
uint8_t timeToWin = 0;
uint8_t lastTtw = 0;
uint32_t pressTime = 0;
uint32_t i1Time = 0;
uint32_t i2Time = 0;

// global (prep mode)
boolean gamePrepared = 0;
boolean gameStarted = 0;
boolean blinkStatus = false;
uint32_t lastBlink = 0;

// global (countdown mode)
uint32_t countdownStart;

// global (game mode)
uint8_t activeTeam = 0;  // 0 nobody, 1 red, 2 green
uint8_t lastActiveTeam;
uint32_t startOffset;
long redCounter;
long greenCounter;
uint32_t lastTick;
uint8_t winningTeam;  // 0 nobody, 1 red, 2 green
boolean winBuzz;
uint8_t buttonSelection = 0;  // 0 nobody, 1 red, 2 green




void setup() {
  if (F_CPU == 16000000)           // fix for trinket
  clock_prescale_set(clock_div_1); // fix for trinket
  pixels.begin();
  pixels.setBrightness(50);        
  //prevTime = millis();
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(GREENBUTTONPIN, INPUT);
  pinMode(REDBUTTONPIN, INPUT);
}

void loop() {

  switch(mode) {
    case 0:
    // program mode
    programLoop();
    break;
    
    case 1:
    // prep mode
    prepLoop();
    break;
    
    case 2:
    // countdown mode
    countdownLoop();
    break;
    
    case 3:
    // game mode
    gameLoop();
    break;
    
    case 4:
    // end mode
    endLoop();
    break;
  }
  
  
  
  animate();
  buzzCheck();
  
  
  // when we startup
  // display sequence of animations for explanation purposes
  // go into programming mode
  
  // game mode
  // win timer set to duracion set in programming mode
  // red team timer set to 0
  // blue team timer set to 0
  // press button to start game
  // countdown starts so person can get to their side
  // loud beep when game start
  // button press shows red color
  // another button press shows blue color
  // small delay so user gets right color
  // active team color counts up
}

void countdownLoop() {
  if (millis() > countdownStart + COUNTDOWNTIME) {
    startOffset = millis();
    nextMode();
  }
}

void programLoop() {
  
  if (firstRun) {
    ackBuzz();
    firstRun = false;
  }
  // program the time needed to win
  // each neopixel represents 2 minutes

  // wait for input
  // if a click, add a neopixel
  // if a long press, move to next programming section
  
  switch(getInput()) {
    
    case 0:
      // no valid input detected
      pixels.setPixelColor(16, yellow);
      pixels.show();
    break;
    
    case 1:
      // click detected
      // add 2 to timetowin
      if (timeToWin < 32) {
        // go no greater than 32
        timeToWin += 2;
      }
      indicateBlue();
      
    break;
    
    case 2:
      // long press detected
      indicateRed();
      ackBuzz();
      nextMode();
    break;
    
  } 
}

void gameLoop() {
  switch(getInput()) {
  
  case 0:
    // no valid input detected

    
  break;
  
  case 1:
    // click detected
    ackBuzz();
    
    // nobody is 0, red is 1, green is 2
    // switch to the team selected by button
    switch(buttonSelection) {
      case 0:
      break;

      case 1:
      // switch to red
      activeTeam = 1; 
      break;
      
      case 2:
      // switch to green
      activeTeam = 1; 
      break;
    }
 
  break;
  
  case 2:
    // long press detected
    //indicateRed();
    //ackBuzz();
    
  break;
  }
  
  // has 1000 ms passed?
  if (millis() > lastTick + 1000) {
    // yes, 1000ms has passed
    
    //ackBuzz();
    
    // add 1s to active team's counter
    if (activeTeam == 1) {
      redCounter += 1000;
    }
      
    if (activeTeam == 2) {
      greenCounter += 1000;
    }
  
    lastTick = millis();
  }
  
  // check to see if team has won
  if (redCounter > timeToWin * 1000 * 60) {
    winningTeam = 1;
    //winBuzz = true;
    ackBuzz();
    pixels.setPixelColor(5, green);
    pixels.show();
    nextMode();
  }
  
  if (greenCounter > timeToWin * 1000 * 60) {
    winningTeam = 2;
    //winBuzz = true;
    ackBuzz();
    pixels.setPixelColor(5, purple);
    pixels.show();
    nextMode(); 
  }
}


void endLoop() {
      pixels.setPixelColor(16, 0xFFFF47);
      pixels.show();  
      delay(1000);
} 

void prepLoop() {
  
  switch(getInput()) {
    
    case 0:
      // no valid input detected
      pixels.setPixelColor(16, yellow);
      pixels.show();
    break;
    
    case 1:
      // click detected
      indicateBlue();
      
    break;
    
    case 2:
      // long press detected
      indicateRed();
      ackBuzz();
      countdownStart = millis();
      lastTick = millis();
      nextMode();
      
    break;
    
  }  
  
  
  // game mode
  // win timer set to duration set in programming mode
  // red team timer set to 0
  // green team timer set to 0
  // press button to start game
  // countdown starts so person can get to their side
  // loud beep when game start
  // button press shows red color
  // another button press shows blue color
  // small delay so user gets right color
  // active team color counts up
 
}

void ackBuzz() {
  digitalWrite(BUZZERPIN, HIGH);
  buzzTimer = millis();
}

void nextMode() {
  
  if (mode != 4) {
    // if we're not at the last mode (game), go to the next mode
    mode ++;
    
  } else {
    // we're at the last mode so go to the first
    mode = 0;
  }
  
}

/**
 * samples button. returns value if valid input (click, press, etc.)
 * otherwise returns false
 */
int getInput() {
  
  
  
  // it is pressed now
  // it was pressed last cycle
  // 2 seconds have elapsed since initial press
  //   it's a long press
  
  // it is not pressed now
  // it was pressed last cycle
  //   it's a click
  
  

  // is a button pressed?  
  if (digitalRead(GREENBUTTONPIN)) {
    digitalWrite(BUZZERPIN, HIGH);
    delay(100);
    digitalWrite(BUZZERPIN, LOW);

    if (digitalRead(GREENBUTTONPIN) == 1) {
        buttonSelection = 2;
    } else {
        buttonSelection = 1;
    }
    
    // have we processed something previously, but the user
    // is still holding down the button?
    if (longPressLastCycle == true) {
      // do nothing until user lets go
      return 0;
    }
    
    // was the button pressed last cycle too?
    if (pressedLastCycle == true) {
      // it was pressed last cycle
      
      // has button been held for 2 seconds or more?
      if (millis() > pressTime + LONGPRESSTIME) {
        // yes, button has been held for 2 seconds or more
        // got long click
        // before we return for processing, require user to let go
        // of button before we accept any more input
        longPressLastCycle = true;
        pressedLastCycle = true;
        
        // return for processing
        return 2;
        
      } else {
        // no, button has not been held for 2s or more.
        // do nothing,
        // return so we can see what happens next cycle
        return 0;
      }
      
      
      
    } else {
      // button was not pressed last cycle
      // so this is the first cycle it is pressed
      // start a timer so we can later see how long the
      // user held the button
      pressTime = millis();
      
      // make sure next loop the press isn't processed
      pressedLastCycle = true;

    }
  } else {
    // no, button is not pressed.
    
    // was it just released? (pressed last cycle but not now)
    if (pressedLastCycle == true) {
      // yes, it was just released.
      
      // was a long press registered last cycle?
      // we ask this because if a long press happened last,
      // and the user is still holding down the button,
      // we don't want to process this button release as a click.
      if (longPressLastCycle == true) {
        // yes, a long press happened last cycle
        
        // allow clicks and presses to be detected next cycle
        pressedLastCycle = false;
        longPressLastCycle = false;
        
        // no input this cycle, return.
        return 0;
        
      } else {
        // no, a long press did not happen last cycle.
        // this means this button release is a click event
        
        pressedLastCycle = false;
        longPressLastCycle = false;
        
        // return for processing
        return 1;
      }
    } else {
      // no, button was not pressed last cycle.
      // it's not pressed this cycle, either.
      pressedLastCycle = false;
      longPressLastCycle = false;
    }
  }
    
  
  return false;
}

void indicateRed() {
  i1Time = millis();
  
  pixels.setPixelColor(14, red);
  pixels.show();  
}

void indicateGreen() {
  i2Time = millis();
 
  pixels.setPixelColor(15, green);
  pixels.show();
}

void indicateBlue() {
 
  pixels.setPixelColor(15, blue);
  pixels.show();
}

/**
 * handle timers for buzzers.
 * does not directly buzz the buzzer, only shuts them up.
 */
void buzzCheck() {
  if (!winBuzz) {
    if (millis() > buzzTimer + BUZZTIME) {
      digitalWrite(BUZZERPIN, LOW); 
    }
  } else {
    if (millis() > buzzTimer + WINBUZZTIME) {
      digitalWrite(BUZZERPIN, LOW);
    }
  }
}

void animate() {
  
//  // clear indicators if their timers are done
//  if (millis() > i1Time + INDICATORTIME) {
//    pixels.setPixelColor(14, 0);
//    pixels.show();
//  } 
//  if (millis() > i2Time + INDICATORTIME) {
//    pixels.setPixelColor(15, 0) ;
//    pixels.show();
//  }
  
  // update display depending on what mode we're in
  switch(mode) {
      
    case 0:
      // program mode
      // for every 2 in timeToWin,
      // light up 1 pixel
      
      //numLightUp = timeToWin / 2;
      
      // has the time to win changed since we last animated?
      if (lastTtw != timeToWin) {
        // yes, the time to win has changed
        // update the display
        
        for (int i = 0; i < timeToWin / 2; i ++) {
          pixels.setPixelColor(i, yellow);        
        }
        pixels.show();
        lastTtw = timeToWin;     
      }
      
      break;
      
       
    case 1:
      // prep mode
      
      // blink if blinks haven't started
      if (!blinkStatus) {
        // make all pixels blue
        
        // is it time to blink on?
        if (millis() > lastBlink + BLINKRATE) {
          
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, purple);
          }
          pixels.show();
          blinkStatus = true;
          lastBlink = millis();
        }
        
      } else {
        // we've blinked previously
        // is it time to blink off?
        if (millis() > lastBlink + BLINKRATE) {
          // yes, time to blink.
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, 0);
          }
          pixels.show();
          blinkStatus = false;
          lastBlink = millis();
        }
      }
      
      break;
      
   case 2:
      // countdown mode
     // beep
      if (!blinkStatus) {
        // make all pixels purple
        
        // is it time to blink on?
        if (millis() > lastBlink + BLINKRATE) {
          
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, purple);
          }
          pixels.show();
          ackBuzz();
          blinkStatus = true;
          lastBlink = millis();
        }
        
      } else {
        // we've blinked previously
        // is it time to blink off?
        if (millis() > lastBlink + BLINKRATE) {
          // yes, time to blink.
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, 0);
          }
          pixels.show();
          ackBuzz();
          blinkStatus = false;
          lastBlink = millis();
        }
      }
          
      break;
      
   case 3:
      // game mode
      
      // only update every so often
      if (millis() > lastTick + ANIMATIONINTERVAL) {
        switch(activeTeam) {
          case 0:
          // nobody
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, blue);
          }
          //lastActiveTeam = 0;
          break;
          
          case 1:
          // red
          //int num = (redCounter / (timeToWin * 1000 * 60)) * 16;
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, red);
          }
          //pixels.setPixelColor(redCounter / 1000, red);
          //lastActiveTeam = 1;
          break;
          
          case 2:
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, green);
          }
          //pixels.setPixelColor(greenCounter / 1000, green);
          //lastActiveTeam = 0;
          break;        
          
        }        
        pixels.show(); 
      }
      break;
      
    case 4:
      // end mode
        if (!blinkStatus) {
        
        // is it time to blink on?
        if (millis() > lastBlink + WINBLINKRATE) {
          
          for (int i = 0; i < 16; i ++) {
            if (winningTeam == 1) {
              pixels.setPixelColor(i, red);
            } else {
              pixels.setPixelColor(i, green);
            }
          }
          pixels.show();
          ackBuzz();
          blinkStatus = true;
          lastBlink = millis();
        }
        
      } else {
        // we've blinked previously
        // is it time to blink off?
        if (millis() > lastBlink + BLINKRATE) {
          // yes, time to blink.
          for (int i = 0; i < 16; i ++) {
            pixels.setPixelColor(i, 0);
          }
          pixels.show();
          blinkStatus = false;
          lastBlink = millis();
        }
      }

      pixels.show();
      break;      
  }
}
