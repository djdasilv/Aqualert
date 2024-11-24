#ifndef HEADER_INTERFACE_H  // Check if HEADER_FILE_NAME_H is not defined
#define HEADER_INTERFACE_H  // Define the macro

#include <TFT_eSPI.h>
#include "image.c"
#include "image2.c"

#define UP_BUTTON 35
#define DOWN_BUTTON 21
#define OK_BUTTON 22

void drawText(TFT_eSPI& tft);
void blinkingSquare(TFT_eSPI& tft);
void clearOldRect(TFT_eSPI& tft);
uint8_t getPeople();
uint8_t getLevel();
uint8_t getVulnerable();
void setPeople(uint8_t amount);
void setLevel(uint8_t amount);
void setVulnerable(uint8_t amount);
uint8_t getMode();
void setMode(uint8_t amount);
void sendInfo();

#endif



