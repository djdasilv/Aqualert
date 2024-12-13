
#include <TFT_eSPI.h>
#include "interface.h"
#include <esp_sleep.h>
#include "button2.h"

#define PEOPLE_MODE 1
#define VULNERABLE_MODE 2
#define WATER_LEVEL 3
#define SEND_INFO 4 
#define HOLD_DURATION 2000

#define BUTTON_PIN_BITMASK(GPIO)      (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex
#define WAKEUP_GPIO_27                GPIO_NUM_27     // Only RTC IO are allowed - ESP32 Pin example
#define WAKEUP_GPIO_22                GPIO_NUM_22     // Only RTC IO are allowed - ESP32 Pin example
uint64_t bitmask =BUTTON_PIN_BITMASK  (WAKEUP_GPIO_27);//|BUTTON_PIN_BITMASK(WAKEUP_GPIO_22);

RTC_DATA_ATTR bool send = false;

RTC_DATA_ATTR Button2 UpButton;
RTC_DATA_ATTR Button2 OkButton;

RTC_DATA_ATTR uint32_t timeButtonUp = 0 ;
RTC_DATA_ATTR uint32_t timeButtonOk = 0 ;

// TFT display object
RTC_DATA_ATTR TFT_eSPI tft = TFT_eSPI();

// Image dimensions
RTC_DATA_ATTR const int imageWidth = 240;
RTC_DATA_ATTR const int imageHeight = 320;

void clearRTC() {
  volatile uint32_t* rtc_mem = (uint32_t*) 0x50000000; // RTC slow memory base address
  for (int i = 0; i < 128; i++) {
    rtc_mem[i] = 0; // Clear 512 bytes (128 * 4 bytes)
  }
}

//Callback function for when up button is pressed
void handleUpButtonPress(Button2& b) {
    timeButtonUp = millis();
    switch (getMode()) {
      case PEOPLE_MODE:
        setPeople(getPeople()+1);
        break;

      case VULNERABLE_MODE:
        setVulnerable(getVulnerable()+1);
        break;

      case WATER_LEVEL:
        if (getLevel()>=WATER_LEVEL){
          setLevel(0);
          break; 
        } else{
          setLevel(getLevel()+1);
          break;}
    }
}

//Callback function for when any button is pressed
void onHoldAnyCallback(Button2& b){
    Serial.println("Holding");
    if (b.getPin()==OK_BUTTON) timeButtonOk = millis();
    if (b.getPin()==UP_BUTTON) timeButtonUp = millis();
    if (getMode()==4) {
      tft.pushImage(0,0,imageWidth,imageHeight,image);
      setMode(1);
    }
}
//Callback function for when ok button is pressed
void handleOkButtonPress(Button2& b) {
      timeButtonOk = millis();
      clearOldRect(tft);
      drawText(tft);
      if (getMode()<WATER_LEVEL) {
        setMode(getMode()+1);
      } else {
        send=true;
        setMode(SEND_INFO);
      }
}

void setup() {
  // Initialize TFT display
  tft.begin();
  tft.fillScreen(TFT_WHITE); // Fill the screen with black at the start
  tft.setRotation(2);  // Set rotation to match your display orientation

  Serial.begin(115200);
  //Configure button pins as inputs
  OkButton.begin(OK_BUTTON,INPUT_PULLDOWN,false);
  UpButton.begin(UP_BUTTON,INPUT_PULLDOWN,false);
  timeButtonUp = millis();
  timeButtonOk = millis();

  UpButton.setLongClickTime(HOLD_DURATION);
  OkButton.setLongClickTime(HOLD_DURATION);
  UpButton.setLongClickDetectedHandler(onHoldAnyCallback);
  OkButton.setLongClickDetectedHandler(onHoldAnyCallback);
  UpButton.setClickHandler(handleUpButtonPress);
  OkButton.setClickHandler(handleOkButtonPress);

  esp_sleep_enable_ext1_wakeup(bitmask, ESP_EXT1_WAKEUP_ANY_HIGH);
  if(getMode()==SEND_INFO) send=true;

  // Draw the background image
  tft.pushImage(0,0,imageWidth,imageHeight,image);
}
#define INTERVAL 10000
void loop() {
  
  OkButton.loop();
  UpButton.loop();
  //Drawing selection square
  if(getMode()<=WATER_LEVEL){
    blinkingSquare(tft);
    drawText(tft);
  } else if (getMode()==4){
    tft.pushImage(0,0,imageWidth,imageHeight,image2);
  }
  if (send and min(millis()-timeButtonOk,millis()-timeButtonUp)>INTERVAL ){
    send=false;
    Serial.println("Going to sleep");
    Serial.println(OkButton.read());
    esp_deep_sleep_start();
  }
  if(min(millis()-timeButtonOk,millis()-timeButtonUp)>INTERVAL){
    esp_deep_sleep_start();
  }
  if (OkButton.isPressed() && UpButton.isPressed()){
    clearRTC();
    esp_restart();
  }
}


