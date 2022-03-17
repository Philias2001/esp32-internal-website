#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "iPhone van Tom";
// Voor veiligheid is de password vervangen door ****
const char* password = "**********";
String titlename = "test";
String body = "<h1>Hello world!</h1>";
String HTML = 
"<!DOCTYPE HTML><html><head><title>" + titlename + "</title></head><body>" + body + "</body></html>";

WebServer server(80);

const int buttonPin = 25;
const int GreenLED = 26;

bool buttonStateOne = 0;
bool buttonStateTwo = 0;
bool lastButtonState = 0;

void setup() {
   Serial.begin(9600);
   pinMode(buttonPin, INPUT);
   pinMode(GreenLED, OUTPUT);
   Serial.println("Connecting to WiFi: ");
   Serial.println(ssid);

   WiFi.begin(ssid, password);

   while(WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected successfully");
   Serial.print("Got IP: ");
   Serial.println(WiFi.localIP());

   server.on("/", handle_root);

   server.begin();
   Serial.println("HTTP Server started");
   delay(100);

   
}

void loop() {
  server.handleClient();
  buttonStateOne = digitalRead(buttonPin);
  delay(10);
  buttonStateTwo = digitalRead(buttonPin);

  if(buttonStateOne == buttonStateTwo){
    if(buttonStateOne != lastButtonState){
      if(buttonStateOne == HIGH){
        Serial.println("LED ON");
        titlename = "Button is pressed";
        body = "<h1>Led is on</h1>";
        digitalWrite(GreenLED, HIGH);
      } else {
        digitalWrite(GreenLED, LOW);
        titlename = "Button is not pressed";
        body = "<h1>Led is off</h1>";
        Serial.println("LED OFF");
      }
    }
  }
  lastButtonState = buttonStateOne;
  HTML = 
  "<!DOCTYPE HTML><html><head><title>" + titlename + "</title></head><body>" + body + "</body></html>";

  while(WiFi.status() == 6 || WiFi.status() == 1 || WiFi.status() == 5) {
    digitalWrite(GreenLED, HIGH);
    delay(750);
    digitalWrite(GreenLED, LOW);
    delay(500);
    WiFi.begin(ssid, password);
    delay(250);
    Serial.print("Trying to reconnect to: ");
    Serial.println(ssid);
  }
}

void handle_root(){
    server.send(200, "text/html", HTML);
  }
