#include <CytronWiFiShield.h>
#include <CytronWiFiServer.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <EEPROM.h>
//#include <TimerOne.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#define WiFi wifi
#define DHT11_PIN 7
#define DHTTYPE DHT11
#define ESP8266_RX 2
#define ESP8266_TX 3


DHT dht(DHT11_PIN, DHTTYPE);
ESP8266Server server(80);

char ssid[] = "123456";      //  le SSID du reseau Wifi
char pass[] = " ";           //le mot de passe

bool status = false;

volatile bool response = false;
volatile int f_wdt=1;


//
void wakeUp(){
  response = true;
}

//watchdog interrupt service, cette fonction est executée chaque fois que le timer genere une interruption
ISR(WDT_vect) {
	if(f_wdt == 0) {
		f_wdt=1;
	}
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //verifie la presence du module wifi. TX = 2 et RX = 3
  if(!WiFi.begin(ESP8266_RX,ESP8266_TX)){
      Serial.println(F("le module wifi n'est pas present"));
      while(true);
  }
  // on se connecte au wifi
  while(!status){
      Serial.println(F("tentative de connection au wifi!"));
      status = WiFi.connectAP(ssid, pass);
  }
  // on demarre le serveur sur le port 80
  dht.begin();
  server.begin();
  printWifiStatus();
  pinMode(ESP8266_RX,INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  //Timer1.initialize(3000000);
  //Timer1.attachInterrupt(StoreSensorData);
  //setupWatchDogTimer();
  attachInterrupt(digitalPinToInterrupt(ESP8266_RX), wakeUp, CHANGE);

}

void loop() {
 
  if(f_wdt == 1) {
    StoreSensorData();
    	f_wdt = 0;
	}else if(response){
    sendResponse();
    response = false;
  }
   /*if(response){
    sendResponse();
    response = false;
  }*/

	//cette fonction fait passer le microcontrolleur en mode veille
	enterSleep();
}

//on lit les données du capteur et on les stockent en memoire
void StoreSensorData(){

  EEPROM.update(1, highByte(int(dht.readTemperature())));
  EEPROM.update(2, lowByte(int(dht.readTemperature())));
  EEPROM.update(3, highByte(int(dht.readHumidity())));
  EEPROM.update(4, lowByte(int(dht.readHumidity())));
  unsigned long myTime = millis();
  Serial.print("Time: ");
  Serial.println(myTime);

}
//renvoie la valeur de temperature ou de l'humidité en fonction de l'indice passé en parametre
int getSensorData(String parameter){
  
   if(parameter == "T"||parameter == "t"){
    return word(EEPROM.read(1), EEPROM.read(2));
   } else if(parameter == "H"|| parameter == "h")
  return word(EEPROM.read(3), EEPROM.read(4));
}

void printWifiStatus() {
 //afficher le ssid du reseau auquel on est connecté
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // affiche l'adresse ip du serveur
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
  // affiche la portée du signal 
  Serial.print(F("signal strength (RSSI):"));
  Serial.print( WiFi.RSSI());
  Serial.println(" dBm");
}
void sendResponse(){
    
    if(server.hasClient()){
      Serial.println(F("new client connected!"));
      Serial.print(F("request: "));
      Serial.println(server.uri());
      String response = "HTTP/1.1 200 OK\n";
      response+= "Content-type: text/html\n\n";
      response+= " <!DOCTYPE HTML>\n";
      Serial.println(server.method());
      Serial.println(server.args());
      if(server.args() > 0 && server.method() == HTTP_GET){
        for(int i = 0; i < server.args(); i++){
            response+= server.argName(i) + " " + String(getSensorData(server.argName(i).substring(0,1)));
            response+="<br>";
        }
        response+= "</body></html>";
      } else {
          response = "HTTP/1.1 404 Not Found\r\n\r\n";
          Serial.println("requette mal formulée!");
      }
      
      server.print(response);
      Serial.println(response);
      server.closeClient();
      Serial.println("client disconnected");
    }
}

//cette fonction permets de mettre le microcontrolleur en mode veille
void enterSleep(void)
{
  Serial.println("start sleep mode");
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
  //le programme reprends à ce nivueau lorsque le microcontrolleur sort
  //du mode veille
	sleep_disable();
	power_all_enable();
  detachInterrupt(digitalPinToInterrupt(ESP8266_RX));
   Serial.println("end sleep mode");
}

//fonctionqui genere une interruption toutes les 2 secondes 
void setupWatchDogTimer() {
	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR  = (1<<WDP3) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0);
	WDTCSR |= _BV(WDIE);
}
