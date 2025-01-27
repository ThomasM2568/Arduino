#include <SPI.h> // The LoRa module includes an interface with SPI bus
#include <RH_RF95.h> // Radiohead Library for LoRa communications
#include <math.h>

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


float frequencyHz = frequency * 1e6; // Convert MHz to Hz
float wavelength = celerity / frequencyHz; // Wavelength in meters

float e = 3;

float beaconX[3] = {0, 8, 0};
float beaconY[3] = {0, 0, 8};
float distances[3] = {0, 0, 0};

float calculateDistance(float rssi) {

  float distance = (wavelength/(4*M_PI*pow(10,(rssi-TX_POWER)/(10*e))));
  return distance; // Distance in meters
}

void calculatePos(float d0, float d1, float d2, float &x, float &y) {
    // Given fixed position of beacons
    float x0 = beaconX[0], y0 = beaconY[0];
    float x1 = beaconX[1], y1 = beaconY[1];
    float x2 = beaconX[2], y2 = beaconY[2];

    // Trilateration equations based on the geometry of the problem
    float A = 2 * (x1 - x0);
    float B = 2 * (y1 - y0);
    float C = pow(d0, 2) - pow(d1, 2) - pow(x0, 2) + pow(x1, 2) - pow(y0, 2) + pow(y1, 2);
    float D = 2 * (x2 - x0);
    float E = 2 * (y2 - y0);
    float F = pow(d0, 2) - pow(d2, 2) - pow(x0, 2) + pow(x2, 2) - pow(y0, 2) + pow(y2, 2);

    // Debug print intermediate values
    Serial.println("Intermediate Values:");
    Serial.println("A: " + String(A));
    Serial.println("B: " + String(B));
    Serial.println("C: " + String(C));
    Serial.println("D: " + String(D));
    Serial.println("E: " + String(E));
    Serial.println("F: " + String(F));

    // Solve for x and y using the derived equations
    x = (C * E - F * B) / (A * E - B * D);
    y = (C * D - A * F) / (B * D - A * E);

    // Debug print final x and y values
    Serial.println("Calculated Position:");
    Serial.println("x: " + String(x));
    Serial.println("y: " + String(y));
}

void setup() {
  Serial.begin(115200);
  rf95.init();
  rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128); // Bandwidth=125kHz Code rate 4/5 Spreading factor =2^7
  rf95.setFrequency(frequency); // 868.1, 868.2 or 868.3
}

void loop() {
  if (rf95.waitAvailableTimeout(4000)) { // Wait for a packet for 4 seconds
    len = 13;
    if (rf95.recv(buf, &len)) {
      String message = (char*)buf;
      if (message != "" && message.indexOf("ID=") == -1) {
        int comma1 = message.indexOf(',');
        int comma2 = message.indexOf(',', comma1 + 1);
        int comma3 = message.indexOf(',', comma2 + 1);
        int comma4 = message.indexOf(',', comma3 + 1);
        int comma5 = message.indexOf(',', comma4 + 1);
        int comma6 = message.indexOf(',', comma5 + 1);

        beaconID = message.substring(comma2 + 1, comma3).toInt();
        Serial.println("BeaconID : " + String(beaconID));

        float rssi = rf95.lastRssi(); 
        Serial.println("RSSI : " + String(rssi));
        
        e = (rssi-TX_POWER)/(10*log10(wavelength/(4*M_PI*2)));
        Serial.println("LAAAAAA");
        Serial.println(e);

        distances[beaconID] = calculateDistance(rssi);
        Serial.println(distances[beaconID]);

        if (distances[0] > 0 && distances[1] > 0 && distances[2] > 0) {
          float x = 0, y = 0;
          
          calculatePos(distances[0], distances[1], distances[2], x, y);

          Serial.println("Calculated Position:");
          Serial.print("x: ");
          Serial.println(x);
          Serial.print("y: ");
          Serial.println(y);

          // Reset distances for the next loop
          distances[0] = 0;
          distances[1] = 0;
          distances[2] = 0;
        }
      }
    }
  }
  else {
    Serial.println("No reception");
  }
}
