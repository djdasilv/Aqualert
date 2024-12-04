#include "stdio.h"
#include <SPI.h>
#include "communication.h"
#include "LoRa.h"

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

void sendInfo(){
  LoRa.beginPacket();
  char text[4];
  snprintf( text, sizeof(text),"%d %d %d" ,getPeople(), getVulnerable(), getLevel()); 
  LoRa.print(text);
  LoRa.endPacket();
}