/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(7,8);                

RF24Network network(radio);      // Network uses that radio
const uint16_t base = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t clusterhead = 01;   // Address of the other node in Octal format
const uint16_t worker1 = 011;
const uint16_t worker2 = 021;
const int gasPin = 2;

struct payload_t {                 // Structure of our payload
  int ppm;
  float humidity;
  float temperature;
};


void setup(void)
{
  Serial.begin(115200);
//  Serial.println("Receiving the data");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ clusterhead);
  dht.begin();
}

void loop(void){
  int selfgas = analogRead(gasPin);
  float selfhum = dht.readHumidity();
  float selftmp = dht.readTemperature();
  
  network.update();                  // Check the network regularly

  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    Serial.println(payload.ppm - selfgas);
    Serial.println(payload.humidity - selfhum);
    Serial.println(payload.temperature - selftmp);
//    Serial.println(payload.ms);
  }
}
