#include "EasyButton.h"
#include "communication.h"

#define PEOPLE_MODE 1
#define VULNERABLE_MODE 2
#define WATER_LEVEL 3
#define SEND_INFO 4
#define HOLD_DURATION 5000

EasyButton UpButton(UP_BUTTON);
EasyButton OkButton(OK_BUTTON);

uint32_t timeLastAlert = 300;

//Timer pointer 
hw_timer_t *My_timer = NULL;

// Timer Interrupt Service Routine
void InfoRoutine() {
    if (getMode()==SEND_INFO){
      timeLastAlert++;
      if (timeLastAlert >= 300) { // 5 minutes = 300 seconds
          timeLastAlert = 0; // Reset the counter
          sendInfo();  // Call your 5-minute task
      }
    }
}

//Callback function for when up button is pressed
void handleUpButtonPress() {
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
          break;
        }
    }
}

//Callback function for when any button is pressed
void onHoldAnyCallback(){
  if (getMode()==4) {
    setMode(PEOPLE_MODE);
    timerAlarmDisable(My_timer);
    timerRestart(My_timer); 
  }
}

//Callback function for when ok button is pressed
void handleOkButtonPress() {
      if (getMode()<WATER_LEVEL) {
        setMode(getMode()+1);
      } else {
        setMode(4);
        timerAlarmEnable(My_timer);
      }
}

void setup() {

  //Configure button pins as inputs
  OkButton.begin();
  OkButton.onPressedFor(HOLD_DURATION, onHoldAnyCallback);
  OkButton.onPressed(handleOkButtonPress);
  
  UpButton.begin();
  UpButton.onPressedFor(HOLD_DURATION, onHoldAnyCallback);
  UpButton.onPressed(handleUpButtonPress);

  //Timer config for routine callback every secon
  My_timer = timerBegin(0, 80, true);  // Timer 0, prescaler 80 -> Clock @ 10MHz, count-up: true
  timerAttachInterrupt(My_timer, &InfoRoutine, true);
  timerAlarmWrite(My_timer, 10000000, true); // 10,000,000 timer counts -> 1 seconds
}

void loop() {
  OkButton.read();
  UpButton.read();
  
  usleep(500);
}


