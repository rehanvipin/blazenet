#include<SPI.h>
#include<DHT.h>
#include<nRF24L01.h>
#include<RF24.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float hum;
float temp;

RF24 radio(7,8);
const byte address[6]="venus";
const int gaspin = A0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  dht.begin();
}

void loop() {
  const char identifier[] = "vader";

  // Get the values
  int gaslevel = analogRead(gaspin);
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  //Print them to the serial monitor
  Serial.println(hum);
  Serial.println(temp);
  Serial.println(gaslevel);

  //Pass them to the nrf to pass on 
  //radio.write(&identifier, sizeof(identifier));
  radio.write(&gaslevel, sizeof(gaslevel));
  radio.write(&hum, sizeof(hum));
  radio.write(&temp, sizeof(temp));
  delay(1000);
}
