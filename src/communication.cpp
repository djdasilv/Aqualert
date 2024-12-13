#include "stdio.h"
#include <SPI.h>
#include "communication.h"
#include "LoRa.h"


#define MAX_PEOPLE 15
#define DEVICE_ID 285623

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
  mode = amount;
}

void lora_begin(){
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(5,14,2);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(7.8E3);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();
  LoRa.setGain(6);

  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF1);
  Serial.println("LoRa Initializing OK!");
}

void sendInfo(){
  RTC_DATA_ATTR static int counter=0;
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("ID "+ String(DEVICE_ID)+
  " ; Loc [46.520402, 6.564847]"+
  " ; Answer ["+ String(getPeople())+","+String(getVulnerable())+","+String(getLevel())+
  "] ; Counter:"+ counter); 
  LoRa.endPacket();
  counter++;
}

void sendReset(){
  LoRa.beginPacket();
  LoRa.print("ID "+ String(DEVICE_ID)+" ; Loc [46.520402, 6.564847] ; Answer [0,0,0] ; Counter:");
  LoRa.endPacket();
}