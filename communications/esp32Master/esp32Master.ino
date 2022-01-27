
//USING TCPIP

/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

// ESP32 #1: TCP CLIENT + A BUTTON/SWITCH
#include <SPI.h>
#include <Ethernet.h>

#define BUTTON_PIN  7
#define SERVER_PORT  4080

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x03};
int val = 3;
IPAddress serverAddress(192, 168, 0, 180);
EthernetClient TCPclient;

void setup() {
  Serial.begin(9600);
  //button.setDebounceTime(50); // set debounce time to 50 milliseconds

  Serial.println("ESP32 #1: TCP CLIENT + A BUTTON/SWITCH");

  // Initialize Ethernet Shield:
  if (Ethernet.begin(mac) == 0)
    Serial.println("ESP32 #1: Failed to configure Ethernet using DHCP");

  // connect to TCP server (ESP32 #2 )
  if (TCPclient.connect(serverAddress, SERVER_PORT))
    Serial.println("ESP32 #1: Connected to TCP server");
  else
    Serial.println("ESP32 #1: Failed to connect to TCP server");
}

void loop() {
  //button.loop(); // MUST call the loop() function first

  if (!TCPclient.connected()) {
    Serial.println("ESP32 #1: Connection is disconnected");
    TCPclient.stop();

    // reconnect to TCP server (ESP32 #2)
    if (TCPclient.connect(serverAddress, SERVER_PORT))
      Serial.println("ESP32 #1: Reconnected to TCP server");
    else
      Serial.println("ESP32 #1: Failed to reconnect to TCP server");
  }

  if (Serial.available()){
    if(Serial.readString() == "on\n"){
      val = 1;
    }
    if(Serial.readString() == "off\n"){
      val = 0;
    }
  }
  
  if (val == 1) {
    TCPclient.write('1');
    TCPclient.flush();
    Serial.println("ESP32 #1: - The button is pressed,  sent command: 1");
    val = 3;
  }

  if (val == 0) {
    TCPclient.write('0');
    TCPclient.flush();
    Serial.println("ESP32 #1: - The button is released, sent command: 0");
    val = 3;
  }
}


// USING WEB CLIENT

////Libraries
//#include <WiFi.h>
////Constants
//#define NUM_SLAVES 1
//#define LED 2
////Parameters
//String nom = "Master";
//const char* ssid = "Lunabotics_Selene_2.4";
//const char* password = "MinesLunaboticsWifi1!";
////Variables
//bool sendCmd = false;
//String slaveCmd = "0";
//String slaveState = "0";
//String command = "0";
//String request;
////Objects
//WiFiServer server(80);
//WiFiClient browser;
//IPAddress ip(192, 168, 0, 103);
//IPAddress gateway(192, 168, 0, 254);
//IPAddress subnet(255, 255, 255, 0);
//
//void setup() {
// //Init Serial USB
// Serial.begin(115200);
// Serial.println(F("Initialize System"));
// //Init ESP32 Wifi
// WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
// WiFi.begin(ssid, password);
// while (WiFi.status() != WL_CONNECTED) {
//   delay(500);
//   Serial.print(F("."));
// }
// server.begin();
// Serial.print(nom);
// Serial.print(F(" connected to Wifi! IP address : http://")); Serial.println(WiFi.localIP()); // Print the IP address
// pinMode(LED, OUTPUT);
//}
//
//void loop() {
//  clientRequest();
//}
//
//void clientRequest( ) { /* function clientRequest */
// ////Check if client connected
// WiFiClient client = server.available();
// client.setTimeout(50);
// if (client) {
//   if (client.connected()) {
//     //Print client IP address
//     Serial.print(" ->");Serial.println(client.remoteIP());
//     String request = client.readStringUntil('\r'); //receives the message from the client
//    
//     if (request.indexOf("Slave0") == 0) {
//       //Handle slave request
//       Serial.print("From "); Serial.println(request);
//       int index = request.indexOf(":");
//       String slaveid = request.substring(0, index);
//       slaveState = request.substring(request.indexOf("x") + 1, request.length());
//       Serial.print("state received: "); Serial.println(slaveState);
//       client.print(nom);
//
//       
//       if (sendCmd) {
//         sendCmd = false;
//         client.println(": Ok " + slaveid + "! Set state to x" + String(slaveCmd) + "\r");
//       } else {
//         client.println(": Hi " + slaveid + "!\r"); // sends the answer to the client
//       }
//       client.stop();                // terminates the connection with the client
//     } else {
//       Serial.print("From Browser : "); Serial.println(request);
//      
//       handleRequest(request);
//       webpage(client);
//     }
//   }
// }
//}
//void handleRequest(String request) { /* function handleRequest */
// ////Check if client connected
// if (request.indexOf("mON") >= 0)  {
//   digitalWrite(LED, LOW);
// }
// if (request.indexOf("mOFF") >= 0)  {
//   digitalWrite(LED, HIGH);
// }
// if (request.indexOf("sON") >= 0)  {
//   sendCmd = true;
//   slaveCmd = "1";
// }
// if (request.indexOf("sOFF") >= 0)  {
//   sendCmd = true;
//   slaveCmd = "0";
// }
//}
//void webpage(WiFiClient browser) { /* function webpage */
// ////Send webpage to browser
// browser.println("HTTP/1.1 200 OK");
// browser.println("Content-Type: text/html");
// browser.println(""); //  do not forget this one
// browser.println("<!DOCTYPE HTML>");
// browser.println("<html>");
// browser.println("<head>");
// browser.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
// browser.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
// browser.println("</head>");
// browser.println("<body style = ' background-color:#000000; color:white;'>");
// browser.println("<hr/><hr>");
// browser.println("<h1><center> Esp32 Electrical Device Control </center></h1>");
// browser.println("<hr/><hr>");
// browser.println("<br><br>");
// browser.println("<br><br>");
// browser.println("<h2> Commands </h2>");
// browser.println("<center>");
// browser.println("Built-in LED");
// browser.println("<a href='/light1on'><button>Turn On </button></a>");
// browser.println("<a href='/light1off'><button>Turn Off </button></a><br />");
// browser.println("</center>");
// browser.println("<br><br>");
// browser.println("<center>");
// browser.println("Device 2");
// browser.println("<a href='/light2on'><button>Turn On </button></a>");
// browser.println("<a href='/light2off'><button>Turn Off </button></a><br />");
// browser.println("</center>");
// browser.println("<br><br>");
// browser.println("<br><br>");
// browser.println("<h2> Data </h2>");
// browser.println("<center>");
// browser.println("<table border='5'>");
// browser.println("<tr>");
// if (digitalRead(LED))
// {
//   browser.print("<td>LED is OFF</td>");
// }
// else
// {
//   browser.print("<td>LED is ON</td>");
// }
// browser.println("<br />");
// if (slaveState == "1") //(digitalRead(4))
// {
//   browser.print("<td>Light 2 is OFF</td>");
// }
// else
// {
//   browser.print("<td>Light 2 is ON</td>");
// }
// browser.println("</tr>");
// browser.println("</table>");
// browser.println("</center>");
// browser.println("</body></html>");
// delay(1);
//}
