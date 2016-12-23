#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Replace with your network credentials
const char* WIFI_HOST = "Disco Party Phone";
const char* WIFI_PASSWORD = "88888888";

const int LED_PIN = 5;
const int METER_PIN = 13;

// Yielding in the loop to keep Wifi running:
// https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/using-the-esp8266-in-arduino
const int SAFE_WAIT_TIME = 1000;

MDNSResponder mdns;

ESP8266WebServer server(80);

bool load_blink = true;

const char* webPage = "<h1>Analog Meter Thing</h1>"
  "<p><a href=\"meter?level=1023\"><button>Meter to 100%</button></a></p>"
  "<p><a href=\"meter?level=512\"><button>Meter to 50%</button></a></p>"
  "<p><a href=\"meter?level=0\"><button>Meter to 0%</button></a></p>";

// LIFECYCLE -------------------------------------------------------------------

void setup(void) {
  // preparing GPIOs
  pinMode(LED_PIN, OUTPUT);
  lightOff();

  delay(1000);
  Serial.begin(115200);
  Serial.println("Starting up...");
  WiFi.begin(WIFI_HOST, WIFI_PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    loadingBlink();
    delay(500);
    Serial.print(".");
  }
  lightOff();
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_HOST);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  server.on("/meter", [](){
    server.send(200, "text/html", "Setting the meter");
    int level = server.arg("level").toInt();
    setMeter(level);
    delay(SAFE_WAIT_TIME);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

// HELPERS ---------------------------------------------------------------------

void loadingBlink() {
  load_blink = !load_blink;
  if (load_blink) {
    lightOn();
  } else {
    lightOff();
  }
}

void lightOn() {
  digitalWrite(LED_PIN, HIGH);
  setMeter(1023);
}

void lightOff() {
  digitalWrite(LED_PIN, LOW);
  setMeter(0);
}

void setMeter(int amount) {
  analogWrite(METER_PIN, amount);
}
