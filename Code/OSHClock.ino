#include <OctoWS2811.h>
#include <TimeLib.h>
/* OSH Clock - Code for the crazy LED clock.
 *  Features: 3 Pad capsense for
 *  Features: RTC
 *  Features: Neopixel LED arms
 *  
 */

// Where do generals keep their armies?    In their sleevies.
const int armies[] = {255,0,1,14,15,8,9,12,13,10,11,6,7};
const int arms       = 12;
const int ledsPerArm = 20;
const int arms_per_leg = 2;
const int leds_total = 320; // There are 80 "missing" leds on the two OctoLegs we're not using.

const int config = WS2811_GRB | WS2811_800kHz;

DMAMEM int displayMemory[leds_total*6];
int drawingMemory[leds_total*6];

OctoWS2811 leds(ledsPerArm * arms_per_leg, displayMemory, drawingMemory, config);

volatile int milli_offset = 0;

int millisync() {
  int milli, lastmilli, lastsec,count;
  count     = 0;
  lastsec   = second();
  lastmilli = millis();
  while (count < 2) {
    if (lastsec != second()) {
//      milli = millis() - lastmilli;
      count++;
      lastmilli = millis();
      lastsec = second();
    }
  }
  return lastmilli;
}

void setup() {
  hourFormat12();
  setSyncProvider(getTeensy3Time);
  
  leds.begin();
  leds.show();
  Serial.begin(9600);
  milli_offset = millisync();
}



time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

int armFactor(int arm) {
  return ledsPerArm*armies[arm];
}

// Light up an entire arm
void lightArm(int arm, boolean on) {
  int color = 0;
  if (on) { color = 0x550055; }
  
  for (int i = 0; i<ledsPerArm; i++){
    leds.setPixel(i+armFactor(arm),color);
  }
}

// Set a pixel on an arm to an R, G, B color.
void lightPixelOnArm(int pixel, int arm, int r, int g, int b) {
  leds.setPixel(pixel + armFactor(arm), r, g, b);
}

// Set a pixel on an arm to a hex color.
void lightPixelOnArm(int pixel, int arm, int color) {
  leds.setPixel(pixel + armFactor(arm), color);
}

void debug(int arm, int h, int m, int s) {
  Serial.print(arm); Serial.print(",");
  Serial.print(h); Serial.print(",");
  Serial.print(m); Serial.print(",");
  Serial.println(s);
}

int l,c;
void loop() {
 // l = micros();
  int arm;
  int hours,minutes,seconds, milli;
 // debug(0,hour(),minute(),second());
  for (arm = 1 ; arm <= 12 ; arm++ ) {
    hours   = procHour(arm);
    minutes = procMinute(arm);
    seconds = procSecond(arm);
    if (seconds > 0) {
      Serial.println(seconds);
      milli = milliseconds();
    } else {
      milli = 0;
    }
    updateClock(arm,hours,minutes,seconds,milli);
  }
  leds.show();
  //c = micros() - l;
  //Serial.println(c);
  delayMicroseconds(random(1000));
}
