#include <Wire.h>
#include <DFRobot_ENS160.h>
#include <Adafruit_AHTX0.h>
#include <LiquidCrystal.h>

DFRobot_ENS160_I2C airQualitySensor(&Wire, 0x53); // ENS160 I2C-adresse
Adafruit_AHTX0 aht; // AHT21 temperatur- og fugtighedssensor
LiquidCrystal lcd(12,11,5,4,3,2);

void setup() {
Serial.begin(115200);
Wire.begin();
delay(100); // Tilføj en lille forsinkelse for at give tid til initialisering
lcd.begin(16,2);


// Initialisering af ENS160
if (airQualitySensor.begin() != 0) {
  // Link to model https://ardustore.dk/produkt/ens160aht21-luftkvalitet-temp-fugt-module
Serial.println("ENS160 sensor kunne ikke initialiseres!");
while (1);
}

// Initialisering af AHT21
if (!aht.begin()) {
Serial.println("AHT21 sensor kunne ikke initialiseres!");
while (1);
}

Serial.println("Sensorer initialiseret.");
}

void ENS160AirQualityDisplay(int* ECO2, int *TVOC, const sensors_event_t* humidity, const sensors_event_t* temp)
{
    String lcdTemperature = "Temp:" + String(temp->temperature,0) + "C";
    String lcdHumidity = "Humi:" + String(humidity->relative_humidity,0) + "%";
    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("CO2:");
    lcd.print(*ECO2);
    
    lcd.setCursor(0,1);
    lcd.print("TVOC:");
    lcd.print(*TVOC);
    
    lcd.setCursor(8,0);
    //lcd.print("Temp:");
    lcd.print(lcdTemperature);
    
    lcd.setCursor(8,1);
    //lcd.print("Humi:");
    lcd.print(lcdHumidity);
}


void loop() {
sensors_event_t humidity, temp;
int tvoc, eco2;
aht.getEvent(&humidity, &temp);
tvoc = airQualitySensor.getTVOC();
eco2 = airQualitySensor.getECO2();

Serial.print("Temperatur: ");
Serial.print(temp.temperature);
Serial.println(" °C");

Serial.print("Fugtighed: ");
Serial.print(humidity.relative_humidity);
Serial.println(" %");

// Læs data fra ENS160
Serial.print("TVOC: ");
Serial.print(tvoc);
Serial.println(" ppb");

Serial.print("eCO2: ");
Serial.print(eco2);
Serial.println(" ppm");

ENS160AirQualityDisplay(&eco2, &tvoc, &humidity, &temp);

delay(1000); // Vent 1 sekund, før næste måling
}