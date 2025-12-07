#include <WiFi.h>

WiFiServer server(80);

int internal_led = 4;
const char* ssid = "ssid";
const char* psswd = "***********";
int count = 0;
String header;

String _index = "<!DOCTYPE html>"
"<html>"
"<head>"
"<div id  = 'server_status'>"
"<p>server status: </p>"
"</div>"
"<title>Server</title>"
"<button>off</button>"
"<button>on</button>";

void setup() {
  pinMode(internal_led, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, psswd);
  Serial.print("connecting to ");
  Serial.print(ssid);
  while(WiFi.status() != WL_CONNECTED and count < 50){
    count++;
    delay(500);
    Serial.print(".");

  }
  if(count < 50){
    IPAddress ip(192,168,1,15);
    IPAddress gateway(192,168,1,1);
    IPAddress subnet(255,255,255,0);
    WiFi.config(ip,gateway,subnet);

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.print("ip to connect: ");
    Serial.print(ip);
    server.begin();
  }
  else{
    Serial.println("");
    Serial.println("network error");
  }
}

void loop() {
  WiFiClient client_ = server.available();
  if(client_){
    Serial.println("Nuevo cliente");
    digitalWrite(internal_led, HIGH);
    String currentLine = "";
    while(client_.connected()){
      if(client_.available()){
        char c = client_.read();
        Serial.write(c);
        
      }      
    }   
  }
  else{
    delay(9000);
    Serial.println("client disconnected");
    digitalWrite(internal_led, LOW);
  }
  // put your main code here, to run repeatedly
}

