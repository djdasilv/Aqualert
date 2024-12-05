#include "stdio.h"
#include <SPI.h>
#include "interface.h"

#define MAX_PEOPLE 15

//Modes 1= people, 2= vulnerable, 3= level
uint8_t RTC_DATA_ATTR mode = 1 ;
//Data that was entered by the people
uint8_t RTC_DATA_ATTR people = 1;
uint8_t RTC_DATA_ATTR vulnerable =0;
uint8_t RTC_DATA_ATTR level = 0;
const char RTC_DATA_ATTR *level_strings[4] = {
        "Ankle",
        "Knee",
        "Stomach",
        "Chest",
};

int RTC_DATA_ATTR squareSize = 80; // Size of the square
int RTC_DATA_ATTR squareX = 240;   // X position of the square (centered)
int RTC_DATA_ATTR squareY = 0;  // Y position of the square (centered)

//Constants for the interface
unsigned long lastToggleTime = 0; // Store last time the square toggled
unsigned long toggleInterval = 500; // Interval for blinking in milliseconds

bool squareVisible = true; // Flag to control visibility of the square


//Constants for the loop
const  uint16_t BlueBackgroundColor= 0xDF5F;


void drawText(TFT_eSPI& tft ){
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(5);
      tft.setCursor(260, 20);  // Set text position
      tft.print(people);  // Static text
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(5);
      tft.setCursor(260, 100);  // Set text position
      tft.print(vulnerable);  // Static text
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(2);
      tft.setCursor(245, 200);  // Set text position
      tft.print(level_strings[level]);  // Static text
};
void blinkingSquare(TFT_eSPI& tft){
  switch (mode) {
    case 1:
      squareX = 240;
      squareY = 0;
      break;
   case 2:
      squareX = 240;
      squareY = 80;
      break;
   case 3:
      squareX = 240;
      squareY = 160;
      break;
    }
   unsigned long currentTime = millis();  // Get the current time
  // Check if it's time to toggle the visibility of the rectangle
  if (currentTime - lastToggleTime >= toggleInterval) {
    lastToggleTime = currentTime;  // Update the last toggle time
    squareVisible = !squareVisible;
    // If the rectangle is visible, remove it by drawing a filled rectangle with background color
    
    if (squareVisible) {
      tft.fillRect(squareX, squareY, squareSize, squareSize-2, ~BlueBackgroundColor); // Clear the old rectangle
    } else {
      // Draw the new blinking rectangle
      tft.fillRect(squareX, squareY, squareSize, squareSize-2, ~TFT_RED); // Draw the border of the rectangle
    }
  }
}

void clearOldRect(TFT_eSPI& tft){
  tft.fillRect(squareX, squareY, squareSize, squareSize-2, ~BlueBackgroundColor); // Clear the old rectangle
}

uint8_t getPeople(){
  return people;
};
uint8_t getLevel(){
  return level;
};
uint8_t getVulnerable(){
  return vulnerable;
};

void setPeople(uint8_t amount){
  if ( amount > MAX_PEOPLE){
    people= 0;
  } else {
    people = amount;
  }
}
void setLevel(uint8_t amount){
  level = amount;
};
void setVulnerable(uint8_t amount){
  if (amount > getPeople()){
    vulnerable = 0;
  } else {vulnerable = amount;
  }
}

uint8_t getMode(){
  return mode;
}
void setMode(uint8_t amount){
  mode= amount;
}