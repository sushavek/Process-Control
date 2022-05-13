
/* This is the ON-OFF type of process controller which controls the SP and with the Hysteresis, LED will act as on and off. 
   This program will show how the On-Off process control will behave by using an LED and will display the value on the LCD display*/

int sensor= A1;                                           // Sensor pin for PV
int LED=10;                                               // LED pin
float PV= 0;                                              // Process variable = LED value
float Setpoint=0;                                         // SP starts at zero
int up=9;                                                 // Pin number for Push Button(UP
int down=8;                                               // Pin number for Push Button(DOWN)
int Hysteresis;                                           // can control the hysteresis value
float CO=0;                                               // can control the hysteresis value
int screen=0;                                             // zero means the main screen (or the beginning)
int nextP=13;                                             // Push Button for switching the screen (next screen)
int prevP=7;                                              // Push Button for the previous screen (previous screen)

#include <LiquidCrystal.h>                                // LCD display library
const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2;           // Pin numbers for each connection 
LiquidCrystal lcd(rs, en ,d4, d5, d6, d7);                // Name or Initials for each connection


void setup() {                                            // put your setup code here, to run once:
 pinMode(LED, OUTPUT);                                    // LED is an output 
 pinMode(9, INPUT);                                       // Pin 9 is the PB to increment a value
 pinMode(8, INPUT);                                       // Pin 8 is the PB to decrement a value                               
 pinMode(7, INPUT);                                       // Pin 7 is the PB to go to the next screen
 pinMode(13, INPUT);                                      // Pin 13 is the PB to go back to the previous screen
 Serial.begin(9600);                                      // 9600 bits per second(baud rate)
 lcd.begin(16,2);                                         // #columns, #rows
}

void loop() {                                             // put your main code here, to run repeatedly:
  if (Serial.available()){                                // gets the stored bytes from the serial port that are available for reading              
    Hysteresis=Serial.parseInt();                         // reads multiple characters and returns an integer number
  }
  
 PV= analogRead(sensor);                                  // read the value from the sensor, the value will be between 0-1024
 analogWrite(LED, CO);                                    // Writing the CO to the actuator
 if (PV <= (Setpoint-Hysteresis)) {                       // condition for CO when PV is less than the minimum of SP-Hysteresis
  CO=255;                                                 // CO=255, it means turn on the LED
 }
 else if (PV>=(Setpoint+Hysteresis)) {                    // condition for CO when PV is greater than the maximum of SP+Hysteresis
  CO=0;                                                   // CO=0, it means turn off the LED
 }
 
 up = digitalRead(9);                                     // Input pin 9 for PB UP
 down = digitalRead(8);                                   // Input pin 8 for PB DOWN    
 

                        /* Condition for changing screens */ 
 nextP=digitalRead(13);                                   // Condition to change screen to another screen
 if (nextP==1){ 
  screen=screen+1;
  lcd.clear();}
  if (screen>2){
   screen=2;
  }
  delay(100);
  
  prevP = digitalRead(7);                                // Condition to change screen to go to previous screen
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
    if (up == 1){                                     // condition of PB UP for set point
  Setpoint = Setpoint + 10;                           // SP with an increment of 10
 }
 if (down== 1){                                       // condition of PB DOWN for set point
 Setpoint = Setpoint - 10;                            // SP with an decrement of 10
 }

  if (Setpoint<0){                                    // caping for set point 
 Setpoint=0;}

                      /* LCD displays SP and PV on the first screen */
    lcd.setCursor(0,0);
    lcd.print("SP=");
    lcd.setCursor(3,0);
    lcd.print(Setpoint);
    lcd.setCursor(0,1);
    lcd.print("PV=");
    lcd.print(PV);
  }
  break;                                              // To end switch case
  
                    /* Switch case for the second screen display */  
  case 2: {
    if (up == 1){                                     // condition of PB UP for Hysteresis
  Hysteresis = Hysteresis + 1;                        // Hysteresis with an increment of 1
 }
 if (down == 1){                                      // condition of PB DOWN for Hysteresis
  Hysteresis = Hysteresis - 1;                        // Hysteresis with an decrement of 1 
 }
 
  if (Hysteresis<0){                                  // Caping for Hysteresis
 Hysteresis=0;
 }
                    /* LCD displays CO and Hysteresis on the second screen */ 
 lcd.setCursor(0,0);                                                           
 lcd.print("CO=");
 lcd.setCursor(3,0);
 lcd.print(CO);
 lcd.setCursor(0,1);
 lcd.print("Hyst=");
 lcd.setCursor(5,1);
 lcd.print(Hysteresis); 
  }
 break;                                              // To end switch case
  default:{                                          // To keep the main screen as the home page
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
 Serial.print("Hysteresis = ");
 Serial.println(Hysteresis);
 delay(200);
}
