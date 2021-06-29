#include <LedControl.h>
#include <WiFi.h>
#include <WebServer.h>

// Coded by Felipe Beserra :) - GPLV3

// /------------------/
// /--Server config---/
// /------------------/

const char* ssid = "FamiliaOliveira - 2G";  // Enter SSID here
const char* password = "oliveira123";  //Enter Password here

const int ledServerOn = 2;

WebServer server(80);

// /------------------/
// /--8x8 Matrix------/
// /------------------/

#define DIN 18
#define CS 19
#define CLK 21

LedControl lc = LedControl(DIN,CLK,CS,1);

int presetIndex = 0;

byte presetDrawns[3][8] = {
  {B00111100,B01000010,B10100101,B10000001,B10100101,B10011001,B01000010,B00111100},
  {B00111100, B01000010,B10100101,B10000001,B10111101,B10000001,B01000010,B00111100},
  {B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B01000010,B00111100}
};

void setup() {
  Serial.begin(115200);
  
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  Serial.println("8x8 Led Matrix setted up");
  Serial.println("Connecting to " + String(ssid));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_Home);
  server.on("/up", handle_Up);
  server.on("/down", handle_Down);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  pinMode(ledServerOn, OUTPUT);
  digitalWrite(ledServerOn, HIGH);
  draw(presetDrawns[presetIndex]);
}

// /------------------/
// /--Server----------/
// /------------------/

int presetSize = sizeof presetDrawns / sizeof presetDrawns[0];

void handle_Home() {
  server.send(200, "text/html", SendHTML()); 
}

void handle_Up() {
  presetIndex==presetSize-1? presetIndex = 0 : presetIndex++;
  draw(presetDrawns[presetIndex]);
  server.send(200, "text/html", SendHTML()); 
}

void handle_Down() {
  presetIndex==0? presetIndex = presetSize-1 : presetIndex--;
  draw(presetDrawns[presetIndex]);
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Beserrovsky: Esp32_8x8</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 + 8x8 Matrix Web Server</h1>\n";
  ptr +="<h2>Preset on: " + String(presetIndex+1) + " / " + String(presetSize) + "</h2>\n";
  ptr +="<div><a href=\"/down\"><button>&#128072;</button></a><a href=\"/up\"><button>&#128073;</button></div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

// /------------------/
// /--8x8 Matrix------/
// /------------------/

void draw(byte drawn[]){
  lc.clearDisplay(0); 
  lc.setRow(0,0,drawn[0]);
  lc.setRow(0,1,drawn[1]);
  lc.setRow(0,2,drawn[2]);
  lc.setRow(0,3,drawn[3]);
  lc.setRow(0,4,drawn[4]);
  lc.setRow(0,5,drawn[5]);
  lc.setRow(0,6,drawn[6]);
  lc.setRow(0,7,drawn[7]); 
}

void loop(){
  server.handleClient();
}
