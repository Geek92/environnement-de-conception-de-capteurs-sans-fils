#include <ESP8266WebServer.h>

String ssid     = "AndroidAPC6EA";  // SSID to connect to
String password = "cqtk5011"; // Our virtual wifi has no password (so dont do your banking stuff on this network)
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
String uri1     = "/update?api_key=MK8LZD4O3VQRPOES&field1=";



void setup() {
  
   // Start our ESP8266 Serial Communication
  Serial.begin(9600);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) Serial.print("cannot start serial connection");
    

  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) Serial.print("cannot connect to wifi");

  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) Serial.print("cannot connect to the host");             
}

void loop() {
  
  // read the temperature 
  //int a = analogRead(A0);
  //int temp = map(((a - 20) * 3.04), 0, 1023, -40, 125);
  int temp = 40;
  // Construct our HTTP call
  String httpPacket = "GET " + uri1 + String(temp) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();

  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10);

  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) return;

  delay(10000);
}
