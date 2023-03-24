#include "LED.h"
LED led13(1);
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  led13.changeState();
}
