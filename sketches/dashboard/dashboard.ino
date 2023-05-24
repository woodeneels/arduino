#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SPEEDO_CTR_X    36
#define SPEEDO_CTR_Y    54
#define SPEEDO_RADIUS   36

int fps = 0;
int speed = 0;

void setup() {
  Serial.begin(9600);

  // init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  needleSweep();
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("fps: ");
  display.setCursor(24, 0);
  display.print(fps);

  speedTest();
  drawSpeedo(true);
  drawNeedle(speed);
  measurefps(5);
  display.display();
}

void needleSweep() {
  int cycle = 2;
  int cspeed = 4;
  while(cycle > 0) {
    speed = speed + cspeed;
    if(speed > 159) {
      cycle = cycle - 1;
      cspeed = cspeed - cspeed * 2;
    }
    if(speed < 1) {
      cycle = cycle - 1;
      cspeed = cspeed + cspeed * 2;
    }

    display.clearDisplay();
    drawSpeedo(false);
    drawNeedle(speed);
    display.display();
  }
}

static inline void measurefps(const int seconds) {
  static unsigned long lastMillis;
  static unsigned long frameCount;
  static unsigned int framesPerSecond;

  unsigned long now = millis();
  frameCount++;
  if(now - lastMillis >= seconds * 1000){
    framesPerSecond = frameCount / seconds;
    fps = framesPerSecond;
    frameCount = 0;
    lastMillis = now;
  }
}

void drawNeedle(int speed) {
  speed = constrain(speed, 0, 160);
  float t = (float)(map(speed, 0, 160, 295, 649)) / 100.0;
  int r = SPEEDO_RADIUS - 4;
  int x = r * cos(t) + SPEEDO_CTR_X;
  int y = r * sin(t) + SPEEDO_CTR_Y;

  display.drawLine(SPEEDO_CTR_X, SPEEDO_CTR_Y, x, y, WHITE);
}

void drawSpeedo(bool text) {
  display.drawCircle(SPEEDO_CTR_X, SPEEDO_CTR_Y, SPEEDO_RADIUS, WHITE);
  
  if(!text) {
    return;
  }

  int cursorPos = 0;
  if(speed < 10) {
    cursorPos = 98;
  }
  else if(speed < 100) {
    cursorPos = 88;
  }
  else {
    cursorPos = 74;
  }
  display.setCursor(cursorPos, 20);
  display.setTextSize(3);
  display.println(speed);
  display.setCursor(96, 43);
  display.setTextSize(1);
  display.println("mph");
}

void speedTest() {
  static unsigned long lastMillis;
  unsigned long now = millis();
  if(now - lastMillis >= 100) {
    speed = speed + 1;
    if(speed > 200) {
      speed = 0;
    }
    lastMillis = now;
  }
}
