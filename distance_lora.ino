#include <SPI.h>      // The LoRa module includes an interface with SPI bus
#include <RH_RF95.h>  // Radiohead Library for LoRa communications
#include <math.h>
#include <map>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

RH_RF95 rf95(RFM95_CS, RFM95_INT);
int TX_POWER = 13;

uint8_t buf[255];
uint8_t len = sizeof(buf);

float frequency = 868.1;
int beaconID;
float celerity = 3 * pow(10, 8);
int x = 0;
int y = 0;

float calculateDistance(float rssi) {
  float frequencyHz = frequency * 1e6;        // Convert MHz to Hz
  float wavelength = celerity / frequencyHz;  // Wavelength in meters

  float distance = (wavelength / (4 * M_PI * pow(10, (rssi - TX_POWER) / 30)));

  return distance;  // Distance in meters
}


void setup() {
  Serial.begin(115200);
  rf95.init();
  rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
  // Bandwidth=125kHz Code rate 4/5 Spreading factor =2^7
  rf95.setFrequency(frequency);  // 868.1, 868.2 or 868.3
}

void loop() {
  if (rf95.waitAvailableTimeout(4000)) {  // Wait for a packet for 4 seconds
    len=13;
    if (rf95.recv(buf, &len)) {
      String message = (char*)buf;
      Serial.printf("m=%s t=%d\n",buf,len);
      Serial.println(message);
      int comma1 = message.indexOf(',');
      int comma2 = message.indexOf(',', comma1 + 1);
      int comma3 = message.indexOf(',', comma2 + 1);
      int comma4 = message.indexOf(',', comma3 + 1);
      Serial.println(message.indexOf("ID="));

      if(message!="" && message.indexOf("ID=") == -1){
        
        beaconID = message.substring(comma2 + 1, comma3).toInt();
        Serial.println("Received message is: " + String(message));
        Serial.println("BeaconID : " + String(beaconID));

        x = message.substring(comma3 + 1, comma4).toInt();
        Serial.print("Beacon coordinates: " + String(x) + ",");

        y = message.substring(comma4 + 1).toInt();
        Serial.println(String(y));


        float rssi = rf95.lastRssi();
        Serial.println("RSSI : " + String(rssi));

        Serial.println("distance : " + String(calculateDistance(rssi)));


      }
    }
  } else Serial.println("No reception");
}