#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebServer  web(80); // Web Server
AsyncWebSocket  ws("/ws"); //Websocket

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  WiFi.mode(WIFI_AP);
  String ssid = "ESPBoardTest";
  WiFi.softAP(ssid.c_str());
  initWeb();
  pinMode(00, OUTPUT);
  pinMode(04, OUTPUT);
  pinMode(05, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void initWeb() {
  ws.onEvent(wsEvent);
  web.addHandler(&ws);
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
  web.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  web.begin();
}

void wsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
        AwsEventType type, void * arg, uint8_t *data, size_t len) {
          char* temp = reinterpret_cast<char*>(data);
          Serial.println(temp);
  switch (type) {
    case WS_EVT_DATA: {
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(reinterpret_cast<char*>(data));
        uint16_t pin = json["pin"].as<int>();
        uint16_t val = json["value"].as<int>();
        Serial.print("Received data ");
        Serial.print(val);
        Serial.print(" at pin ");
        Serial.println(pin);
        analogWrite(pin, val);
      break;
    }
    case WS_EVT_CONNECT:
      Serial.print(F("* WS Connect - "));
      Serial.println(client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.print(F("* WS Disconnect - "));
      Serial.println(client->id());
      break;
    case WS_EVT_PONG:
      Serial.println(F("* WS PONG *"));
      break;
    case WS_EVT_ERROR:
      Serial.println(F("** WS ERROR **"));
      break;
  }
}
