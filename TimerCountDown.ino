/*===================================================================================
 * Timer Countdown function using ESP32 Arduino Environment and 4 digit display module via I2C communication
 * 
 * Author: David Zheng 
 * ================================================================================== */

#include <Wire.h>

#define DigitNum 4 //Total number of digits
#define TM1650_DISPLAY_BASE 0x34 // Address of the left-most digit 
#define TM1650_DCTRL_BASE   0x24 // Address of the control register of the left-most digit
#define TotalTime 180 //Start time to count-down from;

//Declear subfunction
void displayToModule (int digit[]);

//Declear global variables
int digitDisplay[10] = {63,6,91,79,102,109,125,7,127,111};//TM1650 data inputs for number 0-9
hw_timer_t * timer = NULL;
int Time, tempTic;

void setup() {
  Wire.begin();
  Serial.begin(115200); 
  for(int i=0; i<DigitNum; i++) {
      // Turn on display 
      Wire.beginTransmission(TM1650_DCTRL_BASE+i);      
      Wire.write((byte) 1);
      Wire.endTransmission();
      // Clear display
      Wire.beginTransmission(TM1650_DISPLAY_BASE+i); 
      Wire.write((byte) 0);
      Wire.endTransmission();
  }
  Time=-1;
  tempTic = 0;
  timer = timerBegin(0, 80, true);  
}


void loop() {
  //Reset Timer
  if(Time < 0) {
    Time = TotalTime;
    tempTic = timerRead(timer);
  }
  //Countdown every one second
  if(timerRead(timer) - tempTic >=1000000) {
    
    tempTic = timerRead(timer);
    Time=Time-1;  
  }
  Serial.println(timerRead(timer)); 
  //Convert seconds MM:SS format
  int digit[DigitNum];
  digit[0] = Time/600;
  digit[1] = (Time-digit[0]*600)/60;
  digit[2] = (Time-digit[0]*600-digit[1]*60)/10;
  digit[3] = (Time-digit[0]*600-digit[1]*60-digit[2]*10);
  
  displayToModule(digit);
}

void displayToModule (int digit[]) {
  for(int i=0; i<DigitNum; i++) {
      Wire.beginTransmission(TM1650_DISPLAY_BASE+i); 
      
      if(i==1) { //add dot after second digit
        Wire.write((byte)digitDisplay[digit[i]] | 0b10000000);
      } else {
        Wire.write((byte)digitDisplay[digit[i]]);
      }      
      Wire.endTransmission();
  }
}
