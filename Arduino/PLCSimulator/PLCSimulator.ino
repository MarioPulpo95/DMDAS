
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(3, OUTPUT);

   long numero = 12;
  
  delay(1000);
  long durata1 = firstTransmission(numero);
  long durata2 = secondTransmission(numero);
  Serial.println(durata1);
  Serial.println(durata2);
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(durata1);                      // wait
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
  delay(500);

  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(durata2);                       // wait
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW

  
  delay(1000);
  
  
}

// the loop function runs over and over again forever
void loop() {
}

long firstTransmission(long num){

    long mask1 = 0b11111100000;
    long p1 = (num) & mask1;
    p1 = p1 >> 5;
    //Serial.println(p1);
    return 15*(p1+15);
  
  }

 long secondTransmission(long num){

    long mask2 = 0b00000011111;
    long p2 = (num) & mask2;
    //Serial.println(p2);
    
    
    
    return 15*(p2+15);
  
  }
  
