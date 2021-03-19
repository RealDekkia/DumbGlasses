#include <WiFi.h>
#include <WebServer.h>
#include <TFT_eSPI.h>
#include <SPI.h>

const char* ssid = "**********";
const char* password = "**********";

WebServer server(80);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  //Init TFT
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  //Connect to wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 20);
  tft.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/txt", handleNewText);

  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}

String HTML = "<!DOCTYPE html><title>DumbGlasses</title><link href=https://fonts.gstatic.com rel=preconnect><link href='https://fonts.googleapis.com/css2?family=DotGothic16&display=swap'rel=stylesheet><style>*{font-family:DotGothic16,sans-serif;font-size:1.25rem;margin:0;padding:0}body,html{height:100%;width:100%;background-color:#000}.displayBoxContainer{width:16.5rem;text-align:center;margin:1rem;padding:1rem;display:inline-block}.inputLineBox>input{display:block;letter-spacing:.5rem;width:14.5rem;border:none;background-color:transparent}.inputLineBox{width:14.5rem;background-image:radial-gradient(circle farthest-corner at 25% 55%,#fff 45%,red 80%);border:3px solid red;border-radius:.75rem;width:14.5rem}.buttonBox>button{font-size:1rem}</style><div class=displayBoxContainer><div class=inputLineBox><input maxlength=13 placeholder=1............> <input maxlength=13 placeholder=2............> <input maxlength=13 placeholder=3............> <input maxlength=13 placeholder=4............> <input maxlength=13 placeholder=5............> <input maxlength=13 placeholder=6............> <input maxlength=13 placeholder=7............> <input maxlength=13 placeholder=8............></div><div class=buttonBox><button id=clearButton>Clear</button> <button id=updateText>Update</button></div></div><script>function ElongatedMuskrat(t){for(;t.length<13;)t+=' ';return t}document.getElementById('updateText').onclick=function(){var t=Array.from(document.getElementsByClassName('inputLineBox')[0].children),e='';t.forEach(function(t){e+=ElongatedMuskrat(t.value)});var n=new XMLHttpRequest;n.open('GET',document.location.origin+'/txt?txt='+e),n.send()}</script>";

void handleRoot() {
  server.send(200, "text/html", HTML);
}

void handleNewText() {
  if (server.hasArg("txt")) {
    Serial.println(server.arg("txt"));
    
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println(server.arg("txt"));
    
    server.send(200, "text/html", "OK");
  } else {
    server.send(400, "text/html", "lolno");
  }

}


void handleNotFound() {
  server.send(404, "go away");
}
