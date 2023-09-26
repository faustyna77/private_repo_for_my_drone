/*
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";

struct DataPacket {
  int pot1Value;
  int pot2Value;
  int pot3Value;
  

};

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(rxAddr);
}

void loop()
{
  DataPacket packet;
  
  packet.pot1Value = analogRead(A2);
  packet.pot2Value = analogRead(A3);
  packet.pot3Value=analogRead(A4);
  //packet.button=digitalRead(2);
  
  radio.write(&packet, sizeof(DataPacket));
  
  Serial.print("Pot1: ");
  Serial.print(packet.pot1Value);
  Serial.print(" | Pot2: ");
  Serial.println(packet.pot2Value);
  Serial.println(packet.pot3Value);
  
  delay(100);
}

*/
////////

///




#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8);
Servo esc3;
Servo esc5;
Servo esc9;
Servo esc10;

const byte rxAddr[6] = "00001";
const unsigned long responseTime = 5;
int currentLed1Value = 1000; // Początkowe wartości silników
int currentLed2Value = 1000;
int currentLed3Value = 1000;
struct DataPacket {
  int pot1Value;
  int pot2Value;
  int pot3Value;
 // int button;
};


float lerp(float , float, float);


void setup()
{

  esc3.attach(3,  1000, 2000);
    esc5.attach(5,  1000, 2000);
    esc9.attach(9,  1000, 2000);
    esc10.attach(10,  1000, 2000);

 
  delay(2000);
  radio.begin();
  radio.openReadingPipe(1, rxAddr);
  radio.startListening();
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(6,OUTPUT);
    esc3.writeMicroseconds(currentLed1Value);
  esc5.writeMicroseconds(currentLed2Value);
  esc9.writeMicroseconds(currentLed3Value);
  esc10.writeMicroseconds(currentLed3Value);
  //pinMode(2,INPUT_PULLUP);

  
  
  
}

void loop()
{
  if (radio.available())
  {
    DataPacket packet;
    radio.read(&packet, sizeof(DataPacket));
    
    int targetLed1Value = map(packet.pot1Value, 0, 1023, 1000, 2000);
    int targetLed2Value = map(packet.pot2Value, 0, 1023, 1000, 2000);
    int targetLed3Value = map(packet.pot3Value, 0, 1023, 1000, 2000);
    for (unsigned long startTime = millis(); millis() - startTime < responseTime;)
    {
      float progress = static_cast<float>(millis() - startTime) / responseTime;
      currentLed1Value = static_cast<int>(lerp(currentLed1Value, targetLed1Value, progress));
      currentLed2Value = static_cast<int>(lerp(currentLed2Value, targetLed2Value, progress));
      currentLed3Value = static_cast<int>(lerp(currentLed3Value, targetLed3Value, progress));

      
      //analogWrite(6, currentLed3Value);
    }
    if(packet.pot3Value>=0 && packet.pot3Value<523 )
    {
         esc3.writeMicroseconds(currentLed3Value);
      esc5.writeMicroseconds(currentLed3Value);
      esc9.writeMicroseconds(currentLed3Value);
      esc10.writeMicroseconds(currentLed3Value);
    }
     else if(packet.pot3Value>=523 ){

     
       esc3.writeMicroseconds(currentLed1Value);
       esc5.writeMicroseconds(currentLed2Value);
        esc9.writeMicroseconds(currentLed2Value);
      esc10.writeMicroseconds(currentLed1Value);
        
     }else if((packet.pot3Value==0) || (packet.pot1Value==0 && packet.pot2Value==0))
     {
      esc3.writeMicroseconds(1000); // Ustaw minimalną wartość sygnału na starcie
  esc5.writeMicroseconds(1000);
  esc9.writeMicroseconds(1000);
  esc10.writeMicroseconds(1000);

     }
    
   
  }
}


float lerp(float start, float end, float progress)
{
  return start + (end - start) * progress;
}


