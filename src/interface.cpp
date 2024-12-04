#include "stdio.h"
#include <SPI.h>
#include "interface.h"

#define MAX_PEOPLE 15

//Modes 1= people, 2= vulnerable, 3= level
uint8_t mode = 1 ;
//Data that was entered by the people
uint8_t people = 1;
uint8_t vulnerable =0;
uint8_t level = 0;
const char *level_strings[4] = {
        "Ankle",
        "Knee",
        "Stomach",
        "Chest",
};

int squareSize = 80; // Size of the square
int squareX = 240;   // X position of the square (centered)
int squareY = 0;  // Y position of the square (centered)

//Constants for the interface
unsigned long lastToggleTime = 0; // Store last time the square toggled
unsigned long toggleInterval = 500; // Interval for blinking in milliseconds

bool squareVisible = true; // Flag to control visibility of the square


//Constants for the loop
const uint16_t BlueBackgroundColor= 0xDF5F;


void drawText(TFT_eSPI& tft ){
  switch (mode) {
    case 1:
      squareX = 240;
      squareY = 0;
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(5);
      tft.setCursor(squareX+20, squareY+20);  // Set text position
      tft.print(people);  // Static text
      break;
    case 2:
      squareX = 240;
      squareY = 80;
      tft.setTextColor(TFT_WHITE);
       tft.setTextSize(5);
      tft.setCursor(squareX+20, squareY+20);  // Set text position
      tft.print(vulnerable);  // Static text
      break;
    case 3:
      squareX = 240;
      squareY = 160;
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(2);
      tft.setCursor(squareX+5, squareY+40);  // Set text position
      tft.print(level_strings[level]);  // Static text
      break;
  }
};
void blinkingSquare(TFT_eSPI& tft){
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

void sendInfo(){
  
}