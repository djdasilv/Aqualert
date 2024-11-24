
#include <SD.h>
#include <TFT_eSPI.h>
#include "interface.h"
#include <pgmspace.h>

#define PEOPLE_MODE 1
#define VULNERABLE_MODE 2
#define WATER_LEVEL 3


// TFT display object
TFT_eSPI tft = TFT_eSPI();

// Image dimensions (replace with your actual dimensions)
const int imageWidth = 320;
const int imageHeight = 240;

void IRAM_ATTR handleUpButtonPress() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) {
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
  last_interrupt_time = interrupt_time;
}

void IRAM_ATTR handleOkButtonPress() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) {
      clearOldRect(tft);
      drawText(tft);
      
      if (getMode()<WATER_LEVEL) {
        setMode(getMode()+1);
      } else {
        sendInfo();
        setMode(4);
      }
  }
  last_interrupt_time = interrupt_time;
}


void setup() {
  // Initialize TFT display
  tft.begin();
  tft.fillScreen(TFT_WHITE); // Fill the screen with black at the start
  tft.setRotation(1);  // Set rotation to match your display orientation
  //Configure button pins as inputs
  pinMode(UP_BUTTON, INPUT);
  //pinMode(DOWN_BUTTON, INPUT);// Problème car 21 pour la backlight
  pinMode(OK_BUTTON, INPUT); 

  // Attach interrupts to the button pins
  attachInterrupt(digitalPinToInterrupt(UP_BUTTON), handleUpButtonPress, RISING);
  //attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON), handleDownButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(OK_BUTTON), handleOkButtonPress, RISING);

  // Draw the background image
  tft.pushImage(0,0,imageWidth,imageHeight,image);
}

uint8_t mode_copy;
void loop() {
  if(getMode()<=3){
    blinkingSquare(tft);
    drawText(tft);
  } else if (getMode()==4){
      tft.pushImage(0,0,imageWidth,imageHeight,image2);
  }
    usleep(500);
}


