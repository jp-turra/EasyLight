#include <SoftwareSerial.h>
#define RX 2
#define TX 4
#define LAMP1 8
#define LAMP2 9
#define LAMP3 10

boolean state1 = false;
boolean state2 = false;
boolean state3 = false;
boolean state = false;

String acao = "";
String reset = "";
String lampada = "";
int contador = 0;

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
     if(reset!= "||"){
        if (String(dados)!= "|"){
          if (contador==0){
            acao = acao + String(dados);
          }else if (contador==1){
            lampada = lampada + String(dados);         
          }
        }else {
          reset = reset+String(dados);
          contador++;
        }
     }else {
        Serial.println("Infos recebidas:" + acao + " " + lampada);
        executar(acao, lampada);
        resetar();
     }  
  }
}

void resetar () {
  contador = 0;
  acao = "";
  lampada ="";
  reset = "";
}
void executar (String action, String device){
  int num = definirLampada(device);
  if (num==-1) { return; }
  if (action=="ligar"){
    ligar(num);
  }else if (action=="desligar"){
    desligar(num);
  }else if (action=="acionar"){
    acionar(num);
  }else{
    Serial.println("Comando inválido");
  }
}
int definirLampada (String aux){
  if (aux=="1"){
    state = state1;
    return 8;
  }else if (aux=="2"){
    state = state2;
    return 9;
  }else if (aux=="3"){
    state = state3;
    return 10;
  }else {
    Serial.println("Dispositivo inválido");
    return -1;
  }
}
void registrarEstado (int num, bool state){
  if (num==LAMP1){
    state1 = state;
  }else if (num==LAMP2){
    state2 = state;
  }else if (num==LAMP3){
    state3 = state;
  }
}
void acionar (int num){
  if (state==true){
    desligar(num);
  }else if (state==false){
    ligar(num);
  }
}

void ligar(int num){
  state = true;
  digitalWrite(num, HIGH);
  registrarEstado(num, state);
}
void desligar(int num){
  state = false;
  digitalWrite(num, LOW);
  registrarEstado(num, state);
}
