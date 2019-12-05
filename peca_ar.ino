
#include <FirebaseArduino.h>
#include<ESP8266WiFi.h>
 
#define FIREBASE_HOST "ifg-teste-81827.firebaseio.com" //Parte do site que hospedará dados do nodemcu
#define WIFI_SSID "Moto C 0311" //Nome da rede WiFi
#define WIFI_PASSWORD "tefinha29" //Senha da rede WiFi
#define FIREBASE_AUTH "S8BWxzbyiKr26y6vwMuW3Y18F3dm9gGMNLDSiAOP" //Chave secreta do servidor
#include <IRremoteESP8266.h> //Biblioteca de envio de sinais modulados infravermelhos
IRsend irsend(14); //Função da biblioteca 
#define dht_dpin 0      //Porta D3 no nodemcu IRremoteESP8266 utiliza a porta digital D5 do NodeMCU ESP8266 Lolin v.3 para envio de sinais
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(dht_dpin, DHTTYPE); 

 int tamanho = 227; //Tamanho da linha RAW(68 Blocos)
//unimed int tamanho = 359;
int frequencia = 38; //Frequência do sinal IR(38KHz)


int estadoac=0;
 String pedido;
unsigned int  desligar[] = {3524, 1684, 540, 1288, 540, 1292, 504, 456, 492, 456, 492, 460, 492, 1288, 512, 456, 520, 452, 492, 1288, 512, 1296, 540, 452, 488, 1296, 540, 452, 488, 460, 492, 1288, 512, 1292, 540, 464, 488, 1320, 540, 1288, 508, 456, 492, 456, 492, 1288, 512, 456, 492, 456, 492, 1312, 512, 460, 488, 460, 492, 480, 496, 448, 496, 460, 492, 472, 496, 464, 492, 472, 496, 452, 496, 460, 496, 472, 496, 452, 496, 448, 496, 452, 496, 452, 492, 452, 496, 452, 496, 448, 496, 452, 496, 452, 496, 1284, 512, 460, 492, 480, 492, 1284, 512, 1296, 540, 452, 488, 460, 492, 460, 492, 484, 492, 484, 492, 484, 496, 484, 496, 472, 496, 448, 496, 1284, 512, 460, 492, 452, 492, 460, 496, 480, 496, 484, 492, 476, 496, 448, 496, 1296, 520, 472, 496, 448, 492, 460, 496, 480, 496, 472, 496, 452, 492, 464, 496, 472, 496, 452, 492, 452, 496, 452, 496, 452, 492, 452, 496, 452, 496, 452, 492, 452, 496, 452, 496, 452, 492, 452, 496, 484, 496, 1304, 516, 460, 492, 448, 492, 464, 492, 484, 496, 472, 496, 460, 496, 472, 496, 452, 496, 448, 496, 452, 496, 452, 492, 452, 496, 452, 496, 452, 492, 452, 496, 1284, 516, 456, 492, 484, 492, 1316, 512, 460, 492, 480, 492, 1292, 520, 468, 496};
 unsigned int ligar[]={ 3472, 1704, 540, 1292, 508, 1296, 536, 464, 516, 452, 492, 456, 492, 1312, 516, 452, 492, 460, 492, 1292, 508, 1288, 508, 456, 492, 1292, 540, 464, 488, 480, 496, 1288, 512, 1284, 508, 468, 492, 1320, 536, 1308, 492, 456, 492, 456, 492, 1288, 508, 460, 492, 456, 492, 1308, 516, 488, 492, 460, 492, 480, 496, 448, 492, 460, 496, 480, 496, 476, 492, 452, 496, 460, 496, 472, 496, 452, 492, 452, 496, 452, 492, 452, 496, 452, 496, 452, 492, 452, 496, 1288, 508, 460, 520, 488, 492, 1316, 512, 456, 520, 456, 492, 1288, 540, 1308, 524, 464, 488, 480, 492, 460, 496, 480, 492, 484, 496, 484, 492, 472, 496, 464, 492, 476, 492, 1288, 512, 460, 492, 448, 492, 464, 492, 484, 492, 484, 496, 472, 496, 452, 500, 1280, 532, 436, 496, 448, 492, 464, 492, 484, 492, 484, 496, 472, 476, 472, 488, 456, 492, 456, 480, 464, 496, 452, 496, 452, 496, 448, 496, 452, 496, 452, 496, 448, 496, 452, 496, 452, 496, 448, 500, 448, 496, 1288, 536, 432, 520, 456, 508, 468, 504, 472, 496, 484, 496, 472, 496, 452, 508, 440, 492, 456, 492, 452, 496, 452, 492, 452, 496, 452, 504, 444, 492, 452, 496, 1284, 528, 444, 492, 1332, 516, 1264, 532, 436, 492, 456, 492, 1304, 524, 464, 476};
void setup() 
{
 
  irsend.begin(); //Inicializa função de envio de sinais infravermelhos
  Serial.begin(115200);
// conectando ao WiFi

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("conectando");

while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("conectado a: ");
Serial.println(WiFi.localIP());

//Conectando a Firebase
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    
}


void loop() 

{ int temperatura;
  int humidade;
   


float Temperatura = dht.readTemperature();
float Humidade = dht.readHumidity();
//enviando atualização de temperatura e humidade pro banco de dados
Firebase.setInt("temperatura",Temperatura);
Firebase.setInt("humidade",Humidade);


//verificando o valor da variável que pode ter sido alterado pelo usuário do aplicativo
int check = Firebase.getString("ar").toInt();
  // condição para evitar reenvio de comandos
 if((check==1) && (estadoac==0))
  {
   delay(50);
  
     irsend.sendRaw(ligar,tamanho,frequencia);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
     delay(100);
    Serial.println("Comando enviado:Liga");
    delay(50);

     Serial.println(estadoac);
  estadoac=1;
   }

    // condição para evitar reenvio de comandos

 if(check==0 && estadoac==1)
{
     
       irsend.sendRaw(desligar,tamanho,frequencia);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
    Serial.println("Comando enviado:Desliga");
       estadoac=0;
  delay(50);
}

  
  //reboot da peça caso haja falha na firebase ou WiFi
if ((Firebase.failed()) || (WiFi.status() != WL_CONNECTED))
{
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("reiniciando");
  WiFi.forceSleepBegin(); 
  wdt_reset(); 
  ESP.restart(); 
  while(1)wdt_reset();

  }
  
   delay(1000);
   //Variável para indicar visualmente a conexão da peça no aplicativos
   Firebase.setInt("peca_ar",1);
 
}


