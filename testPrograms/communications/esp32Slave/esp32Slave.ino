
// USING TCPIP

/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

// ESP32 #2: TCP SERVER + AN LED
#include <SPI.h>
#include <Ethernet.h>

#define LED_PIN  7
#define SERVER_PORT 4080

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
EthernetServer TCPserver(SERVER_PORT);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("ESP32 #2: TCP SERVER + AN LED");

  // Initialize Ethernet Shield:
  if (Ethernet.begin(mac) == 0)
    Serial.println("ESP32 #2: Failed to configure Ethernet using DHCP");

  // Print your local IP address:
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

  // Listening for a TCP client (from ESP32 #1)
  TCPserver.begin();
}

void loop() {
  // Wait for a TCP client from ESP32 #1:
  EthernetClient client = TCPserver.available();

  if (client) {
    // Read the command from the TCP client:
    char command = client.read();
    Serial.print("ESP32 #2: - Received command: ");
    Serial.println(command);

    if (command == '1')
      digitalWrite(LED_PIN, HIGH); // Turn LED on
    else if (command == '0')
      digitalWrite(LED_PIN, LOW);  // Turn LED off

    Ethernet.maintain();
  }
}

// USING WEB BROWSER

////Libraries
//#include <WiFi.h>
////Constants
//#define LED 2
//#define UPDATE_TIME 500
////Parameters
//String nom = "Slave0";
//const char* ssid = "Lunabotics_Selene_2.4";
//const char* password = "MinesLunaboticsWifi1!";
////Variables
//String command;
//unsigned long previousRequest = 0;
////Objects
//WiFiClient master;
//IPAddress server(192, 168, 0, 103);
//void setup() {
// //Init Serial USB
// Serial.begin(115200);
// Serial.println(F("Initialize System"));
// //Init ESP32 Wifi
// WiFi.begin(ssid, password);
// while (WiFi.status() != WL_CONNECTED) {
//   delay(500);
//   Serial.print(F("."));
// }
// Serial.print(nom);
// Serial.print(F(" connected to Wifi! IP address : ")); Serial.println(WiFi.localIP()); // Print the IP address
// pinMode(LED, OUTPUT);
//}
//void loop() {
// requestMaster();
//}
//void requestMaster( ) { /* function requestMaster */
// ////Request to master
// if ((millis() - previousRequest) > UPDATE_TIME) { // client connect to server every 500ms
//   previousRequest = millis();
//   if (master.connect(server, 80)) { // Connection to the server
//     master.println(nom + ": Hello! my current state is x" + String(!digitalRead(LED)) + "\r");
//     //answer
//     String answer = master.readStringUntil('\r');   // receives the answer from the sever
//     master.flush();
//     Serial.println("from " + answer);
//     if (answer.indexOf("x") >= 0) {
//       command = answer.substring(answer.indexOf("x") + 1, answer.length());
//       Serial.print("command received: "); Serial.println(command);
//       if (command == "1") {
//         Serial.println("LED ON");
//         digitalWrite(LED, LOW);
//       } else {
//         Serial.println("LED OFF");
//         digitalWrite(LED, HIGH);
//       }
//     }
//   }
// }
//}
