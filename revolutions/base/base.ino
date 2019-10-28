#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7,8);
RF24Network network(radio);
const uint16_t base = 00;
const uint16_t clusterhead = 01;

struct payload_t{
  int id;
  int ppm;
  float humidity;
  float temperature;
};

void setup(void) {
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  network.begin(90, base);
}

void loop() {
  RF24NetworkHeader header;
  payload_t payloads[3];

  network.update();

  while(network.available()){
    network.read(header, &payloads, sizeof(payloads));
    for(int i=0;i<3;i++)
    {
      Serial.print(payloads[i].id);
      Serial.print(",");
      Serial.print(payloads[i].ppm);
      Serial.print(",");
      Serial.print(payloads[i].humidity);
      Serial.print(",");
      Serial.println(payloads[i].temperature);
    }
  }
}
