
/*  This program is a P control that is one of the process control. P control will show the error by using an LED   */

/*Analog input pin*/
int sensor= A1;                                    // Sensor pin for PV

/* Input values */
float Error= 0;                                    // Error value
int PV= 0;                                         // Process Variable value
int CO= 0;                                         // Control output value
int screen= 0;                                     // Screen page
int Setpoint= 0;                                   // Setp[oint value
float pGain= 0;                                    // pGain value

/* Input pin number */
int up=9;
int down=8;
int LED=10;
int nextP=13;
int prevP=7;

#include <LiquidCrystal.h>                        // LCD display library
const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2;   // Pin numbers for each connection
LiquidCrystal lcd(rs, en ,d4, d5, d6, d7);        // Name or Initials for each connection


void setup() {                                    // put your setup code here, to run once:
 pinMode(LED, OUTPUT);                               // LED is an output 
 pinMode(9, INPUT);                                  // Pin 9 is the PB to increment a value
 pinMode(8, INPUT);                                  // Pin 8 is the PB to decrement a value  
 pinMode(7, INPUT);                                  // Pin 7 is the PB to go to the next screen
 pinMode(13, INPUT);                                 // Pin 13 is the PB to go back to the previous screen
 Serial.begin(9600);                                 // 9600 bits per second(baud rate)
 lcd.begin(16,2);;                                   // #columns, #rows
}

void loop() {                                    // put your main code here, to run repeatedly:
 analogWrite(LED, CO);                               // Writing the CO to the actuator
 PV= analogRead(sensor);                             // read the value from the sensor, the value will be between 0-1024
 Error= Setpoint-PV;                                 // Error is the difference of Setpoint and the Process variable
 CO = pGain*Error;                                   // CO is the multiplication of Gain and Error
                          /*CO CAPING*/
 if (CO>255){                                        // Use the IF condition to set the limit of 255 max for CO
  CO=255;}

 if (CO<0) {                                         // Use the IF condition to set the limit of 255 min for CO
  CO = 0;}

                          /*PB input*/
 up = digitalRead(9);                                // Input pin 9 for PB UP
 down = digitalRead(8);                              // Input pin 8 for PB DOWN 
 
 
                         /* Condition for changing screens */ 
nextP=digitalRead(13);                               // Condition to change screen to another screen
  if (nextP==1){ 
  screen=screen+1;
  lcd.clear();}
  if (screen>2){
   screen=2;
  }
  delay(100);
prevP = digitalRead(7);                             // Condition to change screen to go to previous screen
  if (prevP == 1){
  screen=screen-1;
  lcd.clear();
  if(screen<0){
  screen = 0;
  }
  delay(100);
}
 
                       /* Switch case for the first screen display */
 switch(screen){
  case 1:{
    if (up == 1){                                 // condition of PB UP for set point
  Setpoint = Setpoint + 10;                       // SP with an increment of 10
 }
  if (down== 1){                                  // condition of PB DOWN for set point
 Setpoint = Setpoint - 10;                        // SP with an decrement of 10
 }

  if (Setpoint<0){                                // caping for set point minimum
 Setpoint=0;}
 
                       /* LCD displays SP,PV and Error on the second screen */
    lcd.setCursor(0,0);
    lcd.print("SP=");
    lcd.setCursor(3,0);
    lcd.print(Setpoint);
    lcd.setCursor(0,1);
    lcd.print("PV=");
    lcd.print(PV);
    lcd.setCursor(6,0);
    lcd.print("Err:");
    lcd.setCursor(10,0);
    lcd.print(Error);
  }
  break;                                         // To end switch case

  
                      /* Switch case for the third screen display */
  case 2: {
    if (up == 1){                                // condition of PB UP for pGain
  pGain = pGain + 0.01;                          // Hysteresis with an increment of 0.01
 }
 if (down == 1){                                 // condition of PB DOWN for pGain
  pGain = pGain - 0.01;                          // pGain with an decrement of 0.01 
 }
  if (pGain<0){                                  // Caping for pGain
 pGain=0;}
 
                    /* LCD displays CO, pGain, and Error on the second screen */ 
 lcd.setCursor(9,0);
 lcd.print("CO=");
 lcd.setCursor(12,0);
 lcd.print(CO);
 lcd.setCursor(0,0);
 lcd.print("Kp:");
 lcd.setCursor(3,0);
 lcd.print(pGain);
 lcd.setCursor(0,1);
 lcd.print("Err:");
 lcd.setCursor(4,1);
 lcd.print(Error); 
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
 Serial.print("pGain= ");
 Serial.print(pGain);
 Serial.print("\t");
 Serial.print("CO= ");
 Serial.print(CO);
 Serial.print("\t");
 Serial.print(" Error = ");
 Serial.println(Error);
 
 delay(200);














  
 }
 
