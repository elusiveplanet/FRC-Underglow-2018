/**
 ***  Arduino Code created by Alexis Renderos of FRC Team 2358 Bearbotics
 ***  Purpose is to drive different "lanes" of LED strips individually on our competition robot.
 ***  This allows for easier creation of patterns using the addressable functions built into the LED strips.
 ***  Code is written for 3 pin 5050 chipset "Neo-Pixel" style LEDs.
**/

/*
   Possible new ideas to test on strips

   Breathing:
   Strips light up from the center of back and spread out through the strips in a U shape
   Derivatives: Rainbow, Blue, Alliance specific

   COMMMANDS FOR SERIAL
   ALL VALUES ARE CHAR

   B-Blue
   C-Red
   D-Blue Delayed
   E-Red Delayed
   F-Off
   G-Fade to off
   P-Blue Bounce
   Q-Red Bounce
   W-White (dont use plos)
   Z-Rainbow Everywhere!

*/

#include <Adafruit_NeoPixel.h>

//pinouts for lanes
#define LANE_ONE     5
#define LANE_TWO     6
#define LANE_THREE   3

#define PIXEL_COUNT        30 //for temp obj

#define PIXEL_COUNT_ONE    144
#define PIXEL_COUNT_TWO    144
#define PIXEL_COUNT_THREE  3

#define PIXEL_BRIGHTNESS   255 //0-255

Adafruit_NeoPixel Lane1 = Adafruit_NeoPixel(PIXEL_COUNT_ONE, LANE_ONE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Lane2 = Adafruit_NeoPixel(PIXEL_COUNT_TWO, LANE_TWO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Lane3 = Adafruit_NeoPixel(PIXEL_COUNT_THREE, LANE_THREE, NEO_GRB + NEO_KHZ800);

bool runDefault = false;

void setup() {
  Serial.begin(9600);
  Lane1.begin(); Lane2.begin(); Lane3.begin();
  Lane1.show();  Lane2.show();  Lane3.show();
  Serial.println("Fully Initialized!");
}

void loop() {
    // Disable code - Takes a time interval and then shuts off the leds if there hasnt been a cereal bowl poured in the last couple of minutes
    /*
    static int timeSince;
    if (millis() + (1000 * 60) <= timeSince)
    {
    interpret('N');
    }
    */
  if (runDefault) {
    Serial.println("Running default code");
    defaultDesign();
  }
  else if (Serial.available() > 0) {
    //timeSince = millis();
    interpret(Serial.read());
    Serial.println("Interpreting");
  }
}

void interpret(char x) {
  switch (x) {
    case 'B':
      ColorAll(Lane1.Color(0, 0, PIXEL_BRIGHTNESS));
      Serial.println("blue");
      break;
    case 'C':
      ColorAll(Lane1.Color(PIXEL_BRIGHTNESS, 0, 0));
      Serial.println("red");
      break;
    case 'D':
      initToColorFade(Lane1.Color(0, 0, PIXEL_BRIGHTNESS), 10);
      Serial.println("Initializing to blue");
      break;
    case 'E':
      initToColorFade(Lane1.Color(PIXEL_BRIGHTNESS, 0, 0), 10);
      Serial.println("Initializing to red");
      break;
    case 'F':
      ColorAll(Lane1.Color(0, 0, 0));
      Serial.println("off");
      break;
    case 'G':
      initToColorFade(Lane1.Color(0, 0, 0), 10);
      Serial.println("painting it black");
      break;
    case 'P':
      for (int x = 0; x < 255; x++){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(0, 0, x));
      }
      delay(0.01);
      for (int y = 255; y > 0; y--){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(0, 0, y));
      }
      break;
    case 'Q':
      for (int x = 0; x < 255; x++){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(x, 0, 0));
      }
      delay(0.01);
      for (int y = 255; y > 0; y--){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(y, 0, 0));
      }
      break;
    case 'W':
      ColorAll(Lane1.Color(PIXEL_BRIGHTNESS, PIXEL_BRIGHTNESS, PIXEL_BRIGHTNESS)); // PLEASE BE CAREFUL, THIS WILL DRAW A LOT OF POWER!
      Serial.println("Painting the strips white");
      break;
    case 'Z':
      Serial.println("Painting a pretty rainbow");
      for (int x = 0;  x < 255; x++) {
          if (Serial.available() > 0){
          break;
          }
        ColorAllWithWait(Wheel(x), 5);
      }
      break;
  }
}

// Initialises all strip lanes to pixel color using the fade methods used when bot is turned on.

void initToColorFade(uint32_t c, uint8_t wait) { // C is color for all strips, wait is, well, wait time in ms for all strips.
  laneOneColorWipe(c, wait); laneTwoColorWipe(c, wait); laneThreeColorWipe(c, wait);
}

