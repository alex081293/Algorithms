#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "filter.h"


#define PB PORTB 
#define startingSpeed 17
#define idealDistance 73

int a1[3] = {0x4000, 0x63E7, 0x290B};
int b1[3] = {0x0524, 0x0A48, 0x0524}; 
static int stop = 0;
static int leftWheelSpeed = 49;
static int rightWheelSpeed = 49;
static flag5 = 0;
static unsigned long int globalCounter = 0;
static int globalRampUp = 0;  
static int globalRampDown = 0;
static int startUpRamp = 1;
static int startUpRampComplete;
static unsigned int turnAroundFlag = 0;
static int turnAround = 0;
static int waitFlag = 0;
static int firstTime =0;
static int Lcounter = 0;
static int Rcounter = 0;
static int Lstep_count = 1;
static int Rstep_count = 1;

void rampUp() {
  static int speed2;

  if(globalCounter == 0) speed2 = leftWheelSpeed;
  globalCounter++;
  if(globalCounter%1000 == 0) speed2-=8;
  if(speed2 < 15) speed2 = startingSpeed;
  if(globalCounter == 10000) {
    globalRampUp = 0;
    globalCounter = 0;
    startUpRampComplete = 1;
    firstTime = 1;
   }
   leftWheelSpeed = rightWheelSpeed = speed2;
}

void rampDown() {
    static int speed; 
    if(globalCounter == 0) speed = startingSpeed;
    globalCounter++;
    if(globalCounter%900 == 0) speed+=8;
    if(ATD0DR0 >= 75) globalCounter == 10000;
    if(globalCounter == 9000) {
      globalRampDown = 0;
      globalCounter = 0;
      turnAroundFlag = 1;
    }
    leftWheelSpeed = rightWheelSpeed = speed;

}

void uTurn() {
  int i;
  turnAroundFlag = 1;
  leftWheelSpeed = rightWheelSpeed = 25;
  turnAround++;
  PTH = 0x00;
  if(turnAround == 0) {
    Lstep_count = 0;
    Rstep_count = 8;
  }
  // 8500 too little
  if(turnAround == 10000) {
   
    turnAround = 0;
    turnAroundFlag = 0;
    globalRampUp = 1;
    leftWheelSpeed = rightWheelSpeed = 60;


    Lcounter = 0;
    Rcounter = 0;
    Lstep_count = 1;
    Rstep_count = 1;
 //   for(i = 0; i<15; i++) frontDistance = butterworth0(4, a1, b1);
    PTH = 0xFF;
    waitFlag = 1; 
  } 
}

void main(void) {
   int i;
  // The distance to the wall we want to be at
  // Actual distance to the wall
  // Sets the CLK to 24 MHz
  SYNR  = 2;
  REFDV = 0;
  while(!(CRGFLG&0x08)){};
  CLKSEL |= 0x80;


  DDRH = 0xFF; 
  MCCTL |= 0x04;
  MCCNT = 150;  // 1/10 ms
  MCCTL = 0xC7; // enable counter, interrupt, modulus mode, prescale 1
  DDRB = 0xFF;
  
  ATD0CTL2 = 0x80;  
  ATD0CTL3 = 0x18;  
  ATD0CTL4 = 0x80;  
  
  startUpRamp = 1;
  EnableInterrupts;

  for(;;){
    ATD0CTL5 = 0x32;
    while (!(ATD0STAT0 & 0x80));
    while (!(MCFLG & 0x80));     
  };
}

