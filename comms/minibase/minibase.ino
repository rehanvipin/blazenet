#include<printf.h>
#include<RF24Network.h>
#include<RF24.h>
#include<SPI.h>

RF24 radio(10,9);   // CE, CSN
RF24Network network(radio);
const uint16_t mars = 01;
const int gaspin = A0;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  printf_begin();
  radio.printDetails();
  network.begin(90, mars);
}

void loop()
{
  network.update();
  while(network.available());
  {
    int gasval = 999;
    float temp=-273, humd=99;
    char text[32] = "error_here";
    RF24NetworkHeader header;
    network.read(header, &text, sizeof(text));
    network.read(header, &gasval, sizeof(gasval));
    network.read(header, &humd, sizeof(humd));
    network.read(header, &temp, sizeof(temp));
    Serial.println(text);
    Serial.print(gasval);
    Serial.print(",");
    Serial.print(humd);
    Serial.print(",");
    Serial.println(temp);
  }
  delay(1000);
}
