#include "AdafruitIO_WiFi.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;

#define IO_USERNAME  "Josemedinan_"
#define IO_KEY       "aio_erUy44Fn68vYvbbOeTMgI7xMhGJq"

#define WIFI_SSID       "HITRON-F5A0"
#define WIFI_PASS       "tigo13101976"

#include "Adafruit_SHT31.h"

Adafruit_SHT31 sensor = Adafruit_SHT31();
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);


AdafruitIO_Feed *tempIO = io.feed("temp");
AdafruitIO_Feed *GasIO = io.feed("Gas");
AdafruitIO_Feed *PressIO = io.feed("Press");
AdafruitIO_Feed *AltiIO = io.feed("Alti");
AdafruitIO_Feed *HumeIO = io.feed("Hume");


void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
}

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
    if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
    }
    Serial.print("Temperature = ");
    Serial.print(bme.temperature);
    Serial.println(" *C");
    tempIO->save(bme.temperature);
    delay(3000);

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");
  GasIO->save(bme.gas_resistance / 1000.0);
  delay(3000);


  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");
  PressIO->save(bme.pressure / 100.0);
  delay(3000);


  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  AltiIO->save(bme.readAltitude(SEALEVELPRESSURE_HPA));
  delay(3000);

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");
  HumeIO->save(bme.humidity);
  delay(3000);

 
}
