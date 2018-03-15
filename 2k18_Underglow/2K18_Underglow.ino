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
   R-Rainbow Everywhere!
   T-Fancy Magenta Things
   U-Fancy Yellow Things
   V-Fancy Cyan Things
   W-White (dont use plos)
   X-Fancy Red Laser Things
   Y-Fancy Blue Laser Things
   Z-Fancy Green Laser Things

*/

#include <Adafruit_NeoPixel.h>

//pinouts for lanes
#define LANE_ONE     5
#define LANE_TWO     6
#define LANE_THREE   10
#define LANE_FOUR    11

#define PIXEL_COUNT        30 //for temp obj

#define PIXEL_COUNT_ONE    45
#define PIXEL_COUNT_TWO    45
#define PIXEL_COUNT_THREE  45
#define PIXEL_COUNT_FOUR   45

#define PIXEL_BRIGHTNESS   255 //0-255

Adafruit_NeoPixel Lane1 = Adafruit_NeoPixel(PIXEL_COUNT_ONE, LANE_ONE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Lane2 = Adafruit_NeoPixel(PIXEL_COUNT_TWO, LANE_TWO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Lane3 = Adafruit_NeoPixel(PIXEL_COUNT_THREE, LANE_THREE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Lane4 = Adafruit_NeoPixel(PIXEL_COUNT_FOUR, LANE_FOUR, NEO_GRB + NEO_KHZ800);

bool runDefault = false;
int PosX = 0;
int rainX = 0;
double interval = 7.1775;

void setup() {
  Serial.begin(9600);
  Lane1.begin(); Lane2.begin(); Lane3.begin(); Lane4.begin();
  Lane1.show();  Lane2.show();  Lane3.show(); Lane4.show();
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
  else if ((Serial.available() > 0) && !runDefault) {
    //timeSince = millis();
    interpret(Serial.read());
    Serial.println("Interpreting");
  }
}

void interpret(char x) {
  switch (x) {
    case 'B': {
      ColorAll(Lane1.Color(0, 0, PIXEL_BRIGHTNESS));
      Serial.println("blue");
      break;
    }
    case 'C': {
      ColorAll(Lane1.Color(PIXEL_BRIGHTNESS, 0, 0));
      Serial.println("red");
      break; 
    }
    case 'D':{
      initToColorFade(Lane1.Color(0, 0, PIXEL_BRIGHTNESS), 10);
      Serial.println("Initializing to blue");
      break;
  }
    case 'E':{
      initToColorFade(Lane1.Color(PIXEL_BRIGHTNESS, 0, 0), 10);
      Serial.println("Initializing to red");
      break;
    }
    case 'F':{
      ColorAll(Lane1.Color(0, 0, 0));
      Serial.println("off");
      break;
    }
    case 'G':{
      initToColorFade(Lane1.Color(0, 0, 0), 10);
      Serial.println("painting it black");
      break;
    }
    case 'P':{
      for (int x = 0; x < 64; x++){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(0, 0, x * 4));
      }
      delay(0.01);
      for (int y = 64; y > 0; y--){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(0, 0, y * 4));
      }
      break;
    }
    case 'Q':{
      for (int x = 0; x < 64; x++){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(x * 4, 0, 0));
      }
      delay(0.01);
      for (int y = 64; y > 0; y--){
          if (Serial.available() > 0){
          break;
          }
        ColorAll(Lane1.Color(y * 4, 0, 0));
      }
      break;
    }
    case 'R':{
      rainX = 0;
      Serial.println("Painting a pretty rainbow");
      while(!(Serial.available() > 0)){
        ColorAllWithWait(Wheel(rainX), 0.5);
        if (rainX > 255){
          rainX = 0;
        } else {
          rainX++;
        }
      }
      break;
    }
    case 'T':{
      PosX = 0;
      while(!(Serial.available() > 0)){
        PosX++;
        MagentaRunningLightsAll(PosX);
        delay(15);
      }
      break;
    }
    case 'U':{
      PosX = 0;
      while(!(Serial.available() > 0)){
        PosX++;
        YellowRunningLightsAll(PosX);
        delay(15);
      }
      break;
    }
    case 'V':{
      PosX = 0;
      while(!(Serial.available() > 0)){
        PosX++;
        CyanRunningLightsAll(PosX);
        delay(15);
      }
      break;
    }
    case 'W':{
      ColorAll(Lane1.Color(PIXEL_BRIGHTNESS, PIXEL_BRIGHTNESS, PIXEL_BRIGHTNESS)); // PLEASE BE CAREFUL, THIS WILL DRAW A LOT OF POWER!
      Serial.println("Painting the strips white");
      break;
    }
    case 'X':{
      PosX = 0;
      while(!(Serial.available() > 0)){
        PosX++;
        RedRunningLightsAll(PosX);
        delay(15);
      }
      break;
    }
    case 'Y':{
      PosX = 0;
      while(!(Serial.available() > 0)){
        PosX++;
        BlueRunningLightsAll(PosX);
        delay(15);
      }
      break;
    }
    case 'Z':{
      PosX = 0;
      while(!(Serial.available() > 0)){
        PosX++;
        GreenRunningLightsAll(PosX);
        delay(15);
      }
      break;
    }
    default:{
      initToColorFade(Lane1.Color(0, 0, 0), 10);
      Serial.println("painting it black");
      break;
    }
  }
}

// Initialises all strip lanes to pixel color using the fade methods used when bot is turned on.

