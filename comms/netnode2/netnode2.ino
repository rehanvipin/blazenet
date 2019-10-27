/* The network node- phobos
 * These are normal worker nodes, they simply collect values and pass it to mini-nodes at some interval, if they have a direct connection to the base, they send the info directly.
 * They communicate with the mini-base to compare values, next level is recalibration.
 * 
 * Octal address:
 *  mars - 01
 *  demos - 011
 *  phobos - 012
 */

#include<RF24.h>
#include<RF24Network.h>
#include<SPI.h>
#include<DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(9,10);
RF24Network network(radio);
const int gaspin = A0;
const uint16_t mars = 01;
const uint16_t phobos = 012;

/*Setup connection with phobos first*/
void setup() {
 Serial.begin(9600);
 SPI.begin();
 radio.begin();
 network.begin(90, phobos);
 dht.begin();
}

void loop() {
  network.update();
  int gasval = analogRead(gaspin);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
//  Serial.print(gasval);
//  Serial.print(",");
//  Serial.print(humidity);
//  Serial.print(",");
//  Serial.println(temperature);
  
  RF24NetworkHeader header(mars);
  bool ok = network.write(header, &gasval, sizeof(gasval));
  ok = ok & network.write(header, &humidity, sizeof(humidity));
  ok = ok & network.write(header, &temperature, sizeof(temperature));
  Serial.println(ok);
  delay(500);
}
