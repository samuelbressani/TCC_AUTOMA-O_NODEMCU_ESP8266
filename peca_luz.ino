
#include <FirebaseArduino.h>
#include<ESP8266WiFi.h>
 
#define FIREBASE_HOST "ifg-teste-81827.firebaseio.com" //Parte do site que hospedará dados do nodemcu
#define WIFI_SSID "Moto C 0311" //Nome da rede WiFi
#define WIFI_PASSWORD "tefinha29" //Senha da rede WiFi
#define FIREBASE_AUTH "S8BWxzbyiKr26y6vwMuW3Y18F3dm9gGMNLDSiAOP" //Chave secreta do servidor


void setup() 
{  
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  digitalWrite(5,HIGH);
digitalWrite(12,HIGH);
digitalWrite(13,HIGH);
digitalWrite(14,HIGH);
  Serial.begin(115200);
// connect to wifi.
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
while (WiFi.status() != WL_CONNECTED) 
{
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() 

{ 
  int Luz1;
  int Luz2;
  int Luz3;
  int Luz4;
   
  Luz1=Firebase.getString("Habitica_1").toInt();
   Luz2=Firebase.getString("Habitica_2").toInt();
    Luz3=Firebase.getString("Habitica_3").toInt();
     Luz4=Firebase.getString("Habitica_4").toInt();
     liga(5,12,13,14,Luz1,Luz2,Luz3,Luz4);


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
   Firebase.setInt("peca_luz",1);
}
//void para ligar atuadores
void liga(int pino1,int  pino2,int  pino3,int  pino4, int  estado1,int  estado2,int  estado3, int  estado4)
{
  
  digitalWrite(pino1,!estado1);
  delay(100);
  digitalWrite(pino2,!estado2);
    delay(100);

  digitalWrite(pino3,!estado3);
      delay(100);

  digitalWrite(pino4,!estado4);
  }


