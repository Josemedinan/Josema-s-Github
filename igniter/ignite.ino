#include <ESP8266WiFi.h>

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
char ssid[] = "HITRON-F5A0";     // your network SSID (name)
char password[] = "tigo13101976"; // your network key

// Initialize Telegram BOT
#define BOTtoken "5787490196:AAF6ibHIfgLoOSLiuA4R6xyVRednQqvEsyw" // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 500;
unsigned long lastTimeBotRan;

int D1Status = 0;
int D2Status = 0;
int D3Status = 0;


void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

if (text == "/Encender_Igniter") {
   digitalWrite(12, 1);

      bot.sendMessage(chat_id, "Igniter Encendido", "");
    }
    if (text == "/Apagar_Igniter") {
   digitalWrite(12, 0);

      bot.sendMessage(chat_id, "Igniter Apagado", "");
    }
   if (text == "/Menu") {

      bot.sendMessage(chat_id, "Bienvenido al Bot de Ignite para \n encender nuestro cohete pulsa \n los siguientes comandos \n 1. /Encender_Igniter \n 2. /Apagar_Igniter", "");
    
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
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    lastTimeBotRan = millis();
  }

}
