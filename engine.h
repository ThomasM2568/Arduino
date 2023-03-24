#include <Arduino.h>
class Engine{

 private:
  const int in1=6;
  const int in2=7;
  const int in3=8;
  const int in4=9;
  const int ENA=5;
  const int ENB=11;
  int velocity;

 public:
 Engine(int s){
    velocity=s;
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    stop();
  }

  void stop(){
    digitalWrite(ENA,LOW);
    digitalWrite(ENB,LOW);
    Serial.println("stop");

  }
  void changeSpeed(int s){
    velocity=s;
    
    }

  
  void goForward(){
    analogWrite(ENA,velocity);
    analogWrite(ENB,velocity);
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    Serial.println("goForward");
  }
  void goBackward(){
    analogWrite(ENA,velocity);
    analogWrite(ENB,velocity);
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    Serial.println("goBackward");
  }
 
  void turnLeft(){
    analogWrite(ENA,velocity);
    analogWrite(ENB,velocity);
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    Serial.println("turnleft");
  }

  void turnRight(){
    analogWrite(ENA,velocity);
    analogWrite(ENB,velocity);
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    Serial.println("turnright");
  }

};
