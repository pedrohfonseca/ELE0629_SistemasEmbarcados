#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int lightBedroom = 2;
const int lightBathroom = 4;
const int lightKitchen = 15;
const int lightLivingroom = 5;
bool Guarda1 = LOW;
bool Guarda2 = LOW;
bool Guarda3 = LOW;
bool Guarda4 = LOW;
int op = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Controle das luzes"); //Bluetooth device name
  Serial.println("O dispositivo iniciou, já pode parear.");
  pinMode(lightBedroom, OUTPUT);
  pinMode(lightBathroom, OUTPUT);
  pinMode(lightKitchen, OUTPUT);
  pinMode(lightLivingroom, OUTPUT);
}

void loop() {
  WriteWord("Deseja ligar ou desligar a luz de algum comodo?\n");
  
  WriteWord("1 - Quarto atualmente: ");
  if(Guarda1 == HIGH) WriteWord("Ligado\n");
  else WriteWord("Desligado\n");
  
  WriteWord("2 - Banheiro atualmente: ");
  if(Guarda2 == HIGH) WriteWord("Ligado\n");
  else WriteWord("Desligado\n");
  
  WriteWord("3 - Cozinha atualmente: ");
  if(Guarda3 == HIGH) WriteWord("Ligado\n");
  else WriteWord("Desligado\n");
  
  WriteWord("4 - Salar de Estar atualmente: ");
  if(Guarda4 == HIGH) WriteWord("Ligado\n");
  else WriteWord("Desligado\n");
  LimparBuffer();

  while(SerialBT.available() == 0);
  
  if (SerialBT.available()) {
    op = SerialBT.read();
    LimparBuffer();
    if(op == '1')
    {
      Guarda1 = !Guarda1;
      digitalWrite(lightBedroom, Guarda1);
      LimparBuffer();
      return;
    }
    if(op == '2')
    {
      Guarda2 = !Guarda2;
      digitalWrite(lightBathroom, Guarda2);
      LimparBuffer();
      return;
    }
    if(op == '3')
    {
      Guarda3 = !Guarda3;
      digitalWrite(lightKitchen, Guarda3);
      LimparBuffer();
      return;
    }
    if(op == '4')
    {
      Guarda4 = !Guarda4;
      digitalWrite(lightLivingroom, Guarda4);
      LimparBuffer();
      return;
    }
    else
    {
      WriteWord("Comando invalido\n");
      LimparBuffer();
      return;
    }
  }
}

void WriteWord(String Word){ // Função para escrever frases
  if(Word.length() > 0){
    for(int i = 0; i < Word.length(); i++){
      SerialBT.write(Word[i]);
    }
  }
}

void LimparBuffer() { // Função para limpar o buffer
  int dados;
  while (SerialBT.available() != 0) {
    dados = SerialBT.read();
  }
}
