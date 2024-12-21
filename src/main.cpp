#include "communication.h"
#include "LoRa.h"
#include <esp_sleep.h>
#include "Button2.h"
#include <esp32/rom/rtc.h>


#define PEOPLE_MODE 1
#define VULNERABLE_MODE 2
#define WATER_LEVEL 3
#define SEND_INFO 4
#define HOLD_DURATION 2000

Button2 UpButton;
Button2 OkButton;

uint32_t timeButtonUp = 0 ;
uint32_t timeButtonOk = 0 ;

bool send=false;

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex
#define WAKEUP_GPIO_26              GPIO_NUM_26     // Only RTC IO are allowed - ESP32 Pin example
#define WAKEUP_GPIO_27              GPIO_NUM_27     // Only RTC IO are allowed - ESP32 Pin example
uint64_t bitmask =  BUTTON_PIN_BITMASK(WAKEUP_GPIO_27);

RTC_DATA_ATTR bool firstSend= true;
RTC_DATA_ATTR uint16_t counter= 0;

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
        Serial.println("Amount of people:" + String(getPeople()));
        break;
      case VULNERABLE_MODE:
        setVulnerable(getVulnerable()+1);
        Serial.println("Amount of vulnerable: "+ String(getVulnerable()));
        break;
      case WATER_LEVEL:
        if (getLevel()>=WATER_LEVEL){
          setLevel(0);
          Serial.println("Water Level:" + String(getLevel()));
          break; 
        } else{
          setLevel(getLevel()+1);
          Serial.println("Water Level: " + String(getLevel()));
          break;
        }
    }
}

//Callback function for when any button is pressed
void onHoldAnyCallback(Button2& b){
  if (b.getPin()==OK_BUTTON) timeButtonOk = millis();
  if (b.getPin()==UP_BUTTON) timeButtonUp = millis();
  if (getMode()==4) {
    setMode(1);
    send=false;
  }
}

//Callback function for when ok button is pressed
void handleOkButtonPress(Button2& b) {
      timeButtonOk = millis();
      if (getMode()<WATER_LEVEL) {
        setMode(getMode()+1);
      } else if (getMode()==WATER_LEVEL and firstSend==true) {
        setMode(4);
        Serial.println("Sending info "+ String(send));
        send=true;
        firstSend= false;
        sendInfo();
      } else {
        setMode(4);
        Serial.println("Sending info "+ String(send));
        send=true;
      }
      Serial.println("Next Mode: "+ String(getMode()));
}

void setup() {
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

  lora_begin();

  esp_sleep_enable_ext1_wakeup(bitmask, ESP_EXT1_WAKEUP_ANY_HIGH);

  const uint64_t uS_TO_S_FACTOR = 1000000; // Conversion factor
  const uint64_t SLEEP_TIME = 0;        // 5 minutes
  esp_sleep_enable_timer_wakeup(SLEEP_TIME * uS_TO_S_FACTOR);
  esp_sleep_enable_ext1_wakeup(bitmask, ESP_EXT1_WAKEUP_ANY_HIGH);
  if(getMode()==SEND_INFO) send=true;
}

#define INTERVAL 5000
void loop() {
  OkButton.loop();
  UpButton.loop();
  if (send and min(millis()-timeButtonOk,millis()-timeButtonUp)>INTERVAL){
    sendInfo();
    send=false;
    Serial.println("Going to sleep");
    esp_deep_sleep_start();
  }
  if(min(millis()-timeButtonOk,millis()-timeButtonUp)>INTERVAL){
    Serial.println("Going to sleep");
    esp_deep_sleep_start();
  }
  if (OkButton.isPressed() && UpButton.isPressed()){
    clearRTC();
    esp_restart();
    sendReset();
  }
}