#pragma TRAP_PROC
#pragma CODE_SEG __SHORT_SEG NON_BANKED
interrupt VectorNumber_Vtimmdcu void mdcuInterrupt () {
  static char LstepArray[8] = {0x08, 0x09, 0x01, 0x03, 0x02, 0x06, 0x04,0x0C};
  static char RstepArray[8] = {0x80, 0x90, 0x10, 0x30, 0x20, 0x60, 0x40,0xC0};
  static int rightDistance;
  static int leftDistance;
  static int frontDistance;
  static int currentE = 0;
  static int prevE = 0;
  static int errorDir = 0;  
  static int flag2 = 0;
  static int negFlag = 0;
  static int readCount = 0;
  static int lastWallF = 1;
  // ALWAYS CLEAR THE FLAG FIRST!
  MCFLG |= 0x80;
  
  
  Lcounter++;
  Rcounter++;
  
  readCount++;
  if(readCount == 5) {
    rightDistance = butterworth2(ATD0DR2H, a1, b1);
    leftDistance = butterworth1(ATD0DR1H, a1, b1);
    frontDistance =  butterworth0(ATD0DR0H, a1, b1);
    readCount = 0;
  }
    
 
  if(startUpRamp == 1) {
    rampUp();
    if(startUpRampComplete == 1)
      startUpRamp = 0;
  } 
  if((frontDistance > 50 && ATD0DR0H > 50 )|| turnAroundFlag == 1 || globalRampUp || globalRampDown) {
      flag2 = ATD0DR0H;
      if (firstTime == 1) {
        globalRampDown = 1;
        firstTime = 0; 
      } else if(globalRampDown == 1) rampDown();
      else if(turnAroundFlag == 1) uTurn();
      else if(globalRampUp == 1) rampUp();
   }    
   // left follow  
   if(lastWallF == 1) {
     if(leftDistance > 40 && globalRampDown == 0 && turnAroundFlag == 0) {
      lastWallF = 1;
      currentE = idealDistance - leftDistance;
      errorDir = currentE - prevE;   
      prevE = currentE;                       
      PTH = 0xF9;
      if(leftDistance > 72 && leftDistance < 76) {
        rightWheelSpeed = leftWheelSpeed = startingSpeed;
      } else {
         rightWheelSpeed = 13;
        leftWheelSpeed =  rightWheelSpeed + (.3 * currentE) + (.6 * errorDir);
      }//  stop = 0;
    }
    // right follow
    else if( rightDistance > 40 && globalRampDown == 0 && turnAroundFlag == 0) {
      lastWallF = 2;
      currentE = idealDistance - rightDistance;
      errorDir = currentE - prevE;
      prevE = currentE;
      PTH = 0xCF;
      if(rightDistance > 72 && rightDistance < 76) {
        leftWheelSpeed = rightWheelSpeed = startingSpeed;
      }
      else {
        leftWheelSpeed = 13;  
        rightWheelSpeed =  leftWheelSpeed + (.3 * currentE) + (.6 * errorDir);
        lastWallF = 1;
      }
     // stop = 0;
    }
    else if(globalRampDown == 0 && turnAroundFlag == 0){
      rightWheelSpeed = leftWheelSpeed = startingSpeed;
      //stop = 0;
    }
   } 
   else if(lastWallF == 2) {
    if( rightDistance > 40 && globalRampDown == 0 && turnAroundFlag == 0) {
    lastWallF = 2;
    currentE = idealDistance - rightDistance;
    errorDir = currentE - prevE;
    prevE = currentE;
    PTH = 0xCF;
    if(rightDistance > 72 && rightDistance < 76) {
      leftWheelSpeed = rightWheelSpeed = startingSpeed;
    }
    else {
      leftWheelSpeed = 13;  
      rightWheelSpeed =  leftWheelSpeed + (.3 * currentE) + (.6 * errorDir);
    }
   // stop = 0;
  } else if(leftDistance > 40 && globalRampDown == 0 && turnAroundFlag == 0) {
      lastWallF = 1;
      currentE = idealDistance - leftDistance;
      errorDir = currentE - prevE;   
      prevE = currentE;                       
      PTH = 0xF9;
      if(leftDistance > 72 && leftDistance < 76) {
        rightWheelSpeed = leftWheelSpeed = startingSpeed;
      } else {
         rightWheelSpeed = 13;
        leftWheelSpeed =  rightWheelSpeed + (.3 * currentE) + (.6 * errorDir);
      }//  stop = 0;
    }
  // right follow
  else if(globalRampDown == 0 && turnAroundFlag == 0){
    rightWheelSpeed = leftWheelSpeed = startingSpeed;
    lastWallF = 2;
    }
  }
     
  // Send current wheel signals
  if(stop != 1) PB = RstepArray[Rstep_count] + LstepArray[Lstep_count];

  // Checks to see if we need to move the mags
  if(Lcounter >= leftWheelSpeed) {
    Lcounter = 0;    
    Lstep_count++;
    if(Lstep_count==8) {
      Lstep_count=0;
    }
  }
  if(turnAroundFlag == 0) {   
    if(Rcounter >= rightWheelSpeed) {
      Rcounter = 0;   
      Rstep_count++;
      if(Rstep_count==8){
       Rstep_count=0;
     }
    }
  } else {
      if(Rcounter >= rightWheelSpeed) {
      Rcounter = 0;   
      Rstep_count--;
      if(Rstep_count==0){
       Rstep_count=8;
     }
    }
  }
}