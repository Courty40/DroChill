#include <Arduino.h>
#include <BleMouse.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define MINUTE 60000
#define X_RANDOM_RANGE 3
#define Y_RANDOM_RANGE 3

BleMouse bleMouse("DroChill Mouse Jiggler v1");

int move_interval = 12; // with lower interval  notify(): << esp_ble_gatts_send_ notify: rc=-1 Unknown ESP_ERR error appears
int loop_interval = MINUTE * 1;
uint16_t min_distance, max_distance;

const char* ssid = "DroChill";
const char* password = "PoDroChill";

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  min_distance = 30;
  max_distance = 600;
  bleMouse.begin();
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
 
  WebSerial.begin(&server);   
  server.begin();
}

void loop()
{
  static int clickCount, moveCount = 0;

  int distance = random(min_distance, max_distance);
  int x = random(X_RANDOM_RANGE) - 1;
  int y = random(Y_RANDOM_RANGE) - 1;

  for (int i = 0; i < distance; i++)
  {
    moveCount++;
    WebSerial.printf("Clicks count: %d", moveCount);
    bleMouse.move(x, y, 0);
    delay(move_interval);
  }

  if (random(0, 7) == 3){
    bleMouse.click(MOUSE_RIGHT);
    clickCount++;
    WebSerial.printf("Clicks count: %d", clickCount);
  }

  delay(loop_interval);
}
