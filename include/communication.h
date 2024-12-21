#ifndef HEADER_INTERFACE_H  // Check if HEADER_FILE_NAME_H is not defined
#define HEADER_INTERFACE_H  // Define the macro

#include <TFT_eSPI.h>

#define UP_BUTTON 27
#define OK_BUTTON 26

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
void lora_begin();
void sendInfo();
void sendReset();

#endif



