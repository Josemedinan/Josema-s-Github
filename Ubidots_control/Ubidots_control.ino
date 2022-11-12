#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
 
#define SEALEVELPRESSURE_HPA (1013.25)
 
Adafruit_BME680 bme; // I2C
 
#define WIFISSID "HITRON-F5A0" // Put your WifiSSID here
#define PASSWORD "tigo13101976" // Put your wifi password here
#define TOKEN "BBFF-OI5Dl6Mby7P02hsnCbyjmYdy3wCLqC" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "ESP32_BME680_Station" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string;
 
/****************************************
  Define Constants
****************************************/
#define VARIABLE_LABEL1 "Temperature" // Assing the variable label
#define VARIABLE_LABEL2 "Humidity" // Assing the variable label
#define VARIABLE_LABEL3 "Pressure"
#define VARIABLE_LABEL4 "Altitude"
#define VARIABLE_LABEL5 "DewPoint"
#define VARIABLE_LABEL6 "Gas"
#define DEVICE_LABEL "ESP32"
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[1000];
char topic1[150];
char topic2[150];
char topic3[150];
char topic4[150];
char topic5[150];
char topic6[150];
 
// Space to store values to send
char str_Temperature[10];
char str_Humidity[10];
char str_Pressure[10];
char str_Altitude[10];
char str_DewPoint[10];
char str_Gas[10];
 
/****************************************
  Auxiliar Functions
****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);
 
 
void callback(char* topic, byte* payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
}
 
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, ""))
    {
      Serial.println("Connected");
    } else
    {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}
 
/****************************************
  Main Functions
****************************************/
 
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("BME680 test"));
 
  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
 
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
 
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.println();
  Serial.print("Waiting for WiFi Connection ..............");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}
 
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  if (! bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
 
  float temperature = (bme.temperature);
  float humidity = (bme.humidity);
  float pressure = (bme.pressure / 100.0);
  float altitude = (bme.readAltitude(SEALEVELPRESSURE_HPA));
  double dewPoint = (dewPointFast(temperature, humidity));
  float gas = (bme.gas_resistance / 1000.0);
 
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
 
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
 
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");
 
  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");
 
  Serial.print("Dew Point = ");
  Serial.print(dewPoint);
  Serial.println(" *C");
 
  Serial.print("Gas = ");
  Serial.print(gas);
  Serial.println(" KOhms");
 
  dtostrf(temperature, 4, 2, str_Temperature);
  dtostrf(humidity, 4, 2, str_Humidity);
  dtostrf(pressure, 4, 2, str_Pressure);
  dtostrf(altitude, 4, 2, str_Altitude);
  dtostrf(dewPoint, 4, 2, str_DewPoint);
  dtostrf(gas, 4, 2, str_Gas);
 
 
  sprintf(topic1, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL1);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_Temperature);
  Serial.println("Publishing temperature to Ubidots Cloud");
  client.publish(topic1, payload);
 
  sprintf(topic2, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL2);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_Humidity);
  Serial.println("Publishing humidity to Ubidots Cloud");
  client.publish(topic2, payload);
 
  sprintf(topic3, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL3);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_Pressure);
  Serial.println("Publishing Pressure data to Ubidots Cloud");
  client.publish(topic3, payload);
 
  sprintf(topic4, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL4);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_Altitude);
  Serial.println("Publishing Altitude data to Ubidots Cloud");
  client.publish(topic4, payload);
 
  sprintf(topic5, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL5);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_DewPoint);
  Serial.println("Publishing Dew Point data to Ubidots Cloud");
  client.publish(topic5, payload);
 
  sprintf(topic6, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL6);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_Gas);
  Serial.println("Publishing Gas data to Ubidots Cloud");
  client.publish(topic6, payload);
 
  Serial.println();
  client.loop();
  delay(5000);
}
 
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}
