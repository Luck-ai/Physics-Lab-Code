#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include <esp_wifi.h>
// libraries for the lcd screen
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"

// library for led strip
#include <FastLED.h>

#define LED_PIN 32
#define BUZZER_PIN 33
#define NUM_LEDS 8
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Initialization of screen parameters
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

// same struct to recieve the information
struct pm
{
  uint32_t pm1;
  uint32_t pm2_5;
  uint32_t pm10;
  uint32_t co2;
  uint32_t temp;
  uint32_t hum;
};

// variables to store the values to display on the screen
char PM2_5_VALUE[10];
char  CO2_VALUE[10];
char  TEMPERATURE_VALUE[10];
char  HUMIDITY_VALUE[10];

// values for the knobs on the lcd
int pm_knob;
int co2_knob;
int temp_knob;
int hum_knob;

pm readings;
WiFiServer server(12345);

void setLEDBrightnessAndColor(uint8_t brightness, CRGB color){
  FastLED.setBrightness(brightness);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
  FastLED.show();
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX = 0, touchY = 0;

  bool touched = false; // tft.getTouch( &touchX, &touchY, 600 );

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print("Data x ");
    Serial.println(touchX);

    Serial.print("Data y ");
    Serial.println(touchY);
  }
}

void SetupLCD(){

  lv_init();

  tft.begin();
  tft.setRotation(3);

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();

  Serial.println("Setup done");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&readings, incomingData, sizeof(readings));
  Serial.printf("Bytes received: %d\nPm1: %d\nPm2.5: %d\nPm10: %d\n CO2: %d\n Temp: %d\n Humidity: %d\n", len, readings.pm1, readings.pm2_5, readings.pm10, readings.co2, readings.temp, readings.hum);
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
   const char* ssid = "ESP-32-AI";
  const char* password = "12345678912345";
  WiFi.softAP(ssid, password);
  Serial.println("\nConnected! IP address: " + WiFi.softAPIP().toString());
  server.begin();
  esp_now_init();

  esp_now_register_recv_cb(OnDataRecv);

  SetupLCD();

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  pinMode(BUZZER_PIN, OUTPUT);
  ledcAttachPin(BUZZER_PIN, 0);
  ledcSetup(0, 1000, 10);
}

void loop()
{

  lv_timer_handler();
  delay(5);

  static unsigned long lastTime = 0;
  const unsigned long interval = 1000;

  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Client connected!");
    while (client.connected())
    {
      unsigned long currentTime = millis();
      if (currentTime - lastTime >= interval)
      {
        lastTime = currentTime;
        client.printf("Pm1: %d\nPm2.5: %d\nPm10: %d\n CO2: %d\n Temp: %d\n Humidity: %d\n", readings.pm1, readings.pm2_5, readings.pm10, readings.co2, readings.temp, readings.hum);
      }
    }
  }
  client.stop();

  static unsigned long lastUpdateTime = 0;
  unsigned long currentTime = millis();
  const unsigned long updateInterval = 1000; 

  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
  if (readings.pm2_5 > 0 && readings.pm2_5 < 12)
  {
    setLEDBrightnessAndColor(50, CRGB::Green);
  }
  else if (readings.pm2_5 > 12 && readings.pm2_5 < 35)
  {
    setLEDBrightnessAndColor(50, CRGB::Yellow);
  }
  else if (readings.pm2_5 > 35 && readings.pm2_5 < 55)
  {
    setLEDBrightnessAndColor(50, CRGB::Yellow);
  }
  else if (readings.pm2_5 > 55 && readings.pm2_5 < 150)
  {
    setLEDBrightnessAndColor(50, CRGB::Red);
  }
  else if (readings.pm2_5 > 150 && readings.pm2_5 < 250)
  {
    setLEDBrightnessAndColor(50, CRGB::Red);
  }
  else if (readings.pm2_5 > 250)
  {
    setLEDBrightnessAndColor(100, CRGB::Purple);
    ledcWrite(0, 1023);
    tone(BUZZER_PIN, 1000);
    delay(1000);
    tone(BUZZER_PIN, 500);
    delay(1000);
  }
  if (readings.pm2_5 <100){
    sprintf(PM2_5_VALUE, "%d.0", readings.pm2_5);
  }
  else{
    sprintf(PM2_5_VALUE, "%d", readings.pm2_5);
  }

  if (readings.hum <100){
    sprintf(HUMIDITY_VALUE, "%d.0", readings.hum);
  }
  else{
    sprintf(HUMIDITY_VALUE, "%d", readings.hum);
  }
  sprintf(CO2_VALUE, "%d", readings.co2);
  sprintf(TEMPERATURE_VALUE, "%d", readings.temp);

  pm_knob = map(readings.pm2_5, 0, 250, 0, 200);
  temp_knob = map(readings.temp, 0, 50, 0, 200);
  hum_knob = map(readings.hum, 0, 100, 0, 200);
  co2_knob = map(readings.co2, 0, 2000, 0, 200);

    lv_arc_set_value(ui_KNOB1, pm_knob);
    lv_arc_set_value(ui_KNOB2, co2_knob);
    lv_arc_set_value(ui_KNOB3, temp_knob);
    lv_arc_set_value(ui_KNOB4, hum_knob);
    lv_label_set_text(ui_AQI_VALUE, PM2_5_VALUE);
    lv_label_set_text(ui_CO2_VALUE, CO2_VALUE);  
    lv_label_set_text(ui_TEMPERATURE_VALUE, TEMPERATURE_VALUE);
    lv_label_set_text(ui_HUMIDITY_VALUE, HUMIDITY_VALUE);
  }
}
