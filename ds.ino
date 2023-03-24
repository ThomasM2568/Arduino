#include <Servo.h>
#include <Arduino.h>
#include "ultrasonic.h"
#include "engine.h"
#include <IRremote.h>
Servo servoMoteur;
bool etat=false;
int distance=0;
UltraSonic robot;
const  int  receiverPin = 12;
const  int LEDPin = 13;
int  state = LOW;
unsigned  long  int  codeC ;
IRrecv  irrecv ( receiverPin ) ;
decode_results  results ;
Engine moteur(255);

void setup() {
  pinMode (LEDPin, OUTPUT);
  Serial.begin (9600);
  irrecv.enableIRIn ();
  servoMoteur.attach(3);
}

void stateChange () {
  state = ! state;
  digitalWrite (LEDPin, state);
}

void loop () {
  int num1, num2, num3;
  num1 = digitalRead(10);
  num2 = digitalRead(4);
  num3 = digitalRead(2);
  if (irrecv.decode(&results)) {
    codeC = results.value;
    Serial.print(codeC); Serial.print (" ");
    Serial.print(codeC, BIN); Serial.print (" ");
    Serial.println(codeC, HEX);

    irrecv.resume ();
    delay (150);
    stateChange ();

    switch (codeC) {
      case 16712445:
        Serial.println("OK");
        if (etat==true){
        Serial.println("STOP");
        moteur.stop();
  break;
  }
        break;
      case 16736925:
  if (etat==true){
        Serial.println("UP");
        moteur.goForward();
  break;
  }
        break;
      case 16754775:
  if (etat==true){
        Serial.println("DOWN");
        moteur.goBackward();
  break;
  }
        break;
      case 16720605:
  if (etat==true){
        Serial.println("LEFT");
        moteur.turnLeft();
  break;
  }
        break;
      case 16761405:
  if (etat==true){
        Serial.println("RIGHT");
        moteur.turnRight();
  break;
  }
        break;
      case 16738455:
        Serial.println("UN"); /*Pilotage Automatique*/
        etat=false;
        state = HIGH;
        distance = robot.getDistance();
        Serial.print("distance = ");
        Serial.println(distance);
        
        if(distance < 200){
          moteur.stop();
          moteur.goBackward();
          delay(1000);
          moteur.turnLeft();
          delay(1000);
          }else{
          moteur.goForward();
          delay(500);
          
          } 
        

        
      break;
      case 16750695:
        Serial.println("DEUX"); /* Suivre Ligne*/
        etat=false;
        if (num2 == 1) {
          moteur.goForward();
          delay(5);
        } else {
          moteur.stop();
        } if ( num1 == 0 && num2 == 1 && num3 == 1) {
          moteur.turnRight();
          delay(5);
        } else if (num1 == 1 && num2 == 1 && num3 == 0) {
          moteur.turnLeft();
          delay(5);
        }
        break;
      case 16756815:
        Serial.println("TROIS"); /* Pilotage Manuel*/
        etat=true;
        moteur.stop();
  
        break;
      case 16724175:
        Serial.println("QUATRE");
        moteur.changeSpeed(20);
        break;
      case 17618055:
        Serial.println("CINQ");
        moteur.changeSpeed(40);
        break;
      case 16743045:
        Serial.println("SIX");
        moteur.changeSpeed(60);
        break;
      case 16716015:
        Serial.println("SEPT");
        moteur.changeSpeed(80);
        break;
      case 16726215:
        Serial.println("HUIT");
        moteur.changeSpeed(100);
        break;
      case 16734885:
        Serial.println("NEUF");
        moteur.changeSpeed(120);
        break;
      case 16730805:
        Serial.println("ZERO");
        break;
      case 16728765:
        Serial.println("ASTERISK");
        break;
      case 16732845:
        Serial.println("HASHTAG");
        break;
    }
  }
}
