  #include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TB6612_ESP32.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Adafruit_NeoPixel.h>
#include <EasyBuzzer.h>
#include <Servo.h>
int BUZZER_PIN = 27;
#define PIXEL_COUNT 4
#define PIXEL_PIN_1 15
#define PIXEL_PIN_2 13
#define BME_SCK 13
#define BME_MISO 22
#define BME_MOSI 21
#define BME_CS 10
#define AIN1 32
#define AIN2 33
#define PWMA 12
 
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;
Servo puerta;
const int offsetA = 1;
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, 0, 5000, 8, 1);
Adafruit_NeoPixel neopixel1(PIXEL_COUNT, PIXEL_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neopixel2(PIXEL_COUNT, PIXEL_PIN_2, NEO_GRB + NEO_KHZ800);


char ssid[] = "HUAWEI nova 8i";     // your network SSID (name)
char password[] = "Josema123"; // your network key

// Initialize Telegram BOT
#define BOTtoken "5523199266:AAEXlc-O1JNaBeHbOHqwt3rBt6XxFDxmuoA" // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 10;
unsigned long lastTimeBotRan;

int D1Status = 0;
int D2Status = 0;
int D3Status = 0;
int D4Status = 0;
int D5Status = 0;



void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    
 if (text == "/Encender_Alarma") {
   EasyBuzzer.beep(500); // Frequency in hertz(HZ).
      delay(500);
      EasyBuzzer.beep(1500); // Frequency in hertz(HZ).
      delay(500);
      EasyBuzzer.beep(200); // Frequency in hertz(HZ).
      delay(500);
      D2Status = 1;
      bot.sendMessage(chat_id, "Alarma encendida", "");
 }

 if (text == "/Apagar_Alarma") {
     EasyBuzzer.stopBeep();
      D2Status = 0;
      bot.sendMessage(chat_id, "Alarma Apagada", "");
 }

     if (text == "/Encender_Ventilador") {
  forward(motor1, motor1, 100);
  delay(500);
  back(motor1, motor1, 100);
  digitalWrite(23, HIGH);
      D3Status = 1;
     bot.sendMessage(chat_id, "Ventilador Encendido", ""); 
     }
   if (text == "/Apagar_Ventilador") {
  brake(motor1, motor1);
  digitalWrite(23, LOW);
     D3Status = 0;
     bot.sendMessage(chat_id, "Ventilador Apagado", ""); 
   }

   if (text == "/Encender_Luces") {
     for (int i = 0; i < PIXEL_COUNT; i++)
        {
            neopixel1.setPixelColor(i, neopixel1.Color(255, 255, 0));
            neopixel2.setPixelColor(i, neopixel2.Color(255, 255, 0));
        }
        neopixel1.show();
        neopixel2.show();
      D1Status = 1;
      bot.sendMessage(chat_id, "Luces encendidas", "");
    }

    

    if (text == "/Apagar_Luces") {
      for (int i = 0; i < PIXEL_COUNT; i++)
        {
            neopixel1.setPixelColor(i, neopixel1.Color(0, 0, 0));
            neopixel2.setPixelColor(i, neopixel2.Color(0, 0, 0));
        }
        neopixel1.show();
        neopixel2.show();
      D1Status = 0;
      
      bot.sendMessage(chat_id, "Luces apagadas", "");
    }

         if (text == "/Abrir_Puerta") {
  puerta.write(90);
      D3Status = 1;
     bot.sendMessage(chat_id, "Puerta Abierta", ""); 
     }
        if (text == "/Cerrar_Puerta") {
  puerta.write(180);
      D3Status = 1;
     bot.sendMessage(chat_id, "Puerta Cerrada", ""); 
     }

        if (text == "/Temperatura") {
 if(bme.temperature>=28){
  bot.sendMessage(chat_id, "ALERTA! Temperatura Alta", "");
  for (int i = 0; i < PIXEL_COUNT; i++)
        {
            neopixel1.setPixelColor(i, neopixel1.Color(255, 0, 0));
            neopixel2.setPixelColor(i, neopixel2.Color(255, 0, 0));
        }
        neopixel1.show();
        neopixel2.show();
      D1Status = 1;
       back(motor1, motor1, 255);
       digitalWrite(23, HIGH);
       delay(3000);
      brake(motor1, motor1);
      digitalWrite(23, LOW);
       EasyBuzzer.beep(500); // Frequency in hertz(HZ).
      delay(500);
      EasyBuzzer.beep(1500); // Frequency in hertz(HZ).
      delay(500);
      EasyBuzzer.beep(200); // Frequency in hertz(HZ).
      delay(5000);
      EasyBuzzer.stopBeep();

      puerta.write(90);
        
 }
 else{
    bot.sendMessage(chat_id, "Tranquilo! Temperatura Normal", "");
      for (int i = 0; i < PIXEL_COUNT; i++)
        {
            neopixel1.setPixelColor(i, neopixel1.Color(0, 255, 0));
            neopixel2.setPixelColor(i, neopixel2.Color(0, 255, 0));
        }
        neopixel1.show();
        neopixel2.show();
 }
        }

        if (text == "/Gas") {
 if(bme.gas_resistance / 1000.0>=30){
  bot.sendMessage(chat_id, "ALERTA! Gas Alto", "");
  for (int i = 0; i < PIXEL_COUNT; i++)
        {
            neopixel1.setPixelColor(i, neopixel1.Color(255, 0, 0));
            neopixel2.setPixelColor(i, neopixel2.Color(255, 0, 0));
        }
        neopixel1.show();
        neopixel2.show();
      D1Status = 1;
       back(motor1, motor1, 255);
       digitalWrite(23, HIGH);
       delay(2000);
       brake(motor1, motor1);
      digitalWrite(23, LOW);
       EasyBuzzer.beep(500); // Frequency in hertz(HZ).
      delay(500);
      EasyBuzzer.beep(1500); // Frequency in hertz(HZ).
      delay(500);
      EasyBuzzer.beep(200); // Frequency in hertz(HZ).
      delay(5000);
      EasyBuzzer.stopBeep();

      puerta.write(90);
        
 }
 else{
    bot.sendMessage(chat_id, "Tranquilo! Gas Normal", "");
      for (int i = 0; i < PIXEL_COUNT; i++)
        {
            neopixel1.setPixelColor(i, neopixel1.Color(0, 255, 0));
            neopixel2.setPixelColor(i, neopixel2.Color(0, 255, 0));
        }
        neopixel1.show();
        neopixel2.show();
 }
        }
 
 
     
     
    

   
     
 
 


   

   
   
   

   

     if (text == "/Menu" ||text == "/menu"||text == "/MENU"){
    bot.sendMessage(chat_id, " 1. /Encender_Alarma \n 2. /Apagar_Alarma  \n 3. /Encender_Luces \n 4. /Apagar_Luces \n 5. /Abrir_Puerta \n 6. /Cerrar_Puerta  \n 7. /Temperatura \n 8. /Gas" , "");
    
  }
  


  
}
        }
  

    
    





  


void setup() {
  Serial.begin(115200);
  client.setInsecure();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);


  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  EasyBuzzer.setPin(BUZZER_PIN);
  puerta.attach(18);
    pinMode(23, OUTPUT);


  while (!Serial);
  Serial.println(F("BME680 Lectura"));

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



void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000);


}
