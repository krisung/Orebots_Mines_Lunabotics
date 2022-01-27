#include <SPI.h>
#include <WiFi.h>
#include "ArduinoTimer.h"
#include "MegunoLink.h"

const uint ServerPort = 80;
WiFiServer Server(ServerPort);
WiFiClient RemoteClient;
ArduinoTimer SendTimer;

const char ssid[] = "Lunabotics_Selene_2.4";
const char pass[] = "MinesLunaboticsWifi1!";
IPAddress ip(192, 168, 0, 103);
IPAddress gateway(192, 168, 0, 254);
IPAddress subnet(255, 255, 255, 0);
int status = WL_IDLE_STATUS;

void CheckForConnections();
//void EchoReceivedData();

void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect to network");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(F("."));
  }
  
  Server.begin();
  Serial.print("Connected to wifi. My address: ");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);
}

void loop() {
  CheckForConnections();
  
  if(RemoteClient){
    while(RemoteClient.connected()){
      while(RemoteClient.available()>0){
        char c = RemoteClient.read();
        Serial.write(c);
      }
      delay(10);
    }

    RemoteClient.stop();
    Serial.println("Client disconnected");
  }
}

void CheckForConnections(){
  if (Server.hasClient()) {
    if (RemoteClient.connected()){  // Prevents connection from multiple computers
      Serial.println("Connection rejected");
      Server.available().stop();
    } else {
      Serial.println("Connection accepted");
      RemoteClient = Server.available();
    }
  }
}

//void EchoReceivedData(){
//  char * ReceiveBuffer[30];
//
//  while(RemoteClient.connected() && RemoteClient.available()){
//    int Received = RemoteClient.read(ReceiveBuffer, sizeof(ReceiveBuffer));
//    RemoteClient.write(ReceiveBuffer, Received);
//    Serial.println(ReceiveBuffer);
//  }
//}
