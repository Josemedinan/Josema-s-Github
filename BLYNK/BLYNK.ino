#define BLYNK_TEMPLATE_ID "TMPLmHlyBz9a"
#define BLYNK_DEVICE_NAME "Weather Station"
#define BLYNK_AUTH_TOKEN "ASPBj1gy8aibpM7SzEWN3ta5yiKFLf5U"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "HUAWEI nova 8i";  // type your wifi name
char pass[] = "Josema123";  // type your wifi password

BlynkTimer timer;
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#define BME_SCK 13
#define BME_MISO 22
#define BME_MOSI 21
#define BME_CS 10


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; 

void setup() {
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sendSensor);
  Serial.begin(115200);
   while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms


}

void sendSensor()
{

  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

   Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

 
  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

    Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

   Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

    
    Blynk.virtualWrite(V0, bme.temperature);
    Blynk.virtualWrite(V1, bme.humidity);
    Blynk.virtualWrite(V2, bme.pressure / 100.0); 
    Blynk.virtualWrite(V3, bme.gas_resistance / 1000.0);
    Blynk.virtualWrite(V4, bme.readAltitude(SEALEVELPRESSURE_HPA)) ;
    delay(2000);
}
void loop()
{
Blynk.run();
  timer.run();

   if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  }
