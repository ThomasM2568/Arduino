#include <Servo.h>
#include <Arduino.h>
#include "ultrasonic.h"
#include "engine.h"

Servo servoMoteur;
UltraSonic robot;
Engine moteur(200);
int distance=0;
int distanceg=0;
int distancec=0;
int distanced=0;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 servoMoteur.attach(3);
}

void loop() {
  distancec=robot.getDistance();  
  Serial.print("Distance = ");
  Serial.println(distance);
  //delay(1000);
  if (distancec < 200){
  moteur.stop();
  servoMoteur.write(65);
  distanced=robot.getDistance();  
  Serial.print("DistanceG = ");
  Serial.println(distanceg);
  delay(500);
  servoMoteur.write(100);
  distancec=robot.getDistance();  
  Serial.print("DistanceC = ");
  Serial.println(distancec);
  delay(500);
  servoMoteur.write(140);
  distanceg=robot.getDistance();  
  Serial.print("Distance D= ");
  Serial.println(distanced);
  delay(500);
  servoMoteur.write(100);

  if (distanceg > distanced){
    moteur.goBackward();
    delay(1000);
    moteur.turnLeft();
    delay(500);
    moteur.goForward();
  }else if (distanced > distanceg){
    moteur.goBackward();
    delay(1000);
    moteur.turnRight();
    delay(500);
    moteur.goForward();
    }
  }else{
    moteur.goForward();
  }
}
  
