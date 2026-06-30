#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5

#define BUZZER_PIN 10
#define SWITCH_1   A0
#define SWITCH_2   A1
#define SWITCH_3   A2

  Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

  unsigned long lastUpdateTime = 0;
  const unsigned long interval = 60000;

  //starting time
  int hours = 12;
  int minutes = 12;

  //alarm time
  int alarm_hours = 10;
  int alarm_minutes = 0;

  bool alarmActive = true;

  void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  tft.init(284, 76); // screen dimensions
  tft.setColRowStart(82, 18); // Offsets to get the screen to line up with the weird size
  tft.setRotation(2); // this makes the screen landscape, could be different, try 0-3
  Serial.println("TFT Initialized!");
  tft.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);

  displayTime();

  pinMode(BUZZER_PIN, OUTPUT);
  
  pinMode(SWITCH_1, INPUT_PULLUP);
  pinMode(SWITCH_2, INPUT_PULLUP);
  pinMode(SWITCH_3, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();

//time
  if(currentTime - lastUpdateTime >= interval) {
    lastUpdateTime = currentTime;

    minutes++;
    if(minutes >= 60) {
      minutes = 0;
      hours++;

      if(hours >= 13) {
        hours = 0;
      }
    }

    displayTime();
  }

  checkAlarm();

}

void displayTime() {
  tft.fillRect(0, 0, 284, 76, ST77XX_BLACK);
  tft.setCursor(10, 20);

  tft.print(hours);
  tft.print(";");

  if(minutes < 10) {
    tft.print("0");
  }
  tft.print(minutes);
}

void checkAlarm() {
  if(alarm_hours == hours && alarm_minutes == minutes) {
    alarmActive = true;
  }

  if (!alarmActive) {
    noTone(BUZZER_PIN);
    return;
  }

  int switchesPressed = 0;
  if (digitalRead(SWITCH_1) == LOW) switchesPressed++;
  if (digitalRead(SWITCH_2) == LOW) switchesPressed++;
  if (digitalRead(SWITCH_3) == LOW) switchesPressed++;

  if (switchesPressed == 0) {
    tone(BUZZER_PIN, 1000);
  } 
  else if (switchesPressed == 1) {
    tone(BUZZER_PIN, 1500); // 1 switch
  } 
  else if (switchesPressed == 2) {
    tone(BUZZER_PIN, 2000); // 2 switches
  } 
  else if (switchesPressed == 3) {
    noTone(BUZZER_PIN);     //3 switches, turns off
  }
}