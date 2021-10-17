//Serial Setup
#include "Parser_A01.h"
Parser parser(Serial);

void setup() {
  Serial.begin(9600);
  //pause(1);
  //Serial.println("1");
  
  
}


void loop() {

char cmd=Serial.read();
 if (cmd=='a'){
int c1=random(0, 5);  
Serial.println(c1);
 }
}

void print_measure(){   
   
   ///////////////////////////////////////
   //Print random value for serial test///
     int c1=random(0, 5);              ///
     //int c2=random(0, 10);             ///
   ///////////////////////////////////////
   
 //Asolute Distance
  Serial.println(c1);
  //Serial.print(";\t");
//Relative Distance
  //Serial.print(c2);
  //Serial.print(";\t");
}
