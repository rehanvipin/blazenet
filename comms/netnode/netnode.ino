#include<printf.h>
#include<RF24Network.h>
#include<RF24.h>
#include<SPI.h>
#include<DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float temperature;
float humidity;

RF24 radio(10,9);   // CE, CSN
RF24Network network(radio);
const uint16_t mars = 01;
const uint16_t demos = 011;
const int gasPin = A0;

void setup(){
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN); 
  printf_begin();
  radio.printDetails();
  network.begin(90, demos);
//  radio.printDetails();
  dht.begin();
}

void loop(){
  network.update();
  int gaslevel = analogRead(gasPin);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  Serial.print(gaslevel);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.println(temperature);
  char text[32] = "header ";
  
  RF24NetworkHeader header(mars);
  bool ok = network.write(header, &text, sizeof(text));
  ok = ok & network.write(header, &gaslevel, sizeof(gaslevel));
  ok = ok & network.write(header, &humidity, sizeof(humidity));
  ok = ok & network.write(header, &temperature, sizeof(temperature));
  Serial.println(ok);
  delay(1000);
}
