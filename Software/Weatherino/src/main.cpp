#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_BME280.h>
#include <hp_BH1750.h>

#define VERSION      "0.2A"
#define dataSpeed    115200

Adafruit_BME280 bmeInt;
Adafruit_BME280 bmeExt;
hp_BH1750 lux;

void readData(void);
void jsonMesures(void);

float bmeIntTemp = 0;
float bmeIntPress = 0;
float bmeIntHum = 0;

float bmeExtTemp = 0;
float bmeExtPress = 0;
float bmeExtHum = 0;

float luxVal = 0;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//********************************
//*    INITIALISATION SYSTEME    *
//********************************
void setup() {

  uint8_t status;

  // Init Serial DATA
  Serial.begin(dataSpeed);
  while(!Serial);
  // Serial.println(F("-- WEATHERINO for F1ZIC --"));
  
  // Init GPIO
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Init I2C
  Wire.begin();
  
  status = bmeInt.begin(0x76);
  if(!status)
  {
    Serial.println(F("BME280 INT not detected"));
  }
  
  status = bmeExt.begin(0x77);
  if(!status)
  {
    Serial.println(F("BME280 EXT not detected"));
  }  
  
  status = lux.begin(BH1750_TO_GROUND);
  if(!status)
  {
    Serial.println(F("BH1750 not detected"));
  }
  else
  {
    lux.calibrateTiming();
    lux.start();
  } 
}

//*****************************
//*    PROGRAMME PRINCIPAL    *
//*****************************
void loop() {
  readData();
  jsonMesures();
  delay(5000);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//*******************
//*    fonctions    *
//*******************
void readData(void)
{
  // BME INT Sensor 
  bmeIntTemp = bmeInt.readTemperature();
  bmeIntPress = (bmeInt.readPressure() / 100.0F);
  bmeIntHum = bmeInt.readHumidity();
  // BME Ext Sensor
  bmeExtTemp = bmeExt.readTemperature();
  bmeExtPress = (bmeExt.readPressure() / 100.0F);
  bmeExtHum = bmeExt.readHumidity();
  // LUX Sensor
  if (lux.hasValue() == true)
  { 
    luxVal = lux.getLux();
    lux.start();
  }
}

void jsonMesures(void)
{
  // BME INT Sensor
  Serial.print(F("{\"bmeIntTemp\":"));
  Serial.print(bmeIntTemp);
  Serial.print(F(",\"bmeIntPress\":"));
  Serial.print(bmeIntPress);
  Serial.print(F(",\"bmeIntHumid\":"));
  Serial.print(bmeIntHum);  
  // BME Ext Sensor
  Serial.print(F(",\"bmeExtTemp\":"));
  Serial.print(bmeExtTemp);
  Serial.print(F(",\"bmeExtPress\":"));
  Serial.print(bmeExtPress);
  Serial.print(F(",\"bmeExtHumid\":"));
  Serial.print(bmeExtHum);  
  // LUX Sensor
  Serial.print(F(",\"lightLux\":"));
  Serial.print(luxVal);

  Serial.println(F("}"));
}