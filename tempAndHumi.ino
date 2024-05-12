#include "DHT.h"
#include "EEPROM.h"

#define DHT11_PIN 7
#define DHTTYPE DHT11
DHT dht(DHT11_PIN, DHTTYPE);
byte tempHi;
byte tempLow;
byte HumidityHi;
byte HumidityLow;
int humidity;
int temperature;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("initialization");
  dht.begin();
}

void loop() {

// put your main code here, to run repeatedly:
 humidity = dht.readHumidity();
 temperature = dht.readTemperature();

//we retreive the high byte and the low byte of the temperature and the humidity
tempHi = highByte(temperature);
tempLow = lowByte(temperature);
HumidityHi = highByte(humidity);
HumidityLow = lowByte(humidity);

//we write the retreived values inside EEPROM
EEPROM.update(1, tempHi);
EEPROM.update(2, tempLow);
EEPROM.update(3, HumidityHi);
EEPROM.update(4, HumidityLow);

delay(1000);

tempHi = EEPROM.read(1);
tempLow = EEPROM.read(2);
HumidityHi = EEPROM.read(3);
HumidityLow = EEPROM.read(4);

temperature = word(tempHi, tempLow);
humidity = word(HumidityHi, HumidityLow);

Serial.print(humidity);
Serial.println(" HR");
Serial.print(temperature);
Serial.println(" °C");
delay(1000);
}
