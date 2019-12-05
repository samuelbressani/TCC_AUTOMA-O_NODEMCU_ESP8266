
#include <FirebaseArduino.h>
#include<ESP8266WiFi.h>
 
#define FIREBASE_HOST "ifg-teste-81827.firebaseio.com" //Parte do site que hospedará dados do nodemcu
#define WIFI_SSID "Moto C 0311" //Nome da rede WiFi
#define WIFI_PASSWORD "tefinha29" //Senha da rede WiFi
#define FIREBASE_AUTH "S8BWxzbyiKr26y6vwMuW3Y18F3dm9gGMNLDSiAOP" //Chave secreta do servidor

int PIR_Input = 5;//Entrada digital do sensor de movimento

void setup()
{  int i;
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
//Calibrando o sensor de presença
 for(i=0;i<35;i++)
 {
 Serial.println("calibrando...");
 delay(1000);
 
 }
}


void ifttt() //conexão com site que hospeda aplicação configura no site IFTTT
{
  const char host[ ]        = "maker.ifttt.com";          // Canal maker do IFTTT
  const char trigger[ ]     = "teste2";                   //nome do gatilho da aplicação web configurada no site IFTTT
  const char APIKey[ ]      = "dNx1iOkinoO7LyoPbmmJJnIqdKcxXaolvTXPisw92EK";      //Senha secreta para ativar o gatinho
  Serial.print("Connect to: ");
  Serial.println(host);
  // Biblioteca WiFiClient para fazer a conexão HTTP 
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("falha na conexão");
    return;
    }

// Construindo a URL
  String url = String("/trigger/") + trigger + String("/with/key/") + APIKey;
  Serial.print("Requesting URL: ");
  Serial.println(url);

// Requisitando conexão com site IFTTT
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"); 
 
   delay(20);

//Obtendo resposta do site 
  Serial.println("Resposta:");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    }
  Serial.println();
  Serial.println("fechando conexão");
  Serial.println(); 
  Serial.println();
  client.stop();  
  delay (20000); //Espera 20 segundos para enviar outro e-mail
 }

void loop()
{
  Serial.println(digitalRead(PIR_Input));
 digitalWrite(D4, digitalRead(PIR_Input));

 if(digitalRead(PIR_Input)==1)
 {
  
 Firebase.setInt("movimento",1);
  Serial.println("sensor ativado");
  ifttt();
 }
  if(digitalRead(PIR_Input)==0)
 {
   
Firebase.setInt("movimento",0);
 Serial.println("sensor ocioso");
 }
 
 else
 {
   Serial.println("...");
  }
 delay(1000);
 //Variável para indicar visualmente a conexão da peça no aplicativos
   Firebase.setInt("peca_movimento",1);
}
