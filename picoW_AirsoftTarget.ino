#include <stdint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LEAmDNS.h>
#include <hardware/structs/systick.h>

#include "secrets.h"
#include "saTarget.h"

#define TARGET_NUM 2

WebServer server(80);
const int led = LED_BUILTIN;

volatile bool pcIntTriggering = false; //  割り込み発生中フラグ
Target targets[TARGET_NUM] = {Target(14,15,"ch1"),Target(16,17,"ch2")};
Target t1 = targets[0];

inline uint32_t get_cvr()
{
  return systick_hw->cvr;
}

void ledControll(){
  digitalWrite(led, 1);
  for (uint8_t i = 0; i < TARGET_NUM; i++){
    String findArg = "led" + String(i);
    Serial.print("find arg: ");
    Serial.print(findArg);
    Serial.print("\n");

    for (uint8_t j = 0; j < server.args(); j++){
      if (server.argName(j) == findArg){
        Serial.printf("arg found!!");
        if (server.arg(j) == "on"){targets[i].ledOn();}
        else if (server.arg(j) == "off"){targets[i].ledOff();}
        
      }
    }
  }
  server.send(200, "text/plain", "hello from pico w!\r\n");
  digitalWrite(led, 0);
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from pico w!\r\n");
  digitalWrite(led, 0);
  Serial.print("[Time] ");
  Serial.println(String(millis()));
  t1.ledOn();
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void hitEvent(void *arg){
  Target *tg = static_cast<Target *>(arg);
  if( !tg->targetHit ){
      if((millis() - tg->hitTime) > 100){
          tg->targetHit = true;
          tg->hitTime = millis();
      }
  }
}

//MEMBER_FUNC f = nullptr;

void setup() {
  Serial.begin(115200);
  delay(500);
  // put your setup code here, to run once:
  uint32_t start_time= get_cvr();
  uint32_t end_time= get_cvr();
  Serial.print("[Time] "); Serial.println(String(start_time));
  Serial.print("[Time] "); Serial.println(String(end_time));

  Serial.println("[status] start setup sequence");
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  delay(500);
  digitalWrite(led, 0);
  
  Serial.println("[status] WiFi configuration");
  WiFi.mode(WIFI_STA);

  WiFi.begin(SSID, WIFI_PASSWD);
  Serial.println("[status] ... OK");
  Serial.print("[status] wait for connection");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.print("[status] Connected to ");
  Serial.println(SSID);
  Serial.print("[status] IP address: ");
  Serial.println(WiFi.localIP());

    if (MDNS.begin("picow")) {
    Serial.println("[status] MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/led", ledControll);
  server.onNotFound(handleNotFound);
  server.addHook([](const String & method, const String & url, WiFiClient * client, WebServer::ContentTypeFunction contentType) {
    (void)method;       // GET, PUT, ...
    (void)url;          // example: /root/myfile.html
    (void)client;       // the webserver tcp client connection
    (void)contentType;  // contentType(".html") => "text/html"
    Serial.printf("A useless web hook has passed\n");
    Serial.printf("%s\n", server.uri());
    return WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  server.begin();
  Serial.println("[status] HTTP server started");

  t1.ledOn();
  //attachInterrupt(t1.sensorPin, hitEvent, RISING, t1);
  attachInterruptParam(t1.sensorPin, hitEvent, RISING, (void *)&t1);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  MDNS.update();
  t1.updateStatus();
}
