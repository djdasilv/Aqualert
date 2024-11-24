
#include <SD.h>
#include <TFT_eSPI.h>

#include <pgmspace.h>
#include "interface.h"

#define PEOPLE_MODE 1
#define VULNERABLE_MODE 2
#define WATER_LEVEL 3

uint8_t mode = 1 ;

// TFT display object
TFT_eSPI tft = TFT_eSPI();

//Constants for the loop
unsigned long lastToggleTime = 0; // Store last time the square toggled
unsigned long toggleInterval = 100; // Interval for blinking in milliseconds
int squareSize = 80; // Size of the square
int squareX = 240;   // X position of the square (centered)
int squareY = 0;  // Y position of the square (centered)
bool squareVisible = true; // Flag to control visibility of the square
const uint16_t BlueBackgroundColor= 0xDF5F;

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


void setup() {
  // Initialize TFT display
  tft.begin();
  tft.fillScreen(TFT_WHITE); // Fill the screen with black at the start
  tft.setRotation(1);  // Set rotation to match your display orientation

  // Draw the background image
  //Try this one instead.
  //tft.pushImage(0,0,imageWidth,imageHeight,image);
  drawImageBackground(tft);
}

uint8_t mode_copy;
void loop() {
 unsigned long currentTime = millis();  // Get the current time

  // Check if it's time to toggle the visibility of the rectangle
  if (currentTime - lastToggleTime >= toggleInterval) {
    lastToggleTime = currentTime;  // Update the last toggle time

    // If the rectangle is visible, remove it by drawing a filled rectangle with background color
    if(mode == mode_copy){
      if (squareVisible) {
        tft.fillRect(squareX, squareY, squareSize, squareSize-2, ~BlueBackgroundColor); // Clear the old rectangle
      } else {
        // Draw the new blinking rectangle
        tft.fillRect(squareX, squareY, squareSize, squareSize-2, ~TFT_RED); // Draw the border of the rectangle
      }
    } else {
       tft.fillRect(squareX, squareY, squareSize, squareSize-2, ~BlueBackgroundColor); // Clear the old rectangle
    }
    

    // Toggle the visibility of the rectangle for the next loop
    squareVisible = !squareVisible;
  }
  
  // Change the rectangle coordinates based on the current mode
  switch (mode) {
    case PEOPLE_MODE:
      squareX = 240;
      squareY = 0;
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(5);
      tft.setCursor(squareX+20, squareY+20);  // Set text position
      tft.print(people);  // Static text
      break;
    case VULNERABLE_MODE:
      squareX = 240;
      squareY = 80;
      tft.setTextColor(TFT_WHITE);
       tft.setTextSize(5);
      tft.setCursor(squareX+20, squareY+20);  // Set text position
      tft.print(vulnerable);  // Static text
      break;
    case WATER_LEVEL:
      squareX = 240;
      squareY = 160;
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(2);
      tft.setCursor(squareX+5, squareY+40);  // Set text position
      tft.print(level_strings[level]);  // Static text
      break;
  }

  // Optionally, you can change the mode here based on some condition, e.g., button presses
  // For demonstration, let's cycle through modes every 3 seconds
    mode_copy = mode;
  if (currentTime % 3000 < 100) {
    mode = (mode == WATER_LEVEL) ? PEOPLE_MODE : mode + 1;  // Cycle through modes 1, 2, 3

  }
}