// Self-explanatory, does not use fade methods
void ColorAll(uint32_t c) { // C is color for all strips
  laneOneColor(c); laneTwoColor(c); laneThreeColor(c);
}

// Colors all strips with added wait to supply rainbow or rainbow-esque method
void ColorAllWithWait(uint32_t c, uint8_t t) { // C is color for all strips, t is time
  laneOneColor(c); laneTwoColor(c); laneThreeColor(c);
  delay(t);
}

// Lane One Color Uniform
void laneOneColor(uint32_t c) { // C is color
  for (uint16_t i = 0; i < Lane1.numPixels(); i++) {
    Lane1.setPixelColor(i, c);
    Lane1.show();
  }
}

// Lane Two Color Uniform
void laneTwoColor(uint32_t c) { // C is color
  for (uint16_t i = 0; i < Lane2.numPixels(); i++) {
    Lane2.setPixelColor(i, c);
    Lane2.show();
  }
}

// Lane Three Color Uniform
void laneThreeColor(uint32_t c) { // C is color
  for (uint16_t i = 0; i < Lane3.numPixels(); i++) {
    Lane3.setPixelColor(i, c);
    Lane3.show();
  }
}

// Lane One Fade Color
void laneOneColorWipe(uint32_t c, uint8_t wait) { // C is color, wait is, well, wait time in ms.
  for (uint16_t i = 0; i < Lane1.numPixels(); i++) {
    Lane1.setPixelColor(i, c);
    Lane1.show();
    delay(wait);
  }
}

// Lane Two Fade Color
void laneTwoColorWipe(uint32_t c, uint8_t wait) { // C is color, wait is, well, wait time in ms.
  for (uint16_t i = 0; i < Lane2.numPixels(); i++) {
    Lane2.setPixelColor(i, c);
    Lane2.show();
    delay(wait);
  }
}

// Lane Three Fade Color
void laneThreeColorWipe(uint32_t c, uint8_t wait) { // C is color, wait is, well, wait time in ms.
  for (uint16_t i = 0; i < Lane3.numPixels(); i++) {
    Lane3.setPixelColor(i, c);
    Lane3.show();
    delay(wait);
  }
}

void defaultDesign() {
  interpret('D'); // long init blue
  delay(10);
  interpret('E');
  delay(10);
  interpret('P');
  delay(10);
  interpret('Q');
  delay(10);
  interpret('G');
  delay(10);
  interpret('Z');
  delay(10);
  interpret('G');
  delay(10);
}

void straightBlueFire()
{
  // 3 Second burst
  for (int iter = 0; iter <= 30; iter++)
  {
    for (int x = 0; x < Lane1.numPixels(); x++)
    {
      int color1 = regulate(random(128, 212) + 255);
      int color2 = regulate(random(128, 212) + 255);
      int color3 = regulate(random(128, 212) + 255);
      Lane1.setPixelColor(x, Wheel(color1));
      Lane2.setPixelColor(x, Wheel(color2));
      Lane3.setPixelColor(x, Wheel(color3));
    }
    Lane1.show();
    Lane2.show();
    Lane3.show();
    delay(100);
  }
}

void straightActualFire()
  //3 Second burst
{
  for (int iter = 0; iter <= 30; iter++)
  {
    for (int x = 0; x < Lane1.numPixels(); x++)
    {
      int color1 = regulate(random(0, 40) + 250);
      int color2 = regulate(random(0, 40) + 250);
      int color3 = regulate(random(0, 40) + 250);
      Serial.println(color1);
      Serial.println(color2);
      Serial.println(color3);
      Lane1.setPixelColor(x, Wheel(color1));
      Lane2.setPixelColor(x, Wheel(color2));
      Lane3.setPixelColor(x, Wheel(color3));
    }
    Lane1.show();
    Lane2.show();
    Lane3.show();
    delay(100);
  }
}

int regulate(int x)
{
  if (x >= 0 && x <= 255)
    return x;
  else if (x > 255)
    return regulate(x - 255);
  else if (x < 0)
    return regulate(x + 255);
  return x;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition in the sequence: red - green - blue - back to red.
// Requires a pixel number
uint32_t Wheel(byte WheelPos) {
  int LANE_TEMP = 2;
  //temp pixel object to return color from
  Adafruit_NeoPixel LaneTemp = Adafruit_NeoPixel(PIXEL_COUNT, LANE_TEMP, NEO_GRB + NEO_KHZ800);
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)    return LaneTemp.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  if (WheelPos < 170) {
    WheelPos -= 85;       return LaneTemp.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;      return LaneTemp.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
