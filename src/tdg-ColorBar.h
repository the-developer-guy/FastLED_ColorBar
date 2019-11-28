#include <Arduino.h>
#include <FastLED.h>

class ColorBar
{
  private:
  CRGB *leds;
  uint16_t distance;
  uint16_t step;
  uint16_t ledCount;
  uint16_t minDistance;
  uint16_t maxDistance;
  
  public:
  ColorBar();
  ColorBar(int ledCount, int dataPin, uint16_t minDistance, uint16_t maxDistance);
  void setDistance(uint16_t distance);
  void display();
};
