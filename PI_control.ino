
/* This program is a process control with a type of  PI control. PI control have no steady state error and 
this will be shown on the serial plotter by controlling an LED light */

/* Analog input value */
int sensor= A1;                         // Sensor pin for PV

/* Input values*/
float Error= 0;                         // Error value will be float
int PV= 0;                              // Process variable value
int CO= 0;                              // Control output value
int screen= 0;                          // Screen page
int Setpoint= 0;                        // Setpoint value
float pGain= 0;                         // pGain value
float iGain= 0;                         // iGain value
int Ti= 0;                              // Integral Action

unsigned long previousMillis= 0;        // declare variable
const long interval = 100;              // declare the interval in which the arduino will perform the control of the loop
unsigned long currentMillis;            // declare the variable to store the current millis value

/* Input pin number */
int up=9;                               // Push button UP pin
int down=8;                             // Push button DOWN pin
int LED=10;                             // LED pin number
int nextP=13;                           // "Next" screen
int prevP=7;                            // "Previous" screen

/*LCD library*/
#include <LiquidCrystal.h>                         // LCD display library
const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2;    // Pin numbers for each connection
LiquidCrystal lcd(rs, en ,d4, d5, d6, d7);         // Name or Initials for each connection


void setup() {                                     // put your setup code here, to run once:
 Serial.begin(9600);                               // 9600 bits per second(baud rate)
 lcd.begin(16,2);                                  // #columns, #rows
 pinMode(LED, OUTPUT);                             // LED is an outpu
 pinMode(9, INPUT);                                // Pin 9 is the PB to increment a value
 pinMode(8, INPUT);                                // Pin 8 is the PB to decrement a value
 pinMode(7, INPUT);                                // Pin 7 is the PB to go to the next screen
 pinMode(13, INPUT);                               // Pin 13 is the PB to go back to the previous screen
}


void loop() {                                     // put your main code here, to run repeatedly:
 analogWrite(LED, CO);                            // Writing the CO to the actuator
 PV= analogRead(sensor);                          // read the value from the sensor, the value will be between 0-1024
 
 currentMillis = millis();                        // reading the Current millis value

 if (currentMillis - previousMillis >= interval) {

 previousMillis = currentMillis;
 Error= Setpoint-PV;                               // Error is SP-PV
 Ti = (Ti + Error);                                // The accumulation of the error
 CO = (pGain*Error)+(iGain*Ti);                    // CO = P control + PI control
 
/*CO CAPING*/
 if (CO>255){                                      // Use the IF condition to set the limit of 255 max for CO
  CO=255;}

 if (CO<0) {                                       // Use the IF condition to set the limit of 255 min for CO
  CO = 0;}
 }
 
 /*PB input*/
 up = digitalRead(9);                              // Input pin 9 for PB UP
 down = digitalRead(8);                            // Input pin 8 for PB DOWN

/* Condition for changing screens */  
 nextP=digitalRead(13);                            // Condition to change screen to another screen
 if (nextP==1){ 
  screen=screen+1;
  lcd.clear();}
  if (screen>3){
   screen=3;
  }
  
  delay(100);
  prevP = digitalRead(7);                         // Condition to change screen to go to previous screen
if (prevP == 1){
  screen=screen-1;
  lcd.clear();
 if(screen<0){                                    // caping for screen limit
 screen = 0;
  }
  delay(100);
}

/* Switch case for the seocnd screen display */
 switch(screen){                     
  case 1:{                                        //SCREEN 2
    if (up == 1){                                 // condition of PB UP for set point
  Setpoint = Setpoint + 10;                       // SP with an increment of 10
 }
 if (down== 1){                                   // condition of PB DOWN for set point
 Setpoint = Setpoint - 10;}                       // SP with an decrement of 10
 
 if (Setpoint<0){                                 // caping for set point minimum
 Setpoint=0;}

/* LCD displays SP,PV and Error on the second screen */
    lcd.setCursor(0,0);
    lcd.print("SP=");
    lcd.setCursor(3,0);
    lcd.print(Setpoint);
    lcd.setCursor(0,1);
    lcd.print("PV=");
    lcd.print(PV);
  }
  break;                                         // To end switch case
  
/* Switch case for the third screen display */
  case 2: {                                      // SCREEN 3
    if (up == 1){                                // condition of PB UP for iGain
  iGain = iGain + 0.01;                          // iGain with an increment of 0.01
 }
 if (down == 1){                                 // condition of PB DOWN for iGain
  iGain = iGain - 0.01;                          // iGain with an decrement of 0.01
 
  if (iGain<0){                                  // caping for iGain(integral time)
 iGain=0;}
 }
 
/* LCD displays iGain, and Error on the second screen */ 
 lcd.setCursor(0,0);
 lcd.print("iGain=");
 lcd.setCursor(6,0);
 lcd.print(iGain);
 lcd.setCursor(0,1);
 lcd.print("Error=");
 lcd.setCursor(6,1);
 lcd.print(Error); 
  }
 break;

/* Switch case for the fourth screen display */
 case 3: {                                       // SCREEN 4
 if (up == 1){                                   // condition of PB UP for pGain
  pGain = pGain + 0.01;                          // pGain with an increment of 0.01
 }
 if (down == 1){                                 // condition of PB DOWN for pGain
  pGain = pGain - 0.01;                          // pGain with an decrement of 0.01 
 }
 
   if (pGain<0){                                 // Caping for pGain
 pGain=0;}

/* LCD displays CO, pGain, and Error on the second screen */ 
 lcd.setCursor(9,1);
 lcd.print("Ti=");
 lcd.setCursor(12,1);
 lcd.print(Ti);
 lcd.setCursor(0,0);
 lcd.print("Kp=");
 lcd.setCursor(3,0);
 lcd.print(pGain);
 lcd.setCursor(0,1);
 lcd.print("CO=");
 lcd.setCursor(3,1);
 lcd.print(CO); 
 }
 break;                                         // To end switch case
 
 default:{                                      // To keep the main(1st) screen as the home page
 lcd.setCursor(3,0);
 lcd.print("HOME PAGE");
 }
 }  
 
 /*  To read the values on the serial monitor  */  
 Serial.print("Setpoint = ");
 Serial.print(Setpoint);
 Serial.print("\t");
 Serial.print(" PV = ");
 Serial.print(PV); 
 Serial.print("\t");
 Serial.print(" pGain= ");
 Serial.print(pGain);
 Serial.print("\t");
 Serial.print("CO= ");
 Serial.print(CO);
 Serial.print("\t");
 Serial.print("iGain= ");
 Serial.println(iGain);
}
