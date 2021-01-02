#include "LedControl.h"

/*
 * Pin 12, 11, 10 are for the SPI interface
 * Pin 12 is connected to the DATA IN-pin of the first MAX7221
 * Pin 11 is connected to the CLK-pin of the first MAX7221
 * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
 * Pin 1 is to indicate the number of MAX7219 used
 * */
LedControl lc=LedControl(12,11,10,1);

//Pins 
const int modeButton = 4;
const int startButton = 5;
const int stopButton = 7;
const int upButton = 2;
const int downButton = 3;
const int relay = 9;

// integer variables
//int startPress;
int modeStatus;
int startStatus;
int upStatus;
int downStatus;
int stopStatus;

int count = 0;
int mode=1;                                    //start at 1st mode

//Float variables
float start = millis();
float finish;
float elapsed = 0;
float timer;
float mode1Time = 3700.0;                      //mode 1 default time
float mode2Time = 1000.0;                      // mode 2 default time
float manualTimer = 0;

void setup(){

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  
  Serial.begin(9600);
  pinMode(modeButton, INPUT);
  pinMode(startButton, INPUT);
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(relay, OUTPUT);
  
  digitalWrite(modeButton, HIGH);
  digitalWrite(startButton, HIGH);
  digitalWrite(stopButton, HIGH);
  digitalWrite(upButton, HIGH);
  digitalWrite(downButton, HIGH);
  digitalWrite(relay, HIGH);
  intro ();
  intro ();
  
  lc.clearDisplay(0);
}

void loop(){
  modeStatus = digitalRead(modeButton);
  
  if (modeStatus == LOW){
    delay(50);
    modeStatus = digitalRead(modeButton);
    if (modeStatus == HIGH){
      mode = mode + 1;
      if (mode == 3){
        timerDisplay(manualTimer);
      } else if (mode == 4){
        lc.setRow(0,1,B00000000);
        lc.setRow(0,2,B00000000);
        lc.setRow(0,3,B00000000);
      }
      
      if (mode > 4){
        mode = 1;
      }
    }
  }
  switch(mode) {

    // Mode Carica Besar
    case 1:
      
      lc.setDigit (0,0,1,false);
      timer = mode1Time;
      timerDisplay(timer/100);
      startStatus = digitalRead(startButton);
      if (startStatus == LOW){
        delay(50);
        startStatus = digitalRead(startButton);
        if (startStatus == HIGH){
          clrDisplay();
          startPress(timer);
          
        }
      }
      break;

    //Mode Carica Kecil  
    case 2:
      
      lc.setDigit (0,0,2,false);
      timer = mode2Time;
      timerDisplay(timer/100);
      startStatus = digitalRead(startButton);
      if (startStatus == LOW){
        delay(50);
        startStatus = digitalRead(startButton);
        if (startStatus == HIGH){
          clrDisplay();
          startPress(timer);
          
        }
      }
      break;
    
    // Mode Manual
    case 3:
      lc.setDigit (0,0,3,false);
      timerDisplay(int (manualTimer * 10));
      
      start = millis();
      startStatus = digitalRead(startButton);
      
      if (startStatus == LOW){
        lc.setRow(0,1,B00000000);
        clrDisplay();
        elapsed = 0;
        while (startStatus == LOW){
          
          finish = millis();
          elapsed = (finish - start) / 1000;
          manualTimer = elapsed;
          int com = elapsed * 10;
          timerDisplay(com);
          Serial.println(com);
          digitalWrite(relay,LOW);
          startStatus = digitalRead(startButton);
        }
      digitalWrite(relay,HIGH);  
      
        
      } 
      break;

      case 4:
        lc.setRow (0,0,B01001110);
        counterDisplay(count);
  }

   if (mode == 1){
    
  
  
    upStatus = digitalRead (upButton);                 // Duration Up
    if (upStatus == LOW){
      delay(50);
      upStatus = digitalRead (upButton);
      if (upStatus == HIGH){
        mode1Time = mode1Time + 100;
      }
    }
  
    downStatus = digitalRead (downButton);
    if (downStatus == LOW){                           // Duration down
      delay(50);
      downStatus = digitalRead (downButton);
      if (downStatus == HIGH){
        mode1Time = mode1Time - 100;
        if (mode1Time <= 100){
          mode1Time = 100;
        }
      }
    }
  
  } else if (mode == 2){

    upStatus = digitalRead (upButton);                 // Duration Up
    if (upStatus == LOW){
      delay(50);
      upStatus = digitalRead (upButton);
      if (upStatus == HIGH){
        mode2Time = mode2Time + 100;
      }
    }
  
    downStatus = digitalRead (downButton);
    if (downStatus == LOW){                           // Duration down
      delay(50);
      downStatus = digitalRead (downButton);
      if (downStatus == HIGH){
        mode2Time = mode2Time - 100;
        if (mode2Time < 100){
          mode2Time = 100;
        }
      }
    }
  }
}  

  
  


// to display counter up to 9999 
void counterDisplay(int v) {
    int ones;
    int tens;
    int hundreds;
    int thousands;
    if (v == 0){
      lc.setDigit(0,3,0,false);
      lc.setDigit(0,2,0x16,false);
      lc.setDigit(0,1,0x16,false);
      lc.setDigit(0,0,0x16,false);
    } else if (v <= 9){
      ones = v%10;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,0x16,false);
      lc.setDigit(0,1,0x16,false);
      lc.setDigit(0,0,0x16,false);
      
    } else if (v <= 99){
      ones = v%10;
      v = v / 10;
      tens = v ;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,(byte)tens,false);
      lc.setDigit(0,1,0x16,false);
      lc.setDigit(0,0,0x16,false);
    } else if (v <= 999){
      ones = v%10;
      v = v / 10;
      tens = v % 10;
      v = v /10;
      hundreds = v;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,(byte)tens,false);
      lc.setDigit(0,1,(byte)hundreds,false);
      lc.setDigit(0,0,0x16,false);
    } else if (v <= 9999){
      ones = v%10;
      v = v / 10;
      tens = v % 10;
      v = v /10;
      hundreds = v % 10;
      v = v / 10;
      thousands = v;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,(byte)tens,false);
      lc.setDigit(0,1,(byte)hundreds,false);
      lc.setDigit(0,0,(byte)thousands,false);
    }
   
}


