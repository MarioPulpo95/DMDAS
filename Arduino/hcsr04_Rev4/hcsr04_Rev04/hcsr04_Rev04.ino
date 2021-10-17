/*

  Created on 2020/01/31

  History
  -28/03/2021 Aggiunta comunicazione Seriale
  -27/03/2021 Comunicazione col PLC più efficiente
  -15/03/2021 Aggiunta comunicazione col PLC + gestione motori
  -2/02/2021  Aggiunta del clock a 100kHz, per il PLC sul Pin 12 di arduino.
  -31/01/2021 Gestione della misura di distanza effettuata con HCSR04.
  

*/

///*----------------------------------------------Library---------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
///*----------------------------------------------Define---------------------------------------------------------*/
//Sensor Constant
#define triggerPort  4
#define echoPort  7



//Communication with PLC

#define pulseport 2 // porta su cui leggere l'impulso
#define plc_dirPin 11  // pin su cui leggere la direzione del motore dal PLC
#define startMotorPin 10 // pin che da il consenso ad arduino per muovere il motore (da parte del plc)
//clock constant
#define Mod 80 //Modulante di di tipo costante d=50%(TOP_val/2)

//stepper constant
#define stepPin 5
#define dirPin 13
#define enablePin 3

#define finecorsa 2000 //da cambiare con la durata della corsa in mm
#define spostamento 1 // 1 decimo di mm per impulso

//sensor variables

long durata;
static float distanza;

//motor variables
int stepsToPerform;
int j=0;
int costanteDiMovimento = 200; // temporaneamente 200(fa fare un giro di motore per ogni unità di 20ms)è funzione del microstepping e dell'attuatore usato
static int GlobalPositionAttuatore = 0;

// communication
bool primaComm = 0;
unsigned long dur1 = 0;
unsigned long dur2 = 0;
long impulsi = 0;
static int moving = 1;
unsigned long uptimestamp = 0;
unsigned long downtimestamp = 0;


///*-------------------------------------------------------------Setup-------------------------------------------------------------------*/ 
void setup() {

  pinMode(pulseport, INPUT_PULLUP);// porta su cui leggere l'impulso
  pinMode(plc_dirPin, INPUT_PULLUP); 
  pinMode(triggerPort, OUTPUT);
  pinMode(echoPort, INPUT);
 //-----------Clock
  //clock register setup
  TCCR1A = 0b10100010;
  TCCR1B = 0b00011001;
  TIMSK1 = 0b00000001;
  DDRB = 0b00000010;
  //global interrupt enable
  sei();//funzione che risetta nei registri i valori da comparare
  ICR1 = 160; //valore di TOP durata del periodi di clock 100kHz
// 
 //------------Stepper
  // We set the enable pin to be an output
  pinMode(enablePin, OUTPUT);
  // then we set it HIGH so that the board is disabled until we
  // get into a known state.
  digitalWrite(enablePin, HIGH);
 pinMode(stepPin, OUTPUT);
 pinMode(dirPin, OUTPUT);
 //-----------Serial
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pulseport), receive_pulse, CHANGE);
  
}

///*--------------------------------------------------------------------------------------------------------------------*/
void loop() {
  
  //porta bassa l'uscita del trigger
  digitalWrite( triggerPort, LOW );
  //invia un impulso di 10microsec su trigger
  digitalWrite( triggerPort, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( triggerPort, LOW );
  durata = pulseIn( echoPort, HIGH ); //misura la durata(in microsecondi) dell'impulso HIGH fornita dal sensore
  
  distanza = (0.034 * durata / 2) / 100;
  
  cmd_parser();
  
}

void receive_pulse(){
  Serial.println("t");
   unsigned long value = millis(); //salvo il time stamp dell'interrupt
   bool rising = digitalRead(pulseport); //capisco se l'impulso è iniziato o è terminato (logica negata per il pull up interno)
   if(rising){
      uptimestamp = value; //aggiorno il time stamp di inizio impulso
     }else{
      downtimestamp = value; // aggiorno il timestamp di fine impulso
      if(primaComm){
        primaComm = 0;
        dur2 = downtimestamp - uptimestamp;
        impulsi = decodifica(dur1,dur2); //conta il numero di unità da 20ms ricevute dall'impulso del plc
        int stepMotore = impulsi * costanteDiMovimento;
        //muovi_motore(stepMotore);
        }else{
                  
          dur1 = (downtimestamp-uptimestamp);
          primaComm =1;
        }
     }
}

long decodifica(unsigned long dur1, unsigned long dur2){
    long p1 = ceil(((dur1)/20.0))-20;
    long p2 = ceil(((dur2)/20.0))-20; 
    return ((p1 <<5)|p2);
  }

void muovi_motore(long stepmot){
  
  moving = 0;
  bool directionState = digitalRead(plc_dirPin); //TODO DIGITAL READ from PLC OUTPUT DIRECTION PIN per scegliere la direzione
  digitalWrite(enablePin, LOW); //abilito il movimento del motore
  if(directionState == HIGH){
        
  GlobalPositionAttuatore += stepmot/costanteDiMovimento;
    
  }else{
    GlobalPositionAttuatore -= stepmot/costanteDiMovimento;
    }
      
  delayMicroseconds(2);
  int del = 1000; //regola la velocità del motore, se è troppo piccolo non fa in tempo a muoversi
  digitalWrite(dirPin, directionState); // setta la direzione del motore (da verificare il senso una volta montato l'attuatore)

  //MOVIMENTO MOTORE
  for (j = 1; j <= stepmot ; j++) {
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
  }
  digitalWrite(enablePin, HIGH);
  moving = 1;
}


//Cmd Parser
void cmd_parser(){
  
  char cmd=Serial.read();
  switch(cmd){
      case 'a': //comunicami la misura del sensore
      Serial.println(distanza);
      break;

      case 'b': //comunicami la posizione motore in openloop 
      Serial.println(GlobalPositionAttuatore);
      break;

      case 'c': //comunicami lo stato del motore (0 running , 1 Stop)
      Serial.println(moving);
      break;
       case 'd': //comunicami il dato letto dal PLC
      Serial.println(impulsi);//da cambiare dur1+dur2 con la formula
      break;     

    }
 
}


//Clock 100kHz
ISR(TIMER1_OVF_vect) {
  static int num;//intero 8 bit definendola statica può essere vista solo da questa funzione
  static char trig1;//char 8 bit
  OCR1A = Mod; //inserisco nel registro di confronto il valore
  trig1 = trig1 ^ 0b00000001; //ogni tc (periodo della carrier) cambio di stato trig1
  //xor Bitwise ovvero faccio una XOR bit a bit ovvero faccio il toggle di tutti i bit dove ho 1 cambia di stato dove è zero resta il valore precedente
  //digitalWrite(clockPin,trig1);
}