void initToColorFade(uint32_t c, uint8_t wait) { // C is color for all strips, wait is, well, wait time in ms for all strips.
  laneOneColorWipe(c, wait); laneTwoColorWipe(c, wait); laneThreeColorWipe(c, wait); laneFourColorWipe(c, wait);
}

// Self-explanatory, does not use fade methods
void ColorAll(uint32_t c) { // C is color for all strips
  laneOneColor(c); laneTwoColor(c); laneThreeColor(c); laneFourColor(c);
}

// Colors all strips with added wait to supply rainbow or rainbow-esque method
void ColorAllWithWait(uint32_t c, uint8_t t) { // C is color for all strips, t is time
  laneOneColor(c); laneTwoColor(c); laneThreeColor(c); laneFourColor(c);
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


// Lane Four Color Uniform
void laneFourColor(uint32_t c) { // C is color
  for (uint16_t i = 0; i < Lane4.numPixels(); i++) {
    Lane4.setPixelColor(i, c);
    Lane4.show();
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

// Lane Four Fade Color
void laneFourColorWipe(uint32_t c, uint8_t wait) { // C is color, wait is, well, wait time in ms.
  for (uint16_t i = 0; i < Lane4.numPixels(); i++) {
    Lane4.setPixelColor(i, c);
    Lane4.show();
    delay(wait);
  }
}

void defaultDesign() {
      PosX = 0;
      for(int i=0; i < Lane4.numPixels() * 5; i++){
        PosX++;
        BlueRunningLightsAll(PosX);
        delay(10);
      }
}

void RedRunningLightsAll(int position) {
      for(int i=0; i<Lane4.numPixels(); i++) {
        int color = ((sin((i+position)/(interval))*128)+128);
        Lane1.setPixelColor(i,Lane1.Color(color, 0, 0));
        Lane2.setPixelColor(i,Lane1.Color(color, 0, 0));
        Lane3.setPixelColor(i,Lane1.Color(color, 0, 0));
        Lane4.setPixelColor(i,Lane1.Color(color, 0, 0));
      }
      Lane1.show();
      Lane2.show();
      Lane3.show();
      Lane4.show();
}

void BlueRunningLightsAll(int position) {
      for(int i=0; i<Lane4.numPixels(); i++) {
        int color = ((sin((i+position)/(interval))*128)+128);
        Lane1.setPixelColor(i,Lane1.Color(0,0,color));
        Lane2.setPixelColor(i,Lane1.Color(0,0,color));
        Lane3.setPixelColor(i,Lane1.Color(0,0,color));
        Lane4.setPixelColor(i,Lane1.Color(0,0,color));
      }
      Lane1.show();
      Lane2.show();
      Lane3.show();
      Lane4.show();
}

void GreenRunningLightsAll(int position) {
      for(int i=0; i<Lane4.numPixels(); i++) {
        int color = ((sin((i+position)/(interval))*128)+128);
        Lane1.setPixelColor(i,Lane1.Color(0,color,0));
        Lane2.setPixelColor(i,Lane1.Color(0,color,0));
        Lane3.setPixelColor(i,Lane1.Color(0,color,0));
        Lane4.setPixelColor(i,Lane1.Color(0,color,0));
      }
      Lane1.show();
      Lane2.show();
      Lane3.show();
      Lane4.show();
}

void MagentaRunningLightsAll(int position) {
      for(int i=0; i<Lane4.numPixels(); i++) {
        int color1 = ((sin(((i + (Lane4.numPixels()/2))+position)/(interval))*128)+128);
        int color2 = ((sin((i+position)/(interval))*128)+128);
        Lane1.setPixelColor(i,Lane1.Color(color1,0,color2));
        Lane2.setPixelColor(i,Lane1.Color(color1,0,color2));
        Lane3.setPixelColor(i,Lane1.Color(color1,0,color2));
        Lane4.setPixelColor(i,Lane1.Color(color1,0,color2));
      }
      Lane1.show();
      Lane2.show();
      Lane3.show();
      Lane4.show();
}

void YellowRunningLightsAll(int position) {
      for(int i=0; i<Lane4.numPixels(); i++) {
        int color1 = ((sin(((i + (Lane4.numPixels()/2))+position)/(interval))*128)+128);
        int color2 = ((sin((i+position)/(interval))*128)+128);
        Lane1.setPixelColor(i,Lane1.Color(color1,color2,0));
        Lane2.setPixelColor(i,Lane1.Color(color1,color2,0));
        Lane3.setPixelColor(i,Lane1.Color(color1,color2,0));
        Lane4.setPixelColor(i,Lane1.Color(color1,color2,0));
      }
      Lane1.show();
      Lane2.show();
      Lane3.show();
      Lane4.show();
}

void CyanRunningLightsAll(int position) {
      for(int i=0; i<Lane4.numPixels(); i++) {
        int color1 = ((sin(((i + (Lane4.numPixels()/2))+position)/(interval))*128)+128);
        int color2 = ((sin((i+position)/(interval))*128)+128);
        Lane1.setPixelColor(i,Lane1.Color(0,color1,color2));
        Lane2.setPixelColor(i,Lane1.Color(0,color1,color2));
        Lane3.setPixelColor(i,Lane1.Color(0,color1,color2));
        Lane4.setPixelColor(i,Lane1.Color(0,color1,color2));
      }
      Lane1.show();
      Lane2.show();
      Lane3.show();
      Lane4.show();
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
