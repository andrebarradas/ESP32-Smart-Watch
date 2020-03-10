#include "Declarations.h"
#include "TimeTracker.h"
#include "Animations.h"
#include "Window.h"
#include "SelectionWindow.h"
#include "Display.h"
#include "Button.h"
#include "Pages.h"
#include "home.h"
#include "settings.h"
#include "apps.h"
#include "Notifications.h"
#include "battery.h"
#include <soc/rtc.h>
#include <esp_clk.h>

void interfaceLoop();
void activate() ;

#define SCREEN_TOUCH_ON_TIME 5000
unsigned long lastTouch = 0;

const int SleepTime = 50000; //we wake the micro-controller up after this many microseconds

RTC_DATA_ATTR int bootCount = 0;

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("bootCount = " + String(bootCount));
  Serial.flush();
#endif
  gpio_hold_dis((gpio_num_t) LCD_LED_CTRL);
  pinMode(TOUCH_INT, INPUT);
  pinMode(LCD_LED_CTRL, OUTPUT);

  struct tm tm;
  tm.tm_year = 2020 - 1900;
  tm.tm_mon = 10;
  tm.tm_mday = 15;
  tm.tm_hour = 14;
  tm.tm_min = 10;
  tm.tm_sec = 10;
  time_t t = mktime(&tm);
  printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);

  if (digitalRead(TOUCH_INT) == 0) {
    activate(bootCount == 0);
  }

  digitalWrite(LCD_LED_CTRL, LOW);
  gpio_hold_en((gpio_num_t)LCD_LED_CTRL);
  gpio_deep_sleep_hold_en();

  esp_sleep_enable_timer_wakeup(SleepTime);
  bootCount++;
  esp_deep_sleep_start();

}


void loop() {

}

void activate(boolean getTime) {
  digitalWrite(LCD_LED_CTRL, LOW);
  pinMode(CHARGING_PIN, OUTPUT);
  pinMode(BATTERY_SENSE, OUTPUT);
  pinMode(TOUCH_INT, INPUT);
  digitalWrite(CHARGING_PIN, LOW);

  initLCD();
  SweepClear();

  if (getTime) {
    getInternetTime();
  }

  page = HOME;
  drawHome();

  digitalWrite(LCD_LED_CTRL, HIGH);

  long ActiveUntil = millis() + 10000;
  long lastHomeDraw = 0;

  while (ActiveUntil > millis()) {
    if (digitalRead(TOUCH_INT) == 0) {
      ActiveUntil = millis() + 10000;
    }
    interfaceLoop();

    Serial.println("Active = " + String(ActiveUntil));

    if (lastHomeDraw + 1000 < millis()) {
      lastHomeDraw = millis();
      if (page == HOME) {
        drawHome();
      }
    }
  }
}




/* things like touch interfacing and other user input go here */
void interfaceLoop() {

  if (ts.touched()) {
    struct point p = getTouchedPosition();
    tft.drawCircle(p.x, p.y, 2, 0x1D07);

    switch (page) {
      case HOME: HomeTouchHandler(p.x, p.y); break;
      case NOTIFICATIONS: NotificationsTouchHandler(p.x, p.y); break;
      case SETTINGS: SettingsTouchHandler(p.x, p.y); break;
    }
  }
}
//
///* things like drawing the time don't need to happen often at all, so we tuck them in here
//*/
//void lowPriority() {
//  if (page == HOME) {
//    drawHome();
//  }
//}
