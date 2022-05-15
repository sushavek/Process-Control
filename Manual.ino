
/*
This program is a manual control to control a LED by using an LDC display with push buttons and an LDR to know the range of the resistance of LDR.  

*/

int sensor= A1;                         // Sensor pin for PV
int inputPin= A5;                       // Potentiometer pin if using the potentiometer as the CO
int actuator=10;                        // LED pin
float PV= 0;                            // Process variable = LED value
int inputValue= A0;                     // Sensor Value for CO if using potentiometer
float CO= 0;                            // This means that CO(Control Output) value starts with 0
                     
             /*using PB to control CO*/
                     
int up=9;                               // Pin number for Push Button(UP) 
int down=8;                             // Pin number for Push Button(DOWN)


#include <LiquidCrystal.h>                             // LCD display library             
const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2;        // Pin numbers for each connection             
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);             // Name or Initials for each connection


void setup() {                                         // put your setup code here, to run once:                                                      
 pinMode(actuator, OUTPUT);                            // Actuator = LED as an output
 Serial.begin(9600);                                   // 9600 bits per sec second(baud rate)                     
 lcd.begin(16,2);                                      // columns, rows
}

void loop() {                                          // put your main code here, to run repeatedly:
 PV= analogRead(sensor);                               // read the value from the sensor, the value will be between 0-1024

                       /* These two comments (inputValue & CO) are for potentiometer when in use */
 //inputValue= analogRead(A0);
 //CO= map(inputValue, 0, 1023, 0, 255);

                      /* PB conditions and is in use */ 
 up = digitalRead(9);                                  // When UP PB is pressed, CO increment to one
 if (up ==1){
  CO= CO + 1;
 }
 down = digitalRead(8);                                // When DOWN PB is pressed, CO decrement to one
 if (down== 1){
  CO= CO - 1;
 }
 if (up > 255) {                                      // UP PB is at 255, CO will stay at 255 = Caping
  CO = 255;                                           // the maximum value of CO it can get

 if (down < 0) {                                      // DOWN PB is at 0, CO will stay at 0 = Caping
  CO = 0;                                             // the minimum value of CO it can get
 }
 }
 analogWrite(actuator, CO);                            // Writing the CO to the actuator
                             
                        /*  To read the values on the serial monitor  */                             
 Serial.print("CO = ");
 Serial.print(CO);
 Serial.print("\t");                                  // To have column on the serial monitor
 Serial.print(" PV = ");
 Serial.println(PV);                                  // Read values vertically
             
                       /* LCD displays CO */   
 lcd.setCursor(0,0);
 lcd.print("CO="); 
 lcd.setCursor(4,0);
 lcd.print(CO);
                      /* LCD displays PV */
 lcd.setCursor(0,1);
 lcd.print("PV=");
 lcd.setCursor(4,1);
 lcd.print(PV);
 delay(200);
}
