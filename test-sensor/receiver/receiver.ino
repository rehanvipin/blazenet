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
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  dht.begin();
}

void loop() {
  if(radio.available()){
  // Get the values
    int gaslevel = 0;
    hum = 0;
    temp = 0;

    //Print them to the serial monitor

    //Pass them to the nrf to pass on 
    //radio.write(&identifier, sizeof(identifier));
    radio.read(&gaslevel, sizeof(gaslevel));
    radio.read(&hum, sizeof(hum));
    radio.read(&temp, sizeof(temp));
  
    Serial.println(hum);
    Serial.println(temp);
    Serial.println(gaslevel);  
    delay(1000);
  }
}
