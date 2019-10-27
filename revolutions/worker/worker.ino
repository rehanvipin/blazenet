/**
 * Simplest possible example of using RF24Network 
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(7,8);                    

RF24Network network(radio);          // Network uses that radio

const uint16_t clusterhead = 01;        // Address of our node in Octal format
const uint16_t worker1 = 011;       // Address of the other node in Octal format

const unsigned long interval = 2000; 

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already
const int gasPin = 2;


struct payload_t {                  // Structure of our payload
  int ppm;
  float humidity;
  float temperature;
};

void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/helloworld_tx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ worker1);
  dht.begin();
}

void loop() {
  
  network.update();                          // Check the network regularly

  
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;
    int gasval = analogRead(gasPin);
    Serial.print("Sending...");
    payload_t payload = { gasval, dht.readHumidity(), dht.readTemperature() };
    RF24NetworkHeader header(/*to node*/ clusterhead);
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}