// to diplay timer with one decimal point

void timerDisplay(int v) {
    int ones;
    int tens;
    int hundreds;
    int thousands;
    if (v == 0){
      lc.setDigit(0,3,0,false);
      lc.setDigit(0,2,0,true);
      lc.setDigit(0,1,0x16,false);
      lc.setDigit(0,0,0x16,false);
    } else if (v <= 9){
      ones = v%10;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,0,true);
      lc.setDigit(0,1,0x16,false);
      lc.setDigit(0,0,0x16,false);
      
    } else if (v <= 99){
      ones = v%10;
      v = v / 10;
      tens = v ;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,(byte)tens,true);
      lc.setDigit(0,1,0x16,false);
      lc.setDigit(0,0,0x16,false);
    } else if (v <= 999){
      ones = v%10;
      v = v / 10;
      tens = v % 10;
      v = v /10;
      hundreds = v;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,(byte)tens,true);
      lc.setDigit(0,1,(byte)hundreds,false);
      lc.setDigit(0,0,0x16,false);
    } else if (v <= 9999){
      ones = v%10;
      v = v / 10;
      tens = v % 10;
      v = v /10;
      hundreds = v % 10;
      v = v / 10;
      thousands = v;
      lc.setDigit(0,3,(byte)ones,false);
      lc.setDigit(0,2,(byte)tens,false);
      lc.setDigit(0,1,(byte)hundreds,false);
      lc.setDigit(0,0,(byte)thousands,false);
    }
   
}


// to clear 2nd, 3rd, 4th digit (doesn't seems to work)
void clrDisplay(){
  lc.setDigit(0,1,0x16,false);
  lc.setDigit(0,2,0x16,false);
  lc.setDigit(0,3,0x16,false);
}

void stopAnimation(){
  lc.setRow(0,1,B01001111);
  lc.setRow(0,2,B00000101);
  lc.setRow(0,3,B00000101);
  delay(1000);
  lc.setRow(0,1,B00000000);
  lc.setRow(0,2,B00000000);
  lc.setRow(0,3,B00000000);
  delay(1000);
  lc.setRow(0,1,B01001111);
  lc.setRow(0,2,B00000101);
  lc.setRow(0,3,B00000101);
  delay(1000);
   lc.setRow(0,1,B00000000);
  lc.setRow(0,2,B00000000);
  lc.setRow(0,3,B00000000);
  delay(1000);
  lc.setRow(0,1,B01001111);
  lc.setRow(0,2,B00000101);
  lc.setRow(0,3,B00000101);
  delay(1000);
  lc.setRow(0,1,B00000000);
  lc.setRow(0,2,B00000000);
  lc.setRow(0,3,B00000000);
  delay(1000);
   
}

// to press and display current time
void startPress (float x){
  clrDisplay();
  start = millis();
  Serial.println(String("x = ")+String(x));
  Serial.println(String("timer = ")+String(timer));
  while (timer >= elapsed){

    stopStatus = digitalRead(stopButton);
    if (stopStatus == LOW){
      digitalWrite(relay, HIGH);
      stopAnimation();
      count = count - 1;
      break;
    }

    
    finish = millis();
    digitalWrite (relay, LOW);
    elapsed = finish - start;
    Serial.println(String("finish = ")+String(finish));
    Serial.println(String("start = ")+String(start));
    Serial.println(String("elapsed = ")+String(elapsed));
    int temp = (x - elapsed) / 100;
    Serial.println(String("temp = ")+String(temp));
    timerDisplay(temp);
   
    
        
  }
  count = count + 1;
  digitalWrite (relay, HIGH);    
  elapsed = 0 ;
}

void intro (){
  byte arr1[18] = {B0000000, B00000000, B00000000, B00000001,B00000001,
                   B01001110, B01110111, B01000110, B00110000,B01001110, B01110111,
                   B00000000, B01110000, B11111111,
                   B00000001, B00000001, B00000000, B00000000};
  byte arr2[18] = {B00000000, B00000000, B00000001,B00000001,
                   B01001110, B01110111, B01000110, B00110000,B01001110, B01110111,
                   B00000000, B01110000, B11111111,
                   B00000001, B00000001, B00000000, B00000000, B0000000};            
  byte arr3[18] = {B00000000, B00000001, B00000001,
                   B01001110, B01110111, B01000110, B00110000,B01001110, B01110111,
                   B00000000, B01110000, B11111111,
                   B00000001, B00000001, B00000000, B00000000, B0000000, B00000000};
  byte arr4[18] = {B00000001,B00000001,
                   B01001110, B01110111, B01000110, B00110000,B01001110, B01110111,
                   B00000000, B01110000, B11111111,
                   B00000001, B00000001, B00000000, B00000000, B0000000, B00000000, B00000000};
  
  for (int i = 0; i<=17; i++){
    //lc.setRow(0,0,arr1[i]);
    lc.setRow(0,1,arr2[i]);
    lc.setRow(0,2,arr3[i]);
    lc.setRow(0,3,arr4[i]);
    delay(100);
  }
}
