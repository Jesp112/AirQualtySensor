#include <Wire.h>
#include <DFRobot_ENS160.h>
#include <Adafruit_AHTX0.h>
#include <LiquidCrystal.h>

DFRobot_ENS160_I2C airQualitySensor(&Wire, 0x53); // ENS160 I2C-adresse
Adafruit_AHTX0 aht; // AHT21 temperatur- og fugtighedssensor
LiquidCrystal lcd(12,11,5,4,3,2);

const String airQualityUBA[5] = {"Excellent", "Good", "Moderate", "Poor", "Unhealthy"};
int currentUBA;

void printScreen(int airUBA)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(airQualityUBA[airUBA]);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcd.begin(16,2);
  delay(100); // Tilføj en lille forsinkelse for at give tid til initialisering
  
  // Initialisering af ENS160
  if (airQualitySensor.begin() != 0) 
  {
    Serial.println("ENS160 sensor kunne ikke initialiseres!");
    while (1);
  }
  
  // Initialisering af AHT21
  if (!aht.begin()) 
  {
    Serial.println("AHT21 sensor kunne ikke initialiseres!");
    while (1);
  }

  Serial.println("Sensorer initialiseret.");
}

void loop() 
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  currentUBA = airQualitySensor.getAQI() - 1;

  Serial.print("Temperatur: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.print("Fugtighed: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  // Læs data fra ENS160
  Serial.print("TVOC: ");
  Serial.print(airQualitySensor.getTVOC());
  Serial.println(" ppb");

  Serial.print("eCO2: ");
  Serial.print(airQualitySensor.getECO2());
  Serial.println(" ppm");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO2:");
  lcd.print(airQualitySensor.getECO2());
  lcd.setCursor(7, 0);
  lcd.print(" TVOC:");
  lcd.print(airQualitySensor.getTVOC());
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(round(temp.temperature));
  lcd.setCursor(8, 1);
  lcd.print("Humi:");
  lcd.print(round(humidity.relative_humidity));
  lcd.print("%");
  
  //printScreen(currentUBA);


  delay(1000); // Vent 1 sekund, før næste måling
}