#include <SoftwareSerial.h>
const int led1 = 7;
const int led2 = 8;
const int led3 = 9;
const int led4 = 10;
const byte rxPin = 2;
const byte txPin = 4;
int info = 0;

// set up a new serial object
SoftwareSerial mySerial (rxPin, txPin);
    void setup() {
      // define pin modes for tx, rx:
      pinMode(rxPin, INPUT);
      pinMode(txPin, OUTPUT);
      
      pinMode(led1, OUTPUT);
      pinMode(led2, OUTPUT);
      pinMode(led3, OUTPUT);
      pinMode(led4, OUTPUT);
      // set the data rate for the SoftwareSerial port
      Serial.begin(9600);
      mySerial.begin(9600);
      mySerial.println("ready");
      Serial.println("ready2");
    }

    void loop() {
      if (Serial.available()>0){
        Serial.println(" Opa: "+Serial.available());
      }else {
        info = Serial.read();
        if (info != -1){Serial.println(" Info: "+info);}
      }
      if (Serial.available()>0){
        info = Serial.read();
        Serial.println(info);
        if (info == 0) {
          digitalWrite(led1, LOW);
        }else if (info == 1) {
          digitalWrite(led1,HIGH);
        }else if (info == 3) {
          digitalWrite(led2,LOW);
        }else if (info == 2) {
          digitalWrite(led2,HIGH);
        }else if (info == 5) {
          digitalWrite(led3,LOW);
        }else if (info == 4) {
          digitalWrite(led3,HIGH);
        }else if (info == 7) {
          digitalWrite(led4,LOW);
        }else if (info == 6) {
          digitalWrite(led4,HIGH);
        }
      }
    }
