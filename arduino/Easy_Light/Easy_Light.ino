#include <SoftwareSerial.h>
#define RX 2
#define TX 4
#define LAMP1 8
#define LAMP2 9
#define LAMP3 10

boolean state = false;
String incoming = "";
SoftwareSerial bluetooth(RX, TX); // RX, TX

void setup() {
  pinMode(LAMP1, OUTPUT);
  pinMode(LAMP2, OUTPUT);
  pinMode(LAMP3, OUTPUT);
  digitalWrite(LAMP1, LOW);
  digitalWrite(LAMP2, LOW);
  digitalWrite(LAMP3, LOW);
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
void definirLampada (){
  
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
  digitalWrite(LAMP1, HIGH);
}
void desligar(){
  state = false;
  digitalWrite(LAMP1, LOW);
}
