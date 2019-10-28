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
int tops[3] = {0,0,0};

struct payload_t {                 // Structure of our payload
  int id;
  int ppm;
  float humidity;
  float temperature;
};

payload_t data_buffer[3][10] = {};


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
  payload_t kamikaze = {0, analogRead(gasPin), dht.readHumidity(), dht.readTemperature()};
  payload_t payload;
  if(tops[0]<10)
  {
    tops[0] = tops[0] + 1;
    data_buffer[0][tops[0]] = kamikaze;
  }
  
  network.update();                  // Check the network regularly

  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;
    network.read(header,&payload,sizeof(payload));
    Serial.print("Got data from worker");
    Serial.println(payload.id);

    if(payload.id==1)
    {
      if(tops[1]<10)
      {
        tops[1] = tops[1]+1;
        data_buffer[1][tops[1]] = payload;
      }
      Serial.print("top1 at ");
      Serial.println(tops[1]);
    }
    else if(payload.id==2)
    {
      if(tops[2]<10)
      {
        tops[2] = tops[2] +1;
        data_buffer[2][tops[2]] = payload;
      }
      Serial.print("top2 at ");
      Serial.println(tops[2]);
    }
    else
    {
      Serial.println("Data from invalid node");
    }
    
    Serial.print(payload.ppm - kamikaze.ppm);
    Serial.print(",");
    Serial.print(payload.humidity - kamikaze.humidity);
    Serial.print(",");
    Serial.println(payload.temperature - kamikaze.temperature);

    if((tops[0]>1)&&(tops[1]>1)&&(tops[2]>1))
    {
      tops[0] = tops[0] -1;
      tops[1] = tops[1] -1;
      tops[2] = tops[2] -1;
      payload_t payload_snoke[3] = {data_buffer[0][tops[0]], data_buffer[1][tops[1]], data_buffer[2][tops[2]]};
      RF24NetworkHeader bheader(base);
//      do
//      {
      bool ok = network.write(bheader, &payload_snoke, sizeof(payload_snoke));
      if(ok)
        Serial.println("Sent successfully");
       else
        Serial.println("Failed to send");
    }
//    Serial.println(payload.ms);
  }
}
