#include <SoftwareSerial.h>
#define RX 2
#define TX 4
#define LAMP 8

boolean state = false;
String incoming = "";
SoftwareSerial bluetooth(RX, TX); // RX, TX

void setup() {
  pinMode(LAMP, OUTPUT);
  digitalWrite(LAMP, LOW);
  delay(300);
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("Setup");
}

void loop (){
  if (bluetooth.available()){
     char dados = bluetooth.read();
     if(String(dados)!= "|"){
        incoming = incoming + String(dados);
     }else {
        Serial.println("Incoming data:" + incoming);
        executar(incoming);
        incoming = "";
     }  
  }
}

void executar (String action){
  if (action=="ligar"){
    ligar();
  }else if (action=="desligar"){
    desligar();
  }else if (action=="acionar"){
    acionar();
  }
}

void acionar (){
  if (state==true){
    desligar();
  }else if (state==false){
    ligar();
  }
}

void ligar(){
  state = true;
  digitalWrite(LAMP, HIGH);
}
void desligar(){
  state = false;
  digitalWrite(LAMP, LOW);
}
