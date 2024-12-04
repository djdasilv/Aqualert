
#include <TFT_eSPI.h>
#include "interface.h"
#include "EasyButton.h"

#define PEOPLE_MODE 1
#define VULNERABLE_MODE 2
#define WATER_LEVEL 3
#define HOLD_DURATION 5000

//Timer pointer 
hw_timer_t *DisplayTimer = NULL;
uint32_t TimerCounter = 0;

EasyButton UpButton(UP_BUTTON);
EasyButton OkButton(OK_BUTTON);

// TFT display object
TFT_eSPI tft = TFT_eSPI();

// Image dimensions
const int imageWidth = 320;
const int imageHeight = 240;

//Callback function for when up button is pressed
void handleUpButtonPress() {
    switch (getMode()) {
      case 1:
        setPeople(getPeople()+1);
        break;

      case 2:
        setVulnerable(getVulnerable()+1);
        break;

      case 3:
        if (getLevel()>=WATER_LEVEL){
          setLevel(0);
          break; 
        } else{
          setLevel(getLevel()+1);
          break;}
    }
}

//Callback function for when any button is pressed
void onHoldAnyCallback(){
  if (getMode()==4) {
    tft.pushImage(0,0,imageWidth,imageHeight,image);
    setMode(PEOPLE_MODE);
  }
  
}
//Callback function for when ok button is pressed
void handleOkButtonPress() {
      clearOldRect(tft);
      drawText(tft);
      if (getMode()<WATER_LEVEL) {
        setMode(getMode()+1);
      } else {
        sendInfo();
        setMode(4);
      }
}

// Timer Interrupt Service Routine
void DisplayOff() {
  TimerCounter++;
  if(TimerCounter > 600 ){
    esp_sleep_enable_ext0_wakeup((gpio_num_t) UP_BUTTON,HIGH);
    esp_sleep_enable_ext0_wakeup((gpio_num_t) OK_BUTTON,HIGH);
    esp_deep_sleep_start();
  }
}

void setup() {
  // Initialize TFT display
  tft.begin();
  tft.fillScreen(TFT_WHITE); // Fill the screen with black at the start
  tft.setRotation(1);  // Set rotation to match your display orientation

  //Configure button pins as inputs
  OkButton.begin();
  UpButton.begin();

  OkButton.onPressedFor(HOLD_DURATION, onHoldAnyCallback);
  UpButton.onPressedFor(HOLD_DURATION, onHoldAnyCallback);

  UpButton.onPressed(handleUpButtonPress);
  OkButton.onPressed(handleOkButtonPress);

  //Timer config for routine callback every secon
  DisplayTimer = timerBegin(0, 80, true);  // Timer 0, prescaler 80 -> Clock @ 10MHz, count-up: true
  timerAttachInterrupt(DisplayTimer, &DisplayOff, true);
  timerAlarmWrite(DisplayTimer, 10000000, true); // 10,000,000 timer counts -> 1 seconds

  // Draw the background image
  tft.pushImage(0,0,imageWidth,imageHeight,image);
}

void loop() {
  OkButton.read();
  UpButton.read();
  if(getMode()<=WATER_LEVEL){
    blinkingSquare(tft);
    drawText(tft);
  } else if (getMode()==4){
      tft.pushImage(0,0,imageWidth,imageHeight,image2);
  }
    usleep(500);
}


