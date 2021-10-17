//Serial Setup
#include "Parser_A01.h"
Parser parser(Serial);

void setup() {
  Serial.begin(9600);
  //pause(1);
  //Serial.println("1");
  
  
}


void loop() {
 //SErial 
  char *s_ptr;
  parser.read();
  if (parser.isOverflow())
  {
    Serial.println("Error: command buffer overflow.");
  }
  s_ptr = parser.nextToken();
  if (s_ptr && strlen(s_ptr) == 1) {
  char  ccmd = s_ptr[0];
    Serial.println("1");
    switch (ccmd)
    {
    case 'a':
      print_measure();
    default:
        Serial.println("");
    }
  }
}





void print_measure(){   
   
   ///////////////////////////////////////
   //Print random value for serial test///
     int c1=random(0, 5);              ///
     int c2=random(0, 10);             ///
   ///////////////////////////////////////
   
 //Asolute Distance
  Serial.print(c1);
  Serial.print(";\t");
//Relative Distance
  Serial.print(c2);
  Serial.print(";\t");
}
