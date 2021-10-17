/*

  Created on 2020/01/31

  History
  -24/04/2019 Modifica comunicazione per funzionare anche senza modalità debug
  -19/04/2021 Aggiunta clock esterno con SI5351
  -1/04/2021 Modifica della comunicazione Seriale asincrona
  -28/03/2021 Aggiunta comunicazione Seriale
  -27/03/2021 Comunicazione col PLC più efficiente
  -15/03/2021 Aggiunta comunicazione col PLC + gestione motori
  -2/02/2021  Aggiunta del clock a 100kHz, per il PLC sul Pin 12 di arduino.
  -31/01/2021 Gestione della misura di distanza effettuata con HCSR04.


*/

///*----------------------------------------------Library---------------------------------------------------------*/
#include <Wire.h>
#include <Adafruit_SI5351.h>
///*----------------------------------------------Define---------------------------------------------------------*/
//Sensor Constant
#define triggerPort  4
#define echoPort  7
#define clockPin 0


//Communication with PLC
#define pulseport 2 // porta su cui leggere l'impulso
#define plc_dirPin 11  // pin su cui leggere la direzione del motore dal PLC
#define startMotorPin 10 // pin che da il consenso ad arduino per muovere il motore (da parte del plc)


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
int j = 0;
int costanteDiMovimento = 200; // temporaneamente 200(fa fare un giro di motore per ogni unità di 20ms)è funzione del microstepping e dell'attuatore usato
static int GlobalPositionAttuatore = 0;


// communication
bool primaComm = 0;
bool readyToComm = 1;  //flag che dice se il device può comunicare via seriale (viene settato a falso durante la comunicazione col PLC)
unsigned long dur1 = 0;
unsigned long dur2 = 0;
long impulsi = 0;
static int moving = 1;
unsigned long uptimestamp = 0;
unsigned long downtimestamp = 0;
unsigned long tconn;
bool timeout = 0;

///*-------------------------------------------------------------Setup-------------------------------------------------------------------*/
void setup() {

  pinMode(pulseport, INPUT);// porta su cui leggere l'impulso
  pinMode(plc_dirPin, INPUT_PULLUP);
  pinMode(triggerPort, OUTPUT);
  pinMode(echoPort, INPUT);
  //-----------Clock
  SetupClock();

  //------------Stepper
  // We set the enable pin to be an output
  pinMode(enablePin, OUTPUT);
  // then we set it HIGH so that the board is disabled until we
  // get into a known state.
  digitalWrite(enablePin, HIGH);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(startMotorPin, INPUT);

  //-----------Serial
  Serial.begin(38400);
  attachInterrupt(digitalPinToInterrupt(pulseport), receive_pulse, CHANGE);
  Serial.println('1');
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

  cmd_parser();

  if (readyToComm && digitalRead(startMotorPin)) {
    int stepMotore = impulsi * costanteDiMovimento;
    muovi_motore(stepMotore);
    readyToComm = 0;
  }

}

///*---------------------------------------------------------------------------------------------------------------------------------------------------------*/

///*-------------------------------------------------------------Difinizione dell funzioni-------------------------------------------------------------------*/

//------Ricezione delle onde quadre del PLC
void receive_pulse() {
  readyToComm = 0;
  moving = 0;
  //Serial.println("t");
  unsigned long value = micros(); //salvo il time stamp dell'interrupt
  bool rising = digitalRead(pulseport); //capisco se l'impulso è iniziato o è terminato (logica negata per il pull up interno)
  if (rising) {
    uptimestamp = value; //aggiorno il time stamp di inizio impulso
  } else {
    downtimestamp = value; // aggiorno il timestamp di fine impulso
    if (primaComm) {
      primaComm = 0;
      dur2 = downtimestamp - uptimestamp;
      impulsi = decodifica(dur1, dur2); //conta il numero di unità da 20ms ricevute dall'impulso del plc
      readyToComm = 1;
      //Serial.println(impulsi);
      //int stepMotore = impulsi * costanteDiMovimento;
      //muovi_motore(stepMotore);
    } else {
      dur1 = (downtimestamp - uptimestamp);
      primaComm = 1;
    }
  }
}
///*---------------------------------------------------------------------*/

//------Decodifica della distanza ricevuta dal PLC
long decodifica(unsigned long dur1, unsigned long dur2) {
  long p1 = ceil(((dur1) / 15000.0)) - 15;
  long p2 = ceil(((dur2) / 15000.0)) - 15;
  return ((p1 << 5) | p2);
}

///*---------------------------------------------------------------------*/

//------Attuazione dello stepper motor
void muovi_motore(long stepmot) {
  //moving = 0;
  bool directionState = digitalRead(plc_dirPin); //TODO DIGITAL READ from PLC OUTPUT DIRECTION PIN per scegliere la direzione
  digitalWrite(enablePin, LOW); //abilito il movimento del motore
  if (directionState == HIGH) {

    GlobalPositionAttuatore += stepmot / costanteDiMovimento;

  } else {
    GlobalPositionAttuatore -= stepmot / costanteDiMovimento;
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
///*---------------------------------------------------------------------*/


//------Cmd Parser
void cmd_parser() {

  char cmd = Serial.read();

  switch (cmd) {

    case 'a': //comunicami la misura del sensore
      distanza = (0.034 * durata / 2) * 100;
      Serial.println('1');
      Serial.println(distanza);
      break;

    case 'b': //comunicami la posizione motore in openloop
      Serial.println('1');
      Serial.println(GlobalPositionAttuatore);
      break;

    case 'c': //comunicami lo stato del motore (0 running , 1 Stop)
      Serial.println('1');
      Serial.println(moving);
      break;
    case 'd': //comunicami il dato letto dal PLC
      Serial.println('1');
      tconn = millis();
      while (!readyToComm) {
        if ((millis() - tconn) > 3000) {
          timeout = 1;
          break;
        }
        delayMicroseconds(1);
      }
      if (!timeout) {
        Serial.println(impulsi);//da cambiare dur1+dur2 con la formula
      }
      else Serial.println("timeout: controlla la connessione con il PLC");
      timeout = 0;
      break;

  }
}
///*---------------------------------------------------------------------*/


//------Setup SI5351 for 100khz clock
void SetupClock() {
  Adafruit_SI5351 clockgen = Adafruit_SI5351();
  if (clockgen.begin() != ERROR_NONE)
  {
    /* There was a problem detecting the IC ... check your connections */
    Serial.print("Ooops, no Si5351 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  clockgen.setupPLLInt(SI5351_PLL_A, 32);
  clockgen.setupMultisynth(clockPin, SI5351_PLL_A, 4, 1000 - 4, 1);
  clockgen.setupRdiv(clockPin, SI5351_R_DIV_8);

  /* Enable the clocks */
  clockgen.enableOutputs(true);
}
///*---------------------------------------------------------------------*/
