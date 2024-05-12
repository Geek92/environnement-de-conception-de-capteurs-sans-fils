#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266WebServer.h>
#include <list>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK "your-password"
#define SERVERPORT 80
#endif
#define DHT11_PIN 2
#define DHTTYPE DHT11

byte tempHi;
byte tempLow;
byte HumidityHi;
byte HumidityLow;
int humidity;
int temperature;
const char* ssid  = STASSID; 
const char* password = STAPSK;

/*struct {
  int temperature;
  int humidity;
} sensorData;*/

std::list <int> sensorData;

//prototypes des fonctions
void write_sensors_values ();
void read_sensors_values ();

DHT dht(DHT11_PIN, DHTTYPE);
ESP8266WebServer server(SERVERPORT);

//on definit les handler pour les differentes requettes du client
void handleRoot() {
  server.send(200, "text/plain", "Hello from ESP8266!");
}

void handleGetRequest(){
  string response = "<!DOCTYPE html><html><head><title>ESP8266 GET Request</title></head><body>";
  if(server.args() > 0){
      for(int value: sensorData)
          response += "<p> "
          response+= server.argName(i);
          response+= ": ";
          response+= value;
          response+= "</p>";
  }
  server.send(200, "text/html", response);
}

void setup() {

  Serial.begin(115200);
  Serial.println("initialisation");
  dht.begin();
  // Connexion au réseau Wi-Fi en envoyant l'identifiant et le mot de passe
  Serial.print("Connexion à ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Afficher les informations sur l'adresse IP et la connexion établie
  Serial.println("");
  Serial.println("Connecté au réseau Wi-Fi.");
  Serial.println("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.println(SERVERPORT)

  //on rajoute les handlers
  server.on("/",handleRoot);
  server.on("/get",handleGetRequest);

  server.begin(); // Démarrer le serveur
  Serial.print("Server started");

  write_sensors_values();

}

void loop(){

    server.handleClient();
    write_sensors_values();
    delay(1000);
}

//
void write_sensors_values(){

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  //on recupere les byte de poids fort et de poids faible
  tempHi = highByte(temperature);
  tempLow = lowByte(temperature);
  HumidityHi = highByte(humidity);
  HumidityLow = lowByte(humidity);

  //on ecrit les données obtenues dans la memoire 
  EEPROM.update(1, tempHi);
  EEPROM.update(2, tempLow);
  EEPROM.update(3, HumidityHi);
  EEPROM.update(4, HumidityLow);
}

int[] read_sensors_values (){

//on les données du capteur stockées en memoire 
  tempHi = EEPROM.read(1);
  tempLow = EEPROM.read(2);
  HumidityHi = EEPROM.read(3);
  HumidityLow = EEPROM.read(4);
  temperature = word(tempHi, tempLow);
  humidity = word(HumidityHi, HumidityLow);
  sensorData.push_back(temperature);
  sensorData.push_back(humidity);

}




