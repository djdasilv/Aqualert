#include "interface.h"
#include "stdio.h"
#include <SPI.h>
#include "image.c"

void drawImageBackground(TFT_eSPI& tft) {
  // Draw the image stored in program memory as the background
  for (int y = 0; y < imageHeight; y++) {
    for (int x = 0; x < imageWidth; x++) {
      uint16_t color = ~(pgm_read_word(&image[y * imageWidth + x]));
      
      // Draw the pixel with the inverted color
      tft.drawPixel(x, y, color);
      //tft.drawPixel(x, y, color);
    }
  }
}
